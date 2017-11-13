#pragma once
using namespace std;
#include <iostream>
#include <ostream>
#include <string>
#include "ListaDupla.h"
#include <math.h>
#include <xhash>

#define DEFAULT_LENGTH 24
#define DEFAULT_TAXA_CRESCIMENTO 4
#define DEFAULT_TAXA_OCUPACAO 50
#define DEFAULT_TAMANHO_MAXIMO_LISTA 3
#define DEFAULT_OPERACAO '*'
#define DEFAULT_DIFERENCA_TAM 2
#define DEFAULT_DIFERENCA_POS 2

template <class TipoKey, class TipoDado>

class HashTable
{
public:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------------------CONSTRUTORES-------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	// Construtor sem argumentos
	HashTable() : size(0), ehPadrao(true), length(DEFAULT_LENGTH), taxaDeCrescimento(DEFAULT_TAXA_CRESCIMENTO), tamMaxDasListas(DEFAULT_TAMANHO_MAXIMO_LISTA), 
		qtdMaxDeDados(DEFAULT_LENGTH * (DEFAULT_TAXA_OCUPACAO / 100.0)), operacao(DEFAULT_OPERACAO), ehRadical(false)
	{
		this->hashTable = (ListaDupla<NoHashTable>*)malloc(this->length * sizeof(ListaDupla<NoHashTable>));
	}

	HashTable(const bool& _ehRadical) : size(0), ehPadrao(true), length(DEFAULT_LENGTH), taxaDeCrescimento(DEFAULT_TAXA_CRESCIMENTO), tamMaxDasListas(DEFAULT_TAMANHO_MAXIMO_LISTA),
		qtdMaxDeDados(DEFAULT_LENGTH * (DEFAULT_TAXA_OCUPACAO / 100.0)), operacao(DEFAULT_OPERACAO), diferencaDeTam(DEFAULT_DIFERENCA_TAM), diferencaDePos(DEFAULT_DIFERENCA_POS),
		ehRadical(_ehRadical)
	{
		this->hashTable = (ListaDupla<NoHashTable>*)malloc(this->length * sizeof(ListaDupla<NoHashTable>));
	}

	// Construtor principal, se o primeiro parâmetro boolean for true, o construtor instância os atributos na forma padrão, se for false o construtor instância com os dados do usuário
	HashTable(const unsigned int& _novoLength, const unsigned int& _novaTaxaDeCrescimento, const unsigned int& _novaTaxaDeOcupacao, const unsigned int& _novoTamanhoMaximoLista, const char& _novaOperacao) : 
			  size(0), ehPadrao(false), length(_novoLength), taxaDeCrescimento(_novaTaxaDeCrescimento), tamMaxDasListas(_novoTamanhoMaximoLista), 
			  qtdMaxDeDados(_novoLength * (_novaTaxaDeOcupacao / 100.0)), operacao(_novaOperacao)
	{
		if (_novoLength <= 0 || _novaTaxaDeCrescimento <= 0 || _novaTaxaDeOcupacao <= 0 || _novoTamanhoMaximoLista <= 0 || (_novaOperacao != '*' && _novaOperacao != '+') || _novaTaxaDeOcupacao >= 100)
			*this = HashTable<TipoKey, TipoDado>();
		else
			this->hashTable = (ListaDupla<NoHashTable>*)malloc(this->length * sizeof(ListaDupla<NoHashTable>));
	}

	//Construtor radical
	HashTable(const bool& ehRadical, const unsigned int& _novoLength, const unsigned int& _novaTaxaDeCrescimento, const unsigned int& _novaTaxaDeOcupacao, const unsigned int& _novoTamanhoMaximoLista, 
			  const char& _novaOperacao, const unsigned int _novaDiferencaDeTam, const unsigned int _novaDiferencaDePos) :
			  size(0), ehPadrao(false), length(_novoLength), taxaDeCrescimento(_novaTaxaDeCrescimento), tamMaxDasListas(_novoTamanhoMaximoLista), qtdMaxDeDados(_novoLength * (_novaTaxaDeOcupacao / 100.0)),
			  operacao(_novaOperacao), diferencaDePos(_novaDiferencaDePos), diferencaDeTam(_novaDiferencaDeTam)
	{
		if (_novoLength <= 0 || _novaTaxaDeCrescimento <= 0 || _novaTaxaDeOcupacao <= 0 || _novoTamanhoMaximoLista <= 0 || (_novaOperacao != '*' && _novaOperacao != '+') || _novaTaxaDeOcupacao >= 100 ||
			_novaDiferencaDePos <= 0 || _novaDiferencaDeTam <= 0)
			*this = HashTable<TipoKey, TipoDado>();
		else
			this->hashTable = (ListaDupla<NoHashTable>*)malloc(this->length * sizeof(ListaDupla<NoHashTable>));
	}

