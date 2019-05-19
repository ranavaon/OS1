
#include "data.h"
//**************************************************************************************
// function name: get_how_old
// Description: calc of the time from starting the job until now
// Parameters: none
// Returns: time
//**************************************************************************************
	time_t job::get_how_old(){
		time_t now = time(NULL);
		return (now - this->birth_time_stamp);
	}
//**************************************************************************************
// function name: set_pwd
// Description: setting the current path to the field pwd
// Parameters: command path, string of the current path
// Returns: none
//**************************************************************************************
	void smash_data::set_pwd(char* new_pwd){
		this->last_pwd = this->current_pwd;
		this->current_pwd = new_pwd;
	}
//**************************************************************************************
// function name: print history
// Description: printing all the commands list from beggining to end
// Parameters: none
// Returns: none
//**************************************************************************************
	void smash_data::print_history()
	{
		for(list<string>::iterator iter = this->history.begin(); iter != this->history.end(); iter++){
		   cout<<*iter<<endl;
		}
	}
//**************************************************************************************
// function name: BgCmd
// Description: adding a new command to the history list
// Parameters: command string, the command to history
// Returns: none
	//**************************************************************************************
void smash_data::add_to_history(string cmdString){
		if (this->history.size()>=MAX_HISTORY){
				this->history.pop_back();
			}
			this->history.push_back(cmdString);
	}
//**************************************************************************************
// function name: print bg jobs
// Description: prints all the Background jobs list
// Parameters: none
// Returns: none
//**************************************************************************************
	void smash_data::print_bg_job(){
		int i=1;
		for(list<job*>::iterator iterator = (this->bg_jobs).begin(); iterator != (this->bg_jobs).end(); iterator++){
			if((*iterator)->get_state())
				cout << "[" << i << "] "<< (*iterator)-> get_name() <<  " : " << (*iterator)->get_pid() << " " << (*iterator)->get_how_old()<< " secs" << endl;
			else
				cout << "[" << i << "] "<< (*iterator)-> get_name() <<  " : " << (*iterator)->get_pid() << (*iterator)->get_how_old()<< "secs" << "(Stopped)" << endl;
			i++;
		}

	}
//**************************************************************************************
// function name: get_job_name
// Description: find and returns the name of a job by his index
// Parameters: int of the job index
// Returns: string of the job wanted
//**************************************************************************************
	string smash_data::get_job_name(unsigned int job_index){
		if(job_index == 0 || job_index > bg_jobs.size()){
			cerr << "no such job!" << endl;
			return "-1";
		}
		list<job*>::iterator iter = this->bg_jobs.begin();
		for(int i=1; i<=job_index; i++){
			iter++;
		}
		if(*iter == NULL){
			return "-1";
		}
		return (*iter)->get_name();
	}
//**************************************************************************************
// function name: move to fg
// Description:moving a background job to foreground
// Parameters: int index of the job
// Returns: 0- succsess -1- if fail
//**************************************************************************************
	int smash_data::move_to_fg(unsigned int job_index){
		list<job*>::iterator iter = this->bg_jobs.begin();
		if(job_index == 0){
			for(unsigned int i=0; i<this->bg_jobs.size()-1; i++){
							iter++;
			}
		}
		else{
			for(unsigned int i=1; i<=job_index; i++){
				iter++;
			}
		}
		if(*iter == NULL){
			return -1;
		}
		this -> fg_job = *iter;
		if(!(*iter)->get_state()){
			cout << "signal SIGCONT was sent to pid " << (*iter)->get_pid() << endl;
			(*iter)->set_state(RUNNING);
			kill((*iter)->get_pid(), SIGCONT);
		}
		waitpid((*iter)->get_pid(), NULL, WUNTRACED);
		(this -> bg_jobs).erase(iter);
		return 0;
	}
//**************************************************************************************
// function name: move to bg
// Description:moving a foreground job to background
// Parameters: int index of the job
// Returns: 0- success -1- fail
//**************************************************************************************
	int smash_data::move_to_bg(unsigned int job_index_to_bg){
		if(job_index_to_bg > bg_jobs.size()){
			cerr << "no such job!" << endl;
			return -1;
		}
		if(job_index_to_bg == 0){// case no job id mentioned
			list<job*>::iterator job_to_bg = this->bg_jobs.begin();
			for(list<job*>::iterator iter = job_to_bg; iter != bg_jobs.end(); iter++){

				if((*iter)->get_state() == SUSPENDED){
					job_to_bg = iter;
				}
			}
			if((*job_to_bg != NULL) && ((job_to_bg != bg_jobs.begin())||((*job_to_bg)->get_state() == SUSPENDED))){// means there is a suspended job
				cout << "signal SIGCONT was sent to pid " << (*job_to_bg)->get_pid() << endl;
				(*job_to_bg)->set_state(RUNNING);
				if(!kill((*job_to_bg)->get_pid(), SIGCONT))
					return 0;
				cerr <<"job- " <<  job_index_to_bg << "could not get the signal!" << endl;
				return -1;
			}

		}
		list<job*>::iterator job_to_bg = this->bg_jobs.begin();
		for(int i=0; i<job_index_to_bg; i++){
			job_to_bg++;
		}
		if ((*job_to_bg)->get_state() == RUNNING){ // in case that job is already running
			return 0;
		}
		cout << "signal SIGCONT was sent to pid " << (*job_to_bg)->get_pid() << endl;
		(*job_to_bg)->set_state(RUNNING);
		if(kill((*job_to_bg)->get_pid(), SIGCONT))
			return 0;
		cerr <<"job- " <<  job_index_to_bg << "could not get the signal!" << endl;
		return -1;

	}
