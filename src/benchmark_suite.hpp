#ifndef BENCHMARK_SUITE_HPP
#define BENCHMARK_SUITE_HPP

#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include "request.hpp"
#include "vehicle.hpp"
#include "planner_gas.hpp"
#include "planner_gaso1.hpp"
#include "planner_gaso2.hpp"
#include "utils.hpp"

struct BenchmarkResult {
    std::string algorithm;
    int parameter_value;
    double total_revenue;
    double execution_time_ms;
    double memory_usage_mb;
    int requests_served;
    int total_requests;
    int total_vehicles;
    std::string parameter_type;
};

class BenchmarkSuite {
private:
    std::vector<BenchmarkResult> results;
    std::string output_directory;
    
    // Medición de tiempo
    std::chrono::high_resolution_clock::time_point start_time;
    
    void startTimer() {
        start_time = std::chrono::high_resolution_clock::now();
    }
    
    double stopTimer() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        return duration.count() / 1000.0; // Convertir a milisegundos
    }
    
    double estimateMemoryUsage(int num_requests, int num_vehicles, int capacity) {
        double request_memory = num_requests * sizeof(Request) / (1024.0 * 1024.0);
        double vehicle_memory = num_vehicles * sizeof(Vehicle) / (1024.0 * 1024.0);
        double tree_memory = 0.0;
        
        if (capacity <= 3) {
            tree_memory = (num_requests + num_requests * (num_requests - 1) / 2) * 64.0 / (1024.0 * 1024.0);
        }
        
        return request_memory + vehicle_memory + tree_memory;
    }
    
    int countServedRequests(const std::vector<Vehicle>& vehicles) {
        int count = 0;
        for (const auto& v : vehicles) {
            count += v.assignedRequestIds.size();
        }
        return count;
    }
    
    double calculateTotalRevenue(const std::vector<Request>& requests, 
                               const std::vector<Vehicle>& vehicles) {
        double total = 0.0;
        for (const auto& v : vehicles) {
            for (int req_id : v.assignedRequestIds) {
                for (const auto& r : requests) {
                    if (r.id == req_id) {
                        total += r.payment;
                        break;
                    }
                }
            }
        }
        return total;
    }

