
//return lala[col][row][row][row][row];#pragma once
using namespace std;
#include "ListaDupla.h"

template <class Tipo>
class Matriz
{
private:

	//---------------------------------------------------------------------------------------------------------------------------------//
	//------------------------------------------------------CLASSES AUXILIARES---------------------------------------------------------//
	//---------------------------------------------------------------------------------------------------------------------------------//

	class CRow
	{
		friend class Matriz;
	public:
		Tipo& operator[](int col) const
		{
			return *parent(row, col);
		}
	private:
		CRow(Matriz &parent_, int row_) : parent(parent_), row(row_) {}

		Matriz& parent;
		int row;
	};

	typedef struct NoMatriz
	{
	public:
		NoMatriz()
		{
			this->pos = new int();
			this->dado = new Tipo();
		}

		NoMatriz(const int& novoPos, const Tipo& novoDado)
		{
			this->pos = new int(novoPos);
			this->dado = new Tipo(novoDado);
		}

		int getPos() const
		{
			return *(this->pos);
		}

		Tipo getDado() const
		{
			return *(this->dado);
		}

		int* getPontPos() const
		{
			return this->pos;
		}

		Tipo* getPontDado() const
		{
			return this->dado;
		}
	private:
		int* pos;
		Tipo* dado;
	};

public:

	//---------------------------------------------------------------------------------------------------------------------------------//
	//---------------------------------------------------------CONSTRUTORES------------------------------------------------------------//
	//---------------------------------------------------------------------------------------------------------------------------------//

	Matriz(int sizeX, int sizeY, Tipo novoValorNormal) : width(sizeX), height(sizeY)
	{
		this->valorNormal = novoValorNormal;
		this->row = new ListaDupla<int>();
		this->col = new ListaDupla<ListaDupla<NoMatriz>*>();
		this->bufferPosX = new int();
		this->bufferPosY = new int();
		this->bufferValor = nullptr;
	}

	Matriz(const Matriz &novaMatriz) : width(novaMatriz.getWidth()), height(novaMatriz.getHeight())
	{
		this->col = novaMatriz.col;
		this->row = novaMatriz.row;
		this->valorNormal = novaMatriz.valorNormal;
		this->bufferPosX = new int();
		this->bufferPosY = new int();
		this->bufferValor = nullptr;
	}

int Matriz::getWidth() const
{
	return this->width;
}

int Matriz::getHeight() const
{
	return this->height;
}

CRow operator[](const int& row)
{
	return CRow(*this, row);
}

protected:
	//---------------------------------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------MÉTODOS PRINCIPAIS-----------------------------------------------------------//
	//---------------------------------------------------------------------------------------------------------------------------------//
	void adicionar(int x, int y, Tipo valor)
	{
		bool linha = this->existeLinha(x);
		NoMatriz novoNo = NoMatriz(y, valor);
		NoMatriz noVazio = NoMatriz();

		if (!linha)
		{
			this->row->inserirNoFim(x);
			this->col->inserirNoFim(new ListaDupla<NoMatriz>());

			for (int i = 0; i < y; i++)
				this->col->operator[](this->col->getTamanho() - 1)->inserirNoFim(noVazio);
			
			this->col->operator[](this->col->getTamanho() - 1)->inserirNoFim(novoNo);
		}
		else
		{
			bool coluna = this->existeColuna(x, y);
			if (!coluna)
				this->col->operator[](this->indexDeRow(x))->inserirNoFim(novoNo);
			else
				this->col->operator[](this->indexDeRow(x))->operator[](this->indexDeColumn(y, x)) = novoNo;
		}
	}

	bool remover(int x, int y)
	{
		if (this->existe(x, y))
		{
			int pos = this->indexDeRow(x);
			//this->row->removerPos(pos);
			return this->col->operator[](pos)->removerPos(this->indexDeColumn(y, x));
		}
	}

	bool existe(int x, int y)
	{
		return (this->existeLinha(x) && this->existeColuna(x, y) ? true : false);
	}

