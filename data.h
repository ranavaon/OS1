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

class job{
public:
	job(pid_t pid_, time_t time, string name_):PID(pid_),birth_time_stamp(time),name(name_){state = RUNNING;}
	int get_pid(){return PID;}
	time_t get_birth_time(){return birth_time_stamp;}
	time_t get_how_old();// needs to be implemented
	run_state get_state(){return state;}
	void set_state(run_state s){state = s;}
	string get_name(){return name;}


private:
	pid_t PID;
	time_t birth_time_stamp;
	run_state state;
	string name;

};




class smash_data: public job{
public:
	smash_data(pid_t smash_pid, time_t time):job(smash_pid,time,"smash"), quit_(false),current_pwd(NULL),last_pwd(NULL){bg_jobs.push_front(this),fg_job=NULL;
	}// check how to get time stamp!
	char* get_current_pwd(){return current_pwd;}
	void set_pwd(char* new_pwd); // need to implement this method
	char* get_last_pwd(){return last_pwd;}
	//char* get_history(){return history;}
	void print_history();
	void add_to_history(char* cmdString);
	void print_bg_job(); // need to implement this method
	string get_job_name(int job_index);// need to implement this method: go over the list with the iterator <job_index> times and return the relevant's name
	int move_to_fg(int job_index_to_fg);// need to implement this method: move that job to fg_job, get this job's pid and use waitpid on it, return the wait's return value. when the job terminates, put NULL in fg_job;
	int move_to_bg(int job_index_to_bg);
	void kill_all_jobs();
	void add_new_bg_job(pid_t pid_, time_t time_, string name_);
	void delete_bg_job(int job_index);
	void delete_fg_job(){delete fg_job;}
	void add_job_to_fg(pid_t pid_, time_t time_, string name_){fg_job =new job(pid_,time_,name_);}
	job* get_fg_job(){return fg_job;}
	void quit(){quit_ = true;}
	bool is_quit(){return quit_;}

private:
	bool quit_;
	char* current_pwd;
	char* last_pwd;
	list<char*> history;
	list<job*> bg_jobs;
	job* fg_job;

};
#endif



