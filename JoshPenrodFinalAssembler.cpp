//Joshua Penrod
//CSC 225
// Project Assembler Part 8
// This project is an assembler that will take commands written in an asm file, place them into memory, and perform the appropriate actions based off of what the commands the asm file contains.

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctype.h>


using namespace std;
char ASM_FILE_NAME[] = "IanAndJoshFinalProject.asm";

const int MAX = 1000;  //size of simulators memory
const int COL = 10;	 //number of columns for output

			//REGISTERS
const int AXREG = 0;
const int BXREG = 1;
const int CXREG = 2;
const int DXREG = 3;

         //commands
const int HALT = 5;
const int MOVREG = 192;
const int MOVMEM = 224;
const int ADD = 160;
const int CMP = 96;
const int JE = 8;
const int JNE = 9;
const int JB = 10;
const int JBE = 11;
const int JA = 12;
const int JAE = 13;
const int JMP = 14;
const int PUT = 7;
const int GET = 6;
const int FUN = 3;
const int RET = 4;
const int PUTC = 2;

enum paramType {reg, mem, constant, arrayBx, arrayBxPlus, none};

typedef short int Memory;

class Registers
{
public:
	int AX;
	int BX;
	int CX;
	int DX;
	int instrAddr;
	int flag;
}regis;
Memory memory[MAX] = {0};
int address;

void printMemoryDump(); //prints memeory with integers commands
void convertToNumber(string line, int &start, int &value);
int whichReg(char regLetter);  //returns the number of the letter registar
void fillMemory( );// takes lines from asm file and puts them in code
void splitCommand(string line, string &command, string &oper1, string &oper2);//decodes the line from the asm file for convertToMachineCode
int checkOper(string checkedOper); //returns a value based off of the oper passed in  
void runCode(); // takes values from memory and places them into registers
void convertToMachineCode(ifstream &fin); //takes code from asm file and puts it into the memory array
void throwFlag(int compare1, int compare2);// takes in two values and changes the flag based on how they compare
void displayIntro();// gives the user some simple info at the beginning of the code
void runAddCommands(Memory reg, int midBits, int bottomBits);//function to excecute add commands 
void runMovRegCommands(Memory reg, int midBits, int bottomBits);//function to excecute move register commands
void runMovMemCommands(Memory reg, int midBits, int bottomBits);//function to excecute move memory commands 
void runCompareCommands(Memory reg, int midBits, int bottomBits);// function to excecute compare commands 
void runJumpCommands(Memory reg, int midBits, int bottomBits);// function to excecute jump commands 
void runFunCommands();//function to excecute fun commands
void runRetCommands();//function to excecute return commands
void printCommandInfo(Memory command, int topBits, int midBits, int bottomBits);// troubleshooting function to display info about a command
void setStack();// saves the state of a few important elements before running through function
int valInReg(int regID);// returns the contents of the requested register
int ripBrackets(string rippedOper);// rips the brackets off a memory address
void convertMovCommands(string oper1, string oper2);// converts all MOV commands to machine code
void convertAddCommands(string oper1, string oper2);// converts all ADD commands to machine code
void convertCompareCommands(string oper1, string oper2);// converts all CMP commands to machine code
void convertJumpCommands(string command, string oper1);// converts all JMP commands to machine code
void convertFunCommands(string oper1, string oper2, string command, ifstream &fin, string line);// converts all FUN commands to machine code

int main( )
{
	//displayIntro();
	fillMemory( );
	runCode();
	//printMemoryDump();
	cout << endl;
	system( "pause" );
	return 0;
}


//fillMemory
//Changes the asm code into numbers and places those numbers into memory.
void fillMemory( )
{
	address = 0;
	ifstream fin;
	fin.open( ASM_FILE_NAME );
	if (fin.fail( ))
	{
		cout << "Error, file didn't open\n";
		system( "pause" );
		exit( 1 );
	}

	for (int i = 0; i< MAX && !fin.fail(); i++)
	{		
		convertToMachineCode( fin );
	}
	cout<<"memory filled\n";
	
}


//whichReg
//Takes in a letter from a reg and returns the cooresponding number.
int whichReg(char regLetter)
{
	if (regLetter == 'a')
	{
		return AXREG;
	}
	else if (regLetter == 'b')
	{
		return BXREG;
	}
	else if (regLetter == 'c')
	{
		return CXREG;
	}
	else if (regLetter == 'd')
	{
		return DXREG;
	}
	return -1;  //something went wrong if -1 is returned
}


