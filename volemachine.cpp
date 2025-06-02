#include "A1_Task4_S13_20230452_20230292_20230284_volemachine.h"


// Constructor to initialize memory with "00"
Memory::Memory() {
    for (int i = 0; i < 256; i++) {
        memory[i] = "00";
    }
}

// Get the value at a specific memory address
string Memory::get_memory(int address) {
    if (address >= 0 && address < 256) {
        return memory[address];
    }
    cout << "Error: Memory address out of range (0-255): " << address << endl;
    return "00";
}


// Set the value at a specific memory address
void Memory::set_memory(int address, const string &value) {
    if (address >= 0 && address < 256) {
        memory[address] = value;
    } else {
        cout << "Error: Invalid memory address or value." << endl;
    }
}


// Display the memory contents
void Memory::display() const {
    cout << "Main Memory" << endl;
    cout << "   ";
    for (int i = 0; i < 16; i++) {
        cout << hex << uppercase << i << "  ";
    }
    cout << endl;
    for (int i = 0; i < 16; i++) {
        cout << hex << uppercase << i << "  ";
        for (int j = 0; j < 16; j++) {
            cout << setw(2) << setfill('0') << memory[i * 16 + j] << " ";
        }
        cout << endl;
    }
}


// Reset the memory contents to "00"
void Memory::reset_memory() {
    for (int i = 0; i < 256; i++) {
        memory[i] = "00";
    }
}


// Check if a string is a valid hexadecimal value
bool Memory::is_hexa(const string &str) {
    for (char c : str) {
        if (!((c >= '0' && c <= '9') || (toupper(c) >= 'A' && toupper(c) <= 'F'))) {
            return false;
        }
    }
    return true;
}






// Constructor to initialize registers with "00"
Register::Register() {
    for (int i = 0; i < 16; i++) {
        registers[i] = "00";
    }
}

// Get the value at a specific register address
string Register::get_register(int address) {
    if (address >= 0 && address < 16) {
        return registers[address];
    }
    cout << "Error: Register address out of range (0-15): " << address << endl;
    return "00";
}


// Set the value at a specific register address
void Register::set_register(int address, const string &value) {
    if (address >= 0 && address < 16) {
        registers[address] = value;
    } else {
        cout << "Error: Invalid register address or value." << endl;
    }
}


// Display the register contents
void Register::display() const {
    for (int i = 0; i < 16; ++i) {
        cout << i << ":" << registers[i] << endl;
    }
}


// Reset the register contents to "00"
void Register::reset_registers() {
    for (int i = 0; i < 16; i++) {
        registers[i] = "00";
    }
}





// Converts a decimal number to a hexadecimal string
string ALU::decimalToHex(int decimal) {
    if (decimal == 0) {
        return "00";
    }

    string hex;   // Initialize an empty string to hold the hexadecimal representation
    while (decimal > 0) {
        int remainder = decimal % 16;
        if (remainder < 10) {
            hex += (remainder + '0'); // Convert remainder to corresponding hex digit
        } else {
            hex += (remainder - 10 + 'A'); // Convert remainder to corresponding hex letter
        }
        decimal /= 16;
    }
    reverse(hex.begin(), hex.end());
    while (hex.length() < 2) {
        hex = "0" + hex;
    }
    return hex;
}


// Converts a hexadecimal string to a decimal number
int ALU::hexToDecimal(const string &hexStr) {
    return stoi(hexStr, nullptr, 16);
}


// Decodes a floating-point number from a hexadecimal string
float ALU::decodeFloatingPoint(const string &hexStr) {
    if (hexStr.length() != 2 || !Memory::is_hexa(hexStr)) {
        cout << "Error: Invalid floating-point format for decoding to hexadecimal." << endl;
        return 0.0;
    }

    int hexValue = hexToDecimal(hexStr); // Convert hex string to decimal integer

    string binaryStr = "";
    for (int i = 0; i < 8; ++i) {
        binaryStr = to_string(hexValue % 2) + binaryStr;
        hexValue /= 2;
    }

    while (binaryStr.length() < 8) {
        binaryStr = "0" + binaryStr;
    }

    // Determine the sign bit (0 for positive, 1 for negative)
    int sign ;
    if (binaryStr[0] == '1') {
        sign = 1;
    }else {
        sign = 0 ;
    }

    // Extract the exponent (bits 1-3) and mantissa (bits 4-7)
    int exponent = stoi(binaryStr.substr(1, 3), nullptr, 2);
    int mantissa = stoi(binaryStr.substr(4, 4), nullptr, 2);

    exponent -= 4; // Adjust the exponent value

    // Get floating-point value
    float value = pow(-1, sign) * (mantissa / 16.0) * pow(2, exponent);
    return value;
}


