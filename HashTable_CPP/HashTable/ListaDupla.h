#pragma once
using namespace std;
#include "NoLista.h"
#include <iostream>
#include <ostream>
#include <string>

template <class Tipo>
class ListaDupla
{
public:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------CONSTRUTORES/DESTRUTOR--------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	ListaDupla() : inicio(nullptr), fim(nullptr), atual(nullptr), tamanho(0), indexAtual(0) {}

	ListaDupla(const Tipo& _no) : inicio(new NoLista<Tipo>(_no)), fim(new NoLista<Tipo>(_no)), atual(nullptr), tamanho(1), indexAtual(0){}

	ListaDupla(const ListaDupla<Tipo>& _original) : inicio(_original.getInicio()), fim(_original.getFim()), atual(_original.getAtual()), tamanho(_original.getTamanho()), indexAtual(_original.getIndexAtual()) {}

	~ListaDupla() {}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------GETTERS E SETTERS-----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	NoLista<Tipo>* getInicio() const
	{
		return this->inicio;
	}

	NoLista<Tipo>* getFim() const
	{
		return this->fim;
	}

	NoLista<Tipo>* getAtual() const
	{
		return this->atual;
	}

	int getIndexAtual() const
	{
		return this->indexAtual;
	}

	int getTamanho() const
	{
		return this->tamanho;
	}

	void setInicio(const Tipo& _novoInicio)
	{
		this->inicio = new NoLista<Tipo>(_novoInicio);
	}

	void setFim(const Tipo& _novoFim)
	{
		this->fim = new NoLista<Tipo>(_novoFim);
	}

