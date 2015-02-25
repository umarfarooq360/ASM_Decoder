/*
*   ASM Decoder  
*   Omar Farooq
*
*/

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <math.h>
#include "ASMDecoder.h"


ASMDecoder::ASMDecoder(string filename)
  // Specify a text file containing MIPS assembly instructions. Function
  // checks syntactic correctness of file and creates a list of Instructions.
{
  Instruction i;
  myFormatCorrect = true;


  //the file stream
  ifstream in;
  in.open(filename.c_str());
  if(in.bad()){
    	cout<<"error1"<<endl;
      myFormatCorrect = false;
  }
  else{
  	string line;  //a line
  	 //cout<<"PRINTING FILE"<<endl;
    while( getline(in, line)){
  	 //cout<<line<<endl;
  	 if (line.size()!= 32 )   {  //not a 32 bit instruction
  	     cout<<"error2"<<endl;
         myFormatCorrect = false;
  	     break;
  	 }else{     //if it is a valid instruction  
    	  string opcode("");
    	  string operand[80];
    	  string funct("");//
    	  InstType instructionType;
    	 
    	 //break up the instruction into into its components
    	  getTokens(line, opcode, operand, funct, instructionType);
    	  //builds an opcode object based on its opcode and fun field
    	  Opcode o = opcodes.getOpcode(opcode, funct);
    	  
    		//if the opcode field is undefined ERROR
    	   if(o == UNDEFINED){
    		// invalid opcode specified
      	   	cout<<"o undefined error3"<<endl;
            myFormatCorrect = false;
        		break;
    	  }
    	  
    	  //this method builds the actual instruction by using the set() method in the instruction class and sets the data fields.
    	  bool success = getOperands(i, o, operand);
    	  if(!success){
          cout<<"get operands error4"<<endl;
        		myFormatCorrect = false;
        		break;
    	  }
    	  //these methods take an instruction and call my encoding method to build a string encoding of the instruction
    	  string encoding = decode(i);
    	  //I then use the set encoding to build an instruction object out of hte string encoding
    	  i.setEncoding(encoding);
    	  //I then push the instruction onto the vector holding my list of instruction every iteration of the while loop which then will be printed to the user.
    	  myInstructions.push_back(i);

  	 }
  }

  myIndex = 0;
  }
}


Instruction ASMDecoder::getNextInstruction()
  // Iterator that returns the next Instruction in the list of Instructions.
{
  
  if(myIndex < (int)(myInstructions.size())){
	myIndex++;
	return myInstructions[myIndex-1];
  }
  
  Instruction i;
  return i;

}

void ASMDecoder::getTokens(string line, string &opcode, string *operand, string &funct, InstType &instructionType)
  // Decomposes a line of assembly code into strings for the opcode field and operands, 
  // checking for syntax errors and counting the number of operands.
{
    opcode = line.substr(0,6);   //get the opcode
    Opcode anOpcode = opcodes.getOpcode(opcode);
    instructionType = opcodes.getInstType(anOpcode);
    if(instructionType == RTYPE)    {
	    	//break down the instruction
	      operand[1] = line.substr(6,5); //rs
	      operand[2] = line.substr(11,5); //rt
	      operand[0] = line.substr(16,5); //rd
	      operand[3] = line.substr(21,5); //shammt
	      operand[4] = line.substr(26,6); //funct
	      //get the funct field 
	      funct = opcodes.getFunctField(opcodes.getOpcode(opcode, operand[4]));
	      
    }
    else if(instructionType == ITYPE)    {
	    if(opcodes.isAddress(anOpcode) == true)//is immidiate field  an address 
	    {
		      operand[0] = line.substr(6,5); //rs
		      operand[1] = line.substr(11, 5); //rt
		      operand[2] = (line.substr(16, 16)+ "00"); //immediate has to shift
	    }
	    else{  //if immediate not address
	         
		      operand[0] = line.substr(6,5); //rs
		      operand[1] = line.substr(11, 5); //rt
		      operand[2] = line.substr(16, 16); //immediate
	    }    
    }
    else if(instructionType == JTYPE) { //if the instruction is JTYPE
          char leadingBit = line.at(0); //check the leading bit and do sign extenstion
	        string signExt = (leadingBit + "" +leadingBit);
	        operand[0] = ("0000" + line.substr(6, 26) + "00"); //(immediate field(need to shift for addressing))
    }
    else{  //ERROR
     	 cerr << "Unknown Type" << endl;
    }
  }


