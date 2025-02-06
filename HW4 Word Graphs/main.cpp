/**************************************
	CS 300 - DATA STRUCTURES - HOMEWORK IV

	Author: Berkay Baris Turan
	Student Id: 28132
	Date: 23.12.2021
***************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <unordered_map>
#include <vector>

using namespace std;

struct vertex
{
	string name;
	string message;

	vertex::vertex(string nm): name(nm) {}
	vertex::vertex(){}
};

int different_index(string word1, string word2)
//returns the only different index of two strings of equal sizes
//if more than one different indecx exists, returns -1
{
	int similarity = 0;
	int diff_index;

	for(int i=0; i < word1.length(); i++){
		if(word1[i] == word2[i])
			similarity++;
		else
			diff_index = i;
	}

	if(similarity + 1 == word1.length())
		return diff_index;

	return -1;
}

//added function
bool one_word_extra(string word1, string word2, string &msg){
	if(word1.length() > word2.length()){ for (int i = 1; i < word2.length(); i++){if((word2.substr(0, i) == word1.substr(0,word2.substr(0, i).length())) && (word2.substr(i) == word1.substr(word2.substr(0, i).length()+1))){ msg = "(delete " + string(1, word1[i]) + " at postion " + to_string(i+1) + ")"; return true;}}}
	else{for (int i = 1; i < word1.length(); i++){if((word1.substr(0, i) == word2.substr(0,word1.substr(0, i).length())) && (word1.substr(i) == word2.substr(word1.substr(0, i).length()+1))){msg = "(insert " + string(1, word2[i]) + " after postion " + to_string(i) + ")"; return true;}}} return false;}

bool convertable(string word1, string word2, string &msg)
// msg is the massage of word2 while converting it from word1
{
	//if lenght of word1 is greater than word2
	if(word1.length() -1 == word2.length()){
		string a = "";
		if(word1.find(word2) != string::npos){
			if(word1.find(word2) == 0)
			{
				a = a + word1[word1.length()-1];
				msg  = "(delete " + a + " at position " +to_string(word1.length()) + ")";
			}
			else if(word1.find(word2) == 1)
			{
				a = a + word1[0];
				msg  = "(delete " + a + " at position 1)";
			} 
			return true;
		} //else return one_word_extra(word1,word2,msg);
	}

	//if lenght of word2 is greater than word1
	else if(word2.length() -1 == word1.length()){
		string a = "";
		if(word2.find(word1) != string::npos){
			if(word2.find(word1) == 0)
			{
				a = a + word2[word2.length()-1];
				msg = "(insert " + a + " after position " +to_string(word1.length()) + ")";
			}
			else if(word2.find(word1) == 1)
			{
				a = a + word2[0];
				msg = "(insert " + a + " after position 0)"; //changed line
			} 
			return true;
		} //else return one_word_extra(word1,word2,msg);
	}
	//if lenghts of words are equal
	else if(word1.length() == word2.length()){
		string a = "";
		string b = "";
		int loc = different_index(word1,word2);
		if(loc != -1)
		{
			a = a + word1[loc];
			b = b + word2[loc];
			msg = "(change " + a + " at position " + to_string(loc+1) + " to "+ b + ")";
			return true;
		}
	}
	return one_word_extra(word1,word2,msg); //changed line
	return false;
}



void bfs(unordered_map<string, vector<vertex>> & graph,unordered_map<string, string> & path, string start, string end)
//applies breadth search to the graph 
{
	deque<vertex> queue;
	unordered_map<string, bool> visited(false);
	vertex v, w, starter(start);
	path[start] = start;
	string current_word;
	bool found = false;
	queue.push_front(starter);

	while(queue.size() > 0 || found == false){
		v = queue.back();
		current_word = v.name;
		queue.pop_back();
		visited[current_word] = true;

		for(int i=0; i < graph[current_word].size(); i++){
			if(visited[graph[current_word][i].name] == false){
				visited[graph[current_word][i].name] = true;
				path[graph[current_word][i].name] = v.name;
				queue.push_front(graph[current_word][i]);
				if(graph[current_word][i].name == end){
					found = true;
				}
			}
		}
	}
}

void pringPath(unordered_map<string, vector<vertex>> &graph, unordered_map<string, string> &path, string start, string end)
//prints the path from start to end string
{
	if(path[end] != end)
	{
		pringPath(graph, path, start, path[end]);
	}
	string msg = "";
	for(int i=0; i< graph[path[end]].size(); i++)
	{
		if(graph[path[end]][i].name == end)
			msg = graph[path[end]][i].message;
	}

	cout << end << msg << endl; 
}


int main(){
	ifstream file;
	string word, word1, word2, filename = "words.txt";
	unordered_map<string, vector<vertex>> graph;
	unordered_map<string, string> path;

	file.open(filename.c_str());

	while(file >> word){
		if(graph.find(word) == graph.end()){
			graph[word];
		}
	}

	//adding edges to unordered_map by iterating over it
	for(auto i: graph){
		for(auto y: graph){
			string msg = "";
			if(i.first != y.first){
				if(convertable(i.first,y.first, msg)){
					vertex added(y.first);
					added.message = msg;
					graph[i.first].push_back(added);
				}
			}
		}
	}

	while(true){
		cout << "Please enter two words: ";
		cin >> word1 >> word2;
		cout << endl;

		if(word1.substr(0,1) == "*"){
			cout << "Program terminated. " << endl;		
			break;
		}

		if((graph.find(word1) != graph.end()) && (graph.find(word2) != graph.end())){
		//If both words exist in the graph
			bfs(graph, path, word1, word2);
			pringPath(graph, path, word1, word2);
			break;
		}
		else{
		//If one or both of the words does not exist in the graph
			cout << "Entered word(s) do(es) not exist in the graph." << endl;
			break;
		}
	}
	//for debugging
	/*for(auto z: graph){
		cout << z.first << ": \n";
		for(int e = 0; e < graph[z.first].size(); e++){
			cout << "   - " <<graph[z.first][e].name << " " <<  graph[z.first][e].message << endl;
		}
		cout << endl;
	}

	for(auto i: path){
		cout << i.second << " --> " << i.first << endl;
	}*/
	cout << endl;
	file.close();

	return 0;
}