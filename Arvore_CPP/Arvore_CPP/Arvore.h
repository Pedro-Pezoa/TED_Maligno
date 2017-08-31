#pragma once
#include "NoArvore.h"
#include "stdio.h"
#include <iostream>
#include <string>
using namespace std;
#define GIRO_PARA_ESQUERDA 0
#define GIRO_PARA_DIREITA 1

template <class Tipo>
class Arvore
{
	public:
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//-------------------------------------------------------------CONSTRUTORES/DESTRUTOR--------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//

		// construtor nulo
		Arvore() : raiz(nullptr){}

		// construtor de cópia
		Arvore(const Arvore<Tipo> &original) : setRaiz(original.getRaiz()) {}

		// construtor com 1 dado inicial
		Arvore(const Tipo &novoDado) : setRaiz(novoDado) {}

		// destrutor
		~Arvore() {}

		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//---------------------------------------------------------------GETTERS E SETTERS-----------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//

		// getter da raiz para saber qual a raiz da árvore diretamente
		Tipo getRaiz() const
		{
			return this->raiz->getDado();
		}

		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//--------------------------------------------------------------------MÉTODOS----------------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	
		// método excluir um dado
		bool excluir(const Tipo &dado)
		{
			// se for a raiz que se deseja excluir
			if (this->raiz == nullptr)
			{
				cout << "BOT> A lista ja esta vazia..." << endl;
				return false;
			}
			bool ret = false;
			if (this->raiz->getDado() == dado)
			{
				// ponteiro auxiliar para podermos
				NoArvore<Tipo>* atual;
				ret = true;
				// menor dos maiores para quando só a direita é nulo ou quando nenhum é nulo
				if ((raiz->getDireita() == nullptr && raiz->getEsquerda() != nullptr) || 
					(raiz->getDireita() != nullptr && raiz->getEsquerda() != nullptr))
				{
					atual = raiz->getEsquerda();
					while (atual->getDireita() != nullptr)
						atual = atual->getDireita();
					Tipo novoDadoRaiz = atual->getDado();
					this->excluirRecur(novoDadoRaiz, raiz);
					this->raiz->setDado(novoDadoRaiz);
				}
				// maior dos menores se somente a esquerda é nula
				else if (raiz->getDireita() != nullptr && raiz->getEsquerda() == nullptr)
				{
					atual = raiz->getDireita();
					while (atual->getEsquerda() != nullptr)
						atual = atual->getEsquerda();
					Tipo novoDadoRaiz = atual->getDado();
					this->excluirRecur(novoDadoRaiz, raiz);
					this->raiz->setDado(novoDadoRaiz);
				}
				// quando a raiz é folha
				else
				{
					this->raiz = nullptr;
				}
			}
			else
			{
				// não é raiz
				ret = this->excluirRecur(dado, this->raiz);
			}
			// falamos que excluiu
			if (ret != false)
			{
				cout << "BOT> Excluido o " << dado << endl;
				// balancear
				this->balancear(1, dado);
			}
			else
				cout << "BOT> Nao existe o valor " << dado << " na lista" << endl;
			return ret;
		}

		void incluir(const Tipo &novoDado)
		{
			// se não tiver nem raiz, incluímos a raiz
			if (this->raiz == nullptr)
			{
				this->raiz = new NoArvore<Tipo>(novoDado);
				cout << "BOT> Incluido na raiz o " << novoDado << endl;
				return;
			}
			// mostramos que incluímos
			cout << "BOT> Incluido o " << novoDado << endl;
			// incluímos
			this->incluir(novoDado, raiz);
			// balanceamos
			this->balancear(0, novoDado);
		}

		friend ostream& operator<< (ostream &os, const Arvore<Tipo> &aArvore)
		{
			// escrevemos o valor da árvore
			return (os << "TREE> " << aArvore.toString());
		}

		string toString() const
		{
			// retornamos o conteúdo da árvore desde a raiz
			return this->toString(this->raiz);
		}

	protected:
		string toString(NoArvore<Tipo> *no) const 
		{
			// método recursivo que coloca os da esquerda e os da direita e o próprio
			if (no == nullptr)
				return "";
			return ((no->getEsquerda()==nullptr)?"": ("("+this->toString(no->getEsquerda()) + ")<-")) + "(" + to_string(no->getDado()) + ")" + ((no->getDireita() == nullptr) ? "" : ("->("+this->toString(no->getDireita())+")"));
		}

