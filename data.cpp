#include "data.h"

	time_t job::get_how_old(){
		time_t now = time(NULL); ////////////////////////////////////////////////////implement this!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		return (now - this->birth_time_stamp);
	}

	void smash_data::set_pwd(char* new_pwd){
		strcpy(this->last_pwd, (this-> current_pwd));
		strcpy(this->current_pwd, new_pwd);
	} // need to implement this method

	void smash_data::print_history()
	{
		for(list<char*>::iterator iter = this->history.begin(); iter != this->history.end(); iter++){
		   cout<<*iter<<endl;
		}
	}
	void smash_data::add_to_history(char* cmdString){
		if (this->history.size()>=MAX_HISTORY){
				this->history.pop_back();
			}
			this->history.push_front(cmdString);
	}
	void smash_data::print_bg_job(){
		int i=1;
		for(auto iterator = this->bg_jobs.begin(); iterator != bg_jobs.end(); iterator++){
			if((*iterator)->get_state())
				cout << "[" << i << "] : " << (*iterator)->get_pid() << (*iterator)->get_how_old()<< "secs" << endl;
			else
				cout << "[" << i << "] : " << (*iterator)->get_pid() << (*iterator)->get_how_old()<< "secs" << "(Stopped)" << endl;
			i++;
		}

	} 


	string smash_data::get_job_name(int job_index){
		auto iter = this->bg_jobs.begin();
		for(int i=1; i<=job_index; i++){
			iter++;
		}
		if(iter == NULL || *iter == NULL){
			return "-1";
		}
		return (*iter)->get_name();
	}// need to implement this method: go over the list with the iterator <job_index> times and return the relevant's name



	int smash_data::move_to_fg(int job_index){
		auto iter = this->bg_jobs.begin();
		for(int i=1; i<=job_index; i++){
			iter++;
		}
		if(iter == NULL || *iter == NULL){
			return 1;
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

	

	int smash_data::move_to_bg(int job_index_to_bg){
		if(job_index_to_bg == 0){// case no job id mentined
			auto job_to_bg = this->bg_jobs.begin();
			for(auto iter = job_to_bg; iter != bg_jobs.end(); iter++){
				if((*iter)->get_state() == SUSPENDED){
					job_to_bg = iter;
				}
			}
			if((job_to_bg != NULL) && ((job_to_bg != bg_jobs.begin())||((*job_to_bg)->get_state == SUSPENDED))){// means there is a suspended job
				cout << "signal SIGCONT was sent to pid " << (*job_to_bg)->get_pid() << endl;
				(*job_to_bg)->set_state(RUNNING);
				kill((*job_to_bg)->get_pid(), SIGCONT);
				return 0;
			}
		}
		auto job_to_bg = this->bg_jobs.begin();
		for(int i=0; i<job_index_to_bg; i++){
			iter++;
		}
		if ((*iter)->get_state() == RUNNING){ // in case that job is already running
			return 0;
		}
		cout << "signal SIGCONT was sent to pid " << (*iter)->get_pid() << endl;
		(*iter)->set_state(true);
		kill((*iter)->get_pid(), SIGCONT);
		return 0;
	}

	void smash_data::add_new_bg_job(pid_t pid_, time_t time_, string name_){
		bg_jobs.push_front(new job(pid_,time_,name_));
	}


	void smash_data::delete_bg_job(int job_index){
		auto job_to_delete = this->bg_jobs.begin();
		for(int i=0; i<job_index; i++){
			iter++;
		}
		bg_jobs.erase(iter);
	}




	void smash_data::kill_all_jobs(){
		int i = 1;
		for(auto iter = bg_jobs.begin(); iter != bg_jobs.end(); iter++){
			cout << "[" << i << "] " << (*iter)->get_name() << " - Sending SIGTERM... ";
			int start = (int)time(NULL);
			kill((*iter)->get_pid(), SIGCONT);
			while (!waitpid((*iter)->get_pid(), NULL, WNOHANG) && ((int)time(NULL) - start < WAITFORSIGTERM));
			if (!waitpid((*iter)->get_pid(), NULL, WNOHANG))
			{
				cout << "(" << WAITFORSIGTERM << " sec passed) Sending SIGKILL... ";
				kill((*it)->getPID(), SIGKILL);
			}
			cout << "Done." << endl;
			bg_jobs.erase(iter);
			i++;
		}
	}