//convertToNumber
//Takes in a line and converts the digits to an integer.
//line: string of code
//start: beginning of number to be returned
//value: the value once line is converted to an int
void convertToNumber(string line, int &start, int &value)
{
	char number[16];
	bool negative = false;
//	cout<< "in convertToNumber before function 1  start is "<<start<<endl;
	int i = 0;
	if (line[start] == '-')
	{
		start++;
		negative = true;
	}
	while (i<16 && line.size() > start&& isdigit(line[start]))
	{
		number[i] = line[start];
					//	cout<<line[start];
		i++;
		start++;
				//		cout<<i<<start;
	}
	number[i] = '\0';
	value = atoi(number);
	if (negative == true)
	{
		value = -value;
	}
//	cout<< "in convertToNumber after function 1  start is "<<start<<endl;
}


//printMemoryCommands
// Prints the memory with letter commands.
void printMemoryCommands()
{
	int i= 0;
	int count;  // which column is currently outputting
	int moveColumn = MAX / COL;
	int index=0;
	while ( i< MAX)
	{
		count = 0;
		while (count < COL && index < MAX)
		{
			index = i + count * moveColumn;
			//As each type of instruction is added you will need to be able to deal with type of command
			//printcommand(index)
		}
		cout<<endl;
			
	}
	cout<<"RA "<< regis.AX << "  BX "<< regis.BX << "  CX "<< regis.CX << "  DX "<< regis.DX<<endl;
	cout<<"\tinstruction "<<regis.instrAddr<<"\tflag "<<regis.flag<<"\n\n";
}


//printMemoryDump
// Displays the array that contains all of the commands that have been placed into memory.
void printMemoryDump()
{
	int numRows=MAX/COL+1;   //number of rows that will print
	int carryOver=MAX%COL;   //number of columns on the bottom row
	int location;   //the current location being called
	cout << "*********************************************************************************************************************" << endl;
	for(int row=0;row<numRows-1;row++)
	{
		cout << "*";
		location=row;
		for(int column=0;location<MAX&&column<COL;column++)
		{
			if(!(numRows-1==row&&carryOver-1<column))
			{
				cout<<setw(5)<<location<<"."<<setw(5)<<memory[location];
				location+=(numRows-(carryOver-1<column));
			}
		}
		cout << "     *";
		cout<<endl;
	}
	cout << "*********************************************************************************************************************" << endl;

	cout<<endl;
	cout<<"AX: "<<regis.AX<<'\t';
	cout<<"BX: "<<regis.BX<<'\t';
	cout<<"CX: "<<regis.CX<<'\t';
	cout<<"DX: "<<regis.DX<<'\t';
	cout<<endl<<endl;
	cout<<"Instruction: "<<address<<endl;
	cout<<"Flag: "<<regis.flag;
	
	cout<<endl<<endl;
}


//changeToLowerCase
//Takes in a line and spits it out with all characters switched to lowercase.
//line: the line to change to lower case
void changeToLowerCase(string &line)
{
	
	int index=0;
	while (index < line.size())
	{
		line[index] = tolower(line[index]);
		index++;
	}
//	cout<<"the line in change"<<line;

}


//splitCommand
//Takes the line from the ASM file and breaks it into a command a oper1 and an oper2
//line: the entire line from the asm file
//command: what the line is supposed to do
//oper1: what register it is supposed to do it to
//oper2: the number it is supposed to put into oper 1
void splitCommand(string line, string &command, string &oper1, string &oper2)
{
	if (line[0] == ';')
	{
		line.erase(line.find(';'), line.length());
	}
	else
	{
		if (line.find(';') != -1)
		{
			line.erase(line.find(';'), line.length());
		}
		int space1 = line.find(' ');
		int space2 = line.rfind(' ');
		command = line.substr(0, space1);
		oper1 = line.substr(space1 + 1, (space2 - space1) - 1);
		oper2 = line.substr(space2 + 1);
	}
}


//runCode
//Calls functions based off of what the numbers in memory indicate.
void runCode()
{
	Memory command;
	Memory topBits, midBits, bottomBits;
	Memory reg = 0;
	address = 0;
	while (memory[address] != 5)
	{
		//cout << "address: " << address << endl;
		command = memory[address];
		topBits = command & 224;
		midBits = command & 24;
		bottomBits = command & 7;
		//printCommandInfo(command, topBits, midBits, bottomBits);
		address++;
		if (topBits == MOVREG)
		{
			runMovRegCommands(reg, midBits, bottomBits);
		}
		else if (topBits == ADD)
		{
			runAddCommands(reg, midBits, bottomBits);
		}
		else if (topBits == MOVMEM)
		{
			runMovMemCommands(reg, midBits, bottomBits);
		}
		else if (topBits == CMP)
		{
			runCompareCommands(reg, midBits, bottomBits);
		}
		else if (topBits == 0 && midBits == 8)
		{
			runJumpCommands(reg, midBits, bottomBits);
		}
		else if (topBits == 0 && midBits == 0 && bottomBits == PUT)// put command
		{
			cout << "Put Command: The Contents of AX are: " << regis.AX << endl;
		}
		else if (topBits == 0 && midBits == 0 && bottomBits == GET)// get command
		{
			cout << "Get Command: Type in the value you want to go into AX: " << endl;
			cin >> regis.AX;
		}
		else if (topBits == 0 && midBits == 0 && bottomBits == FUN)
		{
			runFunCommands();
		}
		else if(topBits == 0 && midBits == 0 && bottomBits == RET)
		{
			runRetCommands();
		}
		else if (topBits == 0 && midBits == 0 && bottomBits == PUTC)
		{
			cout << (char)regis.AX;
		}
	}

}


