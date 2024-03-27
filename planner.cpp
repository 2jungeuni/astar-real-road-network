#include <pybind11/pybind11.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "astar.h"

// find an optimal path
class RoutingPlanner {
public:
    Graph *pGraph;

    RoutingPlanner(double alpha, double beta, double gamma) {
        pGraph = new Graph(alpha, beta, gamma);

        // get graph information
        // file pointer
        std::fstream fnode("./data/nodes_N96570.csv", std::ios::in);
        std::fstream flink("./data/links_L260855.csv", std::ios::in);

        // read the data from the file as string vector
        std::vector<std::string> row;
        std::string line, word;

        if (fnode.is_open()) {
            std::getline(fnode, line);

            while (std::getline(fnode, line)) {
                row.clear();
                std::stringstream str(line);
                while (std::getline(str, word, ',')) {
                    row.push_back(word);
                }
                pGraph->add_vertex(std::stoi(row[1]), std::stod(row[2]), std::stod(row[3]));
            }
        }

        if(flink.is_open()) {
            std::getline(flink, line);

            while (std::getline(flink, line)) {
                row.clear();
                std::stringstream str(line);
                while (std::getline(str, word, ',')) {
                    row.push_back(word);
                }
                // alpha * distance + beta * travel_time + gamma * carbon_emission
                // 7-th column: street length, 8-th column: travel time, 9-th column: speed limit, 10-th column: elevation
                pGraph->add_edge(std::stoi(row[1]), std::stoi(row[2]),
                                 alpha * std::stod(row[7]) + beta * std::stod(row[8])
                                 + gamma * std::stod(row[11]) * std::stod(row[7]));
            }
        }
    }

    double astar(int frm, int to) {
        return this->pGraph->route_planning(frm, to);
    }

    void astar_path(int frm, int to) {
        return this->pGraph->route_planning_details(frm, to);
    }

    // Destructor
    ~RoutingPlanner() {};

};

namespace py = pybind11;

PYBIND11_MODULE(planner, m) {
    py::class_<RoutingPlanner>(m, "RoutingPlanner")
            .def(py::init<int, int, int> ())
            .def("astar", &RoutingPlanner::astar, "get the cost of the optimal cost", py::arg("frm"), py::arg("to"))
            .def("astar_path", &RoutingPlanner::astar_path, "get the coordinates of the optimal path in the result directory", py::arg("frm"), py::arg("to"));
}