public:
    BenchmarkSuite(const std::string& output_dir = "benchmark_results") 
        : output_directory(output_dir) {}
    
    // Benchmark 1: Requests (m)
    void benchmarkRequestVariation(const std::vector<int>& request_counts, 
                                  int fixed_vehicles = 20, 
                                  int fixed_capacity = 3,
                                  int fixed_deadline = 900,
                                  int iterations = 5) {
        
        std::cout << "=== Benchmark: Request Variation ===" << std::endl;
        
        for (int num_requests : request_counts) {
            std::cout << "Testing with " << num_requests << " requests..." << std::endl;
            
            for (int iter = 0; iter < iterations; iter++) {
                auto requests = generateRandomRequests(num_requests, 50, 100, 10);
                auto vehicles = generateVehicles(fixed_vehicles, fixed_capacity, 10);
                
                // Ajustar deadlines
                for (auto& r : requests) {
                    r.deadline = r.releaseTime + fixed_deadline;
                }
                
                runAlgorithmSuite(requests, vehicles, num_requests, "requests");
            }
        }
    }
    
    // Benchmark 2: Vehiculos (n) 
    void benchmarkVehicleVariation(const std::vector<int>& vehicle_counts,
                                  int fixed_requests = 100,
                                  int fixed_capacity = 3,
                                  int fixed_deadline = 900,
                                  int iterations = 5) {
        
        std::cout << "=== Benchmark: Vehicle Variation ===" << std::endl;
        
        for (int num_vehicles : vehicle_counts) {
            std::cout << "Testing with " << num_vehicles << " vehicles..." << std::endl;
            
            for (int iter = 0; iter < iterations; iter++) {
                auto requests = generateRandomRequests(fixed_requests, 50, 100, 10);
                auto vehicles = generateVehicles(num_vehicles, fixed_capacity, 10);
                
                // Ajustar deadlines
                for (auto& r : requests) {
                    r.deadline = r.releaseTime + fixed_deadline;
                }
                
                runAlgorithmSuite(requests, vehicles, num_vehicles, "vehicles");
            }
        }
    }
    
    // Benchmark 3: Capacidad
    void benchmarkCapacityVariation(const std::vector<int>& capacities,
                                   int fixed_requests = 100,
                                   int fixed_vehicles = 20,
                                   int fixed_deadline = 900,
                                   int iterations = 5) {
        
        std::cout << "=== Benchmark: Capacity Variation ===" << std::endl;
        
        for (int capacity : capacities) {
            std::cout << "Testing with capacity " << capacity << "..." << std::endl;
            
            for (int iter = 0; iter < iterations; iter++) {
                auto requests = generateRandomRequests(fixed_requests, 50, 100, 10);
                auto vehicles = generateVehicles(fixed_vehicles, capacity, 10);
                
                // Ajustar deadlines
                for (auto& r : requests) {
                    r.deadline = r.releaseTime + fixed_deadline;
                }
                
                runAlgorithmSuite(requests, vehicles, capacity, "capacity");
            }
        }
    }
    
    // Benchmark 4: Deadline
    void benchmarkDeadlineVariation(const std::vector<int>& deadlines,
                                   int fixed_requests = 100,
                                   int fixed_vehicles = 20,
                                   int fixed_capacity = 3,
                                   int iterations = 5) {
        
        std::cout << "=== Benchmark: Deadline Variation ===" << std::endl;
        
        for (int deadline : deadlines) {
            std::cout << "Testing with deadline " << deadline << "..." << std::endl;
            
            for (int iter = 0; iter < iterations; iter++) {
                auto requests = generateRandomRequests(fixed_requests, 50, 100, 10);
                auto vehicles = generateVehicles(fixed_vehicles, fixed_capacity, 10);
                
                // Ajustar deadlines
                for (auto& r : requests) {
                    r.deadline = r.releaseTime + deadline;
                }
                
                runAlgorithmSuite(requests, vehicles, deadline, "deadline");
            }
        }
    }
    
    void runAlgorithmSuite(std::vector<Request> requests, 
                          std::vector<Vehicle> vehicles,
                          int parameter_value,
                          const std::string& parameter_type) {
        
        // Test GAS
        {
            auto req_copy = requests;
            auto veh_copy = vehicles;
            startTimer();
            planRoutesGAS(req_copy, veh_copy);
            double time_ms = stopTimer();
            
            BenchmarkResult result;
            result.algorithm = "GAS";
            result.parameter_value = parameter_value;
            result.parameter_type = parameter_type;
            result.total_revenue = calculateTotalRevenue(requests, veh_copy);
            result.execution_time_ms = time_ms;
            result.memory_usage_mb = estimateMemoryUsage(requests.size(), vehicles.size(), vehicles[0].capacity);
            result.requests_served = countServedRequests(veh_copy);
            result.total_requests = requests.size();
            result.total_vehicles = vehicles.size();
            
            results.push_back(result);
        }
        
        // Test GAS-O1
        {
            auto req_copy = requests;
            auto veh_copy = vehicles;
            startTimer();
            planRoutesGASO1(req_copy, veh_copy);
            double time_ms = stopTimer();
            
            BenchmarkResult result;
            result.algorithm = "GAS-O1";
            result.parameter_value = parameter_value;
            result.parameter_type = parameter_type;
            result.total_revenue = calculateTotalRevenue(requests, veh_copy);
            result.execution_time_ms = time_ms;
            result.memory_usage_mb = estimateMemoryUsage(requests.size(), vehicles.size(), vehicles[0].capacity);
            result.requests_served = countServedRequests(veh_copy);
            result.total_requests = requests.size();
            result.total_vehicles = vehicles.size();
            
            results.push_back(result);
        }
        
        // Test GAS-O2
        {
            auto req_copy = requests;
            auto veh_copy = vehicles;
            startTimer();
            planRoutesGASO2(req_copy, veh_copy);
            double time_ms = stopTimer();
            
            BenchmarkResult result;
            result.algorithm = "GAS-O2";
            result.parameter_value = parameter_value;
            result.parameter_type = parameter_type;
            result.total_revenue = calculateTotalRevenue(requests, veh_copy);
            result.execution_time_ms = time_ms;
            result.memory_usage_mb = estimateMemoryUsage(requests.size(), vehicles.size(), vehicles[0].capacity);
            result.requests_served = countServedRequests(veh_copy);
            result.total_requests = requests.size();
            result.total_vehicles = vehicles.size();
            
            results.push_back(result);
        }
    }
    
    // Exportar resultados a CSV
    void exportResults(const std::string& filename = "benchmark_results.csv") {
        std::ofstream file(output_directory + "/" + filename);
        
        file << "algorithm,parameter_type,parameter_value,total_revenue,execution_time_ms,"
             << "memory_usage_mb,requests_served,total_requests,total_vehicles,"
             << "service_rate,revenue_per_request\n";
        
        for (const auto& result : results) {
            double service_rate = static_cast<double>(result.requests_served) / result.total_requests;
            double revenue_per_request = result.total_revenue / result.total_requests;
            
            file << result.algorithm << ","
                 << result.parameter_type << ","
                 << result.parameter_value << ","
                 << std::fixed << std::setprecision(2) << result.total_revenue << ","
                 << std::fixed << std::setprecision(3) << result.execution_time_ms << ","
                 << std::fixed << std::setprecision(3) << result.memory_usage_mb << ","
                 << result.requests_served << ","
                 << result.total_requests << ","
                 << result.total_vehicles << ","
                 << std::fixed << std::setprecision(4) << service_rate << ","
                 << std::fixed << std::setprecision(2) << revenue_per_request << "\n";
        }
        
        file.close();
        std::cout << "Results exported to: " << output_directory + "/" + filename << std::endl;
    }
    
    // Ejecutar suite completo de benchmarks
    void runCompleteBenchmarkSuite() {
        std::cout << "Starting Complete Benchmark Suite..." << std::endl;
        
        system(("mkdir -p " + output_directory).c_str());
        
        // Benchmark 1: Request 
        benchmarkRequestVariation({50, 75, 100, 125, 150, 175, 200}, 20, 3, 900, 3);
        
        // Benchmark 2: Vehicle 
        benchmarkVehicleVariation({5, 10, 15, 20, 25, 30, 35}, 100, 3, 900, 3);
        
        // Benchmark 3: Capacity 
        benchmarkCapacityVariation({2, 3, 4, 5}, 100, 20, 900, 3);
        
        // Benchmark 4: Deadline
        benchmarkDeadlineVariation({600, 750, 900, 1050, 1200}, 100, 20, 3, 3);
        
        exportResults();
        
        std::cout << "Complete Benchmark Suite finished!" << std::endl;
        std::cout << "Total experiments conducted: " << results.size() << std::endl;
    }
    
    void clearResults() {
        results.clear();
    }
    
    const std::vector<BenchmarkResult>& getResults() const {
        return results;
    }
};

#endif
