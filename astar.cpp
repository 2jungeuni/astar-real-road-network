//
// Created by jelee on 23. 10. 29.
//

#include "astar.h"

#include <array>
#include <fstream>

const float PI = 3.141592; // Pi

double degree_to_radian(double degree) {
    return degree * PI / 180;
}

// Class: Vertex
Vertex::Vertex(int id, std::pair<double, double> coord) : id(id), coord(coord) {
    this->visited = 0;
    this->distance = std::numeric_limits<double>::infinity();
    this->heuristic = std::numeric_limits<double>::infinity();
    this->cost = std::numeric_limits<double>::infinity();
}

// Initialize
void Vertex::initialize() {
    this->visited = 0;
    this->distance = std::numeric_limits<double>::infinity();
    this->heuristic = std::numeric_limits<double>::infinity();
    this->cost = std::numeric_limits<double>::infinity();
}

// GET
int Vertex::get_id() {return this->id;}

std::set<Vertex*> Vertex::get_connection() {
    std::set<Vertex*> neighbor;
    for (auto adj: this->adjacent) {
        neighbor.insert(adj.first);
    }
    return neighbor;
}

double Vertex::get_weight(Vertex* neighbor) {return this->adjacent[neighbor];}

// ADD
void Vertex::add_neighbor(Vertex *neighbor, double weight) {
    (this->adjacent).insert(std::make_pair(neighbor, weight));
}

// SET
void Vertex::set_distance(double dist) {
    this->distance = dist;
    this->set_cost();
}

void Vertex::set_cost() {this->cost = this->distance + this->heuristic;}

void Vertex::set_visited() {this->visited = 1;}

void Vertex::set_previous(Vertex* prev) {this->prev = prev;}

// Class: Graph
// heuristic function
double Graph::haversine(std::pair<double, double> frm_coord, std::pair<double, double> to_coord) {
    double long1 = degree_to_radian(frm_coord.first);
    double lat1 = degree_to_radian(frm_coord.second);
    double long2 = degree_to_radian(to_coord.first);
    double lat2 = degree_to_radian(to_coord.second);

    // haversine formula
    double dlong = long2 - long1;
    double dlat = lat2 - lat1;
    double a = pow(sin(dlat/2),2) + cos(lat1)*cos(lat2)*pow(sin(dlong/2), 2);
    double c = 2 * asin(sqrt(a));
    int r = 6371;                   // Radius of earth in kilometers.
    return c * r * 1000;            // unit: m
}

double Graph::heu_travel_time(std::pair<double, double> frm_coord, std::pair<double, double> to_coord) {
    return this->haversine(frm_coord, to_coord) / this->max_speed_limit;
}

double Graph::heu_carbon_emission(std::pair<double, double> frm_coord, std::pair<double, double> to_coord) {
    return this->min_carbon_emission * this->haversine(frm_coord, to_coord);
}

double Graph::heuristic(std::pair<double, double> frm_coord, std::pair<double, double> to_coord) {
    return this->alpha * this->haversine(frm_coord, to_coord)
    + this->beta * this->heu_travel_time(frm_coord, to_coord)
    + this->gamma * this->heu_carbon_emission(frm_coord, to_coord);
}

// Initialize
void Graph::initialize() {
    for (auto elem: this->graph) {
        elem.second->initialize();
    }
}

// GET
std::pair<double, double> Graph::get_coordinate(int node_id) {
    return (*graph[node_id]).coord;
}

Vertex* Graph::get_vertex(int id) {return graph[id];}

// ADD
void Graph::add_vertex(int node_id, double longitude, double latitude) {
    Vertex* pVertex = new Vertex(node_id, std::make_pair(longitude, latitude));
    this->graph.insert(std::make_pair(node_id, pVertex));
}

void Graph::add_edge(int frm, int to, double weight) {
    std::pair<double, double> frm_coord = get_coordinate(frm);
    std::pair<double, double> to_coord = get_coordinate(to);
    graph[frm]->add_neighbor(graph[to], weight);
}

void Graph::add_heuristic(int node, int end_node) {
    std::pair<double, double> node_coord = get_coordinate(node);
    std::pair<double, double> end_coord = get_coordinate(end_node);
    graph[node]->heuristic = this->heuristic(node_coord, end_coord);
    graph[node]->set_cost();
}

double Graph::route_planning(int start_node, int end_node) {
    Vertex* current;
    std::set<Vertex*> current_adjacent;
    std::set<Vertex> heap;
    double new_cost;

    Vertex* start = this->get_vertex(start_node);
    Vertex* end = this->get_vertex(end_node);

    start->set_distance(0.0);
    this->add_heuristic(start_node, end_node);


    for (auto itr=this->graph.begin(); itr!=this->graph.end(); itr++) {
        heap.insert(*itr->second);
    }

    while (current != end) {
        auto pop = heap.begin();
        heap.erase(pop);

        current = this->get_vertex(pop->id);
        current->set_visited();

        current_adjacent = current->get_connection();
        for (auto &adj: current_adjacent) {
            //std::cout << *adj << std::endl;
            if (adj->visited == 0) {
                this->add_heuristic(adj->get_id(), end_node);
                new_cost = current->distance + current->get_weight(adj) + adj->heuristic; // new cost
                if (new_cost < adj->cost) {
                    adj->set_distance(current->distance + current->get_weight(adj));
                    adj->set_previous(current);
                }
                heap.insert(*adj);
            } else {
                continue;
            }
        }
    }

    return end->cost;
}

void Graph::route_planning_details(int start_node, int end_node) {
    Vertex* current;
    std::set<Vertex*> current_adjacent;
    std::set<double> heap;
    std::set<Vertex*> unvisited_queue;
    double pop;
    double new_cost;

    std::vector<double> longitude;
    std::vector<double> latitude;

    Vertex* start = this->get_vertex(start_node);
    Vertex* end = this->get_vertex(end_node);

    std::pair<int*, double*> res;

    start->set_distance(0.0);
    this->add_heuristic(start->get_id(), end_node);
    heap.insert(start->cost);
    unvisited_queue.insert(start);

    while (current != end) {
        pop = *(heap.begin());
        heap.erase(heap.begin());

        for (auto &element: unvisited_queue) {
            if ((*element).cost == pop) {
                current = element;
                current->set_visited();
                break;
            }
        }

        current_adjacent = current->get_connection();
        for (auto &adj: current_adjacent) {
            if (adj->visited == 0) {
                this->add_heuristic(adj->get_id(), end_node);
                unvisited_queue.insert(adj);

                new_cost = current->distance + current->get_weight(adj) + adj->heuristic;
                if (new_cost < adj->cost) {
                    adj->set_distance(current->distance + current->get_weight(adj));
                    adj->set_previous(current);
                }
                heap.insert(adj->cost);
            }
        }
        unvisited_queue.erase(current);
    }

    // Find the optimal path ...
    Vertex* cause = end;
    while (cause != start) {
        (longitude).push_back(cause->coord.first);
        (latitude).push_back(cause->coord.second);
        cause = cause->prev;
    }
    (longitude).push_back(start->coord.first);
    (latitude).push_back(start->coord.second);

    std::reverse((longitude).begin(), (longitude).end());
    std::reverse((latitude).begin(), (latitude).end());

    std::ofstream result;
    result.open("./result/optimal_path.csv");

    result << "longitude" << "," << "latitude" << "\n";
    for (int i = 0; i < longitude.size(); i++) {
        result << longitude[i] << "," << latitude[i] << "\n";
    }

    result.close();
}