//throwFlag
//takes in two integers and changes the flag based off of how the numbers compare
//compare1: the first integer
//compare2: the second integer
void throwFlag(int compare1, int compare2)
{
	if (compare1 > compare2)
	{
		regis.flag = 1;
	}
	else if (compare1 == compare2)
	{
		regis.flag = 0;
	}
	else
	{
		regis.flag = -1;
	}
}


//convertToMachineCode
//Takes the command and operands from split command and puts them into the memory array.
//fin: the input from our file
void convertToMachineCode(ifstream &fin)  
{
	string line;  //full command
	string command; //the asm commmand
	string oper1, oper2;  //the two operands could be empty
	getline(fin, line, '\n');
	changeToLowerCase(line);
	splitCommand(line, command, oper1, oper2);
	if (command[0] == 'h')  //halt
	{
		memory[address] = HALT;
	}
	if (command[0] == 'm')  //move into a register
	{
		convertMovCommands(oper1, oper2);
	}
	if (command[0] == 'a')//add 
	{
		convertAddCommands(oper1, oper2);
	}
	if (isdigit(command[0]))// placing a basic integer into memory
	{
		memory[address] = stoi(oper1);
	}
	if (command[0] == 'c')//compare
	{
		convertCompareCommands(oper1, oper2);
	}
	if (command [0] == 'j')// jump
	{
		convertJumpCommands(command, oper1);
	}
	if (command[0]== 'p')// put command
	{
		if (command[3] == 'c')
		{
			memory[address] = PUTC;
		}
		else
		{
			memory[address] = PUT;
		}
	}
	if (command[0] == 'g')// get command
	{
		memory[address] = GET;
	}
	if (command[0] == 'f')// function 
	{
		convertFunCommands(oper1, oper2, command, fin, line);
	}
	if (command[0] == 'r')
	{
		memory[address] = RET;
		address++;
		memory[address] = ripBrackets(oper1);
	}
	address++;
}


//checkOper
//Returns a value to help build the total of the final commands that go into memory.
//checkedOper: string to see if it is a constant, mem address, register, etc...
int checkOper(string checkedOper)
{
	int returnVal = 7;
	if (checkedOper[0] == 'a')
	{
		returnVal = 0;
	}
	else if (checkedOper[0] == 'b')
	{
		returnVal = 1;
	}
	else if (checkedOper[0] == 'c')
	{
		returnVal = 2;
	}
	else if (checkedOper[0] == 'd')
	{
		returnVal = 3;
	}
	if (checkedOper[0] == '[')
	{
		returnVal = 6;
		if (checkedOper[1] == 'b')
		{
			returnVal = 4;
			if(checkedOper[3] == '+')
			{
				returnVal = 5;
			}
		}
	}
	return returnVal;
}


//displayIntro
//A simple function that displays some instruction for the user at the beginning of the program.
void displayIntro()
{
	cout << "********************************************************************" << endl;
	cout << "*  This program will send the user to a function and check inputs  *" << endl;
	cout << "*    until the user enters a value between zero and one-hundred.   *" << endl;
	cout << "********************************************************************" << endl;
}


//printCommandInfo
//A troubleshooting function that displays the info that a command contains when broken down, meant to be called in runCode.
//command: the entire command line from memory
//topBits: result of command being anded with 224
//midBits: result of command being anded with 24
//bottomBits: result of command being anded with 7
void printCommandInfo(Memory command, int topBits, int midBits, int bottomBits)
{
	cout << "COMMAND: " << command << endl;
	cout << "Top Bits: " << topBits << endl;
	cout << "Mid Bits: " << midBits << endl;
	cout << "Bottom Bits: " << bottomBits << endl;
}


//jump
//This function contains the infrastructure to deal with all jump commands.
//reg: the register being dealt with
//midBits: result of command being anded with 24
//bottomBits: result of command being anded with 7
void runJumpCommands(Memory reg, int midBits, int bottomBits)
{
	if (bottomBits == 0)
	{
		if (regis.flag == 0)// je
		{
			address = memory[address];
		}
		else
		{
			address++;
		}
	}
	else if (bottomBits == 1)// jne
	{
		if (regis.flag != 0)
		{
			address = memory[address];
		}
		else
		{
			address++;
		}
	}
	else if (bottomBits == 2)// jb
	{
		if (regis.flag == -1)
		{
			address = memory[address];
		}
		else
		{
			address++;
		}
	}
	else if (bottomBits == 3)// jbe
	{
		if (regis.flag == -1 || regis.flag == 0)
		{
			address = memory[address];
		}
		else
		{
			address++;
		}
	}
	else if (bottomBits == 4)// ja
	{
		if (regis.flag == 1)
		{
			address = memory[address];
		}
		else
		{
			address++;
		}
	}
	else if (bottomBits == 5)//jae
	{
		if (regis.flag == 1 || regis.flag == 0)
		{
			address = memory[address];
		}
		else
		{
			address++;
		}
	}
	else if (bottomBits == 6)//jmp
	{
		address = memory[address];
	}
}


