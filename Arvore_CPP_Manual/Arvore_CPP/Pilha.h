#pragma once
using namespace std;
#include "NoLista.h"
#include <string>

#define TAMANHO_PILHA 100
typedef bool(*fn)(int a, int b);
typedef bool(*fnPodeDesempilhar)(int a, int b);

template <class Tipo>
class Pilha
{
public:
	// ------------------------------------------------------------------------------------------------------------------------------------------------------ -//
	//-------------------------------------------------------------CONSTRUTORES/DESTRUTOR--------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	Pilha(const unsigned int novoTamanhoMax) : tamanhoMax(novoTamanhoMax), topo(-1)
	{
		this->nos = (NoLista<Tipo>*)malloc(novoTamanhoMax*sizeof(int));
	}

	Pilha(const Pilha<Tipo> &original) : tamanhoMax(original.tamanhoMax), topo(original.topo)
	{
		this->nos = (NoLista<Tipo>*)malloc(original.tamanhoMax*sizeof(int));
		for (int i = 0; i <= original.topo; i++)
			this->empilhar(original.nos + i);
	}

	Pilha() : tamanhoMax(TAMANHO_PILHA), topo(-1)
	{
		this->nos = (NoLista<Tipo>*)malloc(TAMANHO_PILHA*sizeof(int));
	}

	~Pilha()
	{
		//free(this->nos);
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//--------------------------------------------------------------CONSTRUTORES DE CÓPIA--------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	Pilha<Tipo>* clone() const
	{
		return new Pilha<Tipo>(*this);
	}

	void operator= (Pilha<Tipo> &outro)
	{
		Pilha<Tipo> aux;
		for (int i = outro.length()-1; i > -1; i--)
		{
			aux.empilhar(outro.desempilhar());
			*(this->nos + i) = (aux.getTopo());
		}
		while (!aux.ehVazia())
			outro.empilhar(aux.desempilhar());
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------GETTERS E SETTERS----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	int length() const
	{
		return this->tamanhoMax;
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------METODOS AUXILIARES----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	bool ehCheia() const
	{
		return this->topo >= this->tamanhoMax;
	}

	bool ehVazia() const
	{
		return this->topo < 0;
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------MÉTODOS PRINCIPAIS---------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	bool empilhar(const Tipo& novoNo)
	{
		if (this->ehCheia())
			return false;
		NoLista<Tipo> no = NoLista<Tipo>(novoNo);
		*(this->nos + ++this->topo) = no;
		return true;
	}

	Tipo desempilhar()
	{
		if (!this->ehVazia())
			return *(this->nos + this->topo--).getDado();
	}

	Tipo desempilhar(fnPodeDesempilhar funcao, const Tipo &tipo)
	{
		if (!this->ehVazia() || (*funcao)(*(this->nos + this->topo).getDado(), tipo))
			return *(this->nos + this->topo--).getDado();
		return nullptr;
	}

	void esvaziar()
	{
		while (!this->ehVazia())
			this->desempilhar();
	}

	void esvaziar(fn funcao, const Tipo &tipo)
	{
		while (!this->ehVazia())
		{
			if ((*funcao)(tipo, this->topo))
				this->desempilhar();
			else
				return;
		}
	}

	NoLista<Tipo> getTopo() const
	{
		return *(this->nos + this->topo);
	}

	friend ostream& operator<< (ostream &os, const Pilha<Tipo> &aPilha)
	{
		return (os << aPilha.toString());
	}

	friend class NotepadCPP;
protected:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------METODO APOCALIPTICO-----------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	string toString() const
	{
		string txt = "[ ";
		for (int i = 0; i <= this->topo; i++)
			txt += to_string((this->nos + i)->getDado()) + "; ";
		return txt + "]";
	}
private:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	NoLista<Tipo> *nos;
	int tamanhoMax, topo;
};