	void saveProgress()
	{
		if (bufferValor == nullptr)
		{
			bufferValor = new Tipo();
			return;
		}

		int indiceRow = indexDeRow(*bufferPosX);
		if (indiceRow != -1) // existe 1
		{
			int indiceColumn = indexDeColumn(*bufferPosY, indiceRow);
			if (indiceColumn != -1) // existe 1
			{
				if (*bufferValor == valorNormal)
					remover(*bufferPosX, *bufferPosY);
				else
					adicionar(*bufferPosX, *bufferPosY, *bufferValor);
			}
			else // não existe 1
			{
				if (*bufferValor != valorNormal)
					adicionar(*bufferPosX, *bufferPosY, *bufferValor);
			}
		}
		else // não existe 2
		{
			if (*bufferValor != valorNormal)
				adicionar(*bufferPosX, *bufferPosY, *bufferValor);
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

	int indexDeRow(int pos)
	{
		this->row->iniciarPercursoSequencial();
		while (this->row->podePercorrer())
		{
			if (*this->row->getAtual()->getDado() == pos)
				return this->row->getIndexAtual();
		}
		return -1;
	}

	int indexDeColumn(int pos, int qualLista)
	{
		ListaDupla<NoMatriz>* listaAux = this->col->operator[](qualLista);
		listaAux->iniciarPercursoSequencial();
		while (listaAux->podePercorrer())
		{
			if (listaAux->getAtual()->getDado()->getPos() == pos)
			{
				return listaAux->getIndexAtual();
				break;
			}
		}
		return -1;
	}

	string toString()
	{
		this->saveProgress();
		string txt = "| ";
		int i = 0;
		int j = 0;

		ListaDupla<NoMatriz>* lista = nullptr;
		NoMatriz dado = NoMatriz();
		while (i < this->height)
		{
			lista = this->col->operator[](i);
			if (lista == nullptr)
				txt += this->valorNormal + " ";

			else
			{
				dado = lista->operator[](j);
				if (dado.getDado() == this->valorNormal || dado.getDado() == Tipo())
					txt += this->valorNormal + " ";
				else
					txt += to_string(dado.getDado()) + " ";
			}

			if (++j >= this->width)
			{
				j = 0;
				i++;
				txt += "|\n| ";
			}
		}
		return txt.substr(0, txt.length()-3);
	}

	//---------------------------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------OPERATORS-------------------------------------------------------------//
	//---------------------------------------------------------------------------------------------------------------------------------//

	Tipo* operator()(const int& o, const int& i)
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
			ListaDupla<NoMatriz>* listaAux = this->col->operator[](this->row->getIndexAtual());
			listaAux->iniciarPercursoSequencial();
			while (listaAux->podePercorrer())
			{
				if (listaAux->getAtual()->getDado()->getPos() == o)
				{
					existe = true;
					break;
				}
			}

			if (existe)
			{
				*bufferValor = listaAux->getAtual()->getDado()->getDado();
				return bufferValor;
			}
			else // buffer
			{
				*bufferValor = valorNormal;
				return bufferValor;
			}
		}
		else // buffers e buffers
		{
			*bufferValor = valorNormal;
			return bufferValor;
		}
	}

	// Printa o conteúdo da classe
	friend ostream& operator<<(ostream &sai, Matriz<Tipo>& outro)
	{
		return (sai << outro.toString());
	}

	//---------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------ATRIBUTOS-----------------------------------------------------------------//
	//---------------------------------------------------------------------------------------------------------------------------------//

	ListaDupla<int>* row;                  // 0:30 1:40 2:50
	ListaDupla<ListaDupla<NoMatriz>*>* col;// 0:[40,50] 1:[30,50] 2:[30,40]

	Tipo* bufferValor, valorNormal;
	int* bufferPosX, *bufferPosY, width, height;

	friend class CRow;
	friend class Matriz;
};