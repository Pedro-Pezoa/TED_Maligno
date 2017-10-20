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
		hash
		this->hashTable = (ListaDupla<NoLista<TipoDado>>*)malloc(this->length * sizeof(ListaDupla<NoLista<TipoDado>>));
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------GETTERS E SETTERS-----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//



	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------MÉTODOS PRINCIPAIS-----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	bool inserir(const TipoKey& key, const TipoKey& dado)
	{
		
	}

	int calcularHash(const TipoKey& key)
	{
		return key.hashCode();
	}

	friend ostream& operator<<(ostream &sai, const HashTable<TipoKey, TipoDado>& outro)
	{
		return (sai << outro.toString() << endl);
	}
	friend class HashTable<TipoKey, TipoDado>;
protected:
	string toString() const
	{
		return "oi";
	}

	int fatorial(const int& numero)
	{
		if (numero < 1)
			return 1;
		if (numero == 1 || numero == 1)
			return numero;
		return numero * this->fatorial(numero-1);
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	
	ListaDupla<NoLista<TipoDado>>* hashTable;
	int size;
	int length;
	int taxaDeCrescimento;
	int tamanhoMaximoDasListas;
	int quantidadeMaximaDeListas;

	float taxaMaximaDeOcupacao;
	bool ehPadrao;

private:
};