		// retorna o nó que possúi o dado a ser achado
		NoArvore<Tipo>* getNoDoDado(int dadoAAchar)
		{
			NoArvore<Tipo>* no;
			no = this->raiz;
			while (no != nullptr)
			{
				if (dadoAAchar > no->getDado())
					no = no->getDireita();
				else if (dadoAAchar < no->getDado())
					no = no->getEsquerda();
				else // achou
					break;
			}
			return no;
		}

		// obtém o menor dos maiores do nó dado
		NoArvore<Tipo>* getMenorDosMaiores(NoArvore<Tipo>* no)
		{
			if (no == nullptr)
				return nullptr;

			NoArvore<Tipo>* aux = no->getDireita();

			if (aux == nullptr)
				return nullptr;

			while (aux->getEsquerda() != nullptr)
				aux = aux->getEsquerda();

			return aux;
		}

		// retorna o maior dos menores do nó dado
		NoArvore<Tipo>* getMaiorDosMenores(NoArvore<Tipo>* no)
		{
			if (no == nullptr)
				return nullptr;

			NoArvore<Tipo>* aux = no->getEsquerda();

			if (aux == nullptr)
				return nullptr;

			while (aux->getDireita() != nullptr)
				aux = aux->getDireita();

			return aux;
		}

		// inclúi recursivamente o dado entregue embaixo do nó dado, se possível
		void incluir(const Tipo &novoDado, NoArvore<Tipo> *no)
		{
			if (novoDado > no->getDado())
			{
				if (no->getDireita() != nullptr)
					incluir(novoDado, no->getDireita());
				else // if(direita é null)
				{
					no->setDireita(new NoArvore<Tipo>(novoDado));
					no->getDireita()->setPai(no);
				}
			}
			else if (novoDado < no->getDado())
			{
				if (no->getEsquerda() != nullptr)
					incluir(novoDado, no->getEsquerda());
				else // if(esquerda é null)
				{
					no->setEsquerda(new NoArvore<Tipo>(novoDado));
					no->getEsquerda()->setPai(no);
				}
			}
			// se for igual, não inclúi
			return;
		}

		// inclúi uma subárvore inteira embaixo do nó
		void incluirSubArvore(NoArvore<Tipo> *no)
		{
			if (no == nullptr)
				return;

			incluir(no->getDado());
			incluirSubArvore(no->getDireita());
			incluirSubArvore(no->getEsquerda());
		}

		// exclúi (deleta) uma subárvore inteira (não precisa estar na árvore)
		void excluirSubArvore(NoArvore<Tipo> *no)
		{
			if (no == nullptr)
				return;

			excluirSubArvore(no->getDireita());
			excluirSubArvore(no->getEsquerda());
			no = nullptr;
		}

		// exclúi recursivamente o nó com o dado desejado
		bool excluirRecur(const Tipo &dado, NoArvore<Tipo> *atual)
		{
			// nó inexistente
			if (atual == nullptr)
				return false;

			if (dado == atual->getDado())
			{
				if (atual->isFolha()) // se é folha, só excluir normal
				{
					if (atual->getDirecaoPai() == PAI_ESQUERDA)
					{
						atual->getPai()->setDireita(nullptr);
					}
					else
					{
						atual->getPai()->setEsquerda(nullptr);
					}
					atual = nullptr;
				}
				else if (atual->getEsquerda() == nullptr) // precisamos fazer quase que uma rotação
				{
					if (atual->getDirecaoPai() == PAI_ESQUERDA)
						atual->getPai()->setDireita(atual->getDireita());
					else
						atual->getPai()->setEsquerda(atual->getDireita());
					atual->getDireita()->setPai(atual->getPai());
					atual = nullptr;
				}
				else if(atual->getDireita() == nullptr) // precisamos fazer quase que uma rotação
				{
					if (atual->getDirecaoPai() == PAI_ESQUERDA)
						atual->getPai()->setDireita(atual->getEsquerda());
					else
						atual->getPai()->setEsquerda(atual->getEsquerda());
					atual->getEsquerda()->setPai(atual->getPai());
					atual = nullptr;
				}
				else // tem dois filhos, então nós colocamos qualquer um
				{
					if (atual->getDirecaoPai() == PAI_ESQUERDA)
						atual->getPai()->setDireita(atual->getEsquerda());
					else
						atual->getPai()->setEsquerda(atual->getEsquerda());
					atual->getEsquerda()->setPai(atual->getPai());
					
					// inclui a subárvore da direita na raiz
					incluirSubArvore(atual->getDireita());
					// exclui a subárvore da direita
					excluirSubArvore(atual->getDireita());

					atual = nullptr;
				}
				return true;
			}
			else if (dado < atual->getDado())
				return this->excluirRecur(dado, atual->getEsquerda());
			else
				return this->excluirRecur(dado, atual->getDireita());
		}

