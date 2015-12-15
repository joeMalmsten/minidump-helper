///////////////////////////////////////////////////////////////////////////////////////////////////
//   Filename: mdump_helper.h
//     Author: Joseph Malmsten
//    Purpose: plugs into most programs easily to provide post mortem crash debugging information.
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef DBGHELP391
#define DBGHELP391

//windows includes
#include <windows.h>

//debug include and library, if you do not have it you can download it from this link
//http://msdn.microsoft.com/en-us/windows/hardware/gg463009.aspx
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

//needed for creating the filename of the minidumps
#include <string>

//a class containing the information needed to create the minidumps from inside a thread
class debug_info{
public:
	debug_info();
	//a handle to the process being tested and a corresponding ID
	HANDLE hProcess;
	DWORD ProcessId;

	//a handle to the file being written in
	HANDLE hFile;
	//the type of dump to be used in the program, easy to switch
	MINIDUMP_TYPE DumpType;

	//the parameters of the exception
	MINIDUMP_EXCEPTION_INFORMATION ExceptionParam;
	
	//a handle to the thread writing the dump
	DWORD threadHandle;

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Function: minidump_setup
	//  Purpose: sets up and creates the minidump thread
	//   Params: pException - the pointer to the exception
	///////////////////////////////////////////////////////////////////////////////////////////////
	LONG WINAPI minidump_setup(EXCEPTION_POINTERS* pException);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function: Thread_It
//  Purpose: Thread function that creates the minidump
//   Params: lpParam - the void pointer to the information needed to write the minidump
///////////////////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI Thread_It(LPVOID lpParam);

#endif