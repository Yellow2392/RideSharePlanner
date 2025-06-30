#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <utility>
#include <random>

inline double euclideanDistance(const std::pair<double, double>& a,
                                const std::pair<double, double>& b) {
    return std::hypot(a.first - b.first, a.second - b.second);
}

inline double calculateMinSlack(const Vehicle& v, const std::vector<Request>& group) {
    double currentTime = 0;
    auto lastLoc = v.location;
    double minSlack = 1e9;

    for (const auto& r : group) {
        currentTime += euclideanDistance(lastLoc, r.origin);
        if (currentTime < r.releaseTime) {
            currentTime = r.releaseTime;
        }

        currentTime += euclideanDistance(r.origin, r.destination);

        double slack = r.deadline - currentTime;
        if (slack < 0) return -1; //violación de tiempo
        minSlack = std::min(minSlack, slack);

        lastLoc = r.destination;
    }

    return minSlack;
}
/*
//? generacion de requests aleatoria para testear
inline std::vector<Request> generateRandomRequests(int n, int maxCoord = 50, int maxTime = 100, int maxPayment = 10) {
    std::vector<Request> requests;
    std::mt19937 rng(42);  // semilla fija para reproducibilidad
    std::uniform_int_distribution<int> coordDist(0, maxCoord);
    std::uniform_int_distribution<int> releaseDist(0, maxTime / 2);
    std::uniform_int_distribution<int> slackDist(10, 40);
    std::uniform_int_distribution<int> payDist(1, maxPayment);

    for (int i = 1; i <= n; ++i) {
        int ox = coordDist(rng), oy = coordDist(rng);
        int dx = coordDist(rng), dy = coordDist(rng);
        int release = releaseDist(rng);
        int slack = slackDist(rng);
        int deadline = release + slack;
        double payment = payDist(rng);

        requests.push_back({i, {ox, oy}, {dx, dy}, release, deadline, payment});
    }
    return requests;
}

inline std::vector<Vehicle> generateVehicles(int m, int capacity = 3, int maxCoord = 10) {
    std::vector<Vehicle> vehicles;
    std::mt19937 rng(7);
    std::uniform_int_distribution<int> coordDist(0, maxCoord);

    for (int i = 1; i <= m; ++i) {
        int x = coordDist(rng);
        int y = coordDist(rng);
        vehicles.push_back({i, {x, y}, capacity});
    }
    return vehicles;
}
*/
#endif
