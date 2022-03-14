# UNIX-shell

Author: Lauren Lobo, 1087364
Date: 5/2/2021
Program: myShell.c
Files included:
	-myShell.c
	-.CIS3110_profile
	-.CIS3110_history
	-makefile
Program Description: Implement a UNIX shell program

Assumptions: files for testing PATH, HOME, history files are named .CIS3110_profile and .CIS3110_history

How to run: compiled using make command and run with ./myShell

***************IMPORTANT FUNCTIONALITY PROBLEMS********************
- The export functionality has not been implemented
- “history n” does not work, but regular history command and history –c are working
- If running commands (not executable file or ps but other commands) & sometimes causes the program to loop and you must ctrl+C out of it
-while a background process is running (ex “./bgSleep 10 &) you can type new commands but they will not execute until background process is done.
-for < > chaining with input and output files you must hit enter as many times on the command line as the amount of lines in the file you are entering. (ex: ./interactive 2 < inputFile.txt > outputFile) you have to hit enter twice on the terminal to finish execution


Improvements:	
-	Could implement export 
-	Could fix functionality of history n
-	Could fix/modify background process execution


References: 
Piping:
https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/pipe.html

Linux shell guide:
https://www.geeksforgeeks.org/making-linux-shell-c/

Waitpid:
https://www.ibm.com/support/knowledgecenter/SSLTBW_2.2.0/com.ibm.zos.v2r2.bpxbd00/rtwaip.htm

