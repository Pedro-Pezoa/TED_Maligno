
//return lala[col][row][row][row][row];#pragma once
using namespace std;
#include "ListaDupla.h"

template <class Tipo>
class Matriz
{
public:
	//---------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------CLASSE AUXILIAR------------------------------------------------------------//
	//---------------------------------------------------------------------------------------------------------------------------------//

	class CRow {
		friend class Matriz;
	public:
		Tipo& operator[](int col) const
		{
			return *parent(row, col);
		}
	private:
		CRow(Matriz &parent_, int row_) :
			parent(parent_),
			row(row_)
		{}

		Matriz& parent;
		int row;
	};

	//---------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------CONSTRUTORES/DESTRUTOR----------------------------------------------------------//
	//---------------------------------------------------------------------------------------------------------------------------------//

	Matriz(int sizeX, int sizeY, Tipo novoValorNormal, Tipo novoValorExiste) : width(sizeY), height(sizeX)
	{
		valorNormal = novoValorNormal;
		valorExiste = novoValorExiste;
		row = new ListaDupla<int>();
		col = new ListaDupla<ListaDupla<int>*>();
		bufferPosX = new int();
		bufferPosY = new int();
		bufferValor = new Tipo();
	}

	Matriz(const Matriz &novaMatriz) : width(novaMatriz.getWidth()), height(novaMatriz.getHeight())
	{
		this->col = novaMatriz.col;
		this->row = novaMatriz.row;
		this->valorNormal = novaMatriz.valorNormal;
		this->valorExiste = novaMatriz.valorExiste;
		bufferPosX = new int();
		bufferPosY = new int();
		bufferValor = new Tipo();
	}

	CRow operator[](const int& row)
	{
		return CRow(*this, row);
	}

int Matriz::getWidth() const
{
	return this->width;
}

int Matriz::getHeight() const
{
	return this->height;
}

protected:
	//---------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------MÉTODOS PRINCIPAIS-----------------------------------------------------------//
	//---------------------------------------------------------------------------------------------------------------------------------//
	void adicionar(int x, int y)
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

	bool remover(int x, int y)
	{
		if (this->existe(x, y))
		{
			int pos = this->indexDeRow(x);
			this->row->removerPos(pos);
			return this->col->operator[](pos)->removerPos(y);
		}
	}

	bool existe(int x, int y)
	{
		return (this->existeLinha(x) && this->existeColuna(x, y) ? true : false);
	}

	void saveProgress()
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
		bufferValor = new Tipo();
		bufferPosX = new int();
		bufferPosY = new int();
		return;
	}

	//---------------------------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------MÉTODOS AUXILIARES----------------------------------------------------------//
	//---------------------------------------------------------------------------------------------------------------------------------//

	bool existeLinha(int x)
	{
		return (this->indexDeRow(x) != -1 ? true : false);
	}

	bool existeColuna(int x, int y)
	{
		return (this->indexDeColumn(y, this->indexDeRow(x)) != -1 ? true : false);
	}

	int indexDeRow(int dado)
	{
		this->row->iniciarPercursoSequencial();
		while (this->row->podePercorrer())
		{
			if (*this->row->getAtual()->getDado() == dado)
				return this->row->getIndexAtual();
		}
		return -1;
	}

	int indexDeColumn(int dado, int qualLista)
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

	//---------------------------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------OPERATORS-------------------------------------------------------------//
	//---------------------------------------------------------------------------------------------------------------------------------//

	Tipo* operator()(const int& i, const int& o)
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

	//---------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------ATRIBUTOS-----------------------------------------------------------------//
	//---------------------------------------------------------------------------------------------------------------------------------//
	ListaDupla<int>* row;            // 0:30 1:40 2:50
	ListaDupla<ListaDupla<int>*>* col;// 0:[40,50] 1:[30,50] 2:[30,40]

	Tipo valorNormal, valorExiste;
	int width, height;
	Tipo* bufferValor;
	int* bufferPosX;
	int* bufferPosY;

	friend class CRow;
	friend class Matriz;
private:
};