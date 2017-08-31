#include "stdafx.h"
#include "Matriz.h"

Matriz::Matriz(int sizeX, int sizeY, int novoValorNormal, int novoValorExiste)
{
	valorNormal = novoValorNormal;
	valorExiste = novoValorExiste;
	this->width = sizeY;
	this->height = sizeX;
	row = new ListaDupla<int>();
	col = new ListaDupla<ListaDupla<int>*>();
	bufferPosX = new int();
	bufferPosY = new int();
	bufferValor = new int();
}

Matriz::Matriz(const Matriz &novaMatriz)
{
	this->col = novaMatriz.col;
	this->row = novaMatriz.row;
	this->valorNormal = novaMatriz.valorNormal;
	this->valorExiste = novaMatriz.valorExiste;
	bufferPosX = new int();
	bufferPosY = new int();
	bufferValor = new int();
}

Matriz::CRow Matriz::operator[](const int& row)
{
	return Matriz::CRow(*this, row);
}

int Matriz::getWidth() const
{
	return this->width;
}

int Matriz::getHeight() const
{
	return this->height;
}

void Matriz::adicionar(int x, int y)
{
	bool linha = this->existeLinha(x);

	if (!linha)
	{
		this->row->inserirNoFim(x);
		this->col->inserirNoFim(new ListaDupla<int>());
		this->col->operator[](this->col->getTamanho() - 1)->inserirNoFim(y);
	}
	else
	{
		bool coluna = this->existeColuna(x, y);
		if (!coluna)
			this->col->operator[](this->indexDeRow(x))->inserirNoFim(y);
	}
}

bool Matriz::remover(int x, int y)
{
	if (this->existe(x, y))
	{
		int pos = this->indexDeRow(x);
		this->row->removerPos(pos);
		return this->col->operator[](pos)->removerPos(y);
	}
}

bool Matriz::existe(int x, int y)
{
	return (this->existeLinha(x) && this->existeColuna(x, y) ? true : false);
}

bool Matriz::existeLinha(int x)
{
	return (this->indexDeRow(x) != -1 ? true : false);
}

bool Matriz::existeColuna(int x, int y)
{
	return (this->indexDeColumn(y, this->indexDeRow(x)) != -1 ? true : false);
}

int* Matriz::operator()(const int& i, const int& o)
{
	this->saveProgress();
	if (i < 0 || o < 0 || i >= this->width || o >= this->height)
		return nullptr;

	*bufferPosX = i;
	*bufferPosY = o;
	bool existe = false;

	this->row->iniciarPercursoSequencial();
	while (this->row->podePercorrer())
	{
		if (*this->row->getAtual()->getDado() == i)
		{
			existe = true;
			break;
		}
	}
	if (existe)
	{
		existe = false;
		ListaDupla<int>* listaAux = this->col->operator[](this->row->getIndexAtual());
		listaAux->iniciarPercursoSequencial();
		while (listaAux->podePercorrer())
		{
			if (*listaAux->getAtual()->getDado() == o)
			{
				existe = true;
				break;
			}
		}

		if (existe)
		{
			*bufferValor = valorExiste;
		}
		else // buffer
		{
			*bufferValor = valorNormal;
		}
	}
	else // buffers e buffers
	{
		*bufferValor = valorNormal;
	}
	return bufferValor;
}

void Matriz::saveProgress()
{
	if (bufferValor == nullptr)
		return;

	int indiceRow = indexDeRow(*bufferPosX);
	if (indiceRow != -1) // existe 1
	{
		int indiceColumn = indexDeColumn(*bufferPosY, indiceRow);
		if (indiceColumn != -1) // existe 1
		{
			if (*bufferValor == valorNormal)
				remover(*bufferPosX, *bufferPosY);
			else
				adicionar(*bufferPosX, *bufferPosY);
		}
		else // não existe 1
		{
			if (*bufferValor != valorNormal)
				adicionar(*bufferPosX, *bufferPosY);
		}
	}
	else // não existe 2
	{
		if (*bufferValor != valorNormal)
			adicionar(*bufferPosX, *bufferPosY);
	}

	delete(bufferValor);
	delete(bufferPosX);
	delete(bufferPosY);
	bufferValor = new int();
	bufferPosX = new int();
	bufferPosY = new int();
	return;
}

int Matriz::indexDeRow(int dado)
{
	this->row->iniciarPercursoSequencial();
	while (this->row->podePercorrer())
	{
		if (*this->row->getAtual()->getDado() == dado)
			return this->row->getIndexAtual();
	}
	return -1;
}

int Matriz::indexDeColumn(int dado, int qualLista)
{
	ListaDupla<int>* listaAux = this->col->operator[](qualLista);
	listaAux->iniciarPercursoSequencial();
	while (listaAux->podePercorrer())
	{
		if (*listaAux->getAtual()->getDado() == dado)
		{
			return listaAux->getIndexAtual();
			break;
		}
	}
	return -1;
}