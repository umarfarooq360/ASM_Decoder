#include "Instruction.h"



Instruction::Instruction()
// Creates a default instruction that has the opcode UNDEFINED
{
  myOpcode = UNDEFINED;
  myRS = myRT = myRD = NumRegisters;
}

Instruction::Instruction(Opcode op, string rs, string rt, string rd, int imm)
// Constructs new instruction and initializes fields according to arguments:
// opcode, first source register, second source register, destination
// register, and immediate value
{
  setValues(op, rs, rt, rd, imm);
}


// Allows you to set all the fields of the Instruction:
// opcode, first source register, second source register, destination
// register, and immediate value
void Instruction::setValues(Opcode op, string rs, string rt, string rd, int imm)
{
  myOpcode = op;
  myRS = rs;
  myRT = rt;
  myRD = rd;
  myImmediate = imm;
}

string Instruction::getString()
// Returns a string which represents all of the fields 
{
  stringstream s ;
  s << "OP: \t" << myOpcode << "\t" << "RD: " << myRD << "\t" << 
    "RS: " << myRS << "\t" << "RT: " << "\t" << myRT << "\t" <<
    "Imm: " << myImmediate;
  
  return s.str();
  
}

