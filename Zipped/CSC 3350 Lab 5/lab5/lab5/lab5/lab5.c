//Charlie Ang
//CSC 3350 Spring 2016
//May 23, 2016
//Lab 5 
//This program achieves concurrent execution of 3 child processes that 
//use anonymous pipes to communicate and synchronize with each other.

#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <winbase.h>

#define DELAY_A_WHILE() {volatile int j; for(j=0; j<5000000; ++j) ; }

int main(int argc, char *argv[])	//argv[1] is srcfile; argv[2] is destfile
{
	if (argc != 3)	//need three arguments for srcfile and destfile
	{
		printf("Must input a srcfile and a destfile.");
		exit(1);	//exit program 
	}

	HANDLE hPipeRead, hPipeWrite;	//for pipe 1
	HANDLE hPipeRead2, hPipeWrite2;	//for pipe 2

	STARTUPINFO StartupInfo;	//for source program 
	STARTUPINFO StartupInfoFilter;	//for Filter program 
	STARTUPINFO StartupInfoSink;	//for Sink program 

	PROCESS_INFORMATION ProcInfoChild;	//for source program 
	PROCESS_INFORMATION ProcInfoChildFilter;	//for filter program 
	PROCESS_INFORMATION ProcInfoChildSink;	//for sink program 

	SECURITY_ATTRIBUTES	PipeAttributes;	//pipe 1
	SECURITY_ATTRIBUTES	PipeAttributes2;	//pipe 2

	char source[256] = "Source.exe";
	char filter[256] = "Filter.exe";
	char sink[256] = "Sink.exe";

	//Source's commandline
	strcat_s(source, 256, " ");	//concatenate a space
	strcat_s(source, 256, argv[1]);	//concatenate the source to source's commandline

	//Sink's commandline 
	strcat_s(sink, 256, " ");	//concatenate a space
	strcat_s(sink, 256, argv[2]);	//concatenate the dest file to sink's commandline 

	//*****************************************************************************************

	//PIPE 1
	PipeAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	PipeAttributes.lpSecurityDescriptor = NULL;		//ignore
	PipeAttributes.bInheritHandle = TRUE;			//child can inherit

	//CREATE PIPE 1
	if (!CreatePipe(&hPipeRead, &hPipeWrite, &PipeAttributes, 0))
	{
		fprintf(stderr, "Error creating pipeL %d\n", GetLastError());
		exit(1);
	}

	//*****************************************************************************************

	//FOR SOURCE  
	GetStartupInfo(&StartupInfo);	//use same as current process
	StartupInfo.dwFlags = StartupInfo.dwFlags | STARTF_USESTDHANDLES;	//manually configure child's standard handles 

	//MAPPING SOURCE 
	StartupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	StartupInfo.hStdOutput = hPipeWrite;	//write to pipe
	StartupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);

	//CREATE SOURCE
	if(!CreateProcess(				
		NULL, source, NULL, NULL,
		TRUE,						//bInheritHandles 
		0, NULL, NULL,
		&StartupInfo,
		&ProcInfoChild))
	{
		fprintf(stderr, "Can't create child process!");
		exit(1);
	}

	//CLOSE HANDLES FOR SOURCE AFTER SOURCE HAS RAN 
	CloseHandle(ProcInfoChild.hProcess);	
	CloseHandle(ProcInfoChild.hThread);

	CloseHandle(hPipeWrite);	//no more writing to pipe 1....so filter does not inherit write handle 
	//hPiperRead still open for pipe 1

	//*****************************************************************************************

	//PIPE 2
	PipeAttributes2.nLength = sizeof(SECURITY_ATTRIBUTES);
	PipeAttributes2.lpSecurityDescriptor = NULL;		//ignore
	PipeAttributes2.bInheritHandle = TRUE;			//child can inherit

	//CREATE A PIPE 2
	if (!CreatePipe(&hPipeRead2, &hPipeWrite2, &PipeAttributes2, 0))
	{
		fprintf(stderr, "Error creating pipeL %d\n", GetLastError());
		exit(1);
	}

	//*****************************************************************************************

	//FOR FILTER 
	GetStartupInfo(&StartupInfoFilter);	
	StartupInfoFilter.dwFlags = StartupInfoFilter.dwFlags | STARTF_USESTDHANDLES;	//manually configure child's standard handles 

	//MAPPING FILTER 
	StartupInfoFilter.hStdInput = hPipeRead;	//filter is reading from pipe 1's read 
	StartupInfoFilter.hStdOutput = hPipeWrite2;	//write to pipe2
	StartupInfoFilter.hStdError = GetStdHandle(STD_ERROR_HANDLE);

	//CREATE FILTER
	if (!CreateProcess(
		NULL, filter, NULL, NULL,
		TRUE,						//bInheritHandles 
		0, NULL, NULL,
		&StartupInfoFilter,
		&ProcInfoChildFilter))
	{
		fprintf(stderr, "Can't create child process!");
		exit(1);
	}

	//CLOSE HANDLE FOR FILTER AFTER FILTER HAS RAN 
	CloseHandle(ProcInfoChildFilter.hProcess);
	CloseHandle(ProcInfoChildFilter.hThread);
	CloseHandle(hPipeRead);	//close pipe 1's read after filter has executed 

	CloseHandle(hPipeWrite2);	//close handle for writing to pipe 2 since we are done executing filter 

	//*****************************************************************************************

	//FOR SINK
	GetStartupInfo(&StartupInfoSink);
	StartupInfoSink.dwFlags = StartupInfoSink.dwFlags | STARTF_USESTDHANDLES;	//manually configure child's standard handles 

	//MAPPING SINK
	StartupInfoSink.hStdInput = hPipeRead2;	//filter is reading from pipe 1's read 
	StartupInfoSink.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);	//writing to stdout...but sink's execution should write to file
	StartupInfoSink.hStdError = GetStdHandle(STD_ERROR_HANDLE);

	//CREATE SINK
	if (!CreateProcess(
		NULL, sink, NULL, NULL,
		TRUE,						//bInheritHandles 
		0, NULL, NULL,
		&StartupInfoSink,
		&ProcInfoChildSink))
	{
		fprintf(stderr, "Can't create child process!");
		exit(1);
	}

	//CLOSE HANDLE FOR SINK AFTER SINK HAS RAN 
	CloseHandle(ProcInfoChildSink.hProcess);
	CloseHandle(ProcInfoChildSink.hThread);

	//*****************************************************************************************

	return 0;
}