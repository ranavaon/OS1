/*
 * commands.cpp
 *
 *  Created on: Nov 30, 2018
 *      Author: os
 */

#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interprets and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(char* lineSize, char* cmdString,smash_data* p_smash)
{
	char* cmd;
	//char* cmdString;
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char const* delimiters = " \t\n";
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
    cmd = strtok(cmdString, delimiters);
	if (cmd == NULL)
		return 0;
	args[0]=cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters);
		if (args[i] != NULL){
			num_arg++;
		}

	}



/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") )
	{
		p_smash->add_to_history(cmdString);
		cout << "trying cd "<< num_arg << endl;
		if(num_arg == 1){
			if(!strcmp(args[1],"-")){
				if(chdir((p_smash->get_last_pwd()).c_str())){
					cerr << args[1] <<" - path not found" << endl;
					return 1;
				}
				p_smash->set_pwd(getcwd(pwd, MAX_LINE_SIZE));
				return 0;
			}
			if(chdir(args[1])){
				cerr << args[1] <<" - path not found" << endl;
				return 1;
			}
			p_smash->set_pwd(getcwd(pwd, MAX_LINE_SIZE));
			return 0;
		}
		illegal_cmd = true;
	}

	/*************************************************/
	else if (!strcmp(cmd, "pwd")){
		p_smash->add_to_history(cmdString);
		if(num_arg == 0){
			getcwd(pwd, MAX_LINE_SIZE);
			if(pwd == NULL){
				cerr << "could not find pwd!"<< endl;
				return 1;
			}
			cout << pwd << endl;
			return 0;
		}
		illegal_cmd = true;
	}
	/*************************************************/
	else if (!strcmp(cmd, "history"))
	{
		if (num_arg==0){
			p_smash->print_history();
			p_smash->add_to_history(cmdString);
			return 0;
		}
		illegal_cmd=true;
	}
	/*************************************************/
	else if (!strcmp(cmd, "mv"))
	{
		p_smash->add_to_history(cmdString);
 		if(num_arg==2)
 		{
 			if(!renameat(AT_FDCWD,args[1],AT_FDCWD,args[2])){
 				cout<<args[1]<<" has been renamed to "<<args[2]<<endl;
 			}

 			return 0;
 		}
 		illegal_cmd=true;
	}
	/*************************************************/

	else if (!strcmp(cmd, "jobs"))
	{
		p_smash->add_to_history(cmdString);
		if(num_arg == 0){
			p_smash->print_bg_job();
			return 0;
		}
 		illegal_cmd = true;
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid"))
	{
		p_smash->add_to_history(cmdString);
		if(num_arg == 0){
			cout <<"smash pid is: "<<(int)p_smash->get_pid() << endl;
			return 0;
		}
		illegal_cmd = true;
	}
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
		{
			p_smash->add_to_history(cmdString);
			char *signal_char = strtok(args[1], "-");
			int signal_int = atoi(signal_char);
			if(num_arg == 2){
				if(!p_smash->signal_job(atoi(args[2]),signal_int)){
					return 0;
				}
				return -1;
			}
			illegal_cmd = true;
		}
		/*************************************************/
	else if (!strcmp(cmd, "fg"))
	{
		p_smash->add_to_history(cmdString);
		if(num_arg == 1){
			string job_name = p_smash -> get_job_name(atoi(args[1])-1);
			cout << job_name << endl;
			if (!p_smash -> move_to_fg(atoi(args[1])-1))
				return 0;
			cerr << "could not move to fg" << endl;
			return 1;
		}
		else if(num_arg == 0){
			if (!p_smash -> move_to_fg(0))
				return 0;
			cerr << "could not move to fg" << endl;
			return 1;
		}
		illegal_cmd = true;
	}
	/*************************************************/
	else if (!strcmp(cmd, "bg"))
	{
		p_smash->add_to_history(cmdString);
  		if(num_arg == 1){
			string job_name = p_smash -> get_job_name(atoi(args[1])-1);
			cout << job_name << endl;
			if (!p_smash -> move_to_bg(atoi(args[1])-1))
				return 0;
			cerr << "could not move to bg" << endl;
			return 1;
		}
		else if(num_arg == 0){
			if (!p_smash -> move_to_bg(0))
				return 0;
			cerr << "could not move to bg" << endl;
			return 1;
		}
		illegal_cmd = true;
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		p_smash->add_to_history(cmdString);
   		if(num_arg == 1 && (!strcmp(args[1],"kill"))){
   			p_smash -> kill_all_jobs();
   			p_smash-> quit();
   			return 0;
   		}
   		if(num_arg == 0){
   			p_smash-> quit();
   			return 0;
   		}
   		illegal_cmd = true;
	}
	/*************************************************/
	else // external command
	{
		p_smash->add_to_history(cmdString);
 		ExeExternal(args, cmdString,p_smash);
	 	return 0;
	}
	if (illegal_cmd == true)
	{
		cerr << "smash error: > \"" << cmdString <<"\"" << endl;
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString, smash_data* p_smash)
{
	int pID;
	string fg;
    switch(pID = fork())
	{
    		case -1:
					cerr << "Fork returned error!" << endl;
					exit(1);
					break;
        	case 0 :
                	// Child Process
               		setpgrp();
					if(execvp(args[0], args) == -1){
						cerr << "error while executing " << cmdString << endl;
						exit(1);
					}
					break;
			default:
				fg=args[0];
				p_smash -> add_job_to_fg(pID, time(NULL), fg );
				waitpid(pID, NULL, WUNTRACED);
	            if (p_smash->get_fg_job()!=NULL)
	            	{p_smash -> delete_fg_job();}
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
    	cout<<"This is a complex command"<<endl;
		return 0;
	}
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, smash_data* p_smash) // working
{
	bool is_bg = false;
	string Command = lineSize;
	string bg;
	char const* delimiters = " \t\n";
	char *args[MAX_ARG];
	char* cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0;
   	args[0] = cmd;
	for (int i=1; i<MAX_ARG; i++)
	{
		char* arg = strtok(NULL, delimiters);
		if (arg != NULL && !strcmp(arg,"&"))
		{
			is_bg = true;
			args[i] = NULL;
		}
		else {
			args[i] = arg;
		}

	}
	if (is_bg)
	{
		p_smash->add_to_history(Command);
		int pID;
	    switch(pID = fork())
		{
	    		case -1:
						cerr << "Fork returned error!" << endl;
						exit(1);
						break;
	        	case 0 :
	               		setpgrp();
						if(execvp(args[0], args) == -1){
							cerr << "error while executing " << lineSize << endl;
							cout<<"smash >";
							exit(1);
						}
						break;
				default:
					bg=args[0];
					p_smash -> add_new_bg_job(pID, time(NULL), bg);

		}
	return 0;
	}
	return -1;
}


