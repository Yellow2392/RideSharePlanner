#include <iostream>
#include "additive_tree.hpp"
#include "planner_gas.hpp"
#include "planner_gaso1.hpp"
#include "planner_gaso2.hpp"
#include "request.hpp"
#include "vehicle.hpp"
#include "utils.hpp"
#include "../external/json.hpp"   

using json = nlohmann::json;

std::vector<Request> parseRequests(const json& jr) {
    std::vector<Request> R;
    for (auto& r : jr) {
        R.push_back({ r["id"], {r["pick"][0], r["pick"][1]},
                      {r["drop"][0], r["drop"][1]},
                      r["release"], r["deadline"], r.value("payment", 0.0) });
    }
    return R;
}

std::vector<Vehicle> parseVehicles(const json& jv) {
    std::vector<Vehicle> V;
    for (auto& v : jv) {
        V.push_back({ v["id"], {v["start"][0], v["start"][1]}, v["capacity"] });
    }
    return V;
}

int main() {
    json payload;
    std::cin >> payload;

    auto requests = parseRequests(payload["requests"]);
    auto vehicles = parseVehicles(payload["vehicles"]);

    enum { GAS, GAS_O1, GAS_O2 };
    for (int alg = GAS; alg <= GAS_O2; ++alg) {
        auto R = requests;
        std::vector<Vehicle> V = vehicles;
        std::cout << (alg==GAS?"[GAS]\n":alg==GAS_O1?"[GAS-O1]\n":"[GAS-O2]\n");
        if (alg==GAS)      planRoutesGAS(R,V);
        else if (alg==GAS_O1) planRoutesGASO1(R,V);
        else               planRoutesGASO2(R,V);
        std::cout << '\n';
    }
    return 0;
}
