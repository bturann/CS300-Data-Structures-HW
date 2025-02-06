/***********************************************
	CS 300 - DATA STRUCTURES - HOMEWORK I

	Author: Berkay Baris Turan
	Student Id: 28132
	Date: 30.10.2021
	Updated: 09.11.2021
************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream> //for ifstream & ofstream
#include <sstream> //for istringstream
using namespace std;

struct process
{
	vector<string> process_values;
	string name;

	process() {} //default constructor
	process(string nm): name(nm) {} //constructor with parameter
};

struct MLFQ
{
	vector<queue<process>> queue_vector;
	int number_of_queues;
	int number_of_processes;
	int size_of_S;
};

/* Functions for controling */
/*
void display_process_object(process p)
{
	cout << endl << "Id of the process: " << p.name << endl << "Content: ";


	for(int i=0;i<p.process_values.size();i++)
	{
		cout << p.process_values[i] << " ";
	}
	cout << endl << endl;
}
*/
/*
void display_content__of_MLFQ(MLFQ sch)
{
	if(!MLFQ_is_empty(sch))
	{
		for(int i=0; i< sch.queue_vector.size(); i++)
		{
			cout << endl <<"Content of Q " << (sch.queue_vector.size()-i) << ":" << endl;
			cout << "Size: " << sch.queue_vector[i].size() << endl;

			while(!sch.queue_vector[i].empty())
			{
				cout << "--> " << sch.queue_vector[i].front().name<< endl;
				display_process_object(sch.queue_vector[i].front());
				sch.queue_vector[i].pop();
			}
			cout << endl;
		}
	}
	else
	{
		cout << "MLFQ is empty." << endl;
	}
}
*/

bool MLFQ_is_empty(MLFQ sch)
//returns true if all processes are done, false vice versa
{
	int counter = 0;
	for(int i=0; i<sch.queue_vector.size(); i++)
	{
		counter += sch.queue_vector[i].size();
	}

	if(counter == 0)
		return true;

	return false;
}

void operation_in_a_time_slice(MLFQ & sch, ofstream & out, int limit)
//does the operation until the limit of time slice is reached
{
	int count = 0;
	bool control = true;

	for(int i=0; (i < sch.queue_vector.size()) && (count < limit); i++)
	{
		while(!sch.queue_vector[i].empty() && (count < limit))
		{
			int current_size = sch.queue_vector[i].front().process_values.size();
			process temp = sch.queue_vector[i].front();
			sch.queue_vector[i].pop();
			
			int j = 0;
			while(temp.process_values[j] == "")
				j++;

			if(temp.process_values[j+1] == "-")
			{
				temp.process_values[j].pop_back();
				temp.process_values[j+1].pop_back();
				out << "E, " << temp.name << ", QX" << endl;
			}
			else if(temp.process_values[j] == "0")
			{
				temp.process_values[j].pop_back();
				sch.queue_vector[i].push(temp);
				out << "0, " << temp.name << ", Q" << (sch.queue_vector.size()-i) << endl;
			}
			else if(temp.process_values[j] == "1")
			{
				if(i == (sch.queue_vector.size()-1))
				{
					temp.process_values[j].pop_back();
					sch.queue_vector[i].push(temp);
					out << "1, " << temp.name << ", Q" << (sch.queue_vector.size()-i) << endl;
				}
				else
				{
					temp.process_values[j].pop_back();
					sch.queue_vector[i+1].push(temp);
					out << "1, " << temp.name << ", Q" << (sch.queue_vector.size()-i-1) << endl;
				}
			}
			count++;
		}
	}
}

void take_all_topmost_queue(MLFQ & sch, ofstream & out)
//ofter time S, this function removes all of the processes to the topmost queue
{
	for(int i=1; i<sch.queue_vector.size(); i++)
	{
		while(!sch.queue_vector[i].empty())
		{
			process temp = sch.queue_vector[i].front();
			sch.queue_vector[i].pop();
			sch.queue_vector[0].push(temp);
			out << "B, " << temp.name << ", Q" << sch.number_of_queues << endl; //UPDATED LINE!!!
		}
	}
}

int main()
{
	MLFQ scheduler;
	string folder_name; 
	ifstream conf_file, process_file; 
	ofstream output;

	cout << "Please enter the process folder name: ";
	cin >> folder_name;
	cout << "When all processes are completed, you can find execution sequence in" << endl << "\""<< folder_name << "/output.txt\"." << endl;

	string conf_file_name = folder_name + "/configuration.txt";

	conf_file.open(conf_file_name.c_str());

	if(conf_file.fail())
		cout << "Cannot open the configuration file." << endl;

	while(!conf_file.eof())
		conf_file >> scheduler.number_of_queues >> scheduler.number_of_processes >> scheduler.size_of_S;

	for(int i=0; i<scheduler.number_of_queues; i++) //initializing priority pueues
	{
		queue<process> q;
		scheduler.queue_vector.push_back(q);
	}

	for(int i=1; i <= scheduler.number_of_processes; i++) //initializing processes and placing them on the highest priority queue
	{
		process p("PC"+to_string(i));
		string file_name = folder_name + "/p" + to_string(i) + ".txt", operation;
		process_file.open(file_name.c_str());

		if(process_file.fail())
			cout << "Cannot open " << file_name << endl;

		while(!process_file.eof())
		{
			process_file >> operation;
			p.process_values.push_back(operation);
		}
		scheduler.queue_vector[0].push(p);

		process_file.close();
	}

	conf_file.close();

	string file_name_output = folder_name + "/output.txt";

	output.open(file_name_output.c_str());

	if(output.fail())
		cout << "Cannot open " << file_name_output  << endl;
	
	while(!MLFQ_is_empty(scheduler)) //scheduling processes until all processes are done
	{
		operation_in_a_time_slice(scheduler, output, scheduler.size_of_S);
		take_all_topmost_queue(scheduler, output);
	}

	output.close();

	return 0;
}