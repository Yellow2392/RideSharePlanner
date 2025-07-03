#include "planner_gaso1.hpp"
#include "additive_tree.hpp"
#include "utils.hpp"
#include <set>
#include <iostream>

void planRoutesGASO1(std::vector<Request>& requests, std::vector<Vehicle>& vehicles) {
    int maxCap = 0;
    for (const auto& v : vehicles) {
        maxCap = std::max(maxCap, v.capacity);
    }

    AdditiveTree tree(requests, maxCap);  //arbol global.
    //! debe ocupar la máxima capacidad que hay entre los vehiculos si va a ser arbol general

    std::set<int> assignedRequestIds;

    for (auto& vehicle : vehicles) {
        TreeNode* best = nullptr;
        double maxProfit = -1.0;

        for (TreeNode* node : tree.getAllNodes()) {
            if (node->requestIds.size() > (size_t)vehicle.capacity) continue;
            
            bool overlap = false; //verifica overlap entre asignaciones
            std::vector<Request> group;
            for (int id : node->requestIds) {
                if (assignedRequestIds.count(id)) {
                    overlap = true;
                    break;
                }
                for (const auto& r : requests) {
                    if (r.id == id) {
                        group.push_back(r);
                        break;
                    }
                }
            }
            if (overlap) continue;

            if (calculateMinSlack(vehicle, group) < 1.0) continue;

            if (node->profit > maxProfit) {
                maxProfit = node->profit;
                best = node;
            }
        }

        if (best) {
            for (int id : best->requestIds) {
                assignedRequestIds.insert(id);
                vehicle.assignedRequestIds.push_back(id);
            }

            std::cout << "Vehicle " << vehicle.id << " assigned requests: ";
            for (int id : best->requestIds){
                std::cout << id << " ";
            } 
            std::cout << "| Total Payment: " << maxProfit << "\n";
        }
    }
}