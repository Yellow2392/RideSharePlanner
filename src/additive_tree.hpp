#ifndef ADDITIVE_TREE_HPP
#define ADDITIVE_TREE_HPP

#include <vector>
#include <set>
#include "request.hpp"
#include "vehicle.hpp"

struct TreeNode {
    std::set<int> requestIds;  // IDs de los requests en este nodo
    double profit = 0.0; // suma de pagos de los requests
    std::vector<TreeNode*> children;  // hijos
    TreeNode* parent = nullptr;  // nodo padre
};

class AdditiveTree {
public:
    TreeNode* root;
    std::vector<Request> allRequests;
    Vehicle vehicleContext;

    AdditiveTree(const std::vector<Request>& requests, int maxCapacity);
    AdditiveTree(const std::vector<Request>& requests, int maxCapacity, const Vehicle& v); //para cada vehículo
    ~AdditiveTree();

    std::vector<TreeNode*> getAllNodes();  
    TreeNode* findMostProfitableGroupForVehicle(const Vehicle& v);

private:
    void build(int maxCapacity);
    void destroy(TreeNode* node);
    double calculateProfit(const std::set<int>& requestIds);
    //bool isFeasible(const std::set<int>& requestIds);
    bool isFeasible(const std::set<int>& requestIds, const Vehicle& v);
};

#endif
