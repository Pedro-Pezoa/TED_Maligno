#pragma once
#include "NoArvore.h"
#include "stdio.h"
#include <iostream>
#include <string>
#include "Pilha.h"

using namespace std;
#define GIRO_PARA_ESQUERDA 0
#define GIRO_PARA_DIREITA 1
#define TIPO_STRUCT_TOSTRING 1
#define TIPO_STRUCT_INCLUIRRECUR 2
#define TIPO_STRUCT_INCLUIRSUBARVORE 3
#define TIPO_STRUCT_EXCLUIRSUBARVORE 4
#define TIPO_STRUCT_VERIFICABALANCEAMENTO 5
#define TIPO_STRUCT_ACHARNOERRONEO 6
#define TIPO_STRUCT_EXCLUIRRECUR 7
#define TIPO_STRUCT_MAIORNIVELRECUR 8

template <class Tipo>
class Arvore
{
	public:
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//-------------------------------------------------------------CONSTRUTORES/DESTRUTOR--------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//

		// construtor nulo
		Arvore() : raiz(nullptr), bufferBalancear(nullptr){}

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
		//---------------------------------------------------------------MÉTODOS PRINCIPAIS----------------------------------------------------------------------//
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
			bool* nada = new bool(false);
			NoArvore<Tipo>* ret = nullptr;
			if (this->raiz->getDado() == dado)
			{
				// ponteiro auxiliar para podermos
				NoArvore<Tipo>* aux = nullptr;
				if (this->raiz->isFolha())
				{
					this->raiz = nullptr;
					return true;
				}

				else if (getNivelMaiorDosMenores(this->raiz) > getNivelMenorDosMaiores(this->raiz))
					aux = new NoArvore<Tipo>(getMaiorDosMenores(this->raiz));

				else
					aux = new NoArvore<Tipo>(getMenorDosMaiores(this->raiz));

				ret = this->excluirRecur(aux->getDado(), raiz, nada);
				this->raiz->setDado(aux->getDado());
				/*
				// menor dos maiores para quando só a direita é nulo ou quando nenhum é nulo
				if ((raiz->getDireita() == nullptr && raiz->getEsquerda() != nullptr) || 
					(raiz->getDireita() != nullptr && raiz->getEsquerda() != nullptr))
				{
					atual = raiz->getEsquerda();
					while (atual->getDireita() != nullptr)
						atual = atual->getDireita();
					Tipo novoDadoRaiz = atual->getDado();
					ret = this->excluirRecur(novoDadoRaiz, raiz, nada);
					this->raiz->setDado(novoDadoRaiz);
				}
				// maior dos menores se somente a esquerda é nula
				else if (raiz->getDireita() != nullptr && raiz->getEsquerda() == nullptr)
				{
					atual = raiz->getDireita();
					while (atual->getEsquerda() != nullptr)
						atual = atual->getEsquerda();
					Tipo novoDadoRaiz = atual->getDado();
					ret = this->excluirRecur(novoDadoRaiz, raiz, nada);
					this->raiz->setDado(novoDadoRaiz);
				}
				// quando a raiz é folha
				else
					this->raiz = nullptr;
					*/

				if (raiz != nullptr)
					cout << "BOT> Nova raiz: " << this->raiz->getDado() << endl;

				if (this->raiz->getEsquerda() == nullptr || this->raiz->getDireita() == nullptr)
					this->balancear(1);
				else
					this->balancear(2);
				return true;
			}
			else
			{
				// não é raiz
				ret = this->excluirRecur(dado, this->raiz, nada);
			}
			// falamos que excluiu
			if (ret != nullptr)
			{
				cout << "BOT> Excluido o " << dado << endl;
				// balancear
				this->balancear(1);
				if (*nada)
				{
					incluirSubArvore(ret->getDireita());
					incluirSubArvore(ret->getEsquerda());
				}
				return true;
			}
			else
				cout << "BOT> Nao existe o valor " << dado << " na lista" << endl;
			return false;
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
			this->incluirRecur(novoDado, raiz);
			// balanceamos
			this->balancear(0);
		}

		friend ostream& operator<< (ostream &os, Arvore<Tipo> &aArvore)
		{
			// escrevemos o valor da árvore
			return (os << "TREE> " << aArvore.toString());
		}

		string toString()
		{
			// retornamos o conteúdo da árvore desde a raiz
			return this->toString(this->raiz);
		}
		friend class Pilha<Tipo>;
	protected:
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//--------------------------------------------------------------------STRUCTS----------------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		struct IEmpilhavel 
		{
			unsigned int indicacao;
			unsigned int tipo;
			virtual ~IEmpilhavel() {};
			virtual IEmpilhavel* clone()
			{
				IEmpilhavel* ret = new IEmpilhavel();
				ret->indicacao = this->indicacao;
				ret->tipo = this->tipo;
				return ret;
			};
		};
		struct structToString : public IEmpilhavel
		{
			NoArvore<Tipo>* no;
			string noEsquerda;
			string noDireita;
			structToString()
			{
				tipo = TIPO_STRUCT_TOSTRING;
			}
			structToString* clone()
			{
				return new structToString(this->no, this->noEsquerda, this->noDireita, indicacao);
			}
		protected:
			structToString(const NoArvore<Tipo>* novoNo, const string &noEsquerda, const string &noDireita, const int &indicacaoNova) : noEsquerda(noEsquerda), noDireita(noDireita)
			{
				this->no = new NoArvore<Tipo>();
				this->no->setPai(novoNo->getPai());
				this->no->setEsquerda(novoNo->getEsquerda());
				this->no->setDireita(novoNo->getDireita());
				this->no->setDado(novoNo->getDado());
				tipo = TIPO_STRUCT_TOSTRING;
				indicacao = indicacaoNova;
			}
		};

		struct structIncluirRecur : IEmpilhavel
		{
			NoArvore<Tipo> *no;
			structIncluirRecur() {
				tipo = TIPO_STRUCT_INCLUIRRECUR;
			}
			structIncluirRecur* clone()
			{
				return new structIncluirRecur(no);
			}
		protected:
			structIncluirRecur(const NoArvore<Tipo>* novoNo)
			{
				this->no = new NoArvore<Tipo>();
				this->no->setPai(novoNo->getPai());
				this->no->setEsquerda(novoNo->getEsquerda());
				this->no->setDireita(novoNo->getDireita());
				this->no->setDado(novoNo->getDado());
				tipo = TIPO_STRUCT_INCLUIRRECUR;
			}
		};

		typedef struct structIncluirSubArvore : IEmpilhavel
		{
			NoArvore<Tipo> *no;
			structIncluirSubArvore() {
				tipo = TIPO_STRUCT_INCLUIRSUBARVORE;
			}
			structIncluirSubArvore* clone()
			{
				return new structIncluirSubArvore(no);
			}
		protected:
			structIncluirSubArvore(const NoArvore<Tipo>* novoNo)
			{
				this->no = new NoArvore<Tipo>();
				this->no->setPai(novoNo->getPai());
				this->no->setEsquerda(novoNo->getEsquerda());
				this->no->setDireita(novoNo->getDireita());
				this->no->setDado(novoNo->getDado());
				tipo = TIPO_STRUCT_INCLUIRSUBARVORE;
			}
		};

		struct structExcluirSubArvore : IEmpilhavel
		{
			NoArvore<Tipo> *no;
			structExcluirSubArvore() {
				tipo = TIPO_STRUCT_EXCLUIRSUBARVORE;
			}
			structExcluirSubArvore* clone()
			{
				return new structExcluirSubArvore(this->no);
			}
		protected:
			structExcluirSubArvore(const NoArvore<Tipo> *novoNo)
			{
				this->no = new NoArvore<Tipo>();
				this->no->setPai(novoNo->getPai());
				this->no->setEsquerda(novoNo->getEsquerda());
				this->no->setDireita(novoNo->getDireita());
				this->no->setDado(novoNo->getDado());
				tipo = TIPO_STRUCT_EXCLUIRSUBARVORE;
			}
		};

		typedef struct structVerificaBalanceamento : IEmpilhavel
		{
			NoArvore<Tipo> *no;
			structVerificaBalanceamento() {
				tipo = TIPO_STRUCT_VERIFICABALANCEAMENTO;
			}
			structVerificaBalanceamento* clone()
			{
				return new structVerificaBalanceamento(this->no);
			}
		protected:
			structVerificaBalanceamento(const NoArvore<Tipo>* novoNo)
			{
				this->no = new NoArvore<Tipo>();
				this->no->setPai(novoNo->getPai());
				this->no->setEsquerda(novoNo->getEsquerda());
				this->no->setDireita(novoNo->getDireita());
				this->no->setDado(novoNo->getDado());
				this->tipo = TIPO_STRUCT_VERIFICABALANCEAMENTO;
			}
		};

		struct structAcharNoErroneo : IEmpilhavel
		{
			NoArvore<Tipo>* no;
			NoArvore<Tipo>* achado;
			structAcharNoErroneo() {
				tipo = TIPO_STRUCT_ACHARNOERRONEO;
			}
			structAcharNoErroneo* clone()
			{
				return new structAcharNoErroneo(no, achado, indicacao);
			}
		protected:
			structAcharNoErroneo(const NoArvore<Tipo>* novoNo, const NoArvore<Tipo>* achado, const int indi)
			{
				this->no = new NoArvore<Tipo>();
				this->no->setPai(novoNo->getPai());
				this->no->setEsquerda(novoNo->getEsquerda());
				this->no->setDireita(novoNo->getDireita());
				this->no->setDado(novoNo->getDado());
				if (achado == nullptr)
					this->achado = nullptr;
				else
				{
					this->achado = new NoArvore<Tipo>();
					this->achado->setPai(achado->getPai());
					this->achado->setEsquerda(achado->getEsquerda());
					this->achado->setDireita(achado->getDireita());
					this->achado->setDado(achado->getDado());
				}
				this->tipo = TIPO_STRUCT_ACHARNOERRONEO;
				indicacao = indi;
			}
		};

		typedef struct structMaiorNivelRecur : IEmpilhavel
		{
			NoArvore<Tipo> *no;
			int aux;
			structMaiorNivelRecur() {
				tipo = TIPO_STRUCT_MAIORNIVELRECUR;
			}
			structMaiorNivelRecur* clone()
			{
				return new structMaiorNivelRecur(this->no, this->aux);
			}
		protected:
			structMaiorNivelRecur(const NoArvore<Tipo>* novoNo, const int aux)
			{
				this->no = new NoArvore<Tipo>();
				this->no->setPai(novoNo->getPai());
				this->no->setEsquerda(novoNo->getEsquerda());
				this->no->setDireita(novoNo->getDireita());
				this->no->setDado(novoNo->getDado());
				this->aux = aux;
				this->tipo = TIPO_STRUCT_MAIORNIVELRECUR;
			}
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//------------------------------------------------------------------METODOS AUXILIARES-------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		
		// método recursivo que coloca os da esquerda e os da direita e o próprio
		string toString(const NoArvore<Tipo> *noParam) 
		{
			if (noParam == nullptr)
				return "";
			string noEsquerda = "", noDireita = "", retorno = "";
			structToString* aStruct = new structToString();
			NoArvore<Tipo> *no = new NoArvore<Tipo>(noParam);
			inicio:
			if (no->getEsquerda() != nullptr)
			{
				//noEsquerda = "(" + this->toString(no->getEsquerda()) + ")<-";
				aStruct->no = new NoArvore<Tipo>(no);
				aStruct->noEsquerda = noEsquerda;
				aStruct->noDireita = noDireita;
				aStruct->indicacao = 1;
				pilha.empilhar(aStruct);
				noEsquerda = "";
				noDireita = "";
				no = new NoArvore<Tipo>(no->getEsquerda());
				goto inicio;
			}
			retNoEsquerda:
			if (no->getDireita() != nullptr)
			{
				//noDireita = "(" + this->toString(no->getDireita()) + ")<-";
				aStruct->no = new NoArvore<Tipo>(no);
				aStruct->noEsquerda = noEsquerda;
				aStruct->noDireita = noDireita;
				aStruct->indicacao = 2;
				pilha.empilhar(aStruct);
				noEsquerda = "";
				noDireita = "";
				no = new NoArvore<Tipo>(no->getDireita());
				goto inicio;
			}
		retNoDireita:
			retorno = "";
			if (noEsquerda != "")
				retorno = "(" + noEsquerda + ")<-";
			retorno += "[" + to_string(no->getDado()) + "]";
			if (noDireita != "")
				retorno += "->(" + noDireita + ")";
			/// RECURSÃO PRA TUDO QUANTO É LADO (só vai na horizontal)
			// if (pilha.consegueDesempilharDoMesmoTipo())
			if (dynamic_cast<structToString*>(pilha.getTopo()))
			{
				bool paiDireto = (no->getDirecaoPai() == PAI_DIREITA);
				aStruct = dynamic_cast<structToString*>(pilha.desempilhar());
				no = aStruct->no;
				if (paiDireto)
				{
					noEsquerda = retorno;
					noDireita = aStruct->noDireita;
				}
				else
				{
					noDireita = retorno;
					noEsquerda = aStruct->noEsquerda;
				}
				if (aStruct->indicacao == 0)
					goto inicio;
				if (aStruct->indicacao == 1)
					goto retNoEsquerda;
				goto retNoDireita;
			}
			else
			{
				return retorno;
			}
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

		void incluirSubArvore(const NoArvore<Tipo> *novoDado)
		{
			if (novoDado == nullptr)
				return;

			incluir(novoDado->getDado());
			incluirSubArvore(novoDado->getDireita());
			incluirSubArvore(novoDado->getEsquerda());
		}

		void incluirRecur(const Tipo &novoDado, NoArvore<Tipo> *no)
		{
			structIncluirRecur* aStruct = new structIncluirRecur();
			inicio:
			if (novoDado > no->getDado())
			{
				if (no->getDireita() != nullptr)
				{
					/// RECURSÃO DO NOH PARA DIREITA
					//incluirRecur(novoDado, no->getDireita());
					aStruct->no = no;
					pilha.empilhar(aStruct);
					no = no->getDireita();
					goto inicio;
				}
				else // if(direita é null)
				{
					no->setDireita(new NoArvore<Tipo>(novoDado));
					no->getDireita()->setPai(no);

					bufferBalancear = new NoArvore<Tipo>(no->getDireita());
				}
			}
			else if (novoDado < no->getDado())
			{
				if (no->getEsquerda() != nullptr)
				{
					/// RECURSÃO DO NOH PARA ESQUERDA
					//incluirRecur(novoDado, no->getEsquerda());
					aStruct->no = no;
					pilha.empilhar(aStruct);
					no = no->getEsquerda();
					goto inicio;
				}
				else // if(esquerda é null)
				{
					no->setEsquerda(new NoArvore<Tipo>(novoDado));
					no->getEsquerda()->setPai(no);
					bufferBalancear = new NoArvore<Tipo>(no->getEsquerda());
				}
			}
			// FIM DE UMA RECURSÃO
			while (true)
			{
				if (!dynamic_cast<structIncluirRecur*>(pilha.getTopo()))
					break;
				pilha.desempilhar();
			}
		}

		// exclúi recursivamente o nó com o dado desejado
		NoArvore<Tipo>* excluirRecur(const Tipo &dado, NoArvore<Tipo> *atual, bool *temSubArvore)
		{
			Tipo auxTipo = dado;
			NoArvore<Tipo>* auxNo = nullptr;
			NoArvore<Tipo>* noClone = nullptr;
			*temSubArvore = false;
			inicio:
			// nó inexistente
			if (atual == nullptr)
				return nullptr;
			
			if (auxTipo == atual->getDado())
			{
				if (atual->isFolha()) // se é folha, só excluir normal
				{
					if (atual->getDirecaoPai() == PAI_ESQUERDA)
						atual->getPai()->setDireita(nullptr);
					else
						atual->getPai()->setEsquerda(nullptr);

					noClone = new NoArvore<Tipo>(atual);
					atual = nullptr;
				}
				else if (atual->getEsquerda() == nullptr) // precisamos fazer quase que uma rotação
				{
					if (atual->getDirecaoPai() == PAI_ESQUERDA)
						atual->getPai()->setDireita(atual->getDireita());
					else
						atual->getPai()->setEsquerda(atual->getDireita());
					atual->getDireita()->setPai(atual->getPai());

					noClone = new NoArvore<Tipo>(atual);
					atual = nullptr;
				}
				else if(atual->getDireita() == nullptr) // precisamos fazer quase que uma rotação
				{
					if (atual->getDirecaoPai() == PAI_ESQUERDA)
						atual->getPai()->setDireita(atual->getEsquerda());
					else
						atual->getPai()->setEsquerda(atual->getEsquerda());
					atual->getEsquerda()->setPai(atual->getPai());

					noClone = new NoArvore<Tipo>(atual);
					atual = nullptr;
				}
				else // tem dois filhos, então nós colocamos qualquer um
				{
					*temSubArvore = true;
					if (getNivelMaiorDosMenores(atual) < getNivelMenorDosMaiores(atual))
					{
						NoArvore<Tipo>* aux = getMenorDosMaiores(atual);

						this->excluir(aux->getDado());

						atual->setDado(aux->getDado());
					}
					else
					{
						NoArvore<Tipo>* aux = getMaiorDosMenores(atual);

						this->excluir(aux->getDado());

						atual->setDado(aux->getDado());
					}
				}
				if (auxNo != nullptr)
					auxNo->setDado(auxTipo);
				///bufferBalancear = noClone; <- não está funfando direito aqui
				return noClone;
			}
			else if (auxTipo < atual->getDado())
				atual = atual->getEsquerda();
			else
				atual = atual->getDireita();
			
			goto inicio;
		}

		// método recursivo que coloca no "maior" o valor do maior nó errado
		void maiorNivelRecur(NoArvore<Tipo>* no, int* maior)
		{
			if (no == nullptr)
				return;
			
			int aux = 1;
			structMaiorNivelRecur* aStruct = new structMaiorNivelRecur();

			inicio:
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
			{
				// coloca no maior se o nível da folha for maior que o maior antigo
				if (aux > *maior)
					*maior = aux;
				
				if (dynamic_cast<structMaiorNivelRecur*>(pilha.getTopo()))
				{
					aStruct = dynamic_cast<structMaiorNivelRecur*>(pilha.desempilhar());
					if (aStruct->no->getEsquerda() == no)
					{
						no = aStruct->no;
						aux = aStruct->aux;
						goto direita;
					}
					else if (aStruct->no->getDireita() == no)
					{
						no = aStruct->no;
						aux = aStruct->aux;
						goto esquerda;
					}
					else
					{
						no = aStruct->no;
						aux = aStruct->aux;
						goto direita;
					}
				}
				else
					return;
			}

			aStruct->no = no;
			aStruct->aux = aux;
			this->pilha.empilhar(aStruct);

			esquerda:
			if (no->getEsquerda() != nullptr)
			{
				no = no->getEsquerda();
				aux++;
				goto inicio;
			}
			
			direita:
			if (no->getDireita() != nullptr)
			{
				no = no->getDireita();
				aux++;
				goto inicio;
			}

			if (dynamic_cast<structMaiorNivelRecur*>(pilha.getTopo()))
			{
				aStruct = dynamic_cast<structMaiorNivelRecur*>(pilha.desempilhar());
				if (aStruct->no->getEsquerda() == no)
				{
					no = aStruct->no;
					aux = aStruct->aux;
					goto direita;
				}
				else if (aStruct->no->getDireita() == no)
				{
					no = aStruct->no;
					aux = aStruct->aux;
					goto esquerda;
				}
				else
				{
					no = aStruct->no;
					aux = aStruct->aux;
					goto direita;
				}
			}
		}

		// retorna a maior altura de um nó
		int maiorNivel(NoArvore<Tipo>* no)
		{
			int* maior = (int*)malloc(sizeof(int));
			*maior = 0;
			maiorNivelRecur(no, maior);
			return *maior;
		}

		// https://www.youtube.com/watch?v=zh27Tp8HV7E
		// verifica se a subárvore inteira está balanceada
		bool verificaBalanceamento(NoArvore<Tipo>* no)
		{
			// se não existe, consideramos balanceado o nó
			if (no == nullptr)
				return true;

			int countD = 0;
			int countE = 0;
			structVerificaBalanceamento* aStruct = new structVerificaBalanceamento();

			inicio:
			// se for folha, consideramos balanceado o nó
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
			{
				desempilhar:
				if (dynamic_cast<structVerificaBalanceamento*>(pilha.getTopo()))
				{
					aStruct = dynamic_cast<structVerificaBalanceamento*>(pilha.desempilhar());
					if (aStruct->no->getEsquerda() == no)
					{
						no = aStruct->no;
						goto direita;
					}
					else if (aStruct->no->getDireita() == no)
					{
						no = aStruct->no;
						goto esquerda;
					}
					else
					{
						no = aStruct->no;
						goto direita;
					}
				}
				else
				{
					while (dynamic_cast<structVerificaBalanceamento*>(pilha.getTopo()))
						this->pilha.desempilhar();
					return true;
				}
			}

			// retornamos se o nó está balanceado, juntamente com os seus filhos
			if (abs(maiorNivel(no->getDireita()) - maiorNivel(no->getEsquerda())) < 2)
			{
				aStruct->no = no;
				this->pilha.empilhar(aStruct);

				esquerda:
				if (no->getEsquerda() != nullptr)
				{
					no = no->getEsquerda();
					goto inicio;
				}

				direita:
				if (no->getDireita() != nullptr)
				{
					no = no->getDireita();
					goto inicio;
				}
				else
					goto desempilhar;
			}
			else
			{
				while (dynamic_cast<structVerificaBalanceamento*>(pilha.getTopo()))
					this->pilha.desempilhar();
				return false;
			}
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
		bool verificaDireitaMaiorIgual(NoArvore<Tipo>* no)
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

		bool verificaDireitaMaior(NoArvore<Tipo>* no)
		{
			// se não existe, consideramos que direita é maior(tanto faz)
			if (no == nullptr)
				return true;

			// se for folha, consideramos que direita é maior(tanto faz)
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
				return false;

			// retornamos se o nó tem a direita maior
			int countD = maiorNivel(no->getDireita());
			int countE = maiorNivel(no->getEsquerda());
			return countD > countE;
		}

		// obtém o valor de balanceamento do nó
		int verificaValor(NoArvore<Tipo>* no)
		{
			if (no == nullptr)
				return 0;

			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
				return 0;

			int countD = maiorNivel(no->getDireita());
			int countE = maiorNivel(no->getEsquerda());
			return abs(countD - countE);
		}

		int verdadeiroVerificaValor(NoArvore<Tipo>* no)
		{
			if (no == nullptr)
				return 0;

			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
				return 0;

			int countD = maiorNivel(no->getDireita());
			int countE = maiorNivel(no->getEsquerda());
			return countD - countE;
		}

		// obtém o valor de balanceamento do nó
		int getNivelMaiorDosMenores(NoArvore<Tipo>* no)
		{
			if (no == nullptr || no->getEsquerda() == nullptr)
				return 0;

			no = no->getEsquerda();
			int nivel;
			for (nivel = 1; no->getDireita() != nullptr; nivel++)
			{
				no = no->getDireita();
			}
			return nivel;
		}

		int getNivelMenorDosMaiores(NoArvore<Tipo>* no)
		{
			if (no == nullptr || no->getDireita() == nullptr)
				return 0;

			no = no->getDireita();
			int nivel;
			for (nivel = 1; no->getEsquerda() != nullptr; nivel++)
			{
				no = no->getEsquerda();
			}
			return nivel;
		}

		// acha o nó errado na árvore que está mais para baixo (maior altura)
		NoArvore<Tipo>* acharNoErroneo(NoArvore<Tipo>* no)
		{
			// se não existe, consideramos não há nó errado
			if (no == nullptr)
				return nullptr;
			structAcharNoErroneo* aStruct = new structAcharNoErroneo();
			aStruct->achado = nullptr;
			NoArvore<Tipo>* achado = nullptr;

			inicio:
			if (no == nullptr)
			{
				aStruct = dynamic_cast<structAcharNoErroneo*>(pilha.desempilhar());
				achado = aStruct->achado;
				no = aStruct->no;
				if (aStruct->indicacao == 1)
					goto depoisNulo;
				else if (aStruct->indicacao == 2)
					goto depoisDireita;
				else if (aStruct->indicacao == 3)
					goto depoisEsquerda;
				else
					goto depoisTudo;
			}

		depoisNulo:
			//achado = acharNoErroneo(no->getDireita());
			if (no->getDireita() != nullptr)
			{
				aStruct->no = no;
				aStruct->achado = achado;
				aStruct->indicacao = 2;
				pilha.empilhar(aStruct);
				no = no->getDireita();
				goto inicio;
			}

		depoisDireita:
			achado = aStruct->achado;
			if (achado == nullptr)
			{
				// se não achamos, procuramos o nó que está errado para a esquerda
				//achado = acharNoErroneo(no->getEsquerda());
				if (no->getEsquerda() != nullptr)
				{
					aStruct->no = no;
					aStruct->achado = achado;
					aStruct->indicacao = 3;
					pilha.empilhar(aStruct);
					no = no->getEsquerda();
					goto inicio;
				}
			depoisEsquerda:
				if (achado != nullptr)
				{
					// se achamos, retornamos ele
					while (dynamic_cast<structAcharNoErroneo*>(pilha.getTopo()))
						pilha.desempilhar();
					return achado;
				}
			}
			else
			{
				// se achamos, retornamos ele
				while (dynamic_cast<structAcharNoErroneo*>(pilha.getTopo()))
					pilha.desempilhar();
				return achado;
			}
		depoisTudo:
			// se o nó atual está desbalanceado, retornamos ele
			if (!verificaBalanceamentoSingle(no)) ////////////////////////////////////////////////////////////////
			{
				while (dynamic_cast<structAcharNoErroneo*>(pilha.getTopo()))
					pilha.desempilhar();
				return no;
			}
			//retornamos nulo se nada está balanceado
			if (dynamic_cast<structAcharNoErroneo*>(pilha.getTopo()))
			{
				aStruct = dynamic_cast<structAcharNoErroneo*>(pilha.desempilhar());
				achado = aStruct->achado;
				no = aStruct->no;
				if (aStruct->indicacao == 1)
					goto depoisNulo;
				else if (aStruct->indicacao == 2)
					goto depoisDireita;
				else if (aStruct->indicacao == 3)
					goto depoisEsquerda;
				else
					goto depoisTudo;
			}
			else
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
				else if (verdadeiroVerificaValor(noFE) < 0)
				{
					noFFD = noFE->getEsquerda();
					metodo = 2;
				}
				// alteramos de posição o nó e o filho dele
				if (metodo == 1)
				{
					noFE->setDireita(no);
					noFE->setPai(no->getPai());
					if (noFE->getPai() != nullptr)
						noFE->getPai()->setDireita(noFE);
					no->setPai(noFE);

					no->setEsquerda(noFFD);
					noFFD->setPai(no);
				}
				else
				{
					noFE->setPai(no->getPai());
					if (noFE->getPai() != nullptr)
					{
						if (no->getDirecaoPai() == PAI_DIREITA)
							noFE->getPai()->setEsquerda(noFE);
						else
							noFE->getPai()->setDireita(noFE);
					}
					no->setPai(noFE);

					no->setEsquerda(noFE->getDireita());
					if (no->getEsquerda() != nullptr)
						no->getEsquerda()->setPai(no);
					noFE->setDireita(no);
				}

				// setamos a raiz se necessário
				if (noFE->getPai() == nullptr)
					this->raiz = noFE;
			}
			else if (direcao == GIRO_PARA_ESQUERDA)
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
					if (noFE->getPai() != nullptr)
					{
						if (no->getDirecaoPai() == PAI_DIREITA)
							noFFD->getPai()->setEsquerda(noFFD);
						else
							noFFD->getPai()->setDireita(noFFD);
					}
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
					if (noFFD->getPai() != nullptr)
					{
						if (no->getDirecaoPai() == PAI_DIREITA)
							noFFD->getPai()->setEsquerda(noFFD);
						if (no->getDirecaoPai() == PAI_ESQUERDA)
							noFFD->getPai()->setDireita(noFFD);
					}
					no->setPai(noFFD);
					no->setDireita(noFFD->getEsquerda());

					if (no->getDireita() != nullptr)
						no->getDireita()->setPai(no);

					noFE->setPai(noFFD);
					noFFD->setEsquerda(no);
					noFFD->setDireita(noFE);

					// setamos a raiz se necessário
					if (noFFD->getPai() == nullptr)
						this->raiz = noFFD;
					else
						noFFD->getPai()->setEsquerda(noFFD);
				}
			}
		}

		void balancear(int tipo) // 0 - inclusão, 1- exclusão
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
						if (verificaDireitaMaiorIgual(oi))
							girar(oi, GIRO_PARA_ESQUERDA);
						else
							girar(oi, GIRO_PARA_DIREITA);

						// colocamos o atual como pai
						oi = pai;
						// enquanto não está com o pai nulo (raiz)
					} while (pai != nullptr);
				}
				// se é inclusão
				else if (tipo == 0)
				{
					// achamos o nó errado
					NoArvore<Tipo>* oi = acharNoErroneo(this->raiz);
					while (oi != nullptr)
					{

						// giramos para o lado certo
						if (verificaDireitaMaiorIgual(oi))
							girar(oi, GIRO_PARA_ESQUERDA);
						else
							girar(oi, GIRO_PARA_DIREITA);
						// enquanto não está balanceado
						oi = acharNoErroneo(this->raiz);
					}
				}
				else
				{
					bool* tem = new bool(false);
					NoArvore<Tipo>* oi = new NoArvore<Tipo>(acharNoErroneo(this->raiz));
					this->excluirRecur(oi->getDado(), this->raiz, tem);
					this->incluirRecur(oi->getDado(), this->raiz);
				}
			}
			else
			{
				cout << "BOT> Nao desbalanceou a arvore" << endl;
			}
			bufferBalancear = nullptr;
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//

		// raiz da árvore inteira
		NoArvore<Tipo> *raiz;
		Pilha<IEmpilhavel*> pilha;
		NoArvore<Tipo>* bufferBalancear;
	private:
		static bool comparaParaEsvaziar(int a, int b)
		{
			return (a == b);
		}
};