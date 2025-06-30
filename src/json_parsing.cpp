#include "json_parsing.hpp"
#include "request.hpp"
#include "vehicle.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <utility>
using json = nlohmann::json;

std::vector<Request> parseRequests(const json& requests_json) {
    std::vector<Request> requests;
    for (const auto& r_json : requests_json) {
        Request req;
        req.id = r_json.at("id").get<int>();
        req.origin = {r_json.at("pick")[0].get<double>(), r_json.at("pick")[1].get<double>()};
        req.destination = {r_json.at("drop")[0].get<double>(), r_json.at("drop")[1].get<double>()};
        req.releaseTime = r_json.at("release").get<int>();
        req.deadline = r_json.at("deadline").get<int>();
        req.payment = r_json.at("payment").get<double>();
        requests.push_back(req);
    }
    return requests;
}

std::vector<Vehicle> parseVehicles(const json& vehicles_json) {
    std::vector<Vehicle> vehicles;
    for (const auto& v_json : vehicles_json) {
        Vehicle veh;
        veh.id = v_json.at("id").get<int>();
        veh.location = {v_json.at("start")[0].get<double>(), v_json.at("start")[1].get<double>()};
        veh.capacity = v_json.at("capacity").get<int>();
        veh.assignedRequestIds = {};
        veh.currentRoutePath = {};
        vehicles.push_back(veh);
    }
    return vehicles;
}

std::pair<std::vector<Request>, std::vector<Vehicle>> readRequestsAndVehiclesFromJson(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open JSON file: " << filePath << std::endl;
        return {{}, {}};
    }

    json data;
    try {
        file >> data;
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parsing error in " << filePath << ": " << e.what() << std::endl;
        return {{}, {}};
    }

    std::vector<Request> requests;
    if (data.count("requests")) {
        requests = parseRequests(data.at("requests"));
    } else {
        std::cerr << "Warning: 'requests' array not found in JSON file." << std::endl;
    }

    std::vector<Vehicle> vehicles;
    if (data.count("vehicles")) {
        vehicles = parseVehicles(data.at("vehicles"));
    } else {
        std::cerr << "Warning: 'vehicles' array not found in JSON file." << std::endl;
    }

    return {requests, vehicles};
}