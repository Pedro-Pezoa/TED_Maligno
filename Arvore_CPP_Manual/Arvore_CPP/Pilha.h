#pragma once
using namespace std;
#include "NoLista.h"

template <class Tipo>
class Pilha
{
public:
	// ------------------------------------------------------------------------------------------------------------------------------------------------------ -//
	//-------------------------------------------------------------CONSTRUTORES/DESTRUTOR--------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	Pilha(const unsigned int novoTamanhoMax) : tamanhoMax(novoTamanhoMax), tamanhoAtual(0)
	{
		NoLista<Tipo> *pont = (NoLista<Tipo>*)malloc(novoTamanhoMax * sizeof(NoLista<Tipo>));
		for (int i = 0; i < novoTamanhoMax; i++)
		{
			*(pont + i) = NoLista<Tipo>();
			(pont + i)->setDado(Tipo());
			if (i != 0)
				(pont+i)->setAnterior(*(pont + i - 1));
		}
		for (int i = 0; i < tamanhoMax; i++)
			if (i != tamanhoMax - 1)
				(pont + i)->setProximo(*(pont + i + 1));
		this->alocaNo(pont, novoTamanhoMax);
	}

	Pilha(const Pilha<Tipo> &original) : tamanhoMax(original.tamanhoMax), tamanhoAtual(original.tamanhoAtual)
	{
		this->alocaNo(original.nos, original.tamanhoMax);
	}

	Pilha() : tamanhoMax(50), tamanhoAtual(0)
	{
		NoLista<Tipo> *pont = (NoLista<Tipo>*)malloc(tamanhoMax * sizeof(NoLista<Tipo>));
		for (int i = 0; i < tamanhoMax; i++)
		{
			*(pont + i) = NoLista<Tipo>();
			(pont + i)->setDado(Tipo());
			if (i != 0)
				(pont+i)->setAnterior(*(pont + i - 1));
		}
		for (int i = 0; i < tamanhoMax; i++)
			if (i != tamanhoMax - 1)
				(pont + i)->setProximo(*(pont + i + 1));

		this->alocaNo(pont, 50);
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
			aux.empilhar(outro.desempilhar().getDado());
			*(this->nos + i) = (aux.getTopo());
		}
		while (!aux.ehVazia())
			outro.empilhar(aux.desempilhar().getDado());
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------MÉTODOS PRINCIPAIS---------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	bool ehCheia() const
	{
		return this->tamanhoAtual >= this->tamanhoMax;
	}

	bool ehVazia() const
	{
		return this->tamanhoAtual <= 0;
	}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------GETTERS E SETTERS-----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	int length() const
	{
		return this->tamanhoAtual;
	}

	String toString() const
	{
		String txt = String();
		for (int i = 0; i < this->tamanhoAtual; i++)
			txt = txt + (this->nos + i)->getDado().toString();
		return String(txt + " " + (int)this->tamanhoAtual + " " + (int)this->tamanhoMax + "\n");
	}

	friend class NotepadCPP;
protected:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------MÉTODOS AUXILIARES---------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	NoLista<Tipo> valorDe(const int &indice) const
	{
		return *(this->acoes + indice);
	}

	void alocaNo(const NoLista<Tipo> *novoNo, const unsigned int tam)
	{
		this->nos = (NoLista<Tipo>*)malloc(tam * sizeof(NoLista<Tipo>));
		for (int i = 0; i < tam; i++)
			*(this->nos + i) = *(novoNo + i);
	}

	NoLista<Tipo> empilhar(const Tipo& feita)
	{
		if (&feita == NULL) throw "Ação Inválida";

		NoLista<Tipo>* no = new NoLista<Tipo>(feita);
		if (this->ehCheia())
		{
			NoLista<Tipo> acc = *(this->nos + 0);
			for (int i = 0; i < this->tamanhoAtual; i++)
				*(this->nos + i) = *(this->nos + (i + 1));
			this->tamanhoAtual--;
			this->empilhar(feita);
			return acc;
		}
		else if (this->ehVazia())
		{
			*(this->nos + this->tamanhoAtual++) = *no;
			return NoLista<Tipo>();
		}

		(this->nos + (this->tamanhoAtual - 1))->setProximo(no);
		no->setAnterior((this->nos + (this->tamanhoAtual - 1)));
		*(this->nos + this->tamanhoAtual++) = *no;
		return NoLista<Tipo>();
	}

	NoLista<Tipo> desempilhar()
	{
		if (this->ehVazia()) throw "Pilha Vazia";
		return *(this->nos + --this->tamanhoAtual);
	}

	void esvaziar()
	{
		while (!this->ehVazia()) this->desempilhar();
	}

	NoLista<Tipo> getTopo() const
	{
		if (this->tamanhoAtual == 0) return *(this->nos + this->tamanhoAtual);
		return *(this->nos + (this->tamanhoAtual - 1));
	}
private:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	NoLista<Tipo> *nos;
	unsigned int tamanhoMax, tamanhoAtual;
};