// Encodes a floating-point number to a hexadecimal string
string ALU::encodeFloatingPoint(float value) {
    // Determine the sign bit (0 for positive, 1 for negative)
    int signBit ;
    if (value < 0) {
        signBit = 1;
    } else{
        signBit = 0;
    }
    value = fabs(value); // Get the absolute value


    int exponent = 0;

    // Make the value to be within [0.5, 1[ range by dividing by 2 until it is less than 1
    while (value >= 1.0) {
        value /= 2.0;
        exponent++;
    }
    // If the value is less than 0.5, multiply by 2 to make it within [0.5, 1[
    while (value < 0.5 && exponent > -4) {
        value *= 2.0;
        exponent--;
    }
    exponent += 4; // Adjust the exponent


    int mantissa = int(value * 16);

    // Combine sign bit, exponent, and mantissa to form the final hex value
    int hexValue = signBit * 128 + exponent * 16 + mantissa;

    // Convert the final value to hexadecimal
    return decimalToHex(hexValue);
}





// Constructor to initialize Machine with memory and register references
Machine::Machine(Memory &mem, Register &reg) : memory(mem), regist(reg), pc(0) {}


// Load instructions into memory starting from a specified address
void Machine::load_instructions(vector<string> &program, int start_address) {
    invalid_instructions.clear();
    pc = start_address; // Set program counter to start address

    check_and_add_halt_instruction(program); // make sure program has a halt instruction

    for (string &inst : program) {
        // If instruction is invalid , add to invalid list
        if (inst.length() != 4 || !Memory::is_hexa(inst)) {
            invalid_instructions.push_back(inst);
        } else {
            // Split instruction into two parts and store in memory
            memory.set_memory(start_address++, inst.substr(0, 2));
            memory.set_memory(start_address++, inst.substr(2, 2));
        }
    }
}



// Check if the program contains a halt instruction, if not add one
void Machine::check_and_add_halt_instruction(vector<string> &program) {
    bool halt_found = false;
    for (const auto &inst : program) {
        if (inst == "C000" || inst == "c000") {
            halt_found = true;
            break;
        }
    }
    if (!halt_found) {
        cout << "-> No halt instruction found. Adding C000 to the end of the program." << endl;
        program.push_back("C000");
    }
}



// Fetches the next instruction and increments the program counter
string Machine::fetch() {
    if (pc < 256) {
        // Get 4 character instruction
        string instruction = memory.get_memory(pc) + memory.get_memory(pc + 1);
        pc += 2; // Move program counter to the next instruction
        return instruction;
    }
    return "";
}



// Decodes instruction into opcode and operands
void Machine::decode(const string &instruction, string &opcode, string &operand1, string &operand2) {
    opcode = instruction.substr(0, 1); // First character is the opcode
    operand1 = instruction.substr(1, 1); // Second character is first operand
    operand2 = instruction.substr(2, 2); // Last two characters are second operand
}