//compare
//This function contains the infrastructure to deal with all compare commands.
//reg: the register being dealt with
//midBits: result of command being anded with 24
//bottomBits: result of command being anded with 7
void runCompareCommands(Memory reg, int midBits, int bottomBits)
{
	reg = midBits >> 3;
	if (bottomBits == 6)// comparing a register to a value in a memory location
	{
		if (reg == AXREG)
		{
			throwFlag(regis.AX, memory[memory[address]]);
		}
		else if (reg == BXREG)
		{
			throwFlag(regis.BX, memory[memory[address]]);
		}
		else if (reg == CXREG)
		{
			throwFlag(regis.CX, memory[memory[address]]);
		}
		else if (reg == DXREG)
		{
			throwFlag(regis.DX, memory[memory[address]]);
		}
		address++;
	}
	else if (bottomBits < 4)//comparing a register to a register
	{
		if (reg == AXREG)
		{
			if (bottomBits == 0)
			{
				throwFlag(regis.AX, regis.AX);
			}
			else if (bottomBits == 1)
			{
				throwFlag(regis.AX, regis.BX);
			}
			else if (bottomBits == 2)
			{
				throwFlag(regis.AX, regis.CX);
			}
			else if (bottomBits == 3)
			{
				throwFlag(regis.AX, regis.DX);
			}
		}
		else if (reg == BXREG)
		{
			if (bottomBits == 0)
			{
				throwFlag(regis.BX, regis.AX);
			}
			else if (bottomBits == 1)
			{
				throwFlag(regis.BX, regis.BX);
			}
			else if (bottomBits == 2)
			{
				throwFlag(regis.BX, regis.CX);
			}
			else if (bottomBits == 3)
			{
				throwFlag(regis.BX, regis.DX);
			}
		}
		else if (reg == CXREG)
		{
			if (bottomBits == 0)
			{
				throwFlag(regis.CX, regis.AX);
			}
			else if (bottomBits == 1)
			{
				throwFlag(regis.CX, regis.BX);
			}
			else if (bottomBits == 2)
			{
				throwFlag(regis.CX, regis.CX);
			}
			else if (bottomBits == 3)
			{
				throwFlag(regis.CX, regis.DX);
			}
		}
		else if (reg == DXREG)
		{
			if (bottomBits == 0)
			{
				throwFlag(regis.DX, regis.AX);
			}
			else if (bottomBits == 1)
			{
				throwFlag(regis.DX, regis.BX);
			}
			else if (bottomBits == 2)
			{
				throwFlag(regis.DX, regis.CX);
			}
			else if (bottomBits == 3)
			{
				throwFlag(regis.DX, regis.DX);
			}
		}
	}
	else if (bottomBits == 7)// comparing a register to a constant
	{
		if (reg == AXREG)
		{
			throwFlag(regis.AX, memory[address]);
		}
		else if (reg == BXREG)
		{
			throwFlag(regis.BX, memory[address]);
		}
		else if (reg == CXREG)
		{
			throwFlag(regis.CX, memory[address]);
		}
		else if (reg == DXREG)
		{
			throwFlag(regis.DX, memory[address]);
		}
		address++;
	}
	else if (bottomBits == 4)
	{
		if (reg == AXREG)
		{
			throwFlag(regis.AX, memory[regis.BX]);
		}
		else if (reg == BXREG)
		{
			throwFlag(regis.BX, memory[regis.BX]);
		}
		else if (reg == CXREG)
		{
			throwFlag(regis.CX, memory[regis.BX]);
		}
		else if (reg == DXREG)
		{
			throwFlag(regis.DX, memory[regis.BX]);
		}
	}
	else if (bottomBits == 5)
	{
		if (reg == AXREG)
		{
			throwFlag(regis.AX, memory[regis.BX+memory[address]]);
		}
		else if (reg == BXREG)
		{
			throwFlag(regis.BX, memory[regis.BX + memory[address]]);
		}
		else if (reg == CXREG)
		{
			throwFlag(regis.CX, memory[regis.BX + memory[address]]);
		}
		else if (reg == DXREG)
		{
			throwFlag(regis.DX, memory[regis.BX + memory[address]]);
		}
		address++;
	}
}


