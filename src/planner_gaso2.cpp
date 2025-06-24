#include "planner_gaso2.hpp"
#include "additive_tree.hpp"
#include "utils.hpp"
#include <iostream>
#include <random>
#include <set>
#include <algorithm>

std::vector<Request> filterFeasibleRequests(const Vehicle& v, const std::vector<Request>& requests) {
    std::vector<Request> result;
    for (const auto& r : requests) {
        double distance = euclideanDistance(v.location, r.origin);
        if (distance <= (r.deadline - r.releaseTime)) {
            result.push_back(r);
        }
    }
    return result;
}

void planRoutesGASO2(std::vector<Request>& requests, std::vector<Vehicle>& vehicles) {
    std::set<int> assignedRequestIds;
    std::random_device rd; //orden aleatorio de vehiculos
    std::mt19937 g(rd());
    std::shuffle(vehicles.begin(), vehicles.end(), g);

    for (auto& vehicle : vehicles) {
        std::vector<Request> unassigned;
        for (const auto& r : requests) {
            if (assignedRequestIds.count(r.id) == 0) {
                unassigned.push_back(r);
            }
        }

        std::vector<Request> feasible = filterFeasibleRequests(vehicle, unassigned);
        if (feasible.empty()) continue;

        AdditiveTree localTree(feasible, vehicle.capacity, vehicle); //construir add.tree solo con estas solicitudes

        TreeNode* best = nullptr;
        double maxProfit = -1;

        for (TreeNode* node : localTree.getAllNodes()) {
            if (node->requestIds.size() > (size_t)vehicle.capacity) continue;

            bool overlap = false;
            for (int id : node->requestIds) {
                if (assignedRequestIds.count(id)) {
                    overlap = true;
                    break;
                }
            }
            if (overlap) continue;

            std::vector<Request> group;
            for (int id : node->requestIds) {
                for (const auto& r : requests) {
                    if (r.id == id) {
                        group.push_back(r);
                        break;
                    }
                }
            }

            if (calculateMinSlack(vehicle, group) < 1.0) continue; // restriccion de min slack time

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
            for (int id : best->requestIds) std::cout << id << " ";
            std::cout << "| Total Payment: " << maxProfit << "\n";
        }
    }
}
