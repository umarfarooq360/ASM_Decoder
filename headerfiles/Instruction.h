#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include "OpcodeTable.h"
#include "RegisterTable.h"
#include <sstream>

/* This class provides an internal representation for a MIPS assembly instruction.
 * Any of the fields can be queried.  Additionally, the class stores a 32 bit binary
 * encoding of the MIPS instruction.
 */


class Instruction{
 public:
  // Creates a default instruction that has the opcode UNDEFINED
  Instruction();

  // Constructs new instruction and initializes fields according to arguments:
  // opcode, first source register, second source register, destination
  // register, and immediate value
  Instruction(Opcode op, string rs, string rt, string rd, int imm);

  // Destructor for the instruction
  ~Instruction() {};

  // Allows you to set all the fields of the Instruction:
  // opcode, first source register, second source register, destination
  // register, and immediate value
  void setValues(Opcode op, string rs, string rt, string rd, int imm);

  // Returns the Opcode of the instruction
  Opcode getOpcode()   { return myOpcode;}

  // Returns the register used as the first source operand
  string getRS()     { return myRS; };

  // Returns the register used as the second source operand
  string getRT()     { return myRT; };

  // Returns the register used as the destination register
  string getRD()     { return myRD; };

  // Returns the value of the instruction's immediate field
  int getImmediate()   { return myImmediate; };

  // Returns a string which represents all of the fields 
  string getString();

  // Stores the 32 bit binary encoding of MIPS instruction passed in
  void setEncoding(string s) { myEncoding = s;};

  // Returns string representing the 32 binary encoding of MIPS instruction
  string getEncoding() { return myEncoding; };

 private:
  Opcode myOpcode;
  string myRS;
  string myRT;
  string myRD;
  int myImmediate;

  string myEncoding;
};

#endif
