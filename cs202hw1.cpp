/*
 * Name: Mitchell Perez, 5005926180, Assignment #1
 * Description: Language Processor in C++ (assembler and interpreter)
 * input: assembly file
 * Output: stored memory and operation
 */

// required libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

// namespace std override
using namespace std;

// Global constant variables
const int LABEL_COUNT = 100; ///< default number of labels
const int OPCODE_COUNT = 18; ///< default number of opcodes
const int MAX_CHARS = 5;     ///< maximum number of characters
const int STACK_SIZE = 1000; ///< maximum capacity of array

///< list of opcodes needed for interpretation
const string OPCODE_LIST = 
"const get put ld st add sub mul div cmp jpos jz j jl jle jg jge halt"; 

// <--
// YOUR CODE GOES HERE

// array of structs of size LABEL_COUNT 
//to place labels and memory location 
struct Symbol
{
    string inst; // placeholder for OPCODE_LIST
    string name;
    int mem;
};

// array of structs of size OPCODE_COUNT 
//to place opcodes and their int values
struct Opcode
{
    string name;
    int code;
};

// required functions
/*
* function_identifier: breaks string into words, each which 
                        represents different instruction 
                        and then stores words to array of opcodes
                       string's value is collection of all instructions
* parameters: string instruction and opcode array struct 
* return value: n/a (void function return type)
*/
void splitOpcodes(string inst, Opcode *opcodes) 
{
    // initializing variables to put into struct Opcode
    string partOfLine;
    int spaceLocation = 0;

    // for the entire size of the index
    for (int i = 0; i < OPCODE_COUNT; i++)
    {
        // find and set location of space to variable
        spaceLocation = inst.find(' ');
        // set isolated instruction to member at index i
        opcodes[i].name = inst.substr(0, spaceLocation);
        // set int i to member code (corresponds)
        opcodes[i].code = i;
        // set rest of instruction to rest of string length
        inst = inst.substr(spaceLocation + 1, inst.length());
    }
}
/*
 * function_identifier: function that returns true 
                        if value of string matches 
                        instructions array of opcodes 
 * parameters: string word and opcode
 * return value: bool value true/false
 */
