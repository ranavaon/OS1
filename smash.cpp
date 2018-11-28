/*
 * smash.cpp
 *
 *  Created on: Nov 27, 2018
 *      Author: os
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#include "data.h"
#define MAX_LINE_SIZE 80
#define MAXARGS 20
#define MAX_PROCESSES 100

char* L_Fg_Cmd;
void* jobs = NULL; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE];
smash_data* p_smash =NULL;
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{

	signal(SIGTSTP,sigHandler);
	signal(SIGINT,sigHandler);
	signal(SIGCHLD,sigHandler);
	//signal declarations
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */

	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/

	/************************************/
	// Init globals
	smash_data my_smash(getpid(),time(NULL));// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!put as an argument this process's PID!!!!!!!!!!!!!!!!!!!!!!!!!!
	p_smash = &my_smash;
	char cmdString[MAX_LINE_SIZE];


	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL)
			exit (-1);
	L_Fg_Cmd[0] = '\0';

    while (!my_smash.is_quit())
    {
	 	cout << "smash > ";
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);
		cmdString[strlen(lineSize)-1]='\0';
					// perform a complicated Command
		if(!ExeComp(lineSize)) continue;
					// background command
	 	if(!BgCmd(lineSize, &my_smash)) continue;
					// built in commands
		ExeCmd(lineSize, cmdString, &my_smash);

		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}



