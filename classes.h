#include <string>
#include <vector>
#include <deque>
#include <fstream>
#include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

#ifndef CLASSES_H_
#define CLASSES_H_

// Enum class representing the type of instructions handled in the pipeline.
enum class InstructionType {
    INVALID,          // Represents an undefined or invalid instruction type.
    INTEGER,          // Represents integer operation instructions.
    FLOATING_POINT,   // Represents floating-point operation instructions.
    BRANCH,           // Represents branch instructions.
    LOAD,             // Represents load operation instructions.
    STORE             // Represents store operation instructions.
};

// Class representing an individual instruction in a pipeline simulation.
class Instruction_class {
public:
    // Default constructor.
    Instruction_class() : Inst_type(InstructionType::INVALID), exec(false), memorized(false) {}

    // Constructor initializes an instruction with specific properties.
    Instruction_class(string Q_counter, InstructionType Inst_type, vector<string> priority)
        : Q_counter(Q_counter), Inst_type(Inst_type), priority(priority), exec(false), memorized(false) {}

    // The queue counter identifying the instruction's position or sequence.
    string Q_counter;

    // The type of the instruction, defined by the InstructionType enum.
    InstructionType Inst_type;

    // Priority vector that may hold dependencies or other relevant data affecting instruction execution.
    vector<string> priority;

    // Execution flag indicating whether the instruction has been executed.
    bool exec;

    // Memorization flag indicating whether the instruction has been stored in memory.
    bool memorized;

    // Adds a dependency or related data to the instruction's priority vector.
    void PushBack(string dependency) {
        priority.push_back(dependency);
    }
};


// Class Executing_Help_Instructions manages the lifecycle of instructions as they progress through various stages of a pipeline.
class Executing_Help_Instructions {
public:
    // Queues representing different stages of the instruction pipeline
    deque<Instruction_class> FetchingQ;
    deque<Instruction_class> DecodingQ;
    deque<Instruction_class> ExecutingQ;
    deque<Instruction_class> MemoryQ;
    deque<Instruction_class> WBQ;

    // Checker flag to indicate some state, the specific use should be defined according to its use in methods.
    bool checker;

    // Constructor initializes checker to false indicating an initial default state
    Executing_Help_Instructions() : checker(false) {}

    // Adds an instruction to the fetching queue
    void FetchingQ_add(Instruction_class instruction) {
        FetchingQ.push_back(instruction);
    }

    // Removes and returns the front instruction from the fetching queue
    Instruction_class Delete_fetch() {
        Instruction_class instruction = FetchingQ.front();
        FetchingQ.pop_front();
        return instruction;
    }

    // Checks if the fetching queue is empty
    bool Fetch_empty_checker() {
        return FetchingQ.empty();
    }

    // Adds an instruction to the decoding queue
    void DecodeQ_add(Instruction_class instruction) {
        DecodingQ.push_back(instruction);
    }

    // Removes and returns the front instruction from the decoding queue
    Instruction_class Delete_decode() {
        Instruction_class instruction = DecodingQ.front();
        DecodingQ.pop_front();
        return instruction;
    }

    // Checks if the decoding queue is empty
    bool Decode_empty_checker() {
        return DecodingQ.empty();
    }

    // Returns the type of the instruction at the front of the decoding queue
    InstructionType Decode_front() {
        return DecodingQ.front().Inst_type;
    }

    // Abstract function to check decoding state; implementation should be defined as needed.
    bool Decode_checker();


    // Adds an instruction to the memory queue
    void MemoryQ_add(Instruction_class instruction) {
        MemoryQ.push_back(instruction);
    }

    // Removes and returns the front instruction from the memory queue
    Instruction_class Delete_Memory() {
        Instruction_class instruction = MemoryQ.front();
        MemoryQ.pop_front();
        return instruction;
    }

    // Checks if the memory queue is empty
    bool Memory_empty_checker() {
        return MemoryQ.empty();
    }

    // Placeholder for method to iterate through memory instructions based on pipeline file_width
    void MemQ_inst_iterator(int file_width);

    // Adds an instruction to the executing queue
    void Execute_add(Instruction_class instruction) {
        ExecutingQ.push_back(instruction);
    }

