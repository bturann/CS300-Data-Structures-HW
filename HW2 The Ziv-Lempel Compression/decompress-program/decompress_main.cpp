/***********************************************
	CS 300 - DATA STRUCTURES - HOMEWORK II
	
	DECOMPRESS PROGRAM

	Author: Berkay Baris Turan
	Student Id: 28132
	Date: 15.11.2021
************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct pairwise
{
	string key;
	int code;
};

void mapASCII(pairwise *dict)
//maps ASCII characters into the vector.
{
	int i = 0;

	for(int i=0; i<256; i++)
	{
		string a = "";
		a += char(i);
		dict[i].key = a;
		dict[i].code = i;
	}
}

void add_to_array(pairwise *dict, string prefix)
//adds given prefix into the array (linearly).
{
	int i = 0;

	while(dict[i].key != "")
	{
		i++;
	}

	dict[i].key = prefix;
	dict[i].code = i;
}

int main()
{
	ifstream inp_file("compout");
	ofstream out_file("decompout");
	pairwise dictionary[4096];
	string line, prefix, prev_prefix = ""; 
	pairwise decompressed;

	mapASCII(&dictionary[0]);

	int number,prev_num = 0;

	while(getline(inp_file,line))
	{
		istringstream input(line);

		while(input >> number)
		{
			if(dictionary[number].key == "")
			{
				prefix = prev_prefix + prev_prefix[0];
				dictionary[number].key = prefix;
				dictionary[number].code = number;

				decompressed = dictionary[number];
				

				out_file << decompressed.key;

				cout << decompressed.key;
				//cout << decompressed.code << " ";
			}
			else
			{
				if(prev_prefix != "")
				{
					prefix = prev_prefix + dictionary[number].key[0];
					add_to_array(dictionary, prefix);
				}

				decompressed = dictionary[number];
				out_file << decompressed.key;

				cout << decompressed.key;
				//cout << decompressed.code << " ";
			}

			prev_num = number;
			prev_prefix = dictionary[prev_num].key;
		}
	}

	cout << "Decompressed into the file decompout" << endl;

	inp_file.close();
	out_file.close();

	return  0;
}