//Charlie Ang
//CSC 3350 Spring 2016
//May 23, 2016
//Lab 5 Sink
//Sink obtains a destination text-file filename from its commandline,
//opens the file for writing, and then reads characters one at a time from 
//standard input and writes each incoming character directly to the destination sink file.

#include<stdio.h>
#define DELAY_A_WHILE() {volatile int j; for(j=0; j<1000000; ++j) ; }

int main(int argc, char *argv[])
{
	FILE *fout;

	if (argc != 2 || fopen_s(&fout, argv[1], "wb"))	//one one cammand allowed 
	{
		fprintf(stderr, "Usage: source outputfilename\n");
		return (1);
	}

	int ch;
	while ((ch = fgetc(stdin)) != EOF)	//while there is input and it is not EOF
	{
		fputc(ch, fout);	//output to standard out 
		DELAY_A_WHILE();
	}

	fclose(fout);	//close output file 
	return 0;
}