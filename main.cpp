
// Purpose: our vole machine lets you load and run programs written as hexadecimal instructions.
//          You can either go through each instruction one at a time or run the entire program all
//          at once. As it runs, the simulator shows changes in the program counter, registers, and
//          memory, helping users see how each part of the machine works together to process instructions.

// Author: Nourhan Adel Mohamed El-Hady
//         Fatma Nazeih Hanfy
//         Caroline Ayman Isaac

// Section: S 13

// Date: 1/11/2024



#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "A1_Task4_S13_20230452_20230292_20230284_volemachine.h"

using namespace std;


// Function to validate if a given address is a valid hexadecimal and has two characters
bool is_valid_hex_address(const string &address) {
    return Memory::is_hexa(address) && address.length() == 2;
}

int main() {
    // Initialize memory and register objects for the Vole Machine
    Memory memory;
    Register reg;
    Machine vole_machine(memory, reg);


    cout << "Welcome to our Vole Machine!" << endl;
    while (true) {
        cout << "Which option do you want: " << endl;
        cout << "1. Load a file of instructions." << endl;
        cout << "2. Exit" << endl;
        cout << "Option 1 or 2 : " << endl;

        string option;
        cin >> option;
        cin.ignore();


        if (option == "1") {
            // file input to load instructions
            string filename;
            while (true) {
                cout << "Please enter the file name that contain the instructions (filename.txt) : " << endl;
                cin >> filename;

                // Check if invalid file name
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid file name.Try again" << endl;
                    continue;
                }

                ifstream my_file(filename);// Open the file

                if (my_file) {
                    // read instructions into the program vector
                    vector<string> program;
                    string instruction;
                    while (my_file >> instruction) {
                        program.push_back(instruction); // Add each instruction to the vector
                    }
                    my_file.close();


                    // Asking for the starting memory address
                    string start_hex;
                    while (true) {
                        cout << "Enter the starting memory location (in Hexadecimal representation): " << endl;
                        cout << "[ Note : from 00 to FF all in uppercase as 0A ] " << endl;
                        cin >> start_hex;

                        // Validate and convert starting address
                        if (is_valid_hex_address(start_hex)) {
                            int start_address = stoi(start_hex, nullptr, 16);
                            if (start_address >= 0 && start_address <= 255) {
                                vole_machine.load_instructions(program, start_address); // Load instructions
                                cout << "-> Program loaded successfully." << endl;
                                vole_machine.display_invalid_instructions(); // Display any invalid instructions
                                break;
                            } else {
                                cout << "Error: Memory address out of range (00-FF).Try again" << endl;
                            }
                        } else {
                            cout << "Error: Invalid hexadecimal address.Try again" << endl;
                        }
                    }


                    // Execution mode selection
                    while (true) {
                        cout << "Which way do you want to execute the instructions :" << endl;
                        cout << "1. Step by step execution " << endl;
                        cout << "   (with showing PC , Register values and memory values at executing each instruction)"<< endl;
                        cout << "2. Execute all at once" << endl;
                        cout << "   (with showing Register values and memory values after executing all the instructions)"<< endl;
                        cout << "Option 1 or 2 : ";
                        string option2;
                        cin >> option2;

                        if (option2 == "1") {
                            cout << "[Note : PC will be represented in hexadecimal representation] " << endl;
                            vole_machine.run_step_by_step();
                            break;
                        } else if (option2 == "2") {
                            vole_machine.run_whole();
                            break;

                        } else {
                            cout << "Invalid execution choice.Try again" << endl;
                        }
                    }

                    break;
                }
                else {
                    cout << "File does not exist , please try again. " << endl;
                }
            }


        }

        else if (option == "2") {
            cout << " Exiting.. " << endl;
            return 0;
        }


        else {
            cout << "Invalid choice, please choose option 1 or option 2." << endl;
        }
    }
}
