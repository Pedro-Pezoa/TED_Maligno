// Matriz_cpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ListaDupla.h"
#include "Matriz.h"
#include <stdlib.h>

int _tmain(int argc, _TCHAR* argv[])
{
	int birl = 0;
	
	Matriz m = Matriz(20000,20000,0,1);
	
	m[19999][19999] = NULL;
	cout << m[19999][19999] << endl;

	m[0][5576]= 0;
	cout << m[0][5576] << endl;

	m[875][0] = 0;
	cout << m[875][0] << endl;

	m[125][15] = 1;
	cout << m[125][15] << endl;

	cin >> birl;

	return 0;
}

