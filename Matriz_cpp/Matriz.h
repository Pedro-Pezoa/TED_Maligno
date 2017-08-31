
//return lala[col][row][row][row][row];#pragma once
using namespace std;
#include "ListaDupla.h"

class Matriz
{
public:
	//---------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------CONSTRUTORES/DESTRUTOR----------------------------------------------------------//
	//---------------------------------------------------------------------------------------------------------------------------------//

	class CRow {
		friend class Matriz;
	public:
		int& operator[](int col) const
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

	Matriz(const Matriz &novaMatriz);
	Matriz(int sizeX, int sizeY, int novoValorNormal, int novoValorExiste);

	Matriz::CRow operator[](const int& row);
	int getWidth() const;
	int getHeight() const;
	
protected:
	void adicionar(int x, int y);
	bool remover(int x, int y);
	bool existe(int x, int y);

	int Matriz::indexDeRow(int dado);
	int Matriz::indexDeColumn(int dado, int qualLista);

	bool existeLinha(int x);
	bool existeColuna(int x, int y);

	void saveProgress();
	int* operator()(const int& i, const int& o);

	//---------------------------------------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------ATRIBUTOS-----------------------------------------------------------------//
	//---------------------------------------------------------------------------------------------------------------------------------//
	ListaDupla<int>* row;            // 0:30 1:40 2:50
	ListaDupla<ListaDupla<int>*>* col;// 0:[40,50] 1:[30,50] 2:[30,40]
	int valorNormal, valorExiste, width, height;
	int* bufferValor;
	int* bufferPosX;
	int* bufferPosY;
	friend class CRow;
	friend class Matriz;
private:
};