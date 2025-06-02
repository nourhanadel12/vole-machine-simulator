#ifndef TASK4_VOLE_MACHINE_H
#define TASK4_VOLE_MACHINE_H


#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <cmath>

using namespace std;



// Memory class to represent main memory
class Memory {
    string memory[256]; // Array to store memory contents
public:
    Memory(); // Constructor to initialize memory
    string get_memory(int address); // function to get the value at a specific memory address
    void set_memory(int address, const string &value);  // function to set the value at a specific memory address
    void display() const; // function to display the memory contents
    void reset_memory(); // function to reset the memory contents
    static bool is_hexa(const string &str); // validate that a string is in hexadecimal representation
};



// Register class to represent the CPU registers
class Register {
    string registers[16];  // Array to store register contents
public:
    Register(); // Constructor to initialize registers
    string get_register(int address);  // function to get the value at a specific register address
    void set_register(int address, const string &value); // function to set the value at a specific register address
    void display() const; // function to display the register contents
    void reset_registers(); // function to reset the register contents
};



// ALU class to handle arithmetic and logic operations
class ALU {
public:
    static string decimalToHex(int decimal); // function to convert a decimal number to a hexadecimal string
    static int hexToDecimal(const string &hexStr); // function to convert a hexadecimal string to a decimal number
    static float decodeFloatingPoint(const string &hexStr); // function to decode a floating-point number from a hexadecimal string
    static string encodeFloatingPoint(float value); // function to encode a floating-point number to a hexadecimal string
};



class Machine {
    Memory &memory; // Reference to a Memory object
    Register &regist; // Reference to a Register object
    int pc;  // Program counter
    vector<string> invalid_instructions; // Stores any invalid instructions
    vector<string> memory_values; // Holds memory(00) values for the program

    // function to add a halt instruction to the program if it is not found
    void check_and_add_halt_instruction(vector<string> &program);

public:
    // Constructor that initializes Machine with references to memory and register objects
    Machine(Memory &mem, Register &reg);

    // Loads instructions into memory starting from a specified address
    void load_instructions(vector<string> &program, int start_address);

    // Fetches the next instruction based on the program counter
    string fetch();

    // Decodes the instruction into opcode and operands
    void decode(const string &instruction, string &opcode, string &operand1, string &operand2);

    // Executes an instruction based on the decoded opcode and operands
    void execute(const string &opcode, const string &operand1, const string &operand2);

    // Runs the program step-by-step
    void run_step_by_step();

    // Runs the entire program all at once
    void run_whole();

    // Displays any invalid instructions
    void display_invalid_instructions();

    // Resets the machine
    void reset_machine();
};

#endif
