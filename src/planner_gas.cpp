#include "planner_gas.hpp"
#include "utils.hpp"
#include <algorithm>
#include <set>
#include <iostream>

void generateCombinations(const std::vector<Request>& requests,
                          int k,
                          std::vector<std::vector<Request>>& output,  //!combina todas las posibles agrupaciones de k solicitudes
                          std::vector<Request> current = {},
                          int index = 0) {
    if (current.size() == k) {
        output.push_back(current);
        return;
    }
    for (size_t i = index; i < requests.size(); i++) {
        current.push_back(requests[i]);
        generateCombinations(requests, k, output, current, i + 1);
        current.pop_back();
    }
}

bool isFeasible(const std::vector<Request>& group, const Vehicle& v) {
    if (group.size() > (size_t)v.capacity) return false;
    return calculateMinSlack(v, group) >= 1.0;  //slack mínimo requerido
}

void planRoutesGAS(std::vector<Request>& requests, std::vector<Vehicle>& vehicles) {
    std::set<int> assignedRequests;
    std::vector<std::vector<Request>> allGroups;

    int maxCap = 0;
    for (const auto& v : vehicles) {
        maxCap = std::max(maxCap, v.capacity);
    }

    // Fase 1: Generar todos los grupos viables
    for (int k = 1; k <= maxCap; k++) {
        std::vector<std::vector<Request>> combinations;
        generateCombinations(requests, k, combinations);
        for (auto& group : combinations) {
            allGroups.push_back(group);
        }
    }

    // Fase 2: Asignación iterativa por vehículo
    for (auto& v : vehicles) {
        double maxProfit = -1;
        std::vector<Request> bestGroup;

        for (auto& group : allGroups) {
            bool valid = true;
            for (auto& r : group) {
                if (assignedRequests.count(r.id)) {
                    valid = false;
                    break;
                }
            }
            if (!valid || !isFeasible(group, v)) continue;

            double profit = 0;
            for (auto& r : group) profit += r.payment;
            if (profit > maxProfit) {
                maxProfit = profit;
                bestGroup = group;
            }
        }

        for (auto& r : bestGroup) {
            v.assignedRequestIds.push_back(r.id);
            assignedRequests.insert(r.id);
        }

        if (!bestGroup.empty()) {
            std::cout << "Vehicle " << v.id << " assigned requests: ";
            for (auto& r : bestGroup) std::cout << r.id << " ";
            std::cout << " | Total Payment: " << maxProfit << "\n";

            /// Poblando currentRoutePath (PARA GUI)
            v.currentRoutePath.clear();
            v.currentRoutePath.push_back(v.location);

            for (const auto& r : bestGroup) {
                v.currentRoutePath.push_back(r.origin);
                v.currentRoutePath.push_back(r.destination);
            }
        }
    }
}
