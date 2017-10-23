// HashTable.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ListaDupla.h"
#include "HashTable.h"
#include <stdlib.h>
#include <string>

int main(int argc, _TCHAR* argv[])
{
	HashTable<char, int> hash = HashTable<char, int>();
	hash.inserir('a', 5);
	//hash.inserir('b', 15);
	//hash.inserir('c', 25);

	cout << hash;

	int birl = 0;
	cin >> birl;
	return 0;
}

