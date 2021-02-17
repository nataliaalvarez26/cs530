#include <strings.h>
#include <stdbool.h>
#include "getword.h"
#include "stdbool.h"
#include <unistd.h>
#include <stdio.h>
#include "stdio.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "CHK.h"
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#define SIZE 25500 /*255 word length * 100 words max for buffer size*/
#define MAXITEM 100 /* max number of words per line */
/*all of the global variables used in getword.c and p2.c*/
extern int backamp;
extern int tildecheck;
extern int greaterthan;
extern int pipecheck;
extern int backslashtrig;
extern int doublelessthan;
extern int lessthan;
extern int dollarcheck; 
extern int lastamp;
extern int firstamp; 
extern bool TEST;
