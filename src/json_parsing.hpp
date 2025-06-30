#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include <vector>
#include <string>
#include <utility>
#include <nlohmann/json.hpp>

struct Request;
struct Vehicle;

std::vector<Request> parseRequests(const nlohmann::json& requests_json);
std::vector<Vehicle> parseVehicles(const nlohmann::json& vehicles_json);
std::pair<std::vector<Request>, std::vector<Vehicle>> readRequestsAndVehiclesFromJson(const std::string& filePath);

#endif //JSON_PARSER_HPP