//movMem
//This function contains the infrastructure to deal with all move memory commands.
//reg: the register being dealt with
//midBits: result of command being anded with 24
//bottomBits: result of command being anded with 7
void runMovMemCommands(Memory reg, int midBits, int bottomBits)
{
	reg = midBits >> 3;
	if (bottomBits == 4 )
	{
	
		if (reg == AXREG)
		{
			memory[regis.BX] = regis.AX;
		}
		else if (reg == BXREG)
		{
			memory[regis.BX] = regis.BX;
		}
		else if (reg == CXREG)
		{
			memory[regis.BX] = regis.CX;
		}
		else if (reg == DXREG)
		{
			memory[regis.BX] = regis.DX;
		}
	}
	if (bottomBits == 6)
	{
		if (reg == AXREG)
		{
			memory[memory[address]] = regis.AX;
		}
		else if (reg == BXREG)
		{
			memory[memory[address]] = regis.BX;
		}
		else if (reg == CXREG)
		{
			memory[memory[address]] = regis.CX;
		}
		else if (reg == DXREG)
		{
			memory[memory[address]] = regis.DX;
		}
		address++;
	}
	if (bottomBits == 5)
	{

		if (reg == AXREG)
		{
			memory[regis.BX + memory[address]] = regis.AX;
		}
		else if (reg == BXREG)
		{
			memory[regis.BX + memory[address]] = regis.BX;
		}
		else if (reg == CXREG)
		{
			memory[regis.BX + memory[address]] = regis.CX;
		}
		else if (reg == DXREG)
		{
			memory[regis.BX + memory[address]] = regis.DX;
		}
		address++;
	}
}


//add
//This function contains the infrastructure to deal with all move memory commands.
//reg: the register being dealt with
//midBits: result of command being anded with 24
//bottomBits: result of command being anded with 7
void runAddCommands(Memory reg, int midBits, int bottomBits)
{
	reg = midBits >> 3;
	if (bottomBits == 7)// adding a constant to the value already in a register 
	{
		if (reg == AXREG)
		{
			regis.AX += memory[address];
		}
		else if (reg == BXREG)
		{
			regis.BX += memory[address];
		}
		else if (reg == CXREG)
		{
			regis.CX += memory[address];
		}
		else if (reg == DXREG)
		{
			regis.DX += memory[address];
		}
		address++;
	}
	else if (bottomBits < 4)// adding register to register
	{
		if (reg == AXREG)
		{
			if (bottomBits == 0)
			{
				regis.AX += regis.AX;
			}
			else if (bottomBits == 1)
			{
				regis.AX += regis.BX;
			}
			else if (bottomBits == 2)
			{
				regis.AX += regis.CX;
			}
			else if (bottomBits == 3)
			{
				regis.AX += regis.DX;
			}
		}
		else if (reg == BXREG)
		{
			if (bottomBits == 0)
			{
				regis.BX += regis.AX;
			}
			else if (bottomBits == 1)
			{
				regis.BX += regis.BX;
			}
			else if (bottomBits == 2)
			{
				regis.BX += regis.CX;
			}
			else if (bottomBits == 3)
			{
				regis.BX += regis.DX;
			}
		}
		else if (reg == CXREG)
		{
			if (bottomBits == 0)
			{
				regis.CX += regis.AX;
			}
			else if (bottomBits == 1)
			{
				regis.CX += regis.BX;
			}
			else if (bottomBits == 2)
			{
				regis.CX += regis.CX;
			}
			else if (bottomBits == 3)
			{
				regis.CX += regis.DX;
			}
		}
		else if (reg == DXREG)
		{
			if (bottomBits == 0)
			{
				regis.DX += regis.AX;
			}
			else if (bottomBits == 1)
			{
				regis.DX += regis.BX;
			}
			else if (bottomBits == 2)
			{
				regis.DX += regis.CX;
			}
			else if (bottomBits == 3)
			{
				regis.DX += regis.DX;
			}
		}
	}
	else if (bottomBits == 6)// adding a value in memory to a register
	{
		if (reg == AXREG)
		{
			regis.AX += memory[memory[address]];
		}
		else if (reg == BXREG)
		{
			regis.BX += memory[memory[address]];
		}
		else if (reg == CXREG)
		{
			regis.CX += memory[memory[address]];
		}
		else if (reg == DXREG)
		{
			regis.DX += memory[memory[address]];
		}
		address++;
	}
	else if (bottomBits == 4)
	{
		if (reg == AXREG)
		{
			regis.AX += memory[regis.BX];
		}
		else if (reg == BXREG)
		{
			regis.BX += memory[regis.BX];
		}
		else if (reg == CXREG)
		{
			regis.CX += memory[regis.BX];
		}
		else if (reg == DXREG)
		{
			regis.DX += memory[regis.BX];
		}
	}
	else if (bottomBits == 5)
	{
		if (reg == AXREG)
		{
			regis.AX += memory[regis.BX + memory[address]];
		}
		else if (reg == BXREG)
		{
			regis.BX += memory[regis.BX + memory[address]];
		}
		else if (reg == CXREG)
		{
			regis.CX += memory[regis.BX + memory[address]];
		}
		else if (reg == DXREG)
		{
			regis.DX += memory[regis.BX + memory[address]];
		}
		address++;
	}
}


