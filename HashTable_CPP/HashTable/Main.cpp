// HashTable.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ListaDupla.h"
#include "HashTable.h"
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>

void split(const string &s, char delim, vector<string> *resultado) 
{
	stringstream streamDeString; // Um stream de string, para ler/escrever em streams
	streamDeString.str(s);       // colocamos a nossa string nesse stream
	string item;                 // criamos uma string para representar cada item lido do stream
	while (getline(streamDeString, item, delim)) // lê a string do stream, guardando o resultado em "item"
		resultado->push_back(item); // adicionamos ao final o item separado pelo delimitador
}

vector<string> split(const string &s, char delim) 
{
	vector<string> elems; // o que será retornado
	split(s, delim, &elems); // usamos ele pro split
	return elems; // retornamos ele
}

string to_string(string str)
{
	return str;
}

bool ehNumero(string str)
{
	char* p; // criamos um ponteiro "p" que não aponta pra nada
	int converted = strtol(str.c_str(), &p, 10); // vemos se "p" tem algo apontado
	return !(*p); // se tiver, deu errado a conversão, e não tiver, não deu erro
}

int main(int argc, _TCHAR* argv[])
{
	HashTable<string, int> hash;
	char espaco = ' ';
	string lido = "";
	string default_command = "";

	string resposta = "";
	string outraResposta = "";
	string valor1 = "";
	string valor2 = "";
	string valor3 = "";
	string valor4 = "";
	string valor5 = "";
	string valor6 = "";
	string valor7 = "";

	// escrevendo o painel padrão
	cout << "BOT> Ola, digite 'insert <chave> <valor>' ou 'inserir <chave> <valor>' para inserir o NUMERO INTEIRO  na CHAVE STRING" << endl;
	cout << "BOT> Digite 'delete X' ou 'deletar X' para deletar a CHAVE X" << endl;
	cout << "BOT> Digite 'exit' ou 'sair' para sair" << endl << endl;

	pedirTipoHashTable:
	cout << "BOT> Deseja criar uma HashTable padrao?" << endl;
	cout << "USER> <[s]/n> ";
	cin >> resposta;

	cout << "BOT> Deseja criar uma HashTable radical?" << endl;
	cout << "USER> <[s]/n> ";
	cin >> outraResposta;
	
	if (resposta == "n" || resposta == "N")
	{
		cout << "BOT> HashTable personalizada selecionada" << endl;
		cout << "BOT> Digite os respectivos dados (sem ser zero)" << endl;

		cout << "BOT> Tamanho: " << endl;
		cin >> valor1;
		cout << "BOT> Taxa de crescimento: " << endl;
		cin >> valor2;
		cout << "BOT> Taxa de ocupacao(em porcentagem): " << endl;
		cin >> valor3;
		cout << "BOT> Tamanho das listas: " << endl;
		cin >> valor4;
		cout << "BOT> Operação(+ ou *): " << endl;
		cin >> valor5;
		if (!ehNumero(valor1) || !ehNumero(valor2) || !ehNumero(valor3) || !ehNumero(valor4) || valor5 != "*" || valor5 != "+")
			goto pedirTipoHashTable;
		
		if (outraResposta == "n" || outraResposta == "N")
		{
			pedirTipoRadicalHashTable:
			cout << "BOT> Diferença de tamanho: " << endl;
			cin >> valor6;
			cout << "BOT> Diferença de posicao: " << endl;
			cin >> valor7;

			hash = HashTable<string, int>(true, atoi(valor1.c_str()), atoi(valor2.c_str()), atoi(valor3.c_str()), atoi(valor4.c_str()), valor5[0], atoi(valor6.c_str()), atoi(valor7.c_str()));
		}
		else
			hash = HashTable<string, int>(atoi(valor1.c_str()), atoi(valor2.c_str()), atoi(valor3.c_str()), atoi(valor4.c_str()), valor5[0]);
	}
	else
	{
		cout << endl;
		if (outraResposta == "n" || outraResposta == "N")
		{
			cout << "BOT> HashTable padrao selecionada" << endl;
			hash = HashTable<string, int>();
		}
		else
		{
			cout << "BOT> HashTable padrao radical selecionada" << endl;
			hash = HashTable<string, int>(true);
		}
		cout << "BOT> Operador de crescimento: fatorial(!) Tamanho: 4!; Quantidade maxima de dados: " << hash.getQtdMaxDeDados() 
			 << "; Tamanho maximo das listas: " << hash.getTamMaxDasListas() << "; ";
		if (hash.isRadical())
			cout << "Diferenca de tamanho: " << hash.getDiferencaDeTam() << "; Diferenca de Posicao: " << hash.getDiferencaDePos() << ";" << endl;
	}
	

	// loop infinito para o menu
	while (1)
	{
		// deixamos o usuário escolher o que deseja
		if (default_command == "")
		{
			cout << endl << endl << "USER> ";
			getline(cin, lido);
		}
		else
		{
			lido = default_command;
			default_command = "";
		}

		// guardamos o que o usuário digitou em um vector, separado por espaços
		vector<string> comando = vector<string>();
		comando = split(lido, espaco);

		// se for apenas uma palavra, então pode ser só 1 comando
		if (comando.size() == 1)
		{
			if (comando[0] == "exit" || comando[0] == "sair")
				break;
		}
		// se forem 2 palavras ou mais, pode ser que seja ou "insert" ou "delete"
		else if (comando.size() >= 2)
		{
			// se for "insert", então inserimos
			if (comando[0] == "insert" || comando[0] == "inserir")
			{
				// para cada item após o "insert", nós adicionamos esse item
				if (comando.size() % 2 == 1)
				{
					for (int i = 1; i < comando.size(); i += 2)
					{
						// mas se não for um número, não adicionamos
						if (ehNumero(comando[i+1]))
							cout << ((hash.inserir(comando[i].c_str(), atoi(comando[i+1].c_str())))?("BOT> Adicionou a chave "+comando[i+1]):("BOT> Ja existe essa chave")) << endl;
						// mostramos para o usuário que ele errou na digitação
						else
							cout << "BOT> '" << comando[i+1] << "' nao eh um numero inteiro" << endl;
					}
				}
				else
					cout << "BOT> Você errou tiu" << endl;
			}
			// se for "delete", então deletamos
			else if (comando[0] == "delete" || comando[0] == "deletar")
			{
				// para cada item após o "delete", nós deletamos esse item
				for (int i = 1; i < comando.size(); i++)
					cout << ((hash.deletar(comando[i])) ? ("BOT> Excluiu a chave " + comando[i]) : ("BOT> Nao existe essa chave")) << endl;
			}
			else if (comando[0] == "ver" || comando[0] == "view")
			{
				int* dado = hash.obter(comando[1]);
				if (dado != nullptr)
					cout << "BOT> O valor de '" << comando[1] << "' eh: " << *dado << endl;
				else
					cout << "BOT> Essa chave nao existe" << endl;
			}
		}
		// printamos o resultado da árvore, como ela está no momento
		cout << hash << endl;

	}
	return 0;
}

