/*
 * commands.cpp
 *
 *  Created on: Nov 27, 2018
 *      Author: os
 */

#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interprets and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
//im here
int ExeCmd(char* lineSize, char* cmdString,smash_data* p_smash)
{
	char* cmd;
	//char* cmdString;
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
    cmd = strtok(cmdString, delimiters);
	if (cmd == NULL)
		return 0;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters);
		if (args[i] != NULL){
			//cout << args[i];
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
				//cout << "trying cd"<< endl;
				if(chdir((p_smash->get_last_pwd()).c_str())){
					//cout << "path not found"<< endl;
					cerr << args[1] <<" - path not found" << endl;
					return 1;
				}
				p_smash->set_pwd(getcwd(pwd, MAX_LINE_SIZE));
				//cout << "pwd set!"<< endl;
				return 0;
			}
			//cout << "still trying cd"<< endl;

			if(chdir(args[1])){
				//cout << "path not found"<< endl;
				cerr << args[1] <<" - path not found" << endl;
				return 1;
			}
			p_smash->set_pwd(getcwd(pwd, MAX_LINE_SIZE));
			return 0;
		}
		illegal_cmd = true;
	}

	/*************************************************/
	else if (!strcmp(cmd, "pwd"))
	{	
		p_smash->add_to_history(cmdString);
		if(num_arg == 0){
			getcwd(pwd, MAX_LINE_SIZE);
			if(pwd == NULL){
				cerr << "could not find pwd!"<< endl;
				return 1;
			}
			cout << pwd << endl;
			//cout<< "in pwd"<<endl;
			//p_smash->set_pwd(pwd);

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
	else if (!strcmp(cmd, "mv")) // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! implement this!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	{
		p_smash->add_to_history(cmdString);
 		if(num_arg==2)
 		{
 			if(renameat(AT_FDCWD,args[1],AT_FDCWD,args[2])){

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
			cout << (int)p_smash->get_pid() << endl;
			return 0;
		}
		illegal_cmd = true;
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg"))
	{
		p_smash->add_to_history(cmdString);
		if(num_arg == 1){
			string job_name = p_smash -> get_job_name(atoi(args[1]));
			cout << job_name << endl;
			if (p_smash -> move_to_fg(atoi(args[1])))
				return 0;
			cerr << "could not move to fg" << endl;
			return 1;
		}
		else if(num_arg == 0){
			if (p_smash -> move_to_fg(0))
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
			string job_name = p_smash -> get_job_name(atoi(args[1]));
			cout << job_name << endl;
			if (p_smash -> move_to_bg(atoi(args[1])))
				return 0;
			cerr << "could not move to fg" << endl;
			return 1;
		}
		else if(num_arg == 0){
			if (p_smash -> move_to_bg(0))
				return 0;
			cerr << "could not move to fg" << endl;
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
    switch(pID = fork())
	{
    		case -1:
					cerr << "Fork returned error!" << endl;
					exit(1);
					break;
        	case 0 :
                	// Child Process
        			cout<< "in child" <<endl;
               		setpgrp();
               		cout<< "still in child" <<endl;
					if(execvp(args[0], args) == -1){
						cout<< "err" <<endl;
						cerr << "error while executing" << cmdString << endl;
						exit(1);
					}
					cout<< "still in child1" <<endl;
					break;// how does the child "kills " himself ?
			default:
				cout<< "child pid: "<<pID <<endl;
				p_smash -> add_job_to_fg(pID, time(NULL), args[0]);
				waitpid(pID, NULL, WUNTRACED);
				cout<< "child ended "<<pID <<endl;
	            p_smash -> delete_fg_job();
	            cout<< "child job deleted "<<pID <<endl;

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

	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
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
int BgCmd(char* lineSize, smash_data* p_smash) // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! implement this !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
	//cout<< "in bgcmd, line:"<< lineSize << endl;
	bool is_bg = false;
	char* Command ;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	int num_arg=0;
	char* cmd = strtok(lineSize, delimiters);
	//cout << "cmd: " << cmd << endl;
	if (cmd == NULL)
		return 0;
   	args[0] = cmd;
	for (int i=1; i<MAX_ARG; i++)
	{
		char* arg = strtok(NULL, delimiters);
		if (arg != NULL)
		{
			//cout << "arg in parse loop: " << arg<< endl;
			if(!strcmp(arg,"&")){
				is_bg = true;
			}
			else{
				args[i] = arg;
				num_arg++;
			}
		}
	}
	if (is_bg)
	{
		//cout << "its a bg!" << endl;
		int pID;
	    switch(pID = fork())
		{
	    		case -1:
						cerr << "Fork returned error!" << endl;
						exit(1);
						break;
	        	case 0 :
	                	// Child Process
	        			//cout << "bg child"<< endl;
	               		setpgrp();
						if(execvp(args[0], args) == -1){
							cerr << "error while executing" << lineSize << endl;
							exit(1);
						}
						//p_smash -> delete_bg_job(args[0]); --- make sure this is functioning with cmd!
						break;// what happens when the child gets here?
				default:
					p_smash -> add_new_bg_job(pID, time(NULL), args[0]);
		            p_smash -> delete_fg_job();
		            return 0;

		}
	}
	return -1;
}


