/***********************************************
	CS 300 - DATA STRUCTURES - HOMEWORK II
	
	COMPRESS PROGRAM

	Author: Berkay Baris Turan
	Student Id: 28132
	Date: 18.11.2021
************************************************/

#include <iostream>
#include <string>
#include <vector>
#include "hashTable.h"
#include <fstream>
#include "pair_key.h"
using namespace std;

int TABLE_SIZE = 4096;
pair_key notFound("-100&",-100);

void mapASCII(HashTable<pair_key> & table)
//maps ASCII characters into the vector.
{
	int i = 0;

	for(int i=0; i<256; i++)
	{
		string a = "";
		a += char(i);
		pair_key obj(a,i);
		table.insert(obj);
	}
}

int main()
{
	HashTable<pair_key> table(notFound,TABLE_SIZE);
	ifstream file("compin"); 
	ofstream out_file("compout");

	char ch;
	string message = "";
	pair_key compressed;

	mapASCII(table);


	while(file.get(ch))
	{
		message += ch;
	}

	while(message!="")
	//compresses longest prefix, then deletes it from message
	{
		int len_current = 1;
		pair_key current(message.substr(0,len_current));
		
		while((table.find(current) != notFound) && len_current<message.length())
		{
			len_current++;
			current.key = message.substr(0,len_current);
		}


		if(len_current<message.length())
		{
			compressed = pair_key(message.substr(0,len_current-1),table.ret_position(message.substr(0,len_current-1)));
			table.insert(current);
			current.code = table.ret_position(current);
			message = message.substr(len_current-1);
		}

		else
		{
			if(table.find(current) != notFound)
			{
				compressed = pair_key(message,table.ret_position(message));
				message = "";
			}

			else
			{
				compressed = pair_key(message.substr(0,len_current-1),table.ret_position(message.substr(0,len_current-1)));
				message = message.substr(len_current-1);
			}
	
		}

		out_file << compressed.code << " ";
	}

	cout << "Compressed into the file compout" << endl;

	file.close();
	out_file.close();
	
	return 0;
}