#ifndef _COMMANDS_H
#define _COMMANDS_H

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "data.h"
#include "signals.h"
#include <fcntl.h> /* Definition of AT_* constants */

#define MAX_PROCESSES 100
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define MKDIR_PERMISSIONS 00777
#define WAITFORSIGTERM 5

using namespace std;

int ExeCmd(char* lineSize, char* cmdString,smash_data* p_smash);
void ExeExternal(char *args[MAX_ARG], char* cmdString, smash_data* p_smash);
int ExeComp(char* lineSize);
int BgCmd(char* lineSize,smash_data* Smash);


#endif


