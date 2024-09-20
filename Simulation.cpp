#include "classes.h"
#include <iostream>
#include <memory>
#include <cstdlib>

using namespace std;

// Constructor for the Simulator class, initializes members and allocates resources.
Simulator::Simulator(File_Worker* trace_input_param, int width_param)
    : Instructions(new Executing_Help_Instructions()),
      Stats(new Statistics()),
      file_width(width_param),
      Input(trace_input_param),
      Cycle(0),
      Total_instructions(0) {
}

// Destructor for the Simulator class, responsible for deallocating dynamically allocated memory.
Simulator::~Simulator() {
    delete Instructions;
    delete Stats;
}

// Function to retire instructions from the write-back stage and update statistics.
void Simulator::WBtoRetire() {
    while (!Instructions->Wb_empty_checker()) {
        Instruction_class current_Instructions = Instructions->Delete_WB();
        Stats->Instruction_counter(current_Instructions);
        Total_instructions--;
    }
}

// Moves instructions from the memory stage to the write-back stage.
void Simulator::MemtoWB() {
    int instruction_counter = 0;
    Instructions->MemQ_inst_iterator(file_width);
    while (instruction_counter < file_width && !Instructions->Memory_empty_checker()) {
        Instruction_class current_Instructions = Instructions->Delete_Memory();
        Instructions->WB_add(current_Instructions);
        instruction_counter++;
    }
}

// Handles the transfer of instructions from the execute stage to the memory stage.
void Simulator::ExtoMem() {
    bool Is_load = false;  // Flag to track if a LOAD operation has occurred.
    bool Is_store = false; // Flag to track if a STORE operation has occurred.
    int instruction_counter = 0;

    Instructions->Instructions_iterator(file_width);  // Process a batch of instructions for execution.

    while (instruction_counter < file_width && !Instructions->Execute_empty_checker()) {
        InstructionType Inst_type = Instructions->Execute_front();
        if ((Inst_type == InstructionType::LOAD && Is_load) ||
            (Inst_type == InstructionType::STORE && Is_store)) {
            break;  // Exit if a second LOAD or STORE is encountered.
        }

        Is_load |= (Inst_type == InstructionType::LOAD);
        Is_store |= (Inst_type == InstructionType::STORE);

        Instruction_class current_Instructions = Instructions->Delete_Execute();
        Instructions->MemoryQ_add(current_Instructions);
        instruction_counter++;
    }
}

// Processes the instruction decode stage, transferring instructions to the execute stage.
void Simulator::IDtoEX() {
    bool Is_int = false;      // Flag to track if an INTEGER operation has occurred.
    bool Is_branch = false;   // Flag to track if a BRANCH operation has occurred.
    bool Is_float = false;    // Flag to track if a FLOATING POINT operation has occurred.
    int instruction_counter = 0;

    while (instruction_counter < file_width && !Instructions->Decode_empty_checker()) {
        if (!Instructions->Decode_checker()) {
            break;  // Stop if dependencies are not resolved.
        }

        InstructionType Inst_type = Instructions->Decode_front();
        bool break_condition = 
            (Inst_type == InstructionType::INTEGER && Is_int) ||
            (Inst_type == InstructionType::FLOATING_POINT && Is_float) ||
            (Inst_type == InstructionType::BRANCH && Is_branch);

        if (break_condition) {
            break;  // Prevent processing further if a duplicate type is detected.
        }

        Is_int |= (Inst_type == InstructionType::INTEGER);
        Is_float |= (Inst_type == InstructionType::FLOATING_POINT);
        Is_branch |= (Inst_type == InstructionType::BRANCH);

        Instruction_class current_Instructions = Instructions->Delete_decode();
        Instructions->Execute_add(current_Instructions);
        instruction_counter++;
    }
}

// Fetches instructions and adds them to the decode queue.
void Simulator::IFtoID() {
    int instruction_counter = 0;
    while (instruction_counter < file_width && !Instructions->Fetch_empty_checker()) {
        Instruction_class current_Instructions = Instructions->Delete_fetch();
        Instructions->DecodeQ_add(current_Instructions);
        instruction_counter++;
    }
}

// Handles fetching new instructions based on pipeline conditions.
void Simulator::newToFetch() {
    int instruction_counter = 0;
    while (instruction_counter < file_width && Input->Fetch_new_instruction() && !Instructions->checker) {
        Instruction_class current_Instructions = Input->next_instruction();
        Instructions->FetchingQ_add(current_Instructions);
        Total_instructions++;

        if (current_Instructions.Inst_type == InstructionType::BRANCH) {
            Instructions->checker = true;  // Halt fetching if a BRANCH is detected.
        }
        instruction_counter++;
    }
}

// Main loop of the simulator, processes all stages of the pipeline.
void Simulator::Start_simulation() {
    while (Total_instructions != 0 || Input->Fetch_new_instruction()) {
        WBtoRetire();
        MemtoWB();
        ExtoMem();
        IDtoEX();
        IFtoID();
        newToFetch();
        Cycle++;  // Increment the cycle count each loop iteration.
    }
    Stats->Simulate_Report(Cycle);  // Generate final report.
    Input->closeFile();  // Ensure input file is properly closed.
}

// Helper function to initialize and start the simulation.
void Sim(const string& traceFileName, int startInst, int instCount, int file_width) {
    unique_ptr<File_Worker> traceInput(new File_Worker(traceFileName, startInst, instCount));
    Simulator pipelineSimulation(traceInput.get(), file_width);
    pipelineSimulation.Start_simulation();
}

// Entry point of the program, processes command-line arguments to start the simulation.
int main(int argc, char* argv[]) {
    if (argc < 5) {
        cout << "Insufficient number of arguments provided!" << endl;
        return 1;
    }
    try {
        string traceFileName = argv[1];
        int startInst = stoi(argv[2]);
        int instCount = stoi(argv[3]);
        int file_width = stoi(argv[4]);
        Sim(traceFileName, startInst, instCount, file_width);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 2;
    }

    return 0;
}
