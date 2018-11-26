// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

void sigHandler(int signal)
{
	int pid;
	job* pjob;
	switch(signal)
	{
		case SIGTSTP:
		break;
		case SIGINT:
		break;
		case SIGCHLD:
		break;
		default:
		break;
	}
}

