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

	HashTable(const int& _novoLength = 120, const int& _novaTaxaDeCrescimento = 5, const int& _novaTaxaDeOcupacao = 50, const int& _novoTamanhoMaximoLista = 3, const int& _novaQuantidadeMaximaLista = 3,
			  const bool& ehPadrao = true) : 
			  size(0), ehPadrao(ehPadrao), length(_novoLength), taxaDeCrescimento(_novaTaxaDeCrescimento),
			  tamanhoMaximoDasListas(_novoTamanhoMaximoLista), quantidadeMaximaDeListas(_novaQuantidadeMaximaLista), taxaMaximaDeOcupacao(_novoLength * (_novaTaxaDeOcupacao / 100))
	{
		this->hashTable = (ListaDupla<TipoDado>*)malloc(this->length * sizeof(ListaDupla<TipoDado>));
		for (int i = 0; i < length; i++)
		{
			this->hashTable[i] = ListaDupla<TipoDado>();
		}
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
		int pos = this->calcularPosicao(key);
		(this->hashTable + pos)->inserirNoFim(dado);
		return true;
	}

	// retorna o dado removido
	bool deletar(const TipoKey& key, bool podeApagarLista = false)
	{
		if (key == nullptr)
			return false;

		int pos = this->calcularPosicao(key);
		if ((this->hashTable + pos) == nullptr)
			return false;

		else if ((this->hashTable + pos)->getTamanho() == 1 || podeApagarLista)
		{
			(this->hashTable + pos)->removerNoFim();
			delete(this->hashTable + pos);
		}
		else
			(this->hashTable + pos)->removerNoFim();
		return true;
	}

	// devolve o dado da chave do parâmetro, caso não exista retorna nullptr
	TipoDado obter(const TipoKey& key) const
	{
		if (key == nullptr)
			return nullptr;

		int pos = this->calcularPosicao(key);
		if (*(this->hashTable + pos) == nullptr)
			return nullptr;

		return (this->hashTable + this->calcularPosicao(key))->getFim();
	}

	// retorna se o dado existe ou não
	bool existe(const TipoKey& key) const
	{
		return this->obter(key) != nullptr;
	}

	friend ostream& operator<<(ostream &sai, const HashTable<TipoKey, TipoDado>& outro)
	{
		return (sai << outro.toString());
	}

	string toString() const
	{
		string texto = "[ ";
		for (int i = 0; i < this->length; i++)
		{
			if ((this->hashTable + i) != nullptr)
				texto += i + "-" + (this->hashTable + i)->toString() + "; ";
		}
		return texto + " ]";
	}

	friend class HashTable<TipoKey, TipoDado>;
protected:

	int calcularPosicao(const TipoKey& key)
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

private:

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	ListaDupla<TipoDado>* hashTable;
	int size;
	int length;
	int taxaDeCrescimento;
	int tamanhoMaximoDasListas;
	int quantidadeMaximaDeListas;

	float taxaMaximaDeOcupacao;
	bool ehPadrao;
};