	// Construtor de cópia
	HashTable(const HashTable<TipoKey, TipoDado>& outro) : size(outro.size), ehPadrao(outro.ehPadrao), length(outro.length), taxaDeCrescimento(outro.taxaDeCrescimento), tamMaxDasListas(outro.tamMaxDasListas), 
		qtdMaxDeDados(outro.qtdMaxDeDados), operacao(outro.operacao), ehRadical(outro.ehRadical), diferencaDeTam(outro.diferencaDeTam), diferencaDePos(outro.diferencaDePos)
	{
		this->hashTable = (ListaDupla<NoHashTable>*)malloc(this->length * sizeof(ListaDupla<NoHashTable>));
		for (int i = 0; i < length; i++)
			*(this->hashTable + i) = *(outro.hashTable + i);
	}

	//--------------------------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------GETTERS E SETTERS-----------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------------------------------------------//

	unsigned int getSize() const
	{
		return this->size;
	}

	unsigned int getLength() const
	{
		return this->length;
	}

	unsigned int getTaxaDeCrescimento() const
	{
		return this->taxaDeCrescimento;
	}

	unsigned int getTamMaxDasListas() const
	{
		return this->tamMaxDasListas;
	}

	unsigned int getQtdMaxDeDados() const
	{
		return this->qtdMaxDeDados;
	}

	unsigned int getDiferencaDeTam() const
	{
		return this->diferencaDeTam;
	}

	unsigned int getDiferencaDePos() const
	{
		return this->diferencaDePos;
	}

	bool isPadrao() const
	{
		return this->ehPadrao;
	}

	bool isRadical() const
	{
		return this->ehRadical;
	}

	//--------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------MÉTODOS PRINCIPAIS-----------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------------------------------------------//

	// Retorna se incluiu com sucesso ou não
	bool inserir(const TipoKey& key, const TipoDado& dado)
	{
		if (existe(key))
			return false;

		NoHashTable novoNo = NoHashTable(key, dado);
		int pos = this->calcularPosicao(key);

		if (this->isEmpty(pos))
			*(this->hashTable + pos) = ListaDupla<NoHashTable>();
		(this->hashTable + pos)->inserirNoFim(novoNo);

		this->size++;
		this->precisaAumentar();
		return true;
	}

	// Retorna o dado removido
	bool deletar(const TipoKey& key)
	{
		if (!existe(key))
			return false;
		int pos = this->calcularPosicao(key);
		if (!this->isEmpty(pos))
		{
			for (int i = 0; i < (this->hashTable + pos)->getTamanho(); i++)
			{
				if ((this->hashTable + pos)->operator[](i).getChave() == key)
				{
					(this->hashTable + pos)->removerPos(i);
					if (this->isEmpty(pos))
						(this->hashTable + pos)->~ListaDupla();
				}
			}
		}
		else
			return false;
		 
		this->size--;
		return true;
	}

	// Retorna o dado da chave do parâmetro, caso não exista retorna nullptr
	TipoDado* obter(const TipoKey& key) const
	{
		int pos = this->calcularPosicao(key);
		if (this->isEmpty(pos))
			return nullptr;
		for (int i = 0; i < (this->hashTable + pos)->getTamanho(); i++)
		{
			if ((this->hashTable + pos)->operator[](i).getChave() == key)
				return (this->hashTable + pos)->operator[](i).getPontDado();
		}
		return nullptr;
	}

	// Retorna se o dado existe ou não
	bool existe(const TipoKey& key) const
	{
		return this->obter(key) != nullptr;
	}

