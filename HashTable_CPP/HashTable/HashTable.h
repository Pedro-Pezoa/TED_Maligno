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
	class NoHashTable 
	{
		friend class HashTable<TipoKey, TipoDado>;
		friend class NoHashTable;
	public:
		NoHashTable(const TipoKey& novaChave = NULL, const TipoDado& novoDado = NULL)
		{
			chave = novaChave;
			dado = novoDado;
		}

		TipoKey getChave() const
		{
			return this->chave;
		}

		TipoDado getDado() const
		{
			return this->dado;
		}

		string toString() const
		{
			return "Key:" + to_string(this->chave) + " Data: " + to_string(this->dado);
		}
	private:
		TipoKey chave;
		TipoDado dado;
	};
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------CONSTRUTORES/DESTRUTOR--------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	HashTable(const bool& ehPadrao, const float& _novoLength = 6, const int& _novaTaxaDeCrescimento = 3, const float& _novaTaxaDeOcupacao = 50, const int& _novoTamanhoMaximoLista = 3) :
		size(0), ehPadrao(ehPadrao), length(_novoLength), taxaDeCrescimento(_novaTaxaDeCrescimento), valorMaximoDeOcupacao(_novaTaxaDeOcupacao),
		tamanhoMaximoDasListas(_novoTamanhoMaximoLista), quantidadeMaximaDeListas(_novoLength * (_novaTaxaDeOcupacao / 100.0)), taxaMaximaDeOcupacao(_novoLength * (_novaTaxaDeOcupacao / 100.0))
	{
		this->hashTable = (ListaDupla<NoHashTable>*)malloc(this->length * sizeof(ListaDupla<NoHashTable>));
		for (int i = 0; i < length; i++)
			*(this->hashTable + i) = ListaDupla<NoHashTable>();
	}

	HashTable(const HashTable<TipoKey, TipoDado>& outro) :
		size(outro.size), ehPadrao(outro.ehPadrao), length(outro.length), taxaDeCrescimento(outro.taxaDeCrescimento), tamanhoMaximoDasListas(outro.tamanhoMaximoDasListas), 
		valorMaximoDeOcupacao(outro.valorMaximoDeOcupacao), quantidadeMaximaDeListas(outro.quantidadeMaximaDeListas), taxaMaximaDeOcupacao(outro.taxaMaximaDeOcupacao)
	{
		this->hashTable = (ListaDupla<NoHashTable>*)malloc(this->length * sizeof(ListaDupla<NoHashTable>));
		for (int i = 0; i < length; i++)
			*(this->hashTable + i) = *(outro.hashTable + i);
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

		this->size++;
		this->balancear();
		return true;
	}

	// retorna o dado removido
	bool deletar(const TipoKey& key)
	{
		int pos = this->calcularPosicao(key);
		if (!this->isEmpty(pos))
		{
			if ((this->hashTable + pos)->getTamanho() == 1)
				(this->hashTable + pos)->removerNoFim();
			else
			{
				for (int i = 0; i < (this->hashTable + pos)->getTamanho(); i++)
				{
					if ((this->hashTable + pos)->operator[](i).chave == key)
						(this->hashTable + pos)->removerPos(i);
				}
			}
		}
		else
			return false;
		 
		this->size--;
		return true;
	}

	// devolve o dado da chave do parâmetro, caso não exista retorna nullptr
	TipoDado obter(const TipoKey& key) const
	{
		int pos = this->calcularPosicao(key);
		for (int i = 0; i < length; i++)
		{
			if ((this->hashTable + pos)->operator[](i).chave == key)
				return (this->hashTable + pos)->operator[](i).dado;
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
		string texto = "{\n";
		for (int i = 0; i < this->length; i++)
		{
			if (!this->isEmpty(i))
				texto += to_string(i) + this->hashTable[i].toString() + ";\n";
		}
		return texto + "}";
	}

	friend class HashTable<TipoKey, TipoDado>;
protected:

	bool balancear()
	{
		bool qtasListasComTamanhoEmExcesso = false;
		int qtasListas = 0;
		int qtosNos = 0;

		for (int i = 0; i < this->length; i++)
		{
			if (!this->isEmpty(i))
			{
				qtosNos++;
				if (this->hashTable[i].getTamanho() > 1)
				{
					qtasListas++;
					if (this->hashTable[i].getTamanho() > this->tamanhoMaximoDasListas)
						qtasListasComTamanhoEmExcesso = true;
				}
			}
		}

		if (qtasListasComTamanhoEmExcesso || qtasListas >= this->quantidadeMaximaDeListas || qtosNos >= this->taxaMaximaDeOcupacao)
			return this->aumentarHashTable();
			
		return false;
	}

	bool aumentarHashTable()
	{
		int novoLength = 0;
		HashTable<TipoKey, TipoDado>* aux = nullptr;
		if (this->ehPadrao)
		{
			novoLength = this->fatorial(++this->taxaDeCrescimento);
			aux = new HashTable<TipoKey, TipoDado>(true, novoLength, this->taxaDeCrescimento);
		}
		else
		{
			novoLength = this->length + this->taxaDeCrescimento;
			aux = new HashTable<TipoKey, TipoDado>(false, novoLength, this->taxaDeCrescimento, this->valorMaximoDeOcupacao, this->tamanhoMaximoDasListas);
		}

		for (int i = 0; i < this->length; i++)
		{
			if (!this->isEmpty(i))
			{
				for (int j = 0; j < (this->hashTable + i)->getTamanho(); j++)
				{
					TipoKey novaChave = (this->hashTable + i)->operator[](j).chave;
					TipoDado novoDado = (this->hashTable + i)->operator[](j).dado;
					aux->inserir(novaChave, novoDado);
				}
			}
		}

		*this = HashTable<TipoKey, TipoDado>(*aux);
		return true;
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

	bool isEmpty(const int& pos) const
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

	int valorMaximoDeOcupacao;
	float taxaMaximaDeOcupacao;
	bool ehPadrao;

	friend class NoHashTable;
};