//movReg
//This function contains the infrastructure to deal with all move register commands.
//reg: the register being dealt with
//midBits: result of command being anded with 24
//bottomBits: result of command being anded with 7
void runMovRegCommands(Memory reg, int midBits, int bottomBits)
{
	
	reg = midBits >> 3;
	if (bottomBits == 7)// moving a constant into a register 
	{
		
		if (reg == AXREG)
		{
			regis.AX = memory[address];
		}
		else if (reg == BXREG)
		{
			regis.BX = memory[address];
		}
		else if (reg == CXREG)
		{
			regis.CX = memory[address];
		}
		else if (reg == DXREG)
		{
			regis.DX = memory[address];
		}
		address++;
	}
	else if (bottomBits == 6)// moving a value in a memory address into a register
	{
		if (reg == AXREG)
		{
			regis.AX = memory[memory[address]];
		}
		else if (reg == BXREG)
		{
			regis.BX = memory[memory[address]];
		}
		else if (reg == CXREG)
		{
			regis.CX = memory[memory[address]];
		}
		else if (reg == DXREG)
		{
			regis.DX = memory[memory[address]];
		}
		address++;
	}
	else if (bottomBits == 4)
	{
		if (reg == AXREG)
		{
			regis.AX = memory[regis.BX];
		}
		else if (reg == BXREG)
		{
			regis.BX = memory[regis.BX];
		}
		else if (reg == CXREG)
		{
			regis.CX = memory[regis.BX];
		}
		else if (reg == DXREG)
		{
			regis.DX = memory[regis.BX];
		}
	}
	else if (bottomBits < 4)// moving a register into a register
	{
		if (reg == AXREG)
		{
			if (bottomBits == 0)
			{
				regis.AX = regis.AX;
			}
			else if (bottomBits == 1)
			{
				regis.AX = regis.BX;
			}
			else if (bottomBits == 2)
			{
				regis.AX = regis.CX;
			}
			else if (bottomBits == 3)
			{
				regis.AX = regis.DX;
			}
		}
		else if (reg == BXREG)
		{
			if (bottomBits == 0)
			{
				regis.BX = regis.AX;
			}
			else if (bottomBits == 1)
			{
				regis.BX = regis.BX;
			}
			else if (bottomBits == 2)
			{
				regis.BX = regis.CX;
			}
			else if (bottomBits == 3)
			{
				regis.BX = regis.DX;
			}
		}
		else if (reg == CXREG)
		{
			if (bottomBits == 0)
			{
				regis.CX = regis.AX;
			}
			else if (bottomBits == 1)
			{
				regis.CX = regis.BX;
			}
			else if (bottomBits == 2)
			{
				regis.CX = regis.CX;
			}
			else if (bottomBits == 3)
			{
				regis.CX = regis.DX;
			}
		}
		else if (reg == DXREG)
		{
			if (bottomBits == 0)
			{
				regis.DX = regis.AX;
			}
			else if (bottomBits == 1)
			{
				regis.DX = regis.BX;
			}
			else if (bottomBits == 2)
			{
				regis.DX = regis.CX;
			}
			else if (bottomBits == 3)
			{
				regis.DX = regis.DX;
			}
		}
	}
	else if (bottomBits == 5)
	{	
		if (reg == AXREG)
		{
			regis.AX = memory[regis.BX + memory[address]];
		}
		else if (reg == BXREG)
		{
			regis.BX = memory[regis.BX + memory[address]];
		}
		else if (reg == CXREG)
		{
			regis.CX = memory[regis.BX + memory[address]];
		}
		else if (reg == DXREG)
		{
			regis.DX = memory[regis.BX + memory[address]];
		}
		address++;
	}
}


//setStack
//This function will assign the state of the flag and registers and save a return address to the last few locations in memory.
void setStack()
{
	memory[MAX - 1] = address;
	memory[MAX - 2] = regis.flag;
	memory[MAX - 3] = regis.AX;
	memory[MAX - 4] = regis.BX;
	memory[MAX - 5] = regis.CX;
	memory[MAX - 6] = regis.DX;
}


//valInReg
//This function will take in a reg id, (0,1,2, or 4) and return the value in the cooresponding register.
//regId: the register being passed in
int valInReg(int regID)
{
	int returnVal = 0;
	if (regID == 0)
	{
		returnVal = regis.AX;
	}
	else if (regID == 1)
	{
		returnVal = regis.BX;
	}
	else if (regID == 2)
	{
		returnVal = regis.CX;
	}
	else if (regID == 3)
	{
		returnVal = regis.DX;
	}
	return returnVal;
}


