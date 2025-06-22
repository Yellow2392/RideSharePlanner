#include <iostream>
#include "request.hpp"
#include "vehicle.hpp"
#include "planner_gas.hpp"
#include "additive_tree.hpp"
#include "planner_gaso1.hpp"
#include "planner_gaso2.hpp"
#include "utils.hpp"

enum GASVariant {
    GAS,
    GAS_O1,
    GAS_O2
};

void benchmarkTest(int numRequests, int numVehicles) { //generacion aleatoria de vehiculos
    auto requests = generateRandomRequests(numRequests);
    auto vehicles = generateVehicles(numVehicles);

    std::cout << "=== Benchmark: " << numRequests << " requests, " << numVehicles << " vehicles ===\n";

    for (int variant = GAS; variant <= GAS_O2; ++variant) {
        std::vector<Request> rCopy = requests;
        std::vector<Vehicle> vCopy = vehicles;

        std::cout << ((variant == GAS)     ? "[GAS]" :
                      (variant == GAS_O1)  ? "[GAS-O1]" :
                      "[GAS-O2]") << "\n";

        switch (variant) {
            case GAS:    planRoutesGAS(rCopy, vCopy); break;
            case GAS_O1: planRoutesGASO1(rCopy, vCopy); break;
            case GAS_O2: planRoutesGASO2(rCopy, vCopy); break;
        }

        std::cout << std::endl;
    }
}

void runTestCase(const std::string& title, const std::vector<Request>& requestsInput, const std::vector<Vehicle>& vehiclesInput) {
    std::cout << "==== CASO: " << title << " ====\n";
    for (int variant = GAS; variant <= GAS_O2; ++variant) {
        std::vector<Request> requests = requestsInput;
        std::vector<Vehicle> vehicles = vehiclesInput;

        switch (variant) {
            case GAS:
                std::cout << "[GAS] -----------------------------\n";
                planRoutesGAS(requests, vehicles);
                break;
            case GAS_O1:
                std::cout << "[GAS-O1] (Add. Tree Global) ---\n";
                planRoutesGASO1(requests, vehicles);
                break;
            case GAS_O2:
                std::cout << "[GAS-O2] (Add. Tree x Vehicle) -\n";
                planRoutesGASO2(requests, vehicles);
                break;
        }
        std::cout << std::endl;
    }
}

void defineTestCases() {
    runTestCase("Caso 0 - Caso de paper",
        {
            {1, {1,1}, {4,7}, 0, 10, 6},
            {2, {1,2}, {4,6}, 0, 9, 5},
            {3, {2,3}, {2,6}, 0, 7, 4},
            {4, {5,3}, {2,4}, 0, 8, 3}
        },
        {
            {1, {0,0}, 3},
            {2, {0,0}, 1}
        });
    runTestCase("Caso 1 - Grupo optimo factible",
        {
            {1, {1,1}, {2,2}, 0, 20, 6},
            {2, {1,2}, {2,2}, 0, 20, 5},
            {3, {1,3}, {2,2}, 0, 20, 4},
            {4, {5,5}, {6,6}, 0, 20, 10}
        },
        {
            {1, {0,0}, 3},
            {2, {0,0}, 3}
        });

    runTestCase("Caso 2 - Deadline limita combinaciones",
        {
            {1, {1,1}, {2,2}, 0, 8, 6},
            {2, {1,2}, {2,2}, 0, 9, 5},
            {3, {1,3}, {2,2}, 0, 10, 4},
            {4, {10,10}, {11,11}, 0, 6, 10}
        },
        {
            {1, {0,0}, 3},
            {2, {0,0}, 3}
        });

    runTestCase("Caso 3 - Slack apretado",
        {
            {1, {1,1}, {10,10}, 0, 25, 15},
            {2, {1,2}, {2,2}, 0, 5, 5},
            {3, {3,3}, {4,4}, 0, 30, 5}
        },
        {
            {1, {0,0}, 2},
            {2, {0,0}, 2}
        });

    runTestCase("Caso 4 - Release time futuro",
        {
            {1, {1,1}, {2,2}, 5, 20, 6},
            {2, {1,2}, {2,2}, 0, 15, 5},
            {3, {1,3}, {2,2}, 0, 15, 4}
        },
        {
            {1, {0,0}, 3}
        });
}

int main() {
    std::cout<<"!=== Testing Algorithms ===!"<<std::endl;
    //defineTestCases();

    benchmarkTest(30, 5);    // mediano
    benchmarkTest(100, 20);  // grande
    //benchmarkTest(300, 50);  // estrés
    return 0;
}
