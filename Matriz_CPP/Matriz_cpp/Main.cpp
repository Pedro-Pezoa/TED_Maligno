// Matriz_cpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ListaDupla.h"
#include "Matriz.h"
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>

void split(const string &s, char delim, vector<string> *resultado) {
	stringstream streamDeString; // Um stream de string, para ler/escrever em streams
	streamDeString.str(s);       // colocamos a nossa string nesse stream
	string item;                 // criamos uma string para representar cada item lido do stream
	while (getline(streamDeString, item, delim)) // l� a string do stream, guardando o resultado em "item"
		resultado->push_back(item); // adicionamos ao final o item separado pelo delimitador
}

vector<string> split(const string &s, char delim) {
	vector<string> elems; // o que ser� retornado
	split(s, delim, &elems); // usamos ele pro split
	return elems; // retornamos ele
}

bool ehNumero(string str)
{
	char* p; // criamos um ponteiro "p" que n�o aponta pra nada
	int converted = strtol(str.c_str(), &p, 10); // vemos se "p" tem algo apontado
	return !(*p); // se tiver, deu errado a convers�o, e n�o tiver, n�o deu erro
}

int _tmain(int argc, _TCHAR* argv[])
{
	// instanciando vari�veis importantes
	string lido = "";
	char espaco = ' ';
	string default_command = "";
	Matriz<int>* matrix;
	vector<string> comando;
	
	// escrevendo o painel padr�o
	cout << "BOT> Ola, digite 'insert X' ou 'inserir X' para inserir o NUMERO INTEIRO X" << endl;
	cout << "BOT> Digite 'delete X' ou 'deletar X' para deletar o NUMERO INTEIRO X" << endl;
	cout << "BOT> Digite 'exit' ou 'sair' para sair" << endl;

	// obtendo o tamanho da matriz pelo
	// insistente input do usu�rio
	comecoLoop:
	do
	{
		cout << "BOT> Digite o tamanho da matriz(n n): " << endl;
		cout << "USER> ";
		getline(cin, lido);
		comando = vector<string>();
		comando = split(lido, espaco);
	} while (comando.size() != 2);

	// se o que o usu�rio digitou n�o est� em formato num�rico, voltamos
	if (!ehNumero(comando[0]) || !ehNumero(comando[1]))
		goto comecoLoop;

	// criando a matriz com o tamanho pedido pelo usu�rio
	matrix = new Matriz<int>(atoi(comando[0].c_str()), atoi(comando[1].c_str()), 0);

	// loop infinito para o menu
	while (1)
	{
		// deixamos o usu�rio escolher o que deseja
		if (default_command == "")
		{
			cout << endl << "USER> ";
			getline(cin, lido);
		}
		else
		{
			lido = default_command;
			default_command = "";
		}

		// guardamos o que o usu�rio digitou em um vector, separado por espa�os
		comando = vector<string>();
		comando = split(lido, espaco);

		// se for apenas uma palavra, ent�o pode ser s� 1 comando
		if (comando.size() == 1)
		{
			if (comando[0] == "exit" || comando[0] == "sair")
				break;
		}
		// se forem 2 palavras ou mais, pode ser que seja ou "insert", "delete" ou "view"
		else if (comando.size() >= 2)
		{
			// se for "insert", ent�o inserimos
			if (comando[0] == "insert" || comando[0] == "inserir")
			{
				if ((comando.size()-1) % 3 == 0)
				{
					// para cada item ap�s o "insert", n�s adicionamos esse item
					for (int i = 1; i < comando.size(); i += 3)
					{
						// mas se n�o for um n�mero, n�o adicionamos
						if (ehNumero(comando[i]) && ehNumero(comando[i + 1]) && ehNumero(comando[i + 2]))
						{
							(*matrix)[atoi(comando[i].c_str())][atoi(comando[i].c_str()) + 1] = atoi(comando[i + 2].c_str());
							cout << "BOT> O valor " << (*matrix)[atoi(comando[i].c_str())][atoi(comando[i].c_str()) + 1] << " foi colocado na matriz." << endl;
						}// mostramos para o usu�rio que ele errou na digita��o
						else
							cout << "BOT> " << comando[i] << " nao eh um numero inteiro." << endl;
					}
				}
				else
					cout << "BOT> Voce precisa digitar algo coerente." << endl;
			}
			// se for "delete", ent�o deletamos
			else if (comando[0] == "delete" || comando[0] == "deletar")
			{
				if (comando.size() % 2 == 1)
				{
					// para cada item ap�s o "delete", n�s deletamos esse item
					for (int i = 1; i < comando.size(); i += 2)
					{
						// mas se n�o for um n�mero, n�o exclu�mos
						if (ehNumero(comando[i]) && ehNumero(comando[i + 1]))
							(*matrix)[atoi(comando[i].c_str())][atoi(comando[i].c_str()) + 1] = 0;
						// mostramos para o usu�rio que ele errou na digita��o
						else
							cout << "BOT> " << comando[i] << " nao eh um numero inteiro." << endl;
					}
				}
				else
					cout << "BOT> Digite dados coerentes." << endl;
			}
			else if (comando[0] == "view" || comando[0] == "ver")
			{
				if (comando.size() % 2 == 1)
				{
					// para cada item ap�s o "view", n�s mostramos esse item
					for (int i = 1; i < comando.size(); i += 2)
					{
						// mas se n�o for um n�mero, n�o mostramos e avisamos o erro
						if (ehNumero(comando[i]))
							cout << "O dado eh: " << (*matrix)[atoi(comando[i].c_str())][atoi(comando[i].c_str()) + 1] << endl;
						else
							cout << "BOT> " << comando[i] << " nao eh um numero inteiro." << endl;
					}
				}
				else
				{
					cout << "BOT> Digite dados coerentes." << endl;
				}
			}
		}
		// printamos o resultado da matriz, como ela est� no momento
		// mas se a matriz for muito grande, ocasionar� erros
		//cout << *matrix << endl;
	}

	return 0; // acabamos o programa
}

