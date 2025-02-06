/*Taken from CS300 lecture slides - HashTables(2) and updated*/
#include "hashTable.h"

template <class HashedObj>
HashTable<HashedObj>::HashTable( const HashedObj & notFound, int size)
	: ITEM_NOT_FOUND( notFound ), table(size), currentSize(0) {}

template <class HashedObj>
int HashTable<HashedObj>::size_of()
{		
	return table.size();
}

template <class HashedObj>
int HashTable<HashedObj>::findPos( const HashedObj & x ) const
{
	int i = 0;   // number of collusions          
	int p = hashf(x,table.size()); // current position

	while( table[p].info != EMPTY && table[p].element != x ) {
		p += 1;  // linear probing.
	}

	if( p >= table.size()){            // perform the mod
		p = p - table.size();                // if necessary
	}
	
	return p;
}

template <class HashedObj>
void HashTable<HashedObj>::makeEmpty()
{
	for(int i=0; i<table.size(); i++)
	{
		table[i].info = EMPTY;
	}
}

template <class HashedObj>
const HashTable<HashedObj> & HashTable<HashedObj>::operator=( const HashTable<HashedObj> & rhs )
{
	rhs.currentSize = this.currentSize;
	rhs.ITEM_NOT_FOUND = this.ITEM_NOT_FOUND;
	rhs.table = std::vector<HashEntry> new_vec(currentSize);

	for(int i=0; i< this.table.size();i++)
	{
		rhs.table[i].element = this.table[i].element;
		rhs.table[i].info = this.table[i].info;
	}
}


template <class HashedObj>
bool HashTable<HashedObj>::isActive( int currentPos ) const
{
	return table[currentPos].info == ACTIVE;
}

template<class HashedObj>
int HashTable<HashedObj>::ret_position(const HashedObj &x)
{
	return findPos(x);
}

		
template <class HashedObj>
void HashTable<HashedObj>::remove( const HashedObj & x )
{
	int currentPos = findPos(x);

	if( isActive(currentPos))
		table[currentPos].info = DELETED;
}

template <class HashedObj>
const HashedObj & HashTable<HashedObj>::find( const HashedObj & x ) const
{
	
	int currentPos = findPos(x);

	if (isActive( currentPos )) 
		return table[currentPos].element; 
 
	return ITEM_NOT_FOUND;

}

template <class HashedObj>
void HashTable<HashedObj>::insert( const HashedObj & x )
{
	// Insert x as active
	int currentPos = findPos(x);

	if(isActive(currentPos))
		return;

	table[currentPos] = HashEntry(x,ACTIVE);

	//enlarge the hash table if necessary 
	if( ++currentSize >= table.size()/2)
	{
		rehash();
	}
}


template <class HashedObj>
void HashTable<HashedObj>::rehash()
{
	std::vector<HashEntry> oldArray = table;

	// Create new double-sized, empty table
	table.resize(( 2 * oldArray.size() ));

	for( int j = 0; j < table.size(); j++ )
		table[j].info = EMPTY;

	// Copy table over
	currentSize = 0;
	for( int i = 0; i < oldArray.size(); i++ )
	{
		if( oldArray[i].info == ACTIVE)
			insert( oldArray[i].element );
	}
}


int hashf(const pair_key & key,int tableSize)
{
	int p = key.code;

	if(p>= tableSize)
		p = p % tableSize;

	return p;
}

int hashf(const std::string & key, int tableSize)
{
	int p = key.length();

	if(p>= tableSize)
		p = p % tableSize;

	return p;
}

int hashf(const int & key, int tableSize)
{
	int p = key;

	if(p>= tableSize)
		p = p % tableSize;

	return p;
}