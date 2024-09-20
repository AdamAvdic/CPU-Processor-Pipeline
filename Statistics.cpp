#include "classes.h"
#include <iostream>
#include <iomanip>
#include <cmath>

// Function to update counters based on instruction Inst_type
void Statistics::Instruction_counter(Instruction_class instruction) {
    // Increment the appropriate counter based on instruction Inst_type
    if (instruction.Inst_type == InstructionType::INTEGER) {
        stat_integer++;
    } else if (instruction.Inst_type == InstructionType::FLOATING_POINT) {
        stat_float++;
    } else if (instruction.Inst_type == InstructionType::BRANCH) {
        stat_branch++;
    } else if (instruction.Inst_type == InstructionType::LOAD) {
        stat_loading++;
    } else if (instruction.Inst_type == InstructionType::STORE) {
        stat_store++;
    }
    // Increment total retired instructions count
    total_stats++;
}

// Function to print report with percentages of retired instructions
void Statistics::Simulate_Report(int exec_time) {
    std::cout << "Total Execution Time= " << exec_time << std::endl;
    std::cout << "Histogram:" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    // Calculate total retired instructions count
    int total = stat_integer + stat_float + stat_branch + stat_loading + stat_store;
    // Print percentage of each instruction Inst_type
    std::cout << "Integer Instructions: " << round(100 * static_cast<float>(stat_integer) / total) << "%" << std::endl;
    std::cout << "Floating Point Instructions: " << round(100 * static_cast<float>(stat_float) / total) << "%" << std::endl;
    std::cout << "Branch Instructions: " << round(100 * static_cast<float>(stat_branch) / total) << "%" << std::endl;
    std::cout << "Load Instructions: " << round(100 * static_cast<float>(stat_loading) / total) << "%" << std::endl;
    std::cout << "Store Instructions: " << round(100 * static_cast<float>(stat_store) / total) << "%" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
}
