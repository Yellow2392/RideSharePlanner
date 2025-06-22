#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <utility>

struct Request {
    int id;
    std::pair<double, double> origin;
    std::pair<double, double> destination;
    int releaseTime;
    int deadline;
    double payment;
};

#endif
