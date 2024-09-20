#include "classes.h"
#include <sstream>
#include <cstddef> // For size_t

// Checks if the first instruction in the decoding queue can proceed to execution.
// It iterates through the instruction's dependencies to ensure none are blocked.
bool Executing_Help_Instructions::Decode_checker() {
    // Access the priority list of the first instruction in the decoding queue.
    const vector<string>& priority = DecodingQ.front().priority;
    for (size_t i = 0; i < priority.size(); ++i) {
        const string& dependent = priority[i];
        Instruction_class instruction;

        // Check if the dependency is still executing and not yet completed.
        for (size_t j = 0; j < ExecutingQ.size(); ++j) {
            instruction = ExecutingQ[j];
            if (dependent == instruction.Q_counter && !instruction.exec) {
                return false; // Dependency is still executing, can't proceed.
            }
        }

        // Check if the dependency is in memory operations and if it's relevant for memory-stage blocking.
        for (size_t j = 0; j < MemoryQ.size(); ++j) {
            instruction = MemoryQ[j];
            if (dependent == instruction.Q_counter && 
                (instruction.Inst_type == InstructionType::LOAD || instruction.Inst_type == InstructionType::STORE) && 
                !instruction.memorized) {
                return false; // Dependency in memory operations is not yet committed.
            }
        }
    }
    return true; // All checks passed; instruction can proceed.
}

// Processes up to 'width' instructions from the executing queue to simulate execution.
void Executing_Help_Instructions::Instructions_iterator(int width) {
    int Q_max = ExecutingQ.size();
    for (int index = 0, iter = 0; index < Q_max && iter < width; ++index) {
        Instruction_class& instruction = ExecutingQ[index];
        if (!instruction.exec) {
            instruction.exec = true; // Mark the instruction as executed.
            if (instruction.Inst_type == InstructionType::BRANCH) {
                checker = false; // A branch instruction might alter control flow.
            }
            ++iter;
        }
    }
}

// Marks up to 'width' instructions in the memory queue as completed or memorized.
void Executing_Help_Instructions::MemQ_inst_iterator(int width) {
// Correcting the loop to use size_t for indices and size comparisons
    for (size_t index = 0, iter = 0; index < MemoryQ.size() && iter < static_cast<size_t>(width); ++index) {
        Instruction_class& instruction = MemoryQ[index];
        if (!instruction.memorized) {
            instruction.memorized = true;
            ++iter;
        }
    }
}