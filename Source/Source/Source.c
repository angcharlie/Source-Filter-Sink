//Charlie Ang
//CSC 3350 Spring 2016
//May 23, 2016
//Lab 5 Source
//Source obtains a source text-file filename from its commandline, opens the file, and reads and 
//copies the file contents one character at a time directly to standard output (stdout).
//When the file has been copied, Source terminates, (closing all of its files and handles).

//LAB 5 INTRO
//This program achieves concurrent execution of 3 child processes that 
//use anonymous pipes to communicate and synchronize with each other.


#include <stdio.h>
#define DELAY_A_WHILE() {volatile int j; for(j=0; j<1000000; ++j) ; }

int main(int argc, char *argv[])
{
	FILE *fin;

	if (argc != 2 || fopen_s(&fin, argv[1], "rb"))	//one one cammand allowed 
	{
		fprintf(stderr, "Usage: source inputfilename\n");
		return (1);
	}

	//read chars and copy to standard out 
	while (1)
	{
		int ch;
		if ((ch = fgetc(fin)) == EOF)
		{
			break;
		}
		fputc(ch, stdout);
		//fputc(ch, stderr);	//optional for debugging
		DELAY_A_WHILE();		//slow down the program: simulate more "work"
	}
	fclose(fin);	//close file
	return (0);
}