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
	
	Matriz<string> m = Matriz<string>(10, 10, "nao", "sim");

	m[0][0]= "sim";
	m[0][1] = "sim";
	m[0][2] = "sim";
	m[0][3] = "sim";
	m[1][3] = "sim";
	m[0][3] = "nao";
	//cout << m[0][0] << endl;

	//cout << m[1][1] << endl;
	cout << m[0][0] << endl;
	cout << m[0][1] << endl;
	cout << m[0][2] << endl;
	cout << m[0][3] << endl;
	cout << m[1][3] << endl;

	cin >> birl;

	return 0;
}

