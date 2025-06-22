#include "additive_tree.hpp"
#include "utils.hpp"
#include <iostream>
#include <algorithm>

AdditiveTree::AdditiveTree(const std::vector<Request>& requests, int maxCapacity)
    : allRequests(requests) {
    root = new TreeNode(); 
    build(maxCapacity);
}

AdditiveTree::AdditiveTree(const std::vector<Request>& requests, int maxCapacity, const Vehicle& v)
    : allRequests(requests), vehicleContext(v) {
    root = new TreeNode();
    build(maxCapacity);
}

AdditiveTree::~AdditiveTree() {
    destroy(root);
}

void AdditiveTree::destroy(TreeNode* node) {
    for (auto child : node->children) {
        destroy(child);
    }
    delete node;
}

double AdditiveTree::calculateProfit(const std::set<int>& requestIds) {
    double total = 0.0;
    for (int id : requestIds) {
        for (const auto& r : allRequests) {
            if (r.id == id) {
                total += r.payment;
                break;
            }
        }
    }
    return total;
}

bool AdditiveTree::isFeasible(const std::set<int>& requestIds, const Vehicle& v) {
    if (requestIds.size() > (size_t)v.capacity) return false;

    std::vector<Request> group;
    for (int id : requestIds) {
        for (const auto& r : allRequests) {
            if (r.id == id) {
                group.push_back(r);
                break;
            }
        }
    }

    return calculateMinSlack(v, group) >= 1.0;
}

void AdditiveTree::build(int maxCapacity) {
    std::vector<TreeNode*> currentLevel;
    for (const auto& r : allRequests) {
        auto node = new TreeNode();
        node->requestIds.insert(r.id);
        node->profit = r.payment;
        node->parent = root;
        root->children.push_back(node);
        currentLevel.push_back(node);
    }

    for (int level = 2; level <= maxCapacity; level++) {
        std::vector<TreeNode*> nextLevel;
        for (size_t i = 0; i < currentLevel.size(); i++) {
            for (size_t j = i + 1; j < currentLevel.size(); j++) {
                std::set<int> combined;
                std::set_union(currentLevel[i]->requestIds.begin(), currentLevel[i]->requestIds.end(),
                               currentLevel[j]->requestIds.begin(), currentLevel[j]->requestIds.end(),
                               std::inserter(combined, combined.begin()));
                if (combined.size() == level && isFeasible(combined, vehicleContext)) {
                    auto node = new TreeNode();
                    node->requestIds = combined;
                    node->profit = calculateProfit(combined);
                    node->parent = currentLevel[i];
                    currentLevel[i]->children.push_back(node);
                    nextLevel.push_back(node);
                }
            }
        }
        currentLevel = nextLevel;
    }
}

std::vector<TreeNode*> AdditiveTree::getAllNodes() {
    std::vector<TreeNode*> nodes;
    std::vector<TreeNode*> stack = {root};
    while (!stack.empty()) {
        TreeNode* node = stack.back();
        stack.pop_back();
        nodes.push_back(node);
        for (auto* child : node->children) {
            stack.push_back(child);
        }
    }
    return nodes;
}

TreeNode* AdditiveTree::findMostProfitableGroupForVehicle(const Vehicle& v) {
    TreeNode* best = nullptr;
    double maxProfit = -1;

    for (TreeNode* node : getAllNodes()) {
        if (node->requestIds.size() <= static_cast<size_t>(v.capacity)) {
            if (node->profit > maxProfit) {
                maxProfit = node->profit;
                best = node;
            }
        }
    }

    return best;
}
