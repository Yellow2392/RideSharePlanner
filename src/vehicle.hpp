#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <utility>
#include <vector>
#include "request.hpp"

struct Vehicle {
    int id;
    std::pair<double, double> location;
    int capacity;
    std::vector<int> assignedRequestIds;
};

#endif