		// método recursivo que coloca no "maior" o valor do maior nó errado
		void maiorNivelRecur(NoArvore<Tipo>* no, int aux, int* maior)
		{
			if (no == nullptr)
				return;

			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
			{
				// coloca no maior se o nível da folha for maior que o maior antigo
				if (aux > *maior)
					*maior = aux;
				return;
			}

			maiorNivelRecur(no->getDireita(), aux+1, maior);
			maiorNivelRecur(no->getEsquerda(), aux+1, maior);
		}

		// retorna a maior altura de um nó
		int maiorNivel(NoArvore<Tipo>* no)
		{
			int* maior = (int*)malloc(sizeof(int));
			*maior = 0;
			maiorNivelRecur(no, 1, maior);
			return *maior;
		}

		// https://www.youtube.com/watch?v=zh27Tp8HV7E
		// verifica se a subárvore inteira está balanceada
		bool verificaBalanceamento(NoArvore<Tipo>* no)
		{
			// se não existe, consideramos balanceado o nó
			if (no == nullptr)
				return true;

			// se for folha, consideramos balanceado o nó
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
				return true;

			// retornamos se o nó está balanceado, juntamente com os seus filhos
			int countD = maiorNivel(no->getDireita());
			int countE = maiorNivel(no->getEsquerda());
			return (abs(countD-countE)) < 2 && verificaBalanceamento(no->getEsquerda()) && verificaBalanceamento(no->getDireita());
		}

		// verifica se o nó específico está balanceado
		bool verificaBalanceamentoSingle(NoArvore<Tipo>* no)
		{
			// se não existe, consideramos balanceado o nó
			if (no == nullptr)
				return true;

			// se for folha, consideramos balanceado o nó
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
				return true;

			// retornamos se o nó está balanceado, separadamente dos seus filhos
			int countD = maiorNivel(no->getDireita());
			int countE = maiorNivel(no->getEsquerda());
			return (abs(countD - countE)) < 2;
		}

		// retorna se a direita do nó é maior que a esquerda
		bool verificaDireitaMaior(NoArvore<Tipo>* no)
		{
			// se não existe, consideramos que direita é maior(tanto faz)
			if (no == nullptr)
				return true;

			// se for folha, consideramos que direita é maior(tanto faz)
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
				return true;

			// retornamos se o nó tem a direita maior
			int countD = maiorNivel(no->getDireita());
			int countE = maiorNivel(no->getEsquerda());
			return countD > countE;
		}

		// obtém o valor de balanceamento do nó
		int verificaValor(NoArvore<Tipo>* no)
		{
			if (no == nullptr)
				return true;

			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
				return true;

			int countD = maiorNivel(no->getDireita());
			int countE = maiorNivel(no->getEsquerda());
			return abs(countD - countE);
		}

		// acha o nó errado na árvore que está mais para baixo (maior altura)
		NoArvore<Tipo>* acharNoErroneo(NoArvore<Tipo>* no)
		{
			// se não existe, consideramos não há nó errado
			if (no == nullptr)
				return nullptr;

			// procuramos o nó que está errado para a direita
			NoArvore<Tipo>* achado = acharNoErroneo(no->getDireita());
			if (achado == nullptr)
			{
				// se não achamos, procuramos o nó que está errado para a esquerda
				achado = acharNoErroneo(no->getEsquerda());
				if (achado != nullptr)
					// se achamos, retornamos ele
					return achado;
			}
			else
				// se achamos, retornamos ele
				return achado;

			// se o nó atual está desbalanceado, retornamos ele
			if (!verificaBalanceamentoSingle(no))
				return no;
			//retornamos nulo se nada está balanceado
			return nullptr;
				
		}

