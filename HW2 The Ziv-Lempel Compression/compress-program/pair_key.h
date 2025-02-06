#ifndef PAIR_KEY_H
#define PAIR_KEY_H

#include <iostream>

class pair_key
{
public:
	pair_key(std::string a, int b= 0);
	pair_key();

	std::string key;
	int code;
	
};

pair_key::pair_key(std::string a, int b)
{
	key = a;
	code = b;
}

pair_key::pair_key() {}

bool operator==(const pair_key& right,const pair_key& left)
{
	return ((right.key == left.key));
}

bool operator !=(const pair_key& right,const pair_key& left)
{
	return ((right.key != left.key));
}


#endif