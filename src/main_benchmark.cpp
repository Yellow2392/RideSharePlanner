#include <iostream>
#include <vector>
#include <string>
#include "benchmark_suite.hpp"

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [option]\n";
    std::cout << "Options:\n";
    std::cout << "  --all          Run complete benchmark suite\n";
    std::cout << "  --requests     Benchmark request variation\n";
    std::cout << "  --vehicles     Benchmark vehicle variation\n";
    std::cout << "  --capacity     Benchmark capacity variation\n";
    std::cout << "  --deadline     Benchmark deadline variation\n";
    std::cout << "  --quick        Run quick benchmark (smaller scale)\n";
    std::cout << "  --help         Show this help message\n";
}

void runQuickBenchmark(BenchmarkSuite& suite) {
    std::cout << "Running Quick Benchmark Suite..." << std::endl;
    
    // Parámetros para pruebas rápidas
    suite.benchmarkRequestVariation({20, 40, 60, 80}, 10, 3, 900, 2);
    suite.benchmarkVehicleVariation({5, 10, 15, 20}, 50, 3, 900, 2);
    suite.benchmarkCapacityVariation({2, 3, 4}, 50, 10, 900, 2);
    suite.benchmarkDeadlineVariation({600, 900, 1200}, 50, 10, 3, 2);
    
    suite.exportResults("quick_benchmark_results.csv");
}

void runFullScaleBenchmark(BenchmarkSuite& suite) {
    std::cout << "Running Full Scale Benchmark Suite..." << std::endl;
    
    // Parámetros del paper (escalados)
    suite.benchmarkRequestVariation({50, 100, 150, 200, 250, 300}, 25, 3, 900, 1);
    suite.benchmarkVehicleVariation({10, 20, 30, 40, 50}, 150, 3, 900, 1);
    suite.benchmarkCapacityVariation({2, 3, 4, 5, 6}, 150, 25, 900, 1);
    suite.benchmarkDeadlineVariation({600, 750, 900, 1050, 1200, 1350}, 150, 25, 3, 1);
    
    suite.exportResults("full_benchmark_results.csv");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string option = argv[1];
    BenchmarkSuite suite("benchmark_results");
    
    try {
        if (option == "--help") {
            printUsage(argv[0]);
            return 0;
        }
        else if (option == "--all") {
            suite.runCompleteBenchmarkSuite();
        }
        else if (option == "--quick") {
            runQuickBenchmark(suite);
        }
        else if (option == "--full") {
            runFullScaleBenchmark(suite);
        }
        else if (option == "--requests") {
            std::cout << "Running Request Variation Benchmark..." << std::endl;
            suite.benchmarkRequestVariation({50, 75, 100, 125, 150, 175, 200, 250}, 20, 3, 900, 5);
            suite.exportResults("request_variation_results.csv");
        }
        else if (option == "--vehicles") {
            std::cout << "Running Vehicle Variation Benchmark..." << std::endl;
            suite.benchmarkVehicleVariation({5, 10, 15, 20, 25, 30, 35, 40}, 150, 3, 900, 5);
            suite.exportResults("vehicle_variation_results.csv");
        }
        else if (option == "--capacity") {
            std::cout << "Running Capacity Variation Benchmark..." << std::endl;
            suite.benchmarkCapacityVariation({2, 3, 4, 5, 6}, 150, 20, 900, 5);
            suite.exportResults("capacity_variation_results.csv");
        }
        else if (option == "--deadline") {
            std::cout << "Running Deadline Variation Benchmark..." << std::endl;
            suite.benchmarkDeadlineVariation({450, 600, 750, 900, 1050, 1200, 1350}, 150, 20, 3, 5);
            suite.exportResults("deadline_variation_results.csv");
        }
        else {
            std::cout << "Unknown option: " << option << std::endl;
            printUsage(argv[0]);
            return 1;
        }
        
        std::cout << "\nBenchmark completed successfully!" << std::endl;
        std::cout << "Results saved in: benchmark_results/" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error during benchmark execution: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
