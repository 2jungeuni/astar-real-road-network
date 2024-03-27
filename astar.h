//
// Created by jelee on 23. 10. 29.
//

#ifndef ROUTING_ASTAR_H
#define ROUTING_ASTAR_H

#include <set>
#include <map>
#include <random>
#include <iostream>
#include <algorithm>

// information of nodes
class Vertex {
public:
    bool visited;
    int id;
    std::pair<double, double> coord;
    std::map<Vertex*, double> adjacent; // neighbors
    double distance;                    // g_hat(n)
    double heuristic;                   // f_hat(n)
    double cost;                        // g_hat(n) + f_hat(n)
    Vertex* prev;

    // constructor
    Vertex(int id, std::pair<double, double> coord);

    // initialize
    void initialize();

    // GET
    int get_id();
    std::set<Vertex*> get_connection();
    double get_weight(Vertex* neighbor);

    // ADD
    void add_neighbor(Vertex* neighbor, double weight=0);   // add a neighbor

    // SET
    void set_distance(double dist);
    void set_cost();
    void set_previous(Vertex* prev);
    void set_visited();

    // <
    bool operator<(const Vertex& v) const {
        if (cost == v.cost) {
            return id < v.id;
        }
        return cost < v.cost;
    };

    // <<
    friend std::ostream& operator<<(std::ostream& o, Vertex& v){
        o << "Node ID: " << v.id << " / (x,y): " << "(" << v.coord.first << "," << v.coord.second << ")" << " / Cost: " << v.cost;
        return o;
    };
};

// road network
class Graph {
public:
    std::map<int, Vertex*> graph;
    double max_speed_limit = 22.352;
    double min_carbon_emission = -3.609;

    // constructor
    Graph(double alpha, double beta, double gamma) : alpha(alpha), beta(beta), gamma(gamma) {};

    // initialize
    void initialize();

    // GET
    std::pair<double, double> get_coordinate(int node_id);
    Vertex* get_vertex(int id);

    // ADD
    void add_vertex(int node_id, double longitude, double latitude);
    void add_edge(int frm, int to, double weight);
    void add_heuristic(int node, int end_node);

    // Heuristic functions
    double haversine(std::pair<double, double> frm_coord, std::pair<double, double> to_coord);
    double heu_travel_time(std::pair<double, double> frm_coord, std::pair<double, double> to_coord);
    double heu_carbon_emission(std::pair<double, double> frm_coord, std::pair<double, double> to_coord);
    double heuristic(std::pair<double, double> frm_coord, std::pair<double, double> to_coord);

    double route_planning(int start_node, int end_node);
    void route_planning_details(int start_node, int end_node);

    ~Graph() {};

private:
    double alpha = 0.0;
    double beta = 0.0;
    double gamma = 0.0;
};

#endif //ROUTING_ASTAR_H