//**************************************************************************************
// function name: add new bg job
// Description: addind a new job to the background jobs list
// Parameters: pid of the new job.time the job created,string of the job's name
// Returns: none
//**************************************************************************************
	void smash_data::add_new_bg_job(pid_t pid_, time_t time_, string name_){
		bg_jobs.push_back(new job(pid_,time_,name_));
	}

//**************************************************************************************
// function name: delete bg job
// Description: delete a job from the background jobs list
// Parameters: pid of the job to delte
// Returns: none
//**************************************************************************************
	void smash_data::delete_bg_job(pid_t pid){
		list<job*>::iterator job_to_delete = this->bg_jobs.begin();
		for(list<job*>::iterator iter = this->bg_jobs.begin(); iter != bg_jobs.end(); iter++){
			if ( (*iter)->get_pid()== pid){
				job_to_delete=iter;
			}
		}
		bg_jobs.erase(job_to_delete);
	}


//**************************************************************************************
// function name: kill all jobs
// Description: a warp methot to the method kill_all_jobs_rec that deletes all the jobs running
// Parameters: none
// Returns: none
//**************************************************************************************
	void smash_data :: kill_all_jobs(){
		list<job*>::iterator iter = bg_jobs.begin();
		int i = 1;
		this-> kill_all_jobs_rec(iter, i);
	}

//**************************************************************************************
// function name: signal job
// Description: handel the kill function for all the signals need a replase from the default
// Parameters: index of the job and t signal number
// Returns: 0- BG success -1- fail
//**************************************************************************************
	int smash_data::signal_job(unsigned int index, int signal){
		if(index > bg_jobs.size()){
			cerr<< "kill " << index <<" – job does not exist"<<endl;
			return -1;
		}
		list<job*>::iterator job_to_delete = this->bg_jobs.begin();
		for(unsigned int i=1 ;i < index ; i++){
			job_to_delete ++;
		}
		if(signal == SIGINT || signal == SIGQUIT || signal == SIGTERM || signal == SIGKILL || signal == SIGHUP)
		{
			cout << "signal " << signal << " was sent to pid " << (*job_to_delete)->get_pid() << endl;
			if (kill((*job_to_delete)->get_pid(), signal))
			{
				cerr << "kill " << index <<" – cannot send signal" << endl;
				return -1;
			}
			this->delete_bg_job((*job_to_delete)->get_pid());
			return 0;
		}
		else if (signal == SIGSTOP)
		{
			if(!(*job_to_delete)->get_state())
				return 0; //already not-running, do nothing

			cout << "signal " << signal << " was sent to pid " << (*job_to_delete)->get_pid() << endl;
			if (kill((*job_to_delete)->get_pid(), signal))
			{
				cerr << "kill " << index <<" – cannot send signal" << endl;
				return -1;
			}
			(*job_to_delete)->set_state(SUSPENDED);
			return 0;
		}
		else if (signal == SIGCONT)
		{
			if((*job_to_delete)->get_state())
				return 0; //already running, do nothing

			cout << "signal " << signal << " was sent to pid " << (*job_to_delete)->get_pid() << endl;
			if (kill((*job_to_delete)->get_pid(), signal))
			{
				cerr << "kill " << index <<" – cannot send signal" << endl;
				return -1;
			}
			(*job_to_delete)->set_state(RUNNING);
			return 0;
		}
		if(!kill((*job_to_delete)->get_pid(),signal))
			return 0;
		cerr << "kill " << index <<" – cannot send signal" << endl;
		return -1;
	}



//**************************************************************************************
// function name: kill all jobs rec
// Description: deleteding all the jobs on the smash and ends the program
// Parameters: iterator to begin from
// Returns: none
//**************************************************************************************
	void smash_data::kill_all_jobs_rec(list<job*>::iterator iter, int i){
		if (iter == bg_jobs.end()){
			return;
		}
		list<job*>::iterator iter_to_send = iter;
		iter_to_send++;
		kill_all_jobs_rec(iter_to_send, i+1);
		cout << "[" << i << "] " << (*iter)->get_name() << " - Sending SIGTERM... ";
		int start = (int)time(NULL);
		kill((*iter)->get_pid(), SIGCONT);
		while (!waitpid((*iter)->get_pid(), NULL, WNOHANG) && ((int)time(NULL) - start < WAITFORSIGTERM));
		if (!waitpid((*iter)->get_pid(), NULL, WNOHANG))
		{
			cout << "(" << WAITFORSIGTERM << " sec passed) Sending SIGKILL... ";
			kill((*iter)->get_pid(), SIGKILL);
		}
		cout << "Done." << endl;
		bg_jobs.erase(iter);
	}