//runFunction
//This function is called in run code and processes the fun command.
void runFunCommands()
{
	int funStart = 0;
	int paramNum = 0;
	funStart = memory[address];
	address++;
	paramNum = memory[address];
	address++;
	for (int i = 0; i < paramNum; i++)
	{
		if (memory[address] < 4)
		{
			memory[funStart - (i + 1)] = valInReg(memory[address]);
			address++;
		}
		else if (memory[address] == 6)
		{
			address++;
			memory[funStart - (i + 1)] = memory[memory[address]];
			address++;
		}
		else if (memory[address] == 7)
		{
			address++;
			memory[funStart - (i + 1)] = memory[address];
			address++;
		}
	}
	if (paramNum == 0)
	{
		//address++;
	}
	setStack();
	address = funStart;
}


//retExcecute
//This function will deal with the ret command
void runRetCommands()
{
	memory[memory[address]] = regis.AX;
	address = memory[MAX - 1];
	regis.flag = memory[MAX - 2];
	regis.AX = memory[MAX - 3];
	regis.BX = memory[MAX - 4];
	regis.CX = memory[MAX - 5];
	regis.DX = memory[MAX - 6];
}


//ripBrackets
//This function will rip the brackets off of a string that is a memory location
//rippedOper: This is the string to rip the brackets off of 
int ripBrackets(string rippedOper)
{
	int bracket = 0;
	int plus = 0;
	int answer = 0;
	if (rippedOper[3] == '+')
	{
		plus = rippedOper.find('+');
		rippedOper = rippedOper.substr(plus + 1, rippedOper.length() - 2);
		answer = stoi(rippedOper);
	}
	else
	{
		bracket = rippedOper.find('[');
		rippedOper = rippedOper.substr(bracket + 1, rippedOper.length() - 2);
		answer = stoi(rippedOper);
	}
	return answer;
}


//convertMovCommands
//This function is called in convertToMachineCode and enters all mov commands as numbers into memory
//oper1: The first operand of a command 
//oper2: The second operand of a command
void convertMovCommands(string oper1, string oper2)
{
	if (checkOper(oper1) == 6) // moving a value to memory
	{
		memory[address] = MOVMEM;
		memory[address] += (whichReg(oper2[0]) << 3);
		memory[address] += 6;
		address++;
		memory[address] = ripBrackets(oper1);
	}
	else if (checkOper(oper2) == 7)// moving a constant to a register
	{
		memory[address] = MOVREG;
		memory[address] += (whichReg(oper1[0]) << 3);
		memory[address] += 7;
  		address++;
		memory[address] = stoi(oper2);
	}
	else if (checkOper(oper2) == 6)// moving a value in memory to a register
	{
		memory[address] = MOVREG;
		memory[address] += (whichReg(oper1[0]) << 3);
		memory[address] += 6;
		address++;
		memory[address] = ripBrackets(oper2);
	}
	else if (checkOper(oper2) < 4 && checkOper(oper1) != 4 && checkOper(oper2) != 5 && checkOper(oper1) != 5)// moving a register to a register
	{
		memory[address] = MOVREG;
		memory[address] += (whichReg(oper1[0]) << 3);
		memory[address] += checkOper(oper2);
	}
	else if (checkOper(oper2) == 4)// moving a value by reference to a register
	{
		memory[address] = MOVREG;
		memory[address] += (whichReg(oper1[0]) << 3);
		memory[address] += 4;
	}
	else if (checkOper(oper1) == 4)//moving a value in a register to a referenced memory location
	{
		memory[address] = MOVMEM;
		memory[address] += (whichReg(oper2[0]) << 3);
		memory[address] += 4;
	}
	else if (checkOper(oper2) == 5)// moving a value to a register from address[bx+xxxx]
	{
		memory[address] = MOVREG;
		memory[address] += (whichReg(oper1[0]) << 3);
		memory[address] += 5;
		address++;
		memory[address] = ripBrackets(oper2);
	}
	else if (checkOper(oper1) == 5)
	{
		memory[address] = MOVMEM;
		memory[address] += (whichReg(oper2[0]) << 3);
		memory[address] += 5;
		address++;
		memory[address] = ripBrackets(oper1);
	}
}