bool isOpcode(string &word, Opcode *opcodes)
{
    //iterate through array index 
    for (int i = 0; i < OPCODE_COUNT; i++)
    {
        if (word == opcodes[i].name)
        {
            //return true if there is match
            return true;
        }
    }
    //return false by default
    return false;
}
/*
* function_identifier: function that returns an integer 
                        that corresponds to 
                        value of string of one the opcodes 
                        in array of opcodes 
                       otherwise returns -1 if string is 
                       not found in array of opcodes
* parameters: string word and opcode
* return value: int value of opcode or -1 if not found
*/
int getOpcode(string &word, Opcode *opcodes)
{
    // iterate through array and return specific opcode 
    for (int i = 0; i < OPCODE_COUNT; i++)
    {
        if (word == opcodes[i].name)
        {
            //return integer corresponding to opcode
            return opcodes[i].code;
        }
    }

    //return -1 by default
    return -1;
}
/*
* function_identifier: function that returns an integer 
                        corresponding to value of string
                        of one the opcodes in the array of opcodes
                       otherwise returns -1 if label does not exist
                       function used in first pass
* parameters: takes string name of label, array of current labels 
            and position of the next label in memory 
            to check if label goes by other name in array
* return value: returns integer that corresponds to 
                value of string or -1 if not found 
*/
int getLocation(string &name, Symbol *labels, int nextLabel)
{
    // returns symbols returned value when calling
    for (int i = 0; i < nextLabel; i++)
    {
        if (name == labels[i].name)
        {
            // return index i if match
            return i;
        }
    }
    //if no match, return -1
    return -1;
}
/*
* function_identifier: function that returns true 
                        if string is valid number 
* parameters: string
* return value: true if valid number, false if not
*/
bool isNumber(string &str)
{
    bool existNumber = true;
    for (int i = 0; i < str.length(); i++)
    {
        if (!isdigit(str[i]))
        {
            existNumber = false;
            break;
        }
    }
    if (existNumber == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}
// -->

// Helper functions

/// Returns the number of spaced needed for padding.
/// @param str the string that needs padding.
/// @param len the maximum number of characters.
/// @return the number of spaces needed.
int padding(string str, int len)
{
    int n = static_cast<int>(str.length());
    return len + abs(len - n);
}

/// Dumps the name and numerical value that each instrcution represents
/// @param op the array of opcodes.
/// @param count the number of opcodes.
/// @return void.
void dumpOpcodes(Opcode *opcodes, int count)
{
    cout << "Opcodes" << endl;
    int pad = 0;
    for (int i = 0; i < count; ++i)
    {
        pad = padding(opcodes[i].name, MAX_CHARS);
        cout << "> " << opcodes[i].name 
        << string(pad, ' ') << opcodes[i].code << endl;
    }
}

/// Dumps the name of a label and its corresponding location in memory.
/// @param sym an array of symbols.
/// @param count the number of symbols in the array.
/// @return void.
void dumpSymbols(Symbol *labels, int count)
{
    std::cout << "Symbols" << std::endl;
    // int pad = 0;
    for (int i = 0; i < count; ++i)
    {
        // pad = padding(labels[i].name, MAX_CHARS);
        cout << "> "
             << "[" << labels[i].name << "] = " 
             << labels[i].mem << endl;
    }
}

/// Dumpds the address of each instruction and label.
/// @param memory the array of addresses.
/// @param count the number of addresses.
/// @return void.
void dumpMemory(int *memory, int count)
{
    cout << "Memory" << endl;
    cout.fill('0');
    for (int i = 0; i < count; ++i)
    {
        cout << setw(8) << memory[i] << endl;
    }
}

/// Entry point of the main program.
/// @param argc the number of command-line arguments.
/// @param argv the vector of command-line arguments.
/// @return 0 if the program runs to complition.
int main(const int argc, const char *argv[])
{
    // <---
    // YOUR CODE GOES HERE

    // create the table of opcodes (array of structs)
    Opcode opcodes[OPCODE_COUNT];
    Symbol labels[LABEL_COUNT];
    // parse the list of opcodes
    splitOpcodes(OPCODE_LIST, opcodes);

    // iterate through entire command line arguments
    //run each file
    ifstream iFile;
    for (int i = 1; i < argc; i++)
    {
        // check command-line arguments
        iFile.open(argv[i]);
        // PASS #1
        //--------------------------------------------
        // initialize output file object
        ofstream oFile;
        // initialize string variables
        string line, partOfLine;
        // open intermediate output file in oFile
        oFile.open("intermediate");
        //int variables to count
        int lineCounter = 0;
        int counter = 0;

        // while getting input from file 
        //and storing into line variable
        while (getline(iFile, line))
        {

            // initialize stringstream object
            stringstream ss(line);

            // initialize bool variable to false
            bool isSecondColumn = false;
            bool isComment = false;
            bool isMiddleComment = false;
            bool isConst = false;

            // while getting the next word and store into partOfLine
            while (ss >> partOfLine)
            {

                // (1) Removing comments ";"
                // if the word is the semicolon (comment)
                if (partOfLine == ";") 
                {
                    //set bool to true
                    isComment = true;
                    break; // go to next line
                }

                else if (isSecondColumn == true)
                {
                    // (4) Write generated code to an output file
                    oFile << " " << partOfLine; // output 3rd column
                    isSecondColumn = false;     // reset bool to false
                    isMiddleComment = true;
                }

                else if (isOpcode(partOfLine, opcodes))
                {
                    // set bool to true-
                    //implies that next is col3 (argument) 
                    isSecondColumn = true;
                    // (4) Write generated code to an output file
                    oFile << partOfLine; // output to intermediate file
                    isMiddleComment = true;
                }

                else // if not opcode
                {
                    // (2) Remember labels
                    if (isConst == false)
                    {
                        labels[lineCounter].name = partOfLine;
                        labels[lineCounter].mem = lineCounter;
                    }

                   //increment counter and set bool to true
                    counter++;
                    isMiddleComment = true;
                }
            }

            //increment counter and output to ofile
            if ((isComment == false || isMiddleComment == true) 
            && line.length() != 0) 
            {
                oFile << '\n';
                lineCounter++;
            }
        }

        //close input & output file
        iFile.close();
        oFile.close();
        // --------------------------------------------
        //
        // PASS #2
        //
        // (1) Read output file

        // open intermediate file as input file
        iFile.open("intermediate");

        //(2) Replace numeric values and pack them into

        // initialize memory with size 1000
        int memory[STACK_SIZE] = {}; 
        // initialize string variables
        string sentence = "";
        string savedOpcode = "";
        string savedLabel = "";

        int prevNum = 0;
        int nextMemory = 0;
        int myCounter = 0; // for debugging

        // stores int equivalent to command and memory location
        // while you get entire line from intermediate line
        while (getline(iFile, sentence))
        {
            // create stringstream object through sentence called ss
            stringstream ss(sentence);

            // pass each word delimited by space to savedOpcode
            ss >> savedOpcode;

            // if there is not a newline, then there is another field
            if (ss.peek() != EOF)
            {
                //intake each string into saved label
                ss >> savedLabel;
                
                // if getLocation fails and returns -1
                if (getLocation(savedLabel, labels, counter) == -1 
                && isNumber(savedLabel)) 
                {
                    //store saved label int into memory
                    memory[nextMemory++] += stoi(savedLabel); 
                }
                //if 2nd field is not number
                else if (!isNumber(savedLabel))
                {

                    //calculate memory int
                    memory[nextMemory++] = STACK_SIZE 
                    * opcodes[getOpcode(savedOpcode, opcodes)].code 
                    + getLocation(savedLabel, labels, STACK_SIZE); 
                }
             
            }
            else // there is only 1 field
            {
                //calculate memory int and use 0 as label value
                memory[nextMemory++] = STACK_SIZE 
                * opcodes[getOpcode(savedOpcode, opcodes)].code; 
            }
        }

        //close input file (intermediate file)
        iFile.close();

        // -------------------------------------------
        //
        // INTEPRETER
        //
        // (1) Execute instructions in sequential order

        cout << "Running program..." << endl;

        // register and instructional pointer variables
        int reg = 0;
        int ip = 0;

        //bool input failure variable
        bool inputFailure = false;

        //while instructional pointer is not negative
        while (ip >= 0)
        {

            // initialize int variables
            int labelAddress = 0;
            int opcodeValue = 0;

            // label address = x % 1000 where x is memory location
            labelAddress = memory[ip] % 1000;

            // opcode value = x / 1000 where x is memory location
            opcodeValue = memory[ip] / 1000;

            // initialize string variable
            string computedCommand = "";

            // itereate through array
            //find corresponding command
            for (int i = 0; i < OPCODE_COUNT; i++)
            {
                if (opcodeValue == opcodes[i].code)
                {
                    computedCommand = opcodes[i].name;
                }
            }

            // 17 cases for commands 

            if (computedCommand == "get") // get
            {
                // reads a number from input
                cin >> reg;
                if (!cin)
                {
                    cout << "break\nERROR: invalid input!\n** " << 
                    "Program terminated with an error code **" 
                    << endl; 
                    inputFailure = true;
                    ip = -1;
                }
                else
                {
                    cout << "read: ";
                    cout << reg << endl;
                    ip++;
                }
            }
            else if (computedCommand == "put") // put
            {
                // writes contents to output
                cout << "result: " << reg << endl;
                ip++;
            }
            else if (computedCommand == "ld") // ld x
            {
                // loads register with contents of memory location x
                reg = memory[labelAddress];
                ip++;
            }
            else if (computedCommand == "st") // st x
            {
                // stores contents of register in memory location x
                memory[labelAddress] = reg;
                ip++;
            }
            else if (computedCommand == "add") // add x
            {
                // adds contents of location x to register
                reg = reg + memory[labelAddress];
                ip++;
            }
            else if (computedCommand == "sub") // sub x
            {
                // subtracts contents of location x from register
                reg = reg - memory[labelAddress];
                ip++;
            }
            else if (computedCommand == "mul") // mul x
            {
                // multiplies contents of location x with register
                reg = reg * memory[labelAddress];
                ip++;
            }
            else if (computedCommand == "div") // div x
            {
                // divides contents of location x by register
                reg = reg / memory[labelAddress];
                ip++;
            }

            else if (computedCommand == "cmp") // cmp x
            {
                // compares contents of location x with register
                reg = reg - memory[labelAddress];
                ip++;
            }

            // anything below here is trash
            else if (computedCommand == "jpos") // jpos x
            {
                // jumps to location x if register is positive
                if (reg > 0)
                {
                    ip = labelAddress;
                }
                else
                {
                    ip++;
                }
            }
            else if (computedCommand == "jz") // jz x
            {
                // jumps to location x if register is 0
                if (reg == 0)
                {
                    ip = labelAddress;
                }
                else
                {
                    ip++;
                }
            }
            else if (computedCommand == "j") // j x
            {
                // jumps to location x
                ip = labelAddress;
            }
            else if (computedCommand == "jl") // jl x
            {
                // jumps to location x if register less than 0
                if (reg < 0)
                {
                    ip = labelAddress;
                }
                else
                {
                    ip++;
                }
            }
            else if (computedCommand == "jle") // jle x
            {
                // jumps to location x 
                //if register less than  or equal to 0 
                if (reg <= 0)
                {
                    ip = labelAddress;
                }
                else
                {
                    ip++;
                }
            }
            else if (computedCommand == "jg") // jg x
            {
                // jumpts to location x 
                //if register is greater than 0
                if (reg > 0)
                {
                    ip = labelAddress;
                }
                else
                {
                    ip++;
                }
            }
            else if (computedCommand == "jge") // jge x
            {
                // jumps to location x 
                //if register is greater than or equal to 0
                if (reg >= 0)
                {
                    ip = labelAddress;
                }
                else
                {
                    ip++;
                }
            }
            else if (computedCommand == "halt")
            {
                //breaks out
                ip = -1;
            }
            else // if not valid command, breaks out
            {
                ip = -1;
            }
        }
        if (inputFailure == false)
        {
            cout << "** Program terminated **" << endl;
        }
        return EXIT_SUCCESS;
    }
}