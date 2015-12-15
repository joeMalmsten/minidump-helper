///////////////////////////////////////////////////////////////////////////////////////////////////
//   Filename: mdump_helper.cpp
//     Author: Joseph Malmsten
//    Purpose: plugs into most programs easily to provide post mortem crash debugging information.
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "mdump_helper.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function: debug_info()
//  Purpose: constructor
///////////////////////////////////////////////////////////////////////////////////////////////////
debug_info::debug_info(){
	//get the current process and the process ID
	hProcess = GetCurrentProcess();
	ProcessId = GetCurrentProcessId();

	//set the dump type for the initial dump
	DumpType = MiniDumpNormal;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function: minidump_setup
//  Purpose: sets up and creates the minidump thread
//   Params: pException - the pointer to the exception
///////////////////////////////////////////////////////////////////////////////////////////////////
LONG WINAPI debug_info::minidump_setup(EXCEPTION_POINTERS* pException){
	//clear the memory of the exception parameters
	ZeroMemory( &ExceptionParam, sizeof(ExceptionParam) );

	//set the parameters after the memory has been cleared
	ExceptionParam.ThreadId = GetCurrentThreadId();
	ExceptionParam.ExceptionPointers = pException;
	ExceptionParam.ClientPointers = FALSE;

	//create the thread that creates the minidump
	HANDLE thread = CreateThread(NULL, 0, Thread_It, (void*)this, 0, &threadHandle);

	//dont continue until the thread has finished
	WaitForSingleObject(thread, INFINITE);

	//return to the exception to allow for other debuggers to do thier thing
	return EXCEPTION_CONTINUE_SEARCH;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Function: Thread_It
//  Purpose: Thread function that creates the minidump
//   Params: lpParam - the void pointer to the information needed to write the minidump
///////////////////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI Thread_It(LPVOID lpParam){
	//reinterpret the void parameter into the information needed for the minidump
	debug_info * dbg_info_ = reinterpret_cast<debug_info*>(lpParam);

	//create the filename for the minidump
	std::wstring temp(L"Mdump.dmp"); 
	LPCWSTR filename_ = temp.c_str();

	//create a blank file for the minidump to write into
	dbg_info_->hFile = CreateFile(filename_, GENERIC_WRITE, FILE_SHARE_DELETE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    //write the minidump
	MiniDumpWriteDump(dbg_info_->hProcess, dbg_info_->ProcessId, dbg_info_->hFile, dbg_info_->DumpType, &dbg_info_->ExceptionParam, NULL, NULL);

	//same process as the last minidump, just a secondary larger minidump for more in depth needs
	std::wstring temp_b(L"Mdump_rw.dmp");
	LPCWSTR filename_b = temp_b.c_str();
	HANDLE hFile_rw = CreateFile(filename_b, GENERIC_WRITE, FILE_SHARE_DELETE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	MiniDumpWriteDump(dbg_info_->hProcess, dbg_info_->ProcessId, hFile_rw, MiniDumpWithPrivateReadWriteMemory, &dbg_info_->ExceptionParam, NULL, NULL);

	return 0;
}
