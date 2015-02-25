#ifndef __ASMDECODER_H__
#define __ASMDECODER_H__



#include <iostream>
#include <fstream>
#include "Instruction.h"
#include "RegisterTable.h"
#include "OpcodeTable.h"
#include <vector>
#include <sstream>
#include <stdlib.h>

/* This class reads in a MIPS assembly file and checks its syntax.  If
 * the file is syntactically correct, this class will retain a list 
 * of Instructions (one for each instruction from the file).  This
 * list of Instructions can be iterated through.
 */
using namespace std;   

class ASMDecoder{
 public:
  // Specify a text file containing MIPS assembly instructions. Function
  // checks syntactic correctness of file and creates a list of Instructions.
  ASMDecoder(string filename);

  // Returns true if the file specified was syntactically correct.  Otherwise,
  // returns false.
  bool isFormatCorrect() { return myFormatCorrect; };

  // Iterator that returns the next Instruction in the list of Instructions.
  Instruction getNextInstruction();

 private:
  vector<Instruction> myInstructions;      // list of Instructions
  int myIndex;                             // iterator index
  bool myFormatCorrect;

  RegisterTable registers;                 // encodings for registers
  OpcodeTable opcodes;                     // encodings of opcodes
  int myLabelAddress ;   // Used to assign labels addresses

  // Decomposes a line of assembly code into strings for the opcode field and operands, 
  // checking for syntax errors and counting the number of operands.
  void getTokens(string line, string &opcode, string *operand, string &funct, InstType &Insty);

  // Given an Opcode, a string representing the operands, and the number of operands, 
  // breaks operands apart and stores fields into Instruction.
  bool getOperands(Instruction &i, Opcode o, string *operand );

  //Converts an integer to a decimal string 
  string getString(int n);

  //used to convert a decimal integer into a hexadecimal string.
  string decimalToHex(int n); 
  
  //Converts a binary string representing a register number to its int value
  int cvtRegisterNum(string n);
  
  //This method converts a binary string representing the immediate fild to its int value 
  //taking into account twos complmnt for negative values
  int cvtImmediate(string n);

    // Converts a 32bit binary instruction to its decoded form
  string decode(Instruction i);

};

#endif
