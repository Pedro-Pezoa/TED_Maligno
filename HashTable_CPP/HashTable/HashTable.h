#pragma once
using namespace std;
#include <iostream>
#include <ostream>
#include <string>
#include "ListaDupla.h"
#include <math.h>
#include <xhash>

template <class TipoKey, class TipoDado>
class HashTable
{
public:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------CONSTRUTORES/DESTRUTOR--------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	HashTable(const int& _novoLength = 720, const int& _novaTaxaDeCrescimento = 6, const int& _novaTaxaDeOcupacao = 50, const int& _novoTamanhoMaximoLista = 3, const int& _novaQuantidadeMaximaLista = 3,
			  const bool& ehPadrao = true) : 
			  size(0), ehPadrao(ehPadrao), length(_novoLength), taxaDeCrescimento(_novaTaxaDeCrescimento),
			  tamanhoMaximoDasListas(_novoTamanhoMaximoLista), quantidadeMaximaDeListas(_novaQuantidadeMaximaLista), taxaMaximaDeOcupacao(_novoLength * (_novaTaxaDeOcupacao / 100))
	{
		this->hashTable = (ListaDupla<NoLista<TipoDado*>*>*)malloc(this->length * sizeof(ListaDupla<NoLista<TipoDado*>*>));
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------GETTERS E SETTERS-----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//



	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------MÉTODOS PRINCIPAIS-----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	// retorna se incluiu com sucesso ou não
	bool inserir(const TipoKey& key, const TipoDado& dado)
	{
		return false;
	}

	// retorna o dado removido
	TipoDado deletar(const TipoKey& key)
	{
		return nullptr;
	}

	// devolve o dado da chave do parâmetro, caso não exista retorna nullptr
	TipoDado obter(const TipoKey& key)
	{
		return nullptr;
	}

	// retorna se o dado existe ou não
	bool existe(const TipoKey& key)
	{
		return false;
	}

	friend ostream& operator<<(ostream &sai, const HashTable<TipoKey, TipoDado>& outro)
	{
		return (sai << outro.toString());
	}

	friend class HashTable<TipoKey, TipoDado>;
protected:

	int calcularHash(const TipoKey& key)
	{
		return hashCode(key);
	}

	int calcularPosicao(const TipoKey& key)
	{
		return hashCode(key) % length;
	}

	string toString() const
	{
		return "oi";
	}

	int fatorial(const int& numero)
	{
		int fact = 1;
		for (int counter = 1; counter <= numero; counter++)
		{
			fact = fact * counter;
		}
		return fact;
	}

private:

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	ListaDupla<TipoDado*>* hashTable;
	int size;
	int length;
	int taxaDeCrescimento;
	int tamanhoMaximoDasListas;
	int quantidadeMaximaDeListas;

	float taxaMaximaDeOcupacao;
	bool ehPadrao;
};