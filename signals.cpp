/*
 * signals.cpp
 *
 *  Created on: Nov 27, 2018
 *      Author: os
 */

// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

void sigHandler(int signal)//not working good yet
{
	int pid;
	job* pjob;
	switch(signal)
	{
		case SIGTSTP:
				if (p_smash->get_fg_job() == NULL)
			break;
		cout << "signal SIGTSTP was sent to pid " << p_smash->get_fg_job()->get_pid() << endl;
		if (kill(p_smash->get_fg_job()->get_pid(), SIGTSTP))
		{
			cerr << "Error sending SIGTSTP signal" << endl;
			return;
		}
		if (p_smash->get_fg_job())// all this part need to be chacked-i'm not sure if move to bg(0) is right
		{
			//p_smash->lastStoppedJobNum = p_smash->getFgJob()->getJobNum();
			p_smash->get_fg_job()->set_state(SUSPENDED);
			p_smash->add_job_to_bg(p_smash->get_fg_job());
			//p_smash->move_to_bg(0);
			p_smash->delete_fg_job();
		}
		break;

		case SIGINT:
			if (p_smash->get_fg_job() == NULL)
				break;
			cout << "signal SIGINT was sent to pid " << p_smash->get_fg_job()->get_pid() << endl;
			if (kill(p_smash->get_fg_job()->get_pid(), SIGINT))
			{
				cerr << "Error sending SIGTSTP signal" << endl;
				return;
			}
			p_smash->get_fg_job()->set_state(SUSPENDED);
			p_smash->delete_fg_job();
		break;

		case SIGCHLD:
		pid = waitpid(-1,NULL,WNOHANG);//find the zombie proccess
		if(pid > 0)
		{
				//need to be fixed- how to find the zombie index and delet it
		}
		break;
		default:
		break;
	}
}