bool ASMDecoder::getOperands(Instruction &i, Opcode o, string *operand)
  // Given an Opcode, an array of strings representing the operands
  // breaks operands apart and stores fields into Instruction.
{
  //Get the information from the opcode about the position of fields
  bool addressFlag = opcodes.isAddress(o);
  int rsPos = opcodes.RSposition(o);
  int rtPos = opcodes.RTposition(o);
  int rdPos = opcodes.RDposition(o);
  int immPos = opcodes.IMMposition(o);


  //get the instruction type and divide the operation into cases
  InstType instructionType = opcodes.getInstType(o);
  
 
  if(instructionType == RTYPE)
  {
    rdPos = 0;
    rsPos = 1;
    rtPos = 2;
  }
  //If we have an Itype instruction these are the postions we want to access for each register in the string array for each register
  if(instructionType == ITYPE)
  {
    //If there is an address field or the immediate field second this the order we want to set the register postions in. 
    if(addressFlag)
    {
      rsPos = 0;
      rtPos = 1;
    }
    if(immPos == 1)
    {
      rsPos = 0;
      rtPos = 1;
    }    
  }

  string rs, rt, rd;
  int imm=0;

	//MULTIPLE CASES: depending on position of fields 
	//Gets the name of rs rt rd   
  if(rsPos != -1){
    rs = registers.getName(cvtRegisterNum(operand[rsPos] ));
  }

  if(rtPos != -1){
    rt = registers.getName(cvtRegisterNum(operand[rtPos]));
   }
  
  if(rdPos != -1){
    rd = registers.getName(cvtRegisterNum(operand[rdPos]));
  }


  //if we have an Rtype with no immediate field 
  if(immPos != -1 && instructionType == RTYPE)
  {
    immPos = 3;
    //get the shift amount
    imm = cvtRegisterNum(operand[immPos]);
  }  
  if(instructionType == ITYPE)
  {
    //only load instructions have this immediate positions so we need to access the 
    //second position which holds the immediate field in the operands array
    if(immPos == 1)
    {
      immPos = 2;
    }
  }
  //This if statment determines if there is an immediate field in the ITYPE and if it does it
  // gets the value from the operands array and convert it from binary to decimal using my address
  // to decimal method which uses two's complement
  if(immPos != -1 && instructionType == ITYPE)
  {
    imm = cvtImmediate(operand[immPos]);
  }
  //In jump instructions the immediate field is always zero so we can set the immediate postion to 0 and then access that value from the operands 
  //array and then convert that value from binary to decimal using my address to decimal method which uses two's complement
  if(instructionType == JTYPE && immPos != -1)
  {
    immPos = 0;
    imm = cvtImmediate(operand[immPos]);
  }
  //This method takes the components I have just all set using helper methods and sets the values of the instruction I am builing using them.
  i.setValues(o, rs, rt, rd, imm);

  return true;

}

string ASMDecoder::decode(Instruction i){
  // Given a valid instruction, returns a string representing the 32 bit MIPS binary encoding
  // of that instruction.
  Opcode opcode = i.getOpcode();
  string decode = "";

  if(opcodes.getInstType(opcode) == RTYPE)
  {
    decode += (opcodes. getName(opcode) + " ");
    //This for loop prints out the correct position values by going through every position inside the instruction being passed and if 
    //it lines up with the register positions we want to print out.
    for(int index = 0; index < opcodes . numOperands(opcode); index++)
    {
      if(opcodes.RSposition(opcode) == index)
      {
        decode+=i.getRS();
      }
      if((opcodes.RTposition(opcode)) == index)
      {
        decode += (i.getRT());
      }
      if((opcodes.RDposition(opcode)) == index)
      {
        decode += (i.getRD());
      }
      if(opcodes. IMMposition(opcode) == index)
      {
        decode += getString(i.getImmediate());
      }
      //print out a comma and a space after every register being printed out until we get to the end.
      if(index < (opcodes . numOperands(opcode)-1))
      {
        decode += ", ";
      }

    }  
  }
  //ITYPE case
  else if(opcodes . getInstType(opcode) == ITYPE)
  {
    decode += opcodes .getName(opcode)+ " ";
    //immediate is only in position one if we have a load or store instruction
  if(opcodes . IMMposition(opcode) == 1)
    {
      if( opcodes . RTposition(opcode) >= 0)
      {
      decode += i.getRT() + ", "; 
      }
      //add the two's complement representation of the immediate field
      decode += getString(i.getImmediate());
      //In load instruction the last rs register needs to be surrounded by parantheses
      if( opcodes . RSposition(opcode) >= 0)
      {
      decode += ("("+(i.getRS())+")"); 
      }
    }
  else
    //If we have a non load or store instruction we print out the instruction in this order
    {
    //This checks if the intructions immediate field is an address if it is we need to use the decimalToHex field for the immediate field
    if(opcodes . isAddress(opcode))
    {
      if( opcodes . RSposition(opcode) >= 0)
      {
      decode += (i.getRS()+ ", "); 
      }
      if( opcodes . RTposition(opcode) >= 0)
      {
      decode += i.getRT() + ", "; 
      }
      //add the two's complement representation of the immediate field
      decode += decimalToHex(i.getImmediate());
    }
    //If we are building an Itype which doesnt have an address as its immedaite field we build it this way by using the getString method
    else
    {
      if( opcodes . RSposition(opcode) >= 0)
      {
      decode += (i.getRS()+ ", "); 
      }
      if( opcodes . RTposition(opcode) >= 0)
      {
      decode += i.getRT() + ", "; 
      }
      //add the two's complement representation of the immediate field
      decode += getString(i.getImmediate());
    }
  }
}
  //The only other type is a Jtype which are all constructed the same way with the name of the instruction and the twos complement hexadecimal immediate field.
  else
  {
    decode += opcodes . getName(opcode) + " ";
    if(opcodes . isIMMLabel(opcode) == true)
    {
      decode += decimalToHex(i.getImmediate());
    }

  } 

  return decode;
}


//This method takes an immediate field string and converts to two's complement 
int ASMDecoder::cvtImmediate(string n)
{
   int counter = n.length() -1; 
   int output = 0;
   if (n[0] == '1'){
	    output = (-1)*pow(2, counter);
	   //int length = n.length();
   }
 
  //Different cases for each size
	for (int i = 1; i < (int)n.length(); ++i)
	{
		counter--;
		if (n[i] == '1'){
			output += 1*pow(2, counter);
		}
	}
  
  return output;
} 

//This method takes a 5 bit binary string and converts to register number
int ASMDecoder::cvtRegisterNum(string n)
{
	if (n.length() == 5){
	 	

    return	strtol(n.c_str(), NULL , 2);
	
  }
	else{
		cerr << "ERROR: register needs to be 5 bits" << endl;
	}
  return 0;
}

//Uses stringstream hex  to convert the integer parameter into a hexadecimal string 
string ASMDecoder::decimalToHex(int n)
{
  stringstream ss;
  ss<<"0x"<<hex<<n;
  string result(ss.str());
  return result;
}

//Uses stringstream to convert int into a string
string ASMDecoder::getString(int n)
{
  stringstream ss;
  ss<<n;
  string result(ss.str());
  return result;
}
//--------------------------------------------------
