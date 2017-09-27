#pragma once
template <class Tipo>
class NoLista
{
public:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------CONSTRUTORES/DESTRUTOR--------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

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
		dado = new Tipo(novoDado.getDado());
	}

	NoLista(NoLista<Tipo> *novoDado) 
	{
		this->dado = new Tipo(novoDado->getDado());
		this->ante = NULL;
		this->prox = NULL;
	}

	NoLista(NoLista<Tipo> *novoDado, NoLista<Tipo> *novoProximo, NoLista<Tipo> *novoAnte) : dado(novoDado->getDado()), prox(novoProximo), ante(novoAnte){}

	NoLista() : dado() {}

	~NoLista(){}

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------------GETTERS E SETTERS-----------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//

	NoLista<Tipo>* getProximo() const
	{
		return prox;
	}

	Tipo getDado() const
	{
		return *dado;
	}

	Tipo getPontDado() const
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
protected:
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	Tipo* dado;
	NoLista<Tipo> *ante, *prox;
private:
};