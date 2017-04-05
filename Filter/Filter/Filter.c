//Charlie Ang
//CSC 3350 Spring 2016
//May 23, 2016
//Lab 5 Filter
//Filter reads a text file from standard input (stdin) and writes to standard output (stdout)
//a copy of the input with all upper-case letters converted to lower-case. Filter is designed to 
//read one character, convert it, output it, and then loop until the incoming data is finished.

#include<stdio.h>
#define DELAY_A_WHILE() {volatile int j; for(j=0; j<1000000; ++j) ; }

int main()
{
	int ch;
	while ((ch = fgetc(stdin)) != EOF)	//while there is input that's not EOF...Ctl-Z is EOF on keyboard 
	{
		if (ch >= 'A' && ch <= 'Z')	//if upper case 
		{
			ch = ('a' + ch - 'A');	//convert to lowercase if upper case 
		}
		fputc(ch, stdout);	//output to standard out 
		DELAY_A_WHILE();
	}

	return 0;
}