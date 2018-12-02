/*
 * signals.h
 *
 *  Created on: Nov 30, 2018
 *      Author: os
 */
#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "data.h"

using namespace std;

void sigHandler(int signal);
extern smash_data* p_smash;

#endif



