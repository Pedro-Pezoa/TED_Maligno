#pragma once
using namespace std;
#include <iostream>
#include <ostream>
#include <string>

template <class Tipo> class ListaDupla;
template <class Tipo>
class NoLista
{
public:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------CONSTRUTORES/DESTRUTOR--------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	NoLista() : prox(nullptr), ante(nullptr), dado(nullptr){}

	NoLista(const Tipo &novoDado)
	{
		dado = new Tipo(novoDado);
	}

	NoLista(const Tipo &novoDado, NoLista* Proximo, NoLista* Anterior) : prox(Proximo), ante(Anterior)
	{
		dado = new Tipo(novoDado);
	}

	NoLista(const NoLista<Tipo> &novoDado) : prox(novoDado.getProximo()), ante(novoDado.getAnterior())
	{
		dado = new Tipo(*(novoDado.getDado()));
	}

	NoLista(NoLista<Tipo> *novoDado)
	{
		this->dado = new Tipo(*novoDado->getDado());
		this->ante = novoDado->getAnterior();
		this->prox = novoDado->getProximo();
	}

	NoLista(NoLista<Tipo> *novoDado, NoLista<Tipo> *novoProximo, NoLista<Tipo> *novoAnte) : dado(novoDado->getDado()), prox(novoProximo), ante(novoAnte){}

	~NoLista(){}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------GETTERS E SETTERS-----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	NoLista<Tipo>* getProximo() const
	{
		return prox;
	}

	Tipo* getDado() const
	{
		return dado;
	}

	NoLista<Tipo>* getAnterior() const
	{
		return ante;
	}

	void setProximo(NoLista *NovoProx)
	{
		this->prox = NovoProx;
	}

	void setAnterior(NoLista *NovoAnte)
	{
		this->ante = NovoAnte;
	}

	void setDado(Tipo novoDado)
	{
		this->dado = new Tipo(novoDado);
	}

	void setAnterior(NoLista &NovoAnte)
	{
		this->ante = &NovoAnte;
	}

	void setProximo(NoLista &NovoProx)
	{
		this->prox = &NovoProx;
	}

	friend ostream& operator<<(ostream &sai, const NoLista<Tipo>& outro)
	{
		return (sai << *(outro.getDado()) << endl);
	}
	friend class ListaDupla<Tipo>;
protected:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	Tipo* dado;
	NoLista<Tipo> *ante, *prox;
private:
};