		// gira a árvore para a direção necessitada
		void girar(NoArvore<Tipo>* no, int direcao)
		{
			// se o nó for nulo, não giramos
			if (no == nullptr)
				return;
			// se o giro for para a direita, giramos para a direita
			if (direcao == GIRO_PARA_DIREITA)
			{
				// armazenamos os auxiliares
				NoArvore<Tipo>* noFE = no->getEsquerda();
				NoArvore<Tipo>* noFFD = noFE->getDireita();

				// método identifica o modo de como será setado a esquerda do nó
				int metodo = 1;
				if (noFFD == nullptr)
				{
					noFFD = noFE->getEsquerda();
					metodo = 2;
				}

				// alteramos de posição o nó e o filho dele
				noFE->setDireita(no);
				noFE->setPai(no->getPai());
				no->setPai(noFE);
				if (metodo == 2)
					no->setEsquerda(nullptr);
				else
				{
					no->setEsquerda(noFFD);
					noFFD->setPai(no);
				}

				// setamos a raiz se necessário
				if (noFE->getPai() == nullptr)
					this->raiz = noFE;
				else
					noFE->getPai()->setEsquerda(noFE);
			}
			else
			{
				NoArvore<Tipo>* noFE = no->getDireita();
				NoArvore<Tipo>* noFFD = noFE->getEsquerda();

				// método identifica o modo de como será setado a direita do nó
				int metodo = 1;
				if (noFFD == nullptr)
				{
					noFFD = noFE->getDireita();
					metodo = 2;
				}

				if (metodo == 2)
				{
					// alteramos de posição o nó e o filho dele
					noFE->setEsquerda(no);
					noFE->setPai(no->getPai());
					no->setPai(noFE);
					no->setDireita(nullptr);
					// setamos a raiz se necessário
					if (noFE->getPai() == nullptr)
						this->raiz = noFE;
					else
						noFE->getPai()->setDireita(noFE);
				}
				else
				{
					// alteramos de posição o nó e o filho dele
					noFE->setEsquerda(nullptr);
					noFFD->setPai(no->getPai());
					no->setPai(noFFD);
					no->setDireita(nullptr);
					noFE->setPai(noFFD);
					noFFD->setEsquerda(no);
					noFFD->setDireita(noFE);
					// setamos a raiz se necessário
					if (noFFD->getPai() == nullptr)
						this->raiz = noFFD;
					else
						noFFD->getPai()->setDireita(noFFD);
				}
			}
		}

		void balancear(int tipo, int dado) // 0 - inclusão, 1- exclusão
		{
			// verificar se o balanceamento está correto
			// se sim, acabou...
			if (!verificaBalanceamento(this->raiz))
			{
				// se não, temos que ajustar
				cout << "BOT> Desbalanceou a arvore" << endl;
				// se é exclusão
				if (tipo == 1)
				{
					// achamos o nó errado e o seu pai
					NoArvore<Tipo>* oi = acharNoErroneo(this->raiz);
					NoArvore<Tipo>* pai = oi;
					do
					{
						// setamos o pai
						pai = pai->getPai();

						// giramos para o lado certo
						if (verificaDireitaMaior(oi))
							girar(oi, GIRO_PARA_ESQUERDA);
						else
							girar(oi, GIRO_PARA_DIREITA);

						// colocamos o atual como pai
						oi = pai;
						// enquanto não está com o pai nulo (raiz)
					} while (pai != nullptr);
				}
				// se é inclusão
				else
				{
					do
					{
						// achamos o nó errado
						NoArvore<Tipo>* oi = acharNoErroneo(this->raiz);

						// giramos para o lado certo
						if (verificaDireitaMaior(oi))
							girar(oi, GIRO_PARA_ESQUERDA);
						else
							girar(oi, GIRO_PARA_DIREITA);
					// enquanto não está balanceado
					} while (!(verificaBalanceamento(this->raiz)));
				}
				cout << "BOT> Tentativa de balanceamento realizada" << endl;
			}
			else
			{
				cout << "BOT> Nao desbalanceou a arvore" << endl;
			}
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//

		// raiz da árvore inteira
		NoArvore<Tipo> *raiz;
	private:
};