//convertAddCommands
//This function is called in convertToMachineCode and enters all add commands as numbers into memory
//oper1: The first operand of a command 
//oper2: The second operand of a command
void convertAddCommands(string oper1, string oper2)
{
	if (checkOper(oper2) == 7)// adding a constant to a register
	{
		memory[address] = ADD;
		memory[address] += (whichReg(oper1[0]) << 3);
		memory[address] += 7;
		address++;
		memory[address] = stoi(oper2);
	}
	else if (checkOper(oper2) == 6)// adding a memory location to a register
	{
		memory[address] = ADD;
		memory[address] += (whichReg(oper1[0]) << 3);
		memory[address] += 6;
		address++;
		memory[address] = ripBrackets(oper2);
	}
	else if (checkOper(oper2) < 4 && checkOper(oper1) != 4 && checkOper(oper2) != 5 && checkOper(oper1) != 5)// adding a register to a register
	{
		memory[address] = ADD;
		memory[address] += (whichReg(oper1[0]) << 3);
		memory[address] += checkOper(oper2);
	}
	else if (checkOper(oper2) == 4)// adding a value by reference to a register
	{
		memory[address] = ADD;
		memory[address] += (whichReg(oper1[0]) << 3);
		memory[address] += 4;
	}
	else if (checkOper(oper2) == 5)// adding a value to a register from address[bx+xxxx]
	{
		memory[address] = ADD;
		memory[address] += (whichReg(oper1[0]) << 3);
		memory[address] += 5;
		address++;
		memory[address] = ripBrackets(oper2);
	}
}


//convertCMPCommands
//This function is called in convertToMachineCode and enters all compare commands as numbers into memory
//oper1: The first operand of a command 
//oper2: The second operand of a command
void convertCompareCommands(string oper1, string oper2)
{
	if (checkOper(oper2) == 6)
	{
		memory[address] = CMP;
		memory[address] += (whichReg(oper1[0]) << 3);
		memory[address] += 6;
		address++;
		memory[address] = ripBrackets(oper2);
	}
	else if (checkOper(oper2) == 7)
	{
		memory[address] = CMP;
		memory[address] += (whichReg(oper1[0]) << 3);
		memory[address] += 7;
		address++;
		memory[address] = stoi(oper2);
	}
	else if (checkOper(oper2) < 4)
	{
		memory[address] = CMP;
		memory[address] += (whichReg(oper1[0]) << 3);
		memory[address] += checkOper(oper2);
	}
	else if (checkOper(oper2) == 4)
	{
		memory[address] = CMP;
		memory[address] += (whichReg(oper1[0]) << 3);
		memory[address] += 4;
	}
	else if (checkOper(oper2) == 5)
	{
		memory[address] = CMP;
		memory[address] += (whichReg(oper1[0]) << 3);
		memory[address] += 5;
		address++;
		memory[address] = ripBrackets(oper2);
	}
}


//convertJumpCommands
//This function is called in convertToMachineCode and enters all jump commands as numbers into memory
//command: the command type that will be excecuted
//oper1: The first operand of a command
void convertJumpCommands(string command, string oper1)
{
	if (command[1] == 'e')
	{
		memory[address] = JE;
		address++;
		memory[address] = ripBrackets(oper1);
	}
	else if (command[1] == 'n')
	{
		memory[address] = JNE;
		address++;
		memory[address] = ripBrackets(oper1);
	}
	else if (command[1] == 'a')
	{
		if (command[2] == 'e')
		{
			memory[address] = JAE;
			address++;
			memory[address] = ripBrackets(oper1);
		}
		else
		{
			memory[address] = JA;
			address++;
			memory[address] = ripBrackets(oper1);
		}
	}
	else if (command[1] == 'b')
	{
		if (command[2] == 'e')
		{
			memory[address] = JBE;
			address++;
			memory[address] = ripBrackets(oper1);
		}
		else
		{
			memory[address] = JB;
			address++;
			memory[address] = ripBrackets(oper1);
		}
	}
	else if (command[1] == 'm')
	{
		memory[address] = JMP;
		address++;
		memory[address] = ripBrackets(oper1);
	}
}

//convertFunCommands
//This function is called in convertToMachineCode and enters all function commands as numbers into memory
//oper1: The first operand of a command 
//oper2: The second operand of a command
//command: the command type that will be excecuted
//&fin: The file being read from
//line: the command, oper1, and oper2 on one line
void convertFunCommands (string oper1, string oper2, string command, ifstream &fin, string line)
{
	int paramCount = 0;
	int paramValue = 0;
	memory[address] = FUN;
	address++;
	memory[address] = ripBrackets(oper1);
	address++;
	memory[address] = stoi(oper2);
	paramCount = stoi(oper2);
	address++;
	for (int i = 0; i < paramCount; i++)
	{
		getline(fin, line, '\n');
		changeToLowerCase(line);
		splitCommand(line, command, oper1, oper2);
		if (checkOper(command) == 6)
		{
			memory[address] = 6;
			address++;
			memory[address] = ripBrackets(command);
			address++;
		}
		else if (checkOper(command) == 7)
		{
			memory[address] = 7;
			address++;
			memory[address] = stoi(command);
			address++;
		}
		else if (checkOper(command) < 4)
		{
			memory[address] = checkOper(command);
			address++;
		}
	}
	address--;
}