// HashTable.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ListaDupla.h"
#include "HashTable.h"
#include <stdlib.h>
#include <string>

int main(int argc, _TCHAR* argv[])
{
	HashTable<char, int> hash = HashTable<char, int>(false, 10, 10, 0);
	//HashTable<char, int> hash = HashTable<char, int>(true);
	hash.inserir('a', 5);
	hash.inserir('f', 15);
	hash.inserir('z', 25);
	hash.inserir('g', 45);
	hash.inserir('i', 95);
	hash.inserir('k', 85);
	hash.inserir('h', 40);
	hash.inserir('l', 90);
	hash.inserir('x', 80);

	cout << hash << endl;
	/*
	hash.deletar('z');

	cout << hash << endl;
	
	cout << hash.obter('a') << endl;
	
	if (hash.existe('z'))
		cout << "existe" << endl;
	else
		cout << "nao existe" << endl;
	*/
	int birl = 0;
	cin >> birl;
	return 0;
}