// Executes an instruction based on the opcode and operands
void Machine::execute(const string &opcode, const string &operand1, const string &operand2) {
    try {
        if (opcode == "1") { // Load register with memory value
            int R = ALU::hexToDecimal(operand1);
            int XY = ALU::hexToDecimal(operand2);
            if (R < 0 || R >= 16) {
                cout << "Error: Register index out of range." << endl;
                return;
            }
            regist.set_register(R, memory.get_memory(XY));
        }

        else if (opcode == "2") { // Load register with a constant
            int R = ALU::hexToDecimal(operand1);
            if (R < 0 || R >= 16) {
                cout << "Error: Register index out of range." << endl;
                return;
            }
            regist.set_register(R, operand2);
        }

        else if (opcode == "3") {  // Store register value in memory
            int R = ALU::hexToDecimal(operand1);
            if (R < 0 || R >= 16) {
                cout << "Error: Register index out of range." << endl;
                return;
            }
            int XY = ALU::hexToDecimal(operand2);
            if (XY < 0 || XY >= 256) {
                cout << "Error: Memory address out of range (0-255)." << endl;
                return;
            }
            memory.set_memory(XY, regist.get_register(R));
            if (operand2 == "00") {
                string value = regist.get_register(R);
                memory_values.push_back(value);
                cout << "Memory 00 : " << value << endl;
            }
        }

        else if (opcode == "4") { // Copy register values
            int R = ALU::hexToDecimal(operand2.substr(0, 1));
            int S = ALU::hexToDecimal(operand2.substr(1, 1));
            if (R < 0 || R >= 16) {
                cout << "Error: Register index out of range." << endl;
                return;
            }
            regist.set_register(S, regist.get_register(R));
        }

        else if (opcode == "5") {  // Add two registers and store result
            int R = ALU::hexToDecimal(operand1);
            int S = ALU::hexToDecimal(operand2.substr(0, 1));
            int T = ALU::hexToDecimal(operand2.substr(1, 1));

            if (R < 0 || R >= 16 || S < 0 || S >= 16 || T < 0 || T >= 16) {
                cout << "Error: Register index out of range." << endl;
                return;
            }

            int registerS = (ALU::hexToDecimal(regist.get_register(S)));
            int registerT = (ALU::hexToDecimal(regist.get_register(T)));

            int result = registerS + registerT;

            stringstream ss;
            ss << hex << uppercase << setw(2) << setfill('0') << (result & 0xFF);
            regist.set_register(R, ss.str());

        }

        else if (opcode == "6") { // Add floating-point values
            int R1 = ALU::hexToDecimal(operand1);
            int R2 = ALU::hexToDecimal(operand2.substr(0, 1));
            int R3 = ALU::hexToDecimal(operand2.substr(1, 1));
            if (R1 < 0 || R1 >= 16 || R2 < 0 || R2 >= 16 || R3 < 0 || R3 >= 16) {
                cout << "Error: Register index out of range." << endl;
                return;
            }

            float val1 = ALU::decodeFloatingPoint(regist.get_register(R2));
            float val2 = ALU::decodeFloatingPoint(regist.get_register(R3));

            float result = val1 + val2;

            string resultHex = ALU::encodeFloatingPoint(result);
            regist.set_register(R1, resultHex);
        }

        else if (opcode == "B" || opcode == "b") { //  jump instruction
            int R = ALU::hexToDecimal(operand1);
            int address = ALU::hexToDecimal(operand2);
            if (regist.get_register(R) == regist.get_register(0)) {
                pc = address;
            }
        }

        else if (opcode == "C" || opcode == "c") { // Halt instruction
            cout << "-> Halt instruction found." << endl;
            pc = 256; // Set pc out of range to stop execution
        }

    } catch (const invalid_argument &e) {
        cout << "Error: Invalid argument for stoi conversion." << endl;
    } catch (const out_of_range &e) {
        cout << "Error: Out of range argument for stoi conversion." << endl;
    }
}



// Executes instructions one by one with output after each step
void Machine::run_step_by_step() {
    while (pc < 256) {
        string instruction = fetch();
        if (instruction.empty()){
            break;
        }

        string opcode, operand1, operand2;
        decode(instruction, opcode, operand1, operand2);
        cout << "PC: " << hex << uppercase << pc - 2 << " | Instruction: " << instruction << endl;
        execute(opcode, operand1, operand2);


        cout << "Registers after execution:" << endl;
        regist.display();
        cout << "Memory after execution:" << endl;
        memory.display();
        cout << "--------------------------------------" << endl;
    }

    if (!memory_values.empty()){
        cout << "Memory 00 :";
        for (const string &val : memory_values) {
            cout << val << " ";
        }
    }

    cout << endl;
    reset_machine();
    cout << "-> Machine reset to initial state. Memory and registers have been cleared." << endl;
}


// Executes the program without stopping between instructions
void Machine::run_whole() {
    while (pc < 256) {
        string instruction = fetch();
        if (instruction.empty()){
            break;
        }


        string opcode, operand1, operand2;
        decode(instruction, opcode, operand1, operand2);
        execute(opcode, operand1, operand2);
    }

    cout << "Final state of Registers:" << endl;
    regist.display();
    cout << "Final state of Memory:" << endl;
    memory.display();
    reset_machine();
    cout << "-> Machine reset to initial state. Memory and registers have been cleared." << endl;
}



// Displays invalid instructions if exist
void Machine::display_invalid_instructions() {
    if (!invalid_instructions.empty()) {
        cout << "Invalid Instructions:" << endl;
        for (const string &inst : invalid_instructions) {
            cout << "Instruction: " << inst << endl;
        }
    }
    else if (invalid_instructions.empty()){
        cout << "-> No invalid instructions found" << endl;
    }
}



// Resets the machine's memory and register
void Machine::reset_machine() {
    memory.reset_memory();
    regist.reset_registers();
    pc = 0;
}


