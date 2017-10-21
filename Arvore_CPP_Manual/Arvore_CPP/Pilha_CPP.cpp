#include "stdafx.h"
#include "Arvore.h"
#include "Pilha.h"
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

int main()
{
	// instanciando vari�veis importantes
	string lido = "";
	Arvore<int> arv = Arvore<int>();
	char espaco = ' ';
	string default_command = "";

	// escrevendo o painel padr�o
	cout << "BOT> Ola, digite 'insert X' ou 'inserir X' para inserir o NUMERO INTEIRO X" << endl;
	cout << "BOT> Digite 'delete X' ou 'deletar X' para deletar o NUMERO INTEIRO X" << endl;
	cout << "BOT> Digite 'exit' ou 'sair' para sair" << endl;

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
		vector<string> comando = vector<string>();
		comando = split(lido, espaco);

		// se for apenas uma palavra, ent�o pode ser s� 1 comando
		if (comando.size() == 1)
		{
			if (comando[0] == "exit" || comando[0] == "sair")
				break;
		}
		// se forem 2 palavras ou mais, pode ser que seja ou "insert" ou "delete"
		else if (comando.size() >= 2)
		{
			// se for "insert", ent�o inserimos
			if (comando[0] == "insert" || comando[0] == "inserir")
			{
				// para cada item ap�s o "insert", n�s adicionamos esse item
				for (int i = 1; i < comando.size(); i++)
				{
					// mas se n�o for um n�mero, n�o adicionamos
					if (ehNumero(comando[i]))
						arv.incluir(atoi(comando[i].c_str()));
					// mostramos para o usu�rio que ele errou na digita��o
					else
						cout << "BOT> " << comando[i] << " nao eh um numero inteiro" << endl;
				}
			}
			// se for "delete", ent�o inserimos
			else if (comando[0] == "delete" || comando[0] == "deletar")
			{
				// para cada item ap�s o "delete", n�s deletamos esse item
				for (int i = 1; i < comando.size(); i++)
				{
					// mas se n�o for um n�mero, n�o exclu�mos
					if (ehNumero(comando[i]))
						arv.excluir(atoi(comando[i].c_str()));
					// mostramos para o usu�rio que ele errou na digita��o
					else
						cout << "BOT> " << comando[i] << " nao eh um numero inteiro" << endl;
				}
			}
		}
		// printamos o resultado da �rvore, como ela est� no momento
		cout << arv << endl;
	}

	return 0; // acabamos o programa
}