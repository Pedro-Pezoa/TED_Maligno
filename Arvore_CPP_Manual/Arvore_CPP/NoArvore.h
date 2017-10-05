#pragma once
#define PAI_ESQUERDA 0
#define PAI_DIREITA 1
template <class Tipo> class Arvore;
template <class Tipo>
class NoArvore
{
	public:
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//-------------------------------------------------------------CONSTRUTORES/DESTRUTOR--------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//

		NoArvore(const Tipo &novoDado) : esq(nullptr), dir(nullptr), pai(nullptr)
		{
			dado = new Tipo(novoDado);
		}

		NoArvore(const Tipo &novoDado, NoArvore* Esquerda, NoArvore* Direita, NoArvore* Pai) : esq(Esquerda), dir(Direita), pai(Pai)
		{
			dado = new Tipo(novoDado);
		}

		NoArvore(const NoArvore<Tipo> &novoDado) : esq(novoDado.getEsquerda()), dir(novoDado.getDireita()), pai(novoDado.getPai())
		{
			dado = new Tipo(novoDado.getDado());
		}

		NoArvore(const NoArvore<Tipo> *novoDado)
		{
			if (novoDado != nullptr)
			{
				this->dado = new Tipo(novoDado->getDado());
				this->dir = novoDado->getDireita();
				this->esq = novoDado->getEsquerda();
				this->pai = novoDado->getPai();
			}
			else
			{
				this->dado = nullptr;
				this->dir = nullptr;
				this->esq = nullptr;
				this->pai = nullptr;
			}
		}

		NoArvore(NoArvore<Tipo> *novoDado, NoArvore<Tipo> *novoEsquerda, NoArvore<Tipo> *novoDireita, NoArvore<Tipo> *novoPai) : dado(novoDado->getDado()), esq(novoEsquerda), dir(novoDireita), pai(novoPai){}

		NoArvore() : dado() {}

		~NoArvore(){}

		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//---------------------------------------------------------------GETTERS E SETTERS-----------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//

		NoArvore<Tipo>* getEsquerda() const
		{
			return esq;
		}

		Tipo getDado() const
		{
			return *dado;
		}

		Tipo getPontDado() const
		{
			return dado;
		}

		NoArvore<Tipo>* getDireita() const
		{
			return dir;
		}

		NoArvore<Tipo>* getPai() const
		{
			return pai;
		}

		int getDirecaoPai() const
		{
			return ((*this->dado > this->getPai()->getDado())?
				(0):(1));
		}

		void setPai(NoArvore *NovoPai)
		{
			this->pai = NovoPai;
		}

		void setEsquerda(NoArvore *NovoEsq)
		{
			this->esq = NovoEsq;
		}

		void setDireita(NoArvore *NovoDir)
		{
			this->dir = NovoDir;
		}

		void setDado(Tipo novoDado)
		{
			this->dado = new Tipo(novoDado);
		}

		void setDireita(NoArvore &NovoDir)
		{
			this->dir = &NovoDir;
		}

		void setEsquerda(NoArvore &NovoEsq)
		{
			this->esq = &NovoEsq;
		}

		void toFolha()
		{
			this->esq = nullptr;
			this->dir = nullptr;
		}

		bool isFolha()
		{
			return (this->dir == nullptr && this->esq == nullptr);
		}

		NoArvore<Tipo> clone() const
		{
			return new NoArvore<Tipo>(this);
		}
	protected:
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		Tipo* dado;
		NoArvore<Tipo> *dir, *esq, *pai;
	private:
};