#include "classes.h"
File_Worker::File_Worker(string FileName, int first_line, int Counter) {
    this->FileName = FileName;
    this->first_line = first_line;
    this->Counter = Counter;
    curr_line = 1;
    failed_to_open_file = false;

    // Open the trace file and prepare it
    trace_file.open(FileName);
    if (!trace_file.is_open()) {
        cout << "Failed to open file: " << FileName << endl;
        failed_to_open_file = true;
        return;
    }

    // Skip lines until reaching the first_line line
    string temp;
    for (int i = 1; i < first_line; ++i) {
        if (!getline(trace_file, temp)) {
            cout << "Failed to reach starting line " << first_line << endl;
            failed_to_open_file = true;
            return;
        }
        curr_line++;
    }
}

bool File_Worker::Fetch_new_instruction() {
    return curr_line < first_line + Counter;
}

Instruction_class File_Worker::next_instruction() {
    Instruction_class instruction;

    // Check if the file is open
    if (!trace_file.is_open()) {
        cout << "File failed to open!\n";
        return instruction; // Return an empty instruction
    }

    // Read a line from the file
    string line;
    if (!getline(trace_file, line)) {
        cout << "End of file reached!\n";
        return instruction; // Return an empty instruction if end of file is reached
    }

    // Split the line by commas
    stringstream line_stream(line);
    string token;
    vector<string> user_input;
    while (getline(line_stream, token, ',')) {
        user_input.push_back(token);
    }

    // Check if the line is valid
    if (user_input.size() < 2) {
        cout << "Invalid line\n";
        return instruction; // Return an empty instruction if line is invalid
    }

    // Parse instruction fields
    instruction.Q_counter = user_input[0];
    int Inst_type;
    if (!(istringstream(user_input[1]) >> Inst_type)) {
        cout << "Invalid instruction Inst_type\n";
        return instruction; // Return an empty instruction if Inst_type conversion fails
    }
    instruction.Inst_type = static_cast<InstructionType>(Inst_type);
    for (size_t i = 2; i < user_input.size(); ++i) {
        instruction.PushBack(user_input[i]);
    }

    curr_line++;
    return instruction;
}

void File_Worker::closeFile() {
    trace_file.close();
}