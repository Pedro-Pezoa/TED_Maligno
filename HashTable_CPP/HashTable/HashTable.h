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

	HashTable(const int& _novoLength = 120, const int& _novaTaxaDeCrescimento = 5, const int& _novaTaxaDeOcupacao = 50, const int& _novoTamanhoMaximoLista = 3, const int& _novaQuantidadeMaximaLista = 10,
			  const bool& ehPadrao = true) : 
			  size(0), ehPadrao(ehPadrao), length(_novoLength), taxaDeCrescimento(_novaTaxaDeCrescimento),
			  tamanhoMaximoDasListas(_novoTamanhoMaximoLista), quantidadeMaximaDeListas(_novaQuantidadeMaximaLista), taxaMaximaDeOcupacao(_novoLength * (_novaTaxaDeOcupacao / 100))
	{
		this->hashTable = (ListaDupla<NoHashTable>*)malloc(this->length * sizeof(ListaDupla<NoHashTable>));
		for (int i = 0; i < length; i++)
			this->hashTable[i] = ListaDupla<NoHashTable>();
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------MÉTODOS PRINCIPAIS-----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	// retorna se incluiu com sucesso ou não
	bool inserir(const TipoKey& key, const TipoDado& dado)
	{
		NoHashTable novoNo = NoHashTable(key, dado);
		int pos = this->calcularPosicao(key);
		(this->hashTable + pos)->inserirNoFim(novoNo);
		this->balancear();
		return true;
	}

	// retorna o dado removido
	bool deletar(const TipoKey& key)
	{
		int pos = this->calcularPosicao(key);
		if (!this->isEmpty(pos))
		{
			for (int i = 0; i < length; i++)
			{
				if (*(this->hashTable + pos)[i].chave == key)
					(this->hashTable + pos)->removerDado(*(this->hashTable + pos)[i])
			}
		}
		else
			return false;
		 
		this->balancear();
		return true;
	}

	// devolve o dado da chave do parâmetro, caso não exista retorna nullptr
	TipoDado obter(const TipoKey& key) const
	{
		int pos = this->calcularPosicao(key);
		for (int i = 0; i < length; i++)
		{
			if (*(this->hashTable + pos)[i].chave == key)
				return *(this->hashTable + pos)[i];
		}
		return NULL;
	}

	// retorna se o dado existe ou não
	bool existe(const TipoKey& key) const
	{
		return this->obter(key) != NULL;
	}

	friend ostream& operator<<(ostream &sai, const HashTable<TipoKey, TipoDado>& outro)
	{
		return (sai << outro.toString());
	}

	string toString() const
	{
		string texto = "{ ";
		for (int i = 0; i < this->length; i++)
		{
			if (this->isEmpty(i))
				texto += this->hashTable[i].toString() + "; ";
		}
		return texto + "}";
	}

	friend class HashTable<TipoKey, TipoDado>;
protected:

	typedef struct NoHashTable
	{
		TipoKey* chave;
		TipoDado* dado;
	};

	bool balancear() const
	{
		bool verificaTamanhoLista = false;
		int verificaQuantidadeLista = 0;
		int verificaTaxaDeOcupacao = 0;

		for (int i = 0; i < this->length; i++)
		{
			if (!this->isEmpty(i))
			{
				verificaTaxaDeOcupacao++;
				if (this->hashTable[i].getTamanho() > 1)
				{
					verificaQuantidadeLista++;
					if (this->hashTable[i].getTamanho() > this->tamanhoMaximoDasListas)
						verificaTamanhoLista = true;
				}
			}
		}

		if (verificaTamanhoLista || verificaQuantidadeLista >= this->quantidadeMaximaDeListas || verificaTaxaDeOcupacao >= this->taxaMaximaDeOcupacao)
			return this->aumentarHashTable();
		return false;
	}

	bool aumentarHashTable()
	{
		if (this->ehPadrao)
		{
			int novoLength = this->fatorial(++this->taxaDeCrescimento);
			HashTable<TipoKey, TipoDado> aux = HashTable<TipoKey, TipoDado>(novoLength, this->taxaDeCrescimento);

			for (int i = 0; i < this->length; i++)
			{
				if (!this->isEmpty(i))
				{
					for (int j = 0; j < this->hashTable[i].getTamanho(); j++)
						aux.inserir(this->hashTable[i][j].getDado().chave, this->hashTable[i][j].getDado().dado);
				}
			}
		}
	}

	int calcularPosicao(const TipoKey& key) const
	{
		hash<TipoDado> hashCode;
		return hashCode(key) % this->length;
	}

	int fatorial(const int& numero)
	{
		int fact = 1;
		for (int counter = 1; counter <= numero; counter++)
			fact *= counter;
		return fact;
	}

	bool isEmpty(const int pos)
	{
		return (this->hashTable + pos)->isEmpty();
	}

private:

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	ListaDupla<NoHashTable>* hashTable;
	int size;
	int length;
	int taxaDeCrescimento;
	int tamanhoMaximoDasListas;
	int quantidadeMaximaDeListas;

	float taxaMaximaDeOcupacao;
	bool ehPadrao;
};