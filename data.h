//Data.h
//Contains the classes job and smash data that needed to handel the smash
#ifndef _DATA_H
#define _DATA_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <map>
#include <list>
#include <time.h>
#include <sys/wait.h>

#define MAX_HISTORY 50
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define MAX_PROCESSES 100
#define WAITFORSIGTERM 5

using namespace std;


typedef enum {SUSPENDED,RUNNING} run_state;
// class job
//containes the data and methodes to handel one job or order
class job{
public:
	job(pid_t pid_, time_t time, string name_):PID(pid_),birth_time_stamp(time),name(name_){state = RUNNING;}
	pid_t get_pid(){return PID;}
	time_t get_birth_time(){return birth_time_stamp;}
	time_t get_how_old();
	run_state get_state(){return state;}
	string get_name(){return name;}
	void set_state(run_state s){state = s;}


private:
	pid_t PID;
	time_t birth_time_stamp;
	run_state state;
	string name;
};



//class smash data
//contaiens all the info about the jobs runninf and the methodes to handel them
class smash_data: public job{
public:
	smash_data(pid_t smash_pid, time_t time,string current_pwd_):job(smash_pid,time,"smash"), quit_(false),current_pwd(current_pwd_){fg_job=NULL; // ,current_pwd(NULL),last_pwd(NULL)
	}
	string get_current_pwd(){return current_pwd;}
	void set_pwd(char* new_pwd);
	string get_last_pwd(){return last_pwd;}

	void print_history();
	void add_to_history(string cmdString);

	void print_bg_job();
	string get_job_name(unsigned int job_index);
	int move_to_fg(unsigned int job_index_to_fg);
	int move_to_bg(unsigned int job_index_to_bg);
	void kill_all_jobs_rec(list<job*>::iterator iter, int i);
	void kill_all_jobs();
	void add_new_bg_job(pid_t pid_, time_t time_, string name_);

	void delete_bg_job(pid_t pid);
	void delete_fg_job(){fg_job = NULL;}

	void add_job_to_fg(pid_t pid_, time_t time_, string name_){fg_job =new job(pid_,time_,name_);}
	void add_job_to_bg(job* fg){bg_jobs.push_front(fg);}
	job* get_fg_job(){return fg_job;}
	int signal_job(unsigned int index, int signal);

	void quit(){quit_ = true;}
	bool is_quit(){return quit_;}

private:
	bool quit_;
	string current_pwd;
	string last_pwd;
	list<string> history;
	list<job*> bg_jobs;
	job* fg_job;

};
#endif

