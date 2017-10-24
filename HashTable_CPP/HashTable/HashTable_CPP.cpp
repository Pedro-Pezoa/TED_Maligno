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
	hash.inserir('f', 15);
	hash.inserir('z', 25);
	hash.inserir('f', 55);

	cout << hash << endl;

	hash.deletar('z');

	cout << hash << endl;

	cout << hash.obter('a') << endl;

	if (hash.existe('a'))
		cout << "existe" << endl;
	else
		cout << "nao existe" << endl;

	int birl = 0;
	cin >> birl;
	return 0;
}

