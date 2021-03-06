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
#define TIPO_STRUCT_SETPAITUDO 9

template <class Tipo>
class Arvore
{
	public:
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//-------------------------------------------------------------CONSTRUTORES/DESTRUTOR--------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//

		// construtor nulo
		Arvore() : raiz(nullptr){}

		// construtor de c�pia
		Arvore(const Arvore<Tipo> &original) : setRaiz(original.getRaiz()) {}

		// construtor com 1 dado inicial
		Arvore(const Tipo &novoDado) : setRaiz(novoDado) {}

		// destrutor
		~Arvore() {}

		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//---------------------------------------------------------------GETTERS E SETTERS-----------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//

		// getter da raiz para saber qual a raiz da �rvore diretamente
		Tipo getRaiz() const
		{
			return this->raiz->getDado();
		}

		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//---------------------------------------------------------------M�TODOS PRINCIPAIS----------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	
		// m�todo excluir um dado
		bool excluir(const Tipo &dado)
		{
			// se for a raiz que se deseja excluir
			if (this->raiz == nullptr)
			{
				cout << "BOT> A arvore ja esta vazia..." << endl;
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
				if (this->raiz->getEsquerda() == nullptr || this->raiz->getDireita() == nullptr)
					this->balancear(1);
				else
					this->balancear(2);

				if (raiz != nullptr)
					cout << "BOT> Nova raiz: " << this->raiz->getDado() << endl;
				return true;
			}
			else
			{
				// n�o � raiz
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
				cout << "BOT> Nao existe o valor " << dado << " na arvore" << endl;
			return false;
		}

		void incluir(const Tipo &novoDado)
		{
			// se n�o tiver nem raiz, inclu�mos a raiz
			if (this->raiz == nullptr)
			{
				this->raiz = new NoArvore<Tipo>(novoDado);
				cout << "BOT> Incluido na raiz o " << novoDado << endl;
				return;
			}
			// mostramos que inclu�mos
			cout << "BOT> Incluido o " << novoDado << endl;
			// inclu�mos
			this->incluirRecur(novoDado, raiz);
			//this->setaPaiTudo(this->raiz);
			// balanceamos
			this->balancear(0);
		}

		friend ostream& operator<< (ostream &os, Arvore<Tipo> &aArvore)
		{
			// escrevemos o valor da �rvore
			return (os << "TREE> " << aArvore.toString());
		}

		string toString()
		{
			// retornamos o conte�do da �rvore desde a raiz
			return this->toString(this->raiz);
		}

		string viewNo(const Tipo& param)
		{
			if (this->raiz == nullptr)
				return "";

			NoArvore<Tipo>* no = getNoTal(this->raiz, param);
			if (no == nullptr)
				return "Sem informacao desse trem ai...";
			return "Info: "+to_string(no->getDado())+
				((no->getDireita() != nullptr)?(", Direita: " + to_string(no->getDireita()->getDado())):(", Direita: nulo"))+
				((no->getEsquerda() != nullptr) ? (", Esquerda: " + to_string(no->getEsquerda()->getDado())) : (", Esquerda: nulo"))+
				((no->getPai() != nullptr) ? (", Pai: " + to_string(no->getPai()->getDado())) : (", Pai: nulo"));
		}

		NoArvore<Tipo>* getNoTal(NoArvore<Tipo>* auxiliar, const int& i)
		{
			if (auxiliar == nullptr)
				return nullptr;

			if (auxiliar->getDado() > i)
				return getNoTal(auxiliar->getEsquerda(), i);
			else if (auxiliar->getDado() < i)
				return getNoTal(auxiliar->getDireita(), i);
			else
				return auxiliar;
		}

		bool verSeTaCertoMesmo(const int& param)
		{
			NoArvore<Tipo>* no = getNoTal(this->raiz, param);
			return verSeTaCerto(no);
		}

		bool verSeTaCerto(NoArvore<Tipo>* auxiliar)
		{
			if (auxiliar == nullptr)
				return true;

			bool esquerda = true, direita = true, esse = true;
			if (auxiliar->getEsquerda() != nullptr)
			{
				esse = auxiliar->getDado() > auxiliar->getEsquerda()->getDado();
				esquerda = verSeTaCerto(auxiliar->getEsquerda());
			}
			if (auxiliar->getDireita() != nullptr)
			{
				esse = esse && auxiliar->getDado() < auxiliar->getDireita()->getDado();
				direita = verSeTaCerto(auxiliar->getDireita());
			}
			return esquerda && direita && esse;
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

		typedef struct structSetPaiTudo : IEmpilhavel
		{
			NoArvore<Tipo> *no;
			structSetPaiTudo() {
				tipo = TIPO_STRUCT_SETPAITUDO;
			}
			structSetPaiTudo* clone()
			{
				return new structSetPaiTudo(this->no);
			}
		protected:
			structSetPaiTudo(const NoArvore<Tipo>* novoNo)
			{
				this->no = new NoArvore<Tipo>();
				this->no->setPai(novoNo->getPai());
				this->no->setEsquerda(novoNo->getEsquerda());
				this->no->setDireita(novoNo->getDireita());
				this->no->setDado(novoNo->getDado());
				this->tipo = TIPO_STRUCT_SETPAITUDO;
			}
		};

		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//------------------------------------------------------------------METODOS AUXILIARES-------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		
		// m�todo recursivo que coloca os da esquerda e os da direita e o pr�prio
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
			/// RECURS�O PRA TUDO QUANTO � LADO (s� vai na horizontal)
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

		// retorna o n� que poss�i o dado a ser achado
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

		// obt�m o menor dos maiores do n� dado
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

		// retorna o maior dos menores do n� dado
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
					/// RECURS�O DO NOH PARA DIREITA
					//incluirRecur(novoDado, no->getDireita());
					aStruct->no = no;
					pilha.empilhar(aStruct);
					no = no->getDireita();
					goto inicio;
				}
				else // if(direita � null)
				{
					no->setDireita(new NoArvore<Tipo>(novoDado));
					no->getDireita()->setPai(no);
				}
			}
			else if (novoDado < no->getDado())
			{
				if (no->getEsquerda() != nullptr)
				{
					/// RECURS�O DO NOH PARA ESQUERDA
					//incluirRecur(novoDado, no->getEsquerda());
					aStruct->no = no;
					pilha.empilhar(aStruct);
					no = no->getEsquerda();
					goto inicio;
				}
				else // if(esquerda � null)
				{
					no->setEsquerda(new NoArvore<Tipo>(novoDado));
					no->getEsquerda()->setPai(no);
				}
			}
			// FIM DE UMA RECURS�O
			while (true)
			{
				if (!dynamic_cast<structIncluirRecur*>(pilha.getTopo()))
					break;
				pilha.desempilhar();
			}
		}

		void setaPaiTudo(NoArvore<Tipo> *no)
		{
			if (no == nullptr)
				return;

			structSetPaiTudo* aStruct = new structSetPaiTudo();
			inicio:
			// se for folha, consideramos balanceado o n�
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
			{
				desempilhar:
				if (dynamic_cast<structSetPaiTudo*>(pilha.getTopo()))
				{
					aStruct = dynamic_cast<structSetPaiTudo*>(pilha.desempilhar());
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
					while (dynamic_cast<structSetPaiTudo*>(pilha.getTopo()))
						this->pilha.desempilhar();
					return;
				}
			}

			if (no->getDireita() != nullptr)
				no->getDireita()->setPai(no);

			if (no->getEsquerda() != nullptr)
				no->getEsquerda()->setPai(no);

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

		// excl�i recursivamente o n� com o dado desejado
		NoArvore<Tipo>* excluirRecur(const Tipo &dado, NoArvore<Tipo> *atual, bool *temSubArvore)
		{
			Tipo auxTipo = dado;
			NoArvore<Tipo>* pai = atual->getPai();
			NoArvore<Tipo>* auxNo = nullptr;
			NoArvore<Tipo>* noClone = nullptr;
			*temSubArvore = false;
			inicio:
			// n� inexistente
			if (atual == nullptr)
				return nullptr;
			
			if (auxTipo == atual->getDado())
			{
				if (atual->isFolha()) // se � folha, s� excluir normal
				{
					int direc = atual->getDirecaoPai();
					if (atual->getDado() == 0)
					{
						noClone = new NoArvore<Tipo>(atual);
						delete(atual);
					}
					
					if (direc == PAI_ESQUERDA)
						pai->setDireita(nullptr);
					else
						pai->setEsquerda(nullptr);

					if (noClone == nullptr)
					{
						noClone = new NoArvore<Tipo>(atual);
						delete(atual);
					}
				}
				else if (atual->getEsquerda() == nullptr) // precisamos fazer quase que uma rota��o
				{
					if (atual->getDirecaoPai() == PAI_ESQUERDA)
						atual->getPai()->setDireita(atual->getDireita());
					else
						atual->getPai()->setEsquerda(atual->getDireita());
					atual->getDireita()->setPai(atual->getPai());

					noClone = new NoArvore<Tipo>(atual);
					atual = nullptr;
				}
				else if(atual->getDireita() == nullptr) // precisamos fazer quase que uma rota��o
				{
					if (atual->getDirecaoPai() == PAI_ESQUERDA)
						atual->getPai()->setDireita(atual->getEsquerda());
					else
						atual->getPai()->setEsquerda(atual->getEsquerda());
					atual->getEsquerda()->setPai(atual->getPai());

					noClone = new NoArvore<Tipo>(atual);
					atual = nullptr;
				}
				else // tem dois filhos, ent�o n�s colocamos qualquer um
				{
					*temSubArvore = true;
					if (getNivelMaiorDosMenores(atual) < getNivelMenorDosMaiores(atual))
					{
						NoArvore<Tipo>* auquis = getMenorDosMaiores(atual);

						int valorExcluir = auquis->getDado();
						int valorAlterar = atual->getDado();
						this->excluir(auquis->getDado());

						this->getNoDoDado(valorAlterar)->setDado(valorExcluir);
					}
					else
					{
						NoArvore<Tipo>* auquis = getMaiorDosMenores(atual);

						int valorExcluir = auquis->getDado();
						int valorAlterar = atual->getDado();
						this->excluir(auquis->getDado());

						this->getNoDoDado(valorAlterar)->setDado(valorExcluir);
					}
				}
				if (auxNo != nullptr)
					auxNo->setDado(auxTipo);
				return noClone;
			}

			else if (auxTipo < atual->getDado())
			{
				pai = atual;
				atual = atual->getEsquerda();
			}
			else
			{
				pai = atual;
				atual = atual->getDireita();
			}
			
			goto inicio;
		}

		// m�todo recursivo que coloca no "maior" o valor do maior n� errado
		void maiorNivelRecur(NoArvore<Tipo>* no, int* maior)
		{
			if (no == nullptr)
				return;
			
			int aux = 1;
			structMaiorNivelRecur* aStruct = new structMaiorNivelRecur();

			inicio:
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
			{
				// coloca no maior se o n�vel da folha for maior que o maior antigo
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

		// retorna a maior altura de um n�
		int maiorNivel(NoArvore<Tipo>* no)
		{
			int* maior = (int*)malloc(sizeof(int));
			*maior = 0;
			maiorNivelRecur(no, maior);
			return *maior;
		}

		// https://www.youtube.com/watch?v=zh27Tp8HV7E
		// verifica se a sub�rvore inteira est� balanceada
		bool verificaBalanceamento(NoArvore<Tipo>* no)
		{
			// se n�o existe, consideramos balanceado o n�
			if (no == nullptr)
				return true;

			int countD = 0;
			int countE = 0;
			structVerificaBalanceamento* aStruct = new structVerificaBalanceamento();

			inicio:
			// se for folha, consideramos balanceado o n�
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

			// retornamos se o n� est� balanceado, juntamente com os seus filhos
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

		// verifica se o n� espec�fico est� balanceado
		bool verificaBalanceamentoSingle(NoArvore<Tipo>* no)
		{
			// se n�o existe, consideramos balanceado o n�
			if (no == nullptr)
				return true;

			// se for folha, consideramos balanceado o n�
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
				return true;

			// retornamos se o n� est� balanceado, separadamente dos seus filhos
			int countD = maiorNivel(no->getDireita());
			int countE = maiorNivel(no->getEsquerda());
			return (abs(countD - countE)) < 2;
		}

		// retorna se a direita do n� � maior que a esquerda
		bool verificaDireitaMaiorIgual(NoArvore<Tipo>* no)
		{
			// se n�o existe, consideramos que direita � maior(tanto faz)
			if (no == nullptr)
				return true;

			// se for folha, consideramos que direita � maior(tanto faz)
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
				return true;

			// retornamos se o n� tem a direita maior
			int countD = maiorNivel(no->getDireita());
			int countE = maiorNivel(no->getEsquerda());
			return countD > countE;
		}

		bool verificaDireitaMaior(NoArvore<Tipo>* no)
		{
			// se n�o existe, consideramos que direita � maior(tanto faz)
			if (no == nullptr)
				return true;

			// se for folha, consideramos que direita � maior(tanto faz)
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
				return false;

			// retornamos se o n� tem a direita maior
			int countD = maiorNivel(no->getDireita());
			int countE = maiorNivel(no->getEsquerda());
			return countD > countE;
		}

		// obt�m o valor de balanceamento do n�
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

		// obt�m o valor de balanceamento do n�
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

		// acha o n� errado na �rvore que est� mais para baixo (maior altura)
		NoArvore<Tipo>* acharNoErroneo(NoArvore<Tipo>* no)
		{
			// se n�o existe, consideramos n�o h� n� errado
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
				// se n�o achamos, procuramos o n� que est� errado para a esquerda
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
			// se o n� atual est� desbalanceado, retornamos ele
			if (!verificaBalanceamentoSingle(no))
			{
				while (dynamic_cast<structAcharNoErroneo*>(pilha.getTopo()))
					pilha.desempilhar();
				return no;
			}
			//retornamos nulo se nada est� balanceado
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

		// gira a �rvore para a dire��o necessitada
		void girar(NoArvore<Tipo>* no, int direcao)
		{
			// se o n� for nulo, n�o giramos
			if (no == nullptr)
				return;
			// se o giro for para a direita, giramos para a direita
			if (direcao == GIRO_PARA_DIREITA)
			{
				// armazenamos os auxiliares
				NoArvore<Tipo>* noFE = no->getEsquerda();
				NoArvore<Tipo>* noFFD = noFE->getDireita();

				// m�todo identifica o modo de como ser� setado a esquerda do n�
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
				// alteramos de posi��o o n� e o filho dele
				if (metodo == 1)
				{
					noFFD->setPai(no->getPai());
					if (noFFD->getPai() != nullptr)
					{
						if (no->getDirecaoPai() == PAI_DIREITA)
							noFFD->getPai()->setEsquerda(noFFD);
						else
							noFFD->getPai()->setDireita(noFFD);
					}

					no->setEsquerda(noFFD->getDireita());
					if (no->getDireita() != nullptr)
						no->getDireita()->setPai(no);

					noFE->setPai(noFFD);
					no->setPai(noFFD);
					noFE->setDireita(noFFD->getEsquerda());
					if (noFE->getEsquerda() != nullptr)
						noFE->getEsquerda()->setPai(noFE);
					noFFD->setDireita(no);
					noFFD->setEsquerda(noFE);

					// setamos a raiz se necess�rio
					if (noFFD->getPai() == nullptr)
						this->raiz = noFFD;
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

					// setamos a raiz se necess�rio
					if (noFE->getPai() == nullptr)
						this->raiz = noFE;
				}
			}
			else if (direcao == GIRO_PARA_ESQUERDA)
			{
				NoArvore<Tipo>* noFE = no->getDireita();
				NoArvore<Tipo>* noFFD = noFE->getEsquerda();

				// m�todo identifica o modo de como ser� setado a direita do n�
				int metodo = 1;
				if (noFFD == nullptr)
				{
					noFFD = noFE->getDireita();
					metodo = 2;
				}
				else if (verdadeiroVerificaValor(noFE) > 0)
				{
					noFFD = noFE->getDireita();
					metodo = 2;
				}

				if (metodo == 2)
				{
					// alteramos de posi��o o n� e o filho dele
					noFE->setPai(no->getPai());
					if (noFE->getPai() != nullptr)
					{
						if (no->getDirecaoPai() == PAI_DIREITA)
							noFE->getPai()->setEsquerda(noFE);
						else
							noFE->getPai()->setDireita(noFE);
					}
					no->setPai(noFE);
					no->setDireita(noFE->getEsquerda());
					if (no->getDireita() != nullptr)
						no->getDireita()->setPai(no);
					noFE->setEsquerda(no);
					// setamos a raiz se necess�rio
					if (noFE->getPai() == nullptr)
						this->raiz = noFE;
				}
				else
				{
					// alteramos de posi��o o n� e o filho dele
					//noFE->setEsquerda(nullptr);
					noFFD->setPai(no->getPai());
					if (noFFD->getPai() != nullptr)
					{
						if (no->getDirecaoPai() == PAI_DIREITA)
							no->getPai()->setEsquerda(noFFD);
						if (no->getDirecaoPai() == PAI_ESQUERDA)
							no->getPai()->setDireita(noFFD);
					}
					no->setPai(noFFD);
					no->setDireita(noFFD->getEsquerda());

					if (no->getDireita() != nullptr)
						no->getDireita()->setPai(no);

					noFE->setPai(noFFD);
					noFE->setEsquerda(noFFD->getDireita());
					if (noFE->getEsquerda() != nullptr)
						noFE->getEsquerda()->setPai(noFE);
					noFFD->setEsquerda(no);
					noFFD->setDireita(noFE);

					// setamos a raiz se necess�rio
					if (noFFD->getPai() == nullptr)
						this->raiz = noFFD;
				}
			}
		}

		void balancear(int tipo) // 0 - inclus�o, 1- exclus�o
		{
			// verificar se o balanceamento est� correto
			// se sim, acabou...
			if (!verificaBalanceamento(this->raiz))
			{
				// se n�o, temos que ajustar
				cout << "BOT> Desbalanceou a arvore" << endl;
				// se � exclus�o
				if (tipo == 1)
				{
					// achamos o n� errado e o seu pai
					NoArvore<Tipo>* oi = acharNoErroneo(this->raiz);
					while (oi != nullptr)
					{

						// giramos para o lado certo
						if (verificaDireitaMaiorIgual(oi))
							girar(oi, GIRO_PARA_ESQUERDA);
						else
							girar(oi, GIRO_PARA_DIREITA);
						// enquanto n�o est� balanceado
						oi = acharNoErroneo(this->raiz);
					}
				}
				// se � inclus�o
				else if (tipo == 0)
				{
					// achamos o n� errado
					NoArvore<Tipo>* oi = acharNoErroneo(this->raiz);
					while (oi != nullptr)
					{

						// giramos para o lado certo
						if (verificaDireitaMaiorIgual(oi))
							girar(oi, GIRO_PARA_ESQUERDA);
						else
							girar(oi, GIRO_PARA_DIREITA);
						// enquanto n�o est� balanceado
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
			this->setaPaiTudo(this->raiz);
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//
		//-------------------------------------------------------------------ATRIBUTOS---------------------------------------------------------------------------//
		//-------------------------------------------------------------------------------------------------------------------------------------------------------//

		// raiz da �rvore inteira
		NoArvore<Tipo> *raiz;
		Pilha<IEmpilhavel*> pilha;
	private:
};