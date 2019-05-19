// signals.cpp
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: sigHandler
   Synopsis: handle the Control-C, Control-Z and Zombies */
#include "signals.h"

void sigHandler(int signal)
{
	pid_t pid;
	switch(signal)
	{
		case SIGTSTP://handel Control Z
				if (p_smash->get_fg_job() == NULL)
			break;
		cout << "signal SIGTSTP was sent to pid " << p_smash->get_fg_job()->get_pid() << endl;
		if (kill(p_smash->get_fg_job()->get_pid(), SIGTSTP))
		{
			cerr << "Error sending SIGTSTP signal" << endl;
			return;
		}
		if (p_smash->get_fg_job()!=NULL)
		{
			p_smash->get_fg_job()->set_state(SUSPENDED);
			p_smash->add_job_to_bg(p_smash->get_fg_job());
            if (p_smash->get_fg_job()!=NULL)
            	{
            	p_smash -> delete_fg_job();}
		}
		break;

		case SIGINT://handel Control -C
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

		case SIGCHLD://handel zombies
		pid=1;
		while (pid > 0)
		{
			pid = waitpid(-1,NULL,WNOHANG);

			if (pid<=0)
				return;
			p_smash->delete_bg_job(pid);
		}
		break;
		default:
		break;
	}
}



