/*
 * smash.cpp
 *
 *  Created on: Nov 30, 2018
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


	/************************************/
	// Init globals
	char pwd_[MAX_LINE_SIZE];
	smash_data my_smash(getpid(),time(NULL),getcwd(pwd_, MAX_LINE_SIZE));
	p_smash = &my_smash;
	char cmdString_Bg[MAX_LINE_SIZE];
	char cmdString_Exe[MAX_LINE_SIZE];


	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL)
			exit (-1);
	L_Fg_Cmd[0] = '\0';

    while (!my_smash.is_quit())
    {
	 	cout << "smash > ";
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString_Bg, lineSize);
		cmdString_Bg[strlen(lineSize)-1]='\0';
		strcpy(cmdString_Exe, cmdString_Bg);
					// perform a complicated Command
		if(!ExeComp(lineSize)) continue;
					// background command
	 	if(!BgCmd(cmdString_Bg, &my_smash)) continue;
					// built in commands
		ExeCmd(lineSize, cmdString_Exe, &my_smash);

		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString_Bg[0]='\0';
		cmdString_Exe[0]='\0';
	}
    return 0;
}