	// Retorna uma string com os dados da classe
	string toString() const
	{
		string texto = "{";
		for (int i = 0; i < this->length; i++)
		{
			if (!this->isEmpty(i))
				texto += to_string(i) + this->hashTable[i].toString() + ";";
		}
		return texto + "}";
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------OPERATOR---------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	// Printa o conteúdo da classe
	friend ostream& operator<<(ostream &sai, const HashTable<TipoKey, TipoDado>& outro)
	{
		return (sai << outro.toString());
	}

	friend class HashTable<TipoKey, TipoDado>;
protected:

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------STRUCTS AUXLILIAR----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	class NoHashTable
	{
	public:
		NoHashTable(const TipoKey& novaChave = NULL, const TipoDado& novoDado = NULL)
		{
			chave = new TipoKey(novaChave);
			dado = new TipoDado(novoDado);
		}

		TipoKey getChave() const
		{
			return *this->chave;
		}

		TipoDado getDado() const
		{
			return *this->dado;
		}

		TipoKey* getPontChave() const
		{
			return this->chave;
		}

		TipoDado* getPontDado() const
		{
			return this->dado;
		}

		string toString() const
		{
			return "Key:" + to_string(*this->chave) + " Data: " + to_string(*this->dado);
		}
	private:
		TipoKey* chave;
		TipoDado* dado;
	};

	//--------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------MÉTODOS AUXILIARES-----------------------------------------------------------------------//
	//--------------------------------------------------------------------------------------------------------------------------------------------------------//

	// Verifica se precisa aumentar a HashTable
	bool precisaAumentar()
	{
		if (this->size >= this->qtdMaxDeDados)
			return this->aumentarHashTable();
		else
		{
			for (int i = 0; i < this->length; i++)
			{
				if (!this->isEmpty(i) && (this->hashTable + i)->getTamanho() >= this->tamMaxDasListas)
					return this->aumentarHashTable();
			}
		}

		if (ehRadical)
		{
			int tamMin = this->tamMaxDasListas+1;
			int tamMax = 0;
			/////////////////////////////////////////////////////////////
			int atual = 0;
			int menorValor = this->length+1;

			for (int i = 0; i < this->length; i++)
			{
				if (!this->isEmpty(i) && (this->hashTable + i)->getTamanho() < tamMin)
					tamMin = (this->hashTable + i)->getTamanho();

				if (!this->isEmpty(i) && (this->hashTable + i)->getTamanho() > tamMax)
					tamMax = (this->hashTable + i)->getTamanho();
				/////////////////////////////////////////////////////////
				if (!this->isEmpty(i + 1) && ((i + 1) - atual) < menorValor)
				{
					menorValor = (i + 1) - atual;
					atual = (i + 1);
				}
			}

			if (tamMax - tamMin >= this->diferencaDeTam || menorValor >= this->diferencaDePos)
				return this->aumentarHashTable();
		}
		return false;
	}

	// Aumentar a HashTable ou na forma padrão ou na forma personalizada, dependendo da variável ehPadrao
	bool aumentarHashTable()
	{
		cout << "BOT>Desbalaceou a HASHTABLE" << endl;
		int novoLength = 0;
		HashTable<TipoKey, TipoDado>* aux = nullptr;
		if (this->ehPadrao)
		{
			if (ehRadical)
				aux = new HashTable<TipoKey, TipoDado>(true, this->fatorial(++this->taxaDeCrescimento), this->taxaDeCrescimento, (this->qtdMaxDeDados * 100) / this->length, this->tamMaxDasListas, this->operacao, this->diferencaDeTam, this->diferencaDePos);
			else
				aux = new HashTable<TipoKey, TipoDado>(this->fatorial(++this->taxaDeCrescimento), this->taxaDeCrescimento, (this->qtdMaxDeDados * 100) / this->length, this->tamMaxDasListas, this->operacao);
			aux->ehPadrao = true;
		}
		else
		{
			if (this->operacao == '+')
				novoLength = this->length + this->taxaDeCrescimento;
			else
				novoLength = this->length * this->taxaDeCrescimento;

			if (ehRadical)
				aux = new HashTable<TipoKey, TipoDado>(true, novoLength, this->taxaDeCrescimento, (this->qtdMaxDeDados * 100) / this->length, this->tamMaxDasListas, this->operacao, this->diferencaDeTam, this->diferencaDePos);
			else
				aux = new HashTable<TipoKey, TipoDado>(novoLength, this->taxaDeCrescimento, (this->qtdMaxDeDados * 100) / this->length, this->tamMaxDasListas, this->operacao);
		}

		for (int i = 0; i < this->length; i++)
		{
			if (!this->isEmpty(i))
				this->transfereDados(*(this->hashTable + i), *aux);
		}

		*this = HashTable<TipoKey, TipoDado>(*aux);
		cout << "BOT>HASHTABLE alterada --> Tamanho: " << this->getLength() << "; Quantidade maxima de dados: " << this->getQtdMaxDeDados() << "; Tamanho maximo das listas: " << this->getTamMaxDasListas() << "; " << endl;
		if (this->ehRadical)
			cout << "Diferenca de tamanho: " << this->diferencaDeTam << "; Diferenca de Posicao: " << this->diferencaDePos << ";" << endl;
		cout << endl;
		return true;
	}

	// Transfere dados de uma lista para uma HashTable
	void transfereDados(const ListaDupla<NoHashTable>& lista, HashTable<TipoKey, TipoDado>& hash)
	{
		for (int i = 0; i < lista.getTamanho(); i++)
			hash.inserir(lista.operator[](i).getChave(), lista.operator[](i).getDado());
	}

	// Método que calcula a posição do dado com a sua respectiva chave
	int calcularPosicao(const TipoKey& key) const
	{
		hash<TipoKey> hashCode;
		return hashCode(key) % this->length;
	}

	// Retorna o fatorial de um número
	int fatorial(const int& numero)
	{
		int fact = 1;
		for (int counter = 1; counter <= numero; counter++)
			fact *= counter;
		return fact;
	}

	// Verifica se a lista de uma determinada posição está vaiza
	bool isEmpty(const int& pos) const
	{
		return (this->hashTable + pos) == nullptr || (this->hashTable + pos)->isEmpty();
	}
private:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	ListaDupla<NoHashTable>* hashTable;
	unsigned int size;
	unsigned int length;
	unsigned int taxaDeCrescimento;
	unsigned int tamMaxDasListas;

	float qtdMaxDeDados;
	char operacao;
	bool ehPadrao;

	bool ehRadical;
	unsigned int diferencaDeTam;
	unsigned int diferencaDePos;
};