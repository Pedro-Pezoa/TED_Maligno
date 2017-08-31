// Matriz_cpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ListaDupla.h"
#include "Matriz.h"
#include <stdlib.h>
#include <string>

int _tmain(int argc, _TCHAR* argv[])
{
	int birl = 0;
	
	Matriz<string> m = Matriz<string>(20000, 20000, "nao", "sim");
	
	m[19999][19999] = "";
	cout << m[19999][19999] << endl;

	m[0][5576]= "birl";
	cout << m[0][5576] << endl;

	m[875][0] = "khbwfa";
	cout << m[875][0] << endl;

	m[125][15] = "nao";
	cout << m[125][15] << endl << endl;

	cout << m[125][17] << endl;

	cin >> birl;

	return 0;
}