	void setAtual(const Tipo& _novoAtual)
	{
		this->atual = new NoLista<Tipo>(_novoAtual);
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------MÉTODOS PRINCIPAIS---------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	void inserirNoComeco(const Tipo& _novoDado)
	{
		if (this->isEmpty())
		{
			this->setInicio(_novoDado);
			this->setFim(_novoDado);
		}
		else
		{
			NoLista<Tipo>* novoInicio = new NoLista<Tipo>(_novoDado);

			novoInicio->setAnterior(nullptr);
			novoInicio->setProximo(this->inicio);
			this->inicio->setAnterior(novoInicio);
			this->setInicio(novoInicio);
		}
		tamanho++;
	}

	void inserirOrdenado(const Tipo& _novoDado, int _pos)
	{
		if (this->isEmpty())
		{
			this->setInicio(_novoDado);
			this->setFim(_novoDado);
		}
		else
		{
			NoLista<Tipo>* novoNo = new NoLista<Tipo>(_novoDado);
			if (_novoDado > this->operator[](_pos)->getDado())
			{
				NoLista<Tipo>* noAnterior = this->operator[](_pos);
				NoLista<Tipo>* noSeguinte = this->operator[](_pos)->getProximo();
				
				noAnterior->setProximo(novoNo);
				noSeguinte->setAnt(novoNo);
				novoNo->setAnterior(noAnterior);
				novoNo->setProx(noSeguinte);
			}
			else if (_novoDado < this->operator[](_pos)->getDado())
			{
				NoLista<Tipo>* noAnterior = this->operator[](_pos)->getAnterior();
				NoLista<Tipo>* noSeguinte = this->operator[](_pos);

				noAnterior->setProximo(novoNo);
				noSeguinte->setAnt(novoNo);
				novoNo->setAnterior(noAnterior);
				novoNo->setProx(noSeguinte);
			}
		}
		tamanho++;
	}

	void inserirNoFim(const Tipo& _novoDado)
	{
		if (this->isEmpty())
		{
			this->setInicio(_novoDado);
			this->setFim(_novoDado);
		}
		else if (this->tamanho == 1)
		{
			this->setFim(_novoDado);
			this->inicio->setProximo(this->fim);
			this->fim->setAnterior(this->inicio);
			this->fim->setProximo(nullptr);
		}
		else
		{
			NoLista<Tipo>* novoFim = new NoLista<Tipo>(_novoDado);
			novoFim->setAnterior(this->fim);
			novoFim->setProximo(nullptr);
			this->fim->setProximo(novoFim);
			this->fim = novoFim;
		}
		tamanho++;
	}

	bool removerNoComeco()
	{
		if (!this->isEmpty())
		{
			NoLista<Tipo>* novoInicio = this->inicio->getProximo();
			novoInicio->setAnterior(nullptr);
			delete(this->inicio);
			this->inicio = new NoLista<Tipo>(novoInicio);
			tamanho--;
			return true;
		}
		return false;
	}

	
	bool removerPos(int _pos)
	{
		if (!this->isEmpty() && _pos < tamanho && _pos >= 0)
		{
			if (_pos == 0)
				return this->removerNoComeco();
			else if (_pos == tamanho-1)
				return this->removerNoFim();
			else
			{
				NoLista<Tipo>* noExcluido = this->acharNoPos(_pos);
				NoLista<Tipo>* noAnterior = noExcluido->getAnterior();
				NoLista<Tipo>* noSeguinte = noAnterior->getProximo();

				noExcluido = nullptr;
				noAnterior->setProximo(noSeguinte);
				noSeguinte->setAnterior(noAnterior);
				tamanho--;
			}
			return true;
		}
		return false;
	}

	bool removerDado(int _dado)
	{
		if (!this->isEmpty() && _dado < tamanho)
		{
			if (_dado == 0)
				return this->removerNoFim();
			else if (_dado == tamanho - 1)
				return this->removerNoComeco();
			else
			{
				NoLista<Tipo>* noExcluido = this->acharNoDado(_dado);
				NoLista<Tipo>* noAnterior = new NoLista<Tipo>(noExcluido->getAnterior());
				NoLista<Tipo>* noSeguinte = new NoLista<Tipo>(noAnterior->getProximo());

				delete(noExcluido);
				noAnterior->setProximo(noSeguinte);
				noSeguinte->setAnterior(noAnterior);
			}
			tamanho--;
			return true;
		}
		return false;
	}

	bool removerNoFim()
	{
		if (this->isEmpty())
			return false;
		else if (tamanho == 1)
		{
			this->fim = nullptr;
			this->inicio = nullptr;
			this->atual = nullptr;
		}
		else
		{
			this->fim = this->fim->getAnterior();
			fim->setProximo(nullptr);
			if (tamanho == 2)
				this->inicio = this->fim;
			tamanho--;
		}
		return true;
	}

	//----------------------------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------MÉTODOS APOCALIPTICOS---------------------------------------------------------------------//
	//----------------------------------------------------------------------------------------------------------------------------------------------------------//

	friend ostream& operator<< (ostream &OS, const ListaDupla<Tipo>& aLista)
	{
		return (OS << aLista.toString());
	}

	string toString() const
	{
		if (this->inicio == nullptr)
			return "[ ]";
		string txt = "[ ";

		for(int i = 0; i < this->tamanho; i++)
		{
			txt = txt + to_string(this->operator[](i)) + ", ";
		}

		return txt.substr(0, txt.length()-2) + " ]";
	}

	//----------------------------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------MÉTODOS DE PERCURSSÃO---------------------------------------------------------------------//
	//----------------------------------------------------------------------------------------------------------------------------------------------------------//

	void iniciarPercursoSequencial(const bool &paraFrente = true)
	{
		this->atual = nullptr;
		this->indoParaFrente = paraFrente;
		this->indexAtual = (paraFrente)?-1:this->tamanho;
	}

	bool podePercorrer()
	{
		this->avancar();
		return this->indexAtual < this->tamanho;
	}

	void avancar()
	{
		if (this->indexAtual == -1)
			this->atual = this->inicio;
		else
			this->atual = this->atual->getProximo();
		this->indexAtual++;
	}

	void voltar()
	{
		if (this->indexAtual == this->tamanho)
			this->atual = this->fim;
		else
			this->atual = this->atual->getAnterior();
		this->indexAtual--;
	}

	//----------------------------------------------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------------OPERATORS-----------------------------------------------------------------------------//
	//----------------------------------------------------------------------------------------------------------------------------------------------------------//

	Tipo& operator[](const int &indice) const
	{
		if (indice < 0 || indice >= tamanho)
			return *(new Tipo());

		NoLista<Tipo>* atualReserva = this->inicio;
		int contador = 0;

		while (contador < indice && atualReserva != nullptr)
		{
			atualReserva = atualReserva->getProximo();
			contador++;
		}
		return *atualReserva->getDado();
	}

	bool operator=(const ListaDupla&)
	{
		return true;
	}

protected:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------MÉTODOS AUXILIARES---------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	bool isEmpty()
	{
		return (this->tamanho == 0);
	}

	NoLista<Tipo>* acharNoPos(const int &indice) const
	{
		if (indice < 0 || indice >= tamanho)
			return nullptr;

		NoLista<Tipo>* atualReserva = this->inicio;
		int contador = 0;

		while (contador < indice && atualReserva != nullptr)
		{
			atualReserva = atualReserva->getProximo();
			contador++;
		}
		return atualReserva;
	}

	NoLista<Tipo>* acharNoDado(const int &indice) const
	{
		if (indice < 0 || indice >= tamanho)
			return nullptr;

		NoLista<Tipo>* atualReserva = this->inicio;
		int contador = 0;

		while (contador < indice && atualReserva != nullptr)
		{
			atualReserva = atualReserva->getProximo();
			contador++;
		}
		return atualReserva;
	}
	
private:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	NoLista<Tipo> *inicio, *fim, *atual;
	bool indoParaFrente;
	int tamanho, indexAtual;
};