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
	//------------------------------------------------------------------CONSTRUTORES-------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	HashTable(const bool& ehPadrao, const float& _novoLength = 6, const int& _novaTaxaDeCrescimento = 3, const float& _novaTaxaDeOcupacao = 50, const int& _novoTamanhoMaximoLista = 3) :
		size(0), ehPadrao(ehPadrao), length(_novoLength), taxaDeCrescimento(_novaTaxaDeCrescimento), 
		tamanhoMaximoDasListas(_novoTamanhoMaximoLista), quantidadeMaximaDeListas(_novoLength * (_novaTaxaDeOcupacao / 100.0)), taxaMaximaDeOcupacao(_novoLength * (_novaTaxaDeOcupacao / 100.0))
	{
		if (_novoLength <= 0 || _novaTaxaDeCrescimento <= 0 || _novaTaxaDeOcupacao <= 0 || _novoTamanhoMaximoLista <= 0)
			*this = HashTable<TipoKey, TipoDado>(true);
		else
		{
			this->hashTable = (ListaDupla<NoHashTable>*)malloc(this->length * sizeof(ListaDupla<NoHashTable>));
			for (int i = 0; i < length; i++)
				*(this->hashTable + i) = ListaDupla<NoHashTable>();
		}
	}

	HashTable(const HashTable<TipoKey, TipoDado>& outro) :
		size(outro.size), ehPadrao(outro.ehPadrao), length(outro.length), taxaDeCrescimento(outro.taxaDeCrescimento), tamanhoMaximoDasListas(outro.tamanhoMaximoDasListas), 
		quantidadeMaximaDeListas(outro.quantidadeMaximaDeListas), taxaMaximaDeOcupacao(outro.taxaMaximaDeOcupacao)
	{
		this->hashTable = (ListaDupla<NoHashTable>*)malloc(this->length * sizeof(ListaDupla<NoHashTable>));
		for (int i = 0; i < length; i++)
			*(this->hashTable + i) = *(outro.hashTable + i);
	}

	//--------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------MÉTODOS PRINCIPAIS-----------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------------------------------------------//

	// retorna se incluiu com sucesso ou não
	bool inserir(const TipoKey& key, const TipoDado& dado)
	{
		NoHashTable novoNo = NoHashTable(key, dado);
		int pos = this->calcularPosicao(key);
		(this->hashTable + pos)->inserirNoFim(novoNo);

		this->size++;
		this->precisaAumentar();
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
			if ((this->hashTable + pos)->operator[](i).getChave() == key)
				return (this->hashTable + pos)->operator[](i).getDado();
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

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------CLASSE AUXLILIAR-----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	class NoHashTable
	{
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

	//--------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------MÉTODOS AUXILIARES-----------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------------------------------------------//

	bool precisaAumentar()
	{
		int qtasListas = 0;

		if (this->size >= this->taxaMaximaDeOcupacao)
			return this->aumentarHashTable();
		else
		{
			for (int i = 0; i < this->length; i++)
			{
				if (this->hashTable[i].getTamanho() > 1)
				{
					qtasListas++;
					if (this->hashTable[i].getTamanho() >= this->tamanhoMaximoDasListas)
						return this->aumentarHashTable();
				}
			}
		}

		if (qtasListas >= this->quantidadeMaximaDeListas)
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
			aux = new HashTable<TipoKey, TipoDado>(false, novoLength, this->taxaDeCrescimento, (this->taxaMaximaDeOcupacao*100) / this->length, this->tamanhoMaximoDasListas);
		}

		for (int i = 0; i < this->length; i++)
		{
			if (!this->isEmpty(i))
				this->transfereDados(*(this->hashTable + i), *aux);
		}

		*this = HashTable<TipoKey, TipoDado>(*aux);
		return true;
	}

	void transfereDados(const ListaDupla<NoHashTable>& lista, HashTable<TipoKey, TipoDado>& hash)
	{
		for (int i = 0; i < lista.getTamanho(); i++)
			hash.inserir(lista.operator[](i).getChave(), lista.operator[](i).getDado());
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

	float taxaMaximaDeOcupacao;
	bool ehPadrao;
};