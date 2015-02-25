# makefile
# use to compile and test the assembler ASM for the STACK computer and
# its various components

DEBUG_FLAG= -DDEBUG -g -Wall
CFLAGS=-DDEBUG -g -Wall

.SUFFIXES: .cpp .o

.cpp.o:
	g++ $(CFLAGS) -c $<


ASM: ASM.o Instruction.o OpcodeTable.o RegisterTable.o ASMDecoder.o
	g++ -o ASM ASM.o OpcodeTable.o ASMDecoder.o RegisterTable.o Instruction.o

ASMDecoder.o: ASMDecoder.h OpcodeTable.h RegisterTable.h Instruction.h 

Instruction.o: OpcodeTable.h RegisterTable.h Instruction.h 

OpcodeTable.o: OpcodeTable.h 

RegisterTable.o: RegisterTable.h  

clean:
	/bin/rm -f ASM *.o core