    // Removes and returns the front instruction from the executing queue
    Instruction_class Delete_Execute() {
        Instruction_class instruction = ExecutingQ.front();
        ExecutingQ.pop_front();
        return instruction;
    }

    // Returns the type of the instruction at the front of the executing queue
    InstructionType Execute_front() {
        return ExecutingQ.front().Inst_type;
    }

    // Checks if the executing queue is empty
    bool Execute_empty_checker() {
        return ExecutingQ.empty();
    }

    // Placeholder for method to process instructions per cycle based on pipeline file_width
    void Instructions_iterator(int file_width);

    // Adds an instruction to the write-back queue
    void WB_add(Instruction_class instruction) {
        WBQ.push_back(instruction);
    }

    // Removes and returns the front instruction from the write-back queue
    Instruction_class Delete_WB() {
        Instruction_class instruction = WBQ.front();
        WBQ.pop_front();
        return instruction;
    }

    // Checks if the write-back queue is empty
    bool Wb_empty_checker() {
        return WBQ.empty();
    }
};

// Statistics class for accumulating and reporting various instruction-related statistics.
class Statistics {
public:
    int stat_integer;    // Count of integer instructions processed.
    int stat_float;      // Count of floating-point instructions processed.
    int stat_branch;     // Count of branch instructions processed.
    int stat_loading;    // Count of load instructions processed.
    int stat_store;      // Count of store instructions processed.
    int total_stats;     // Total count of all instructions processed.

    // Constructor initializes all statistical counters to zero.
    Statistics() : stat_integer(0), stat_float(0), stat_branch(0), stat_loading(0), stat_store(0), total_stats(0) {}

    // Updates the appropriate statistical counters based on the type of instruction processed.
    void Instruction_counter(Instruction_class instruction);

    // Reports the collected statistics at the end of the simulation or at a specific cycle.
    void Simulate_Report(int Cycle);
    
};

// Handles file operations and instruction fetching for the simulator.
class File_Worker {
public:
    string FileName;     // Name of the file containing instruction data.
    int first_line;      // The first line of the file to start processing.
    int Counter;         // Total number of instructions to process.
    int curr_line;       // Current line number being processed in the file.
    ifstream trace_file; // Input file stream used to read the instruction data.

    // Constructor opens the file and sets up processing starting at `first_line`.
    File_Worker(string FileName, int first_line, int Counter);

    bool failed_to_open_file;  // Flag indicating if the file failed to open.

    // Checks if there are more instructions to fetch based on the current line and total count.
    bool Fetch_new_instruction();

    // Fetches the next instruction from the file, converting it into an Instruction_class object.
    Instruction_class next_instruction();

    // Closes the file stream upon completion of file processing or when the simulation ends.
    void closeFile();
};

// Main class that manages the simulation of instruction processing.
class Simulator {
public:
    Executing_Help_Instructions* Instructions; // Manages the state and transitions of instructions across various pipeline stages.
    Statistics* Stats;                          // Stores and reports statistics on instruction processing.
    int file_width;                             // The width of the file or number of instructions that can be processed concurrently.
    File_Worker* Input;                         // Handles input file operations and instruction fetching.
    int Cycle;                                  // Current cycle count of the simulation.
    int Total_instructions;                     // Total number of instructions processed.

    // Constructs a Simulator with a specific input file and width for the instruction pipeline.
    Simulator(File_Worker* Input, int file_width);

    // Destructor cleans up dynamic allocations to prevent memory leaks.
    ~Simulator();

    // Retires instructions from the write-back stage and updates statistics.
    void WBtoRetire();

    // Moves instructions from the memory stage to the write-back stage.
    void MemtoWB();

    // Executes instructions and transitions them from the execution stage to the memory stage.
    void ExtoMem();

    // Decodes instructions and prepares them for execution.
    void IDtoEX();

    // Fetches instructions into the decode queue.
    void IFtoID();

    // Fetches new instructions based on the availability and status checks.
    void newToFetch();

    // Starts the simulation, processing through various stages until all instructions are processed.
    void Start_simulation();
};

#endif