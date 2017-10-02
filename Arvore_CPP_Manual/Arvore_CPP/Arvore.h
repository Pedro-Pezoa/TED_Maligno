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
					this->raiz = nullptr;
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
			this->incluirRecur(novoDado, raiz);
			// balanceamos
			this->balancear(0, novoDado);
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
				tipo = 3;
			}
		}structIncluirSubArvore;

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
			int countD;
			int countE;
			structVerificaBalanceamento() {
				tipo = 5;
			}
		}structVerificaBalanceamento;

		struct structAcharNoErroneo : IEmpilhavel
		{
			NoArvore<Tipo> *no;
			NoArvore<Tipo>* achado;
			structAcharNoErroneo() {
				tipo = TIPO_STRUCT_ACHARNOERRONEO;
			}
			structAcharNoErroneo* clone()
			{
				return new structAcharNoErroneo(no, achado);
			}
		protected:
			structAcharNoErroneo(const NoArvore<Tipo> *novoNo, const NoArvore<Tipo>* achado)
			{
				this->no = new NoArvore<Tipo>();
				this->no->setPai(novoNo->getPai());
				this->no->setEsquerda(novoNo->getEsquerda());
				this->no->setDireita(novoNo->getDireita());
				this->no->setDado(novoNo->getDado());
				this->achado = new NoArvore<Tipo>();
				this->achado->setPai(achado->getPai());
				this->achado->setEsquerda(achado->getEsquerda());
				this->achado->setDireita(achado->getDireita());
				this->achado->setDado(achado->getDado());
				tipo = TIPO_STRUCT_ACHARNOERRONEO;
			}
		};

		typedef struct structExcluirRecur : IEmpilhavel
		{
			NoArvore<Tipo> *no;
			structExcluirRecur() {
				tipo = 7;
			}
		}structExcluirRecur;

		typedef struct structMaiorNivelRecur : IEmpilhavel
		{
			NoArvore<Tipo> *no;
			structMaiorNivelRecur() {
				tipo = 8;
			}
		}structMaiorNivelRecur;

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

		// inclúi uma subárvore inteira embaixo do nó
		void incluirSubArvore(NoArvore<Tipo> *no)
		{
			if (no == nullptr)
			{
				/// ACABANDO UMA RECURSÃO
				return;
			}

			incluir(no->getDado());

			/// RECURSÃO DO NOH PARA DIREITA
			incluirSubArvore(no->getDireita());
			/// RECURSÃO DO NOH PARA ESQUERDA
			incluirSubArvore(no->getEsquerda());
			/// ACABANDO UMA RECURSÃO
		}

		// exclúi (deleta) uma subárvore inteira (não precisa estar na árvore)
		void excluirSubArvore(NoArvore<Tipo> *no)
		{
			if (no == nullptr)
				return;
			structExcluirSubArvore* aStruct = new structExcluirSubArvore();
			inicio:

			/// RECURSÃO DO NOH PARA DIREITA
			//excluirSubArvore(no->getDireita());
			if (no->getDireita() != nullptr)
			{
				aStruct->no = no;
				aStruct->indicacao = 1;
				pilha.empilhar(aStruct);
				no = no->getDireita();
				goto inicio;
			}
			retDaDireita:
			/// RECURSÃO DO NOH PARA ESQUERDA
			if (no->getEsquerda() != nullptr)
			{
				//excluirSubArvore(no->getEsquerda());
				aStruct->no = no;
				aStruct->indicacao = 2;
				pilha.empilhar(aStruct);
				no = no->getEsquerda();
			}
			retDaEsquerda:
			no = nullptr;
			if (dynamic_cast<structExcluirSubArvore*>(pilha.getTopo()))
			{
				aStruct = dynamic_cast<structExcluirSubArvore*>(pilha.desempilhar());
				no = aStruct->no;
				if (aStruct->indicacao == 1)
					goto retDaDireita;
				goto retDaEsquerda;
			}
			/// ACABANDO UMA RECURSÃO
		}

		// exclúi recursivamente o nó com o dado desejado
		bool excluirRecur(const Tipo &dado, NoArvore<Tipo> *atual)
		{
			// nó inexistente
			if (atual == nullptr)
			{
				/// ACABANDO UMA RECURSÃO
				return false;
			}

			if (dado == atual->getDado())
			{
				if (atual->isFolha()) // se é folha, só excluir normal
				{
					if (atual->getDirecaoPai() == PAI_ESQUERDA)
						atual->getPai()->setDireita(nullptr);
					else
						atual->getPai()->setEsquerda(nullptr);
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
				/// ACABANDO UMA RECURSÃO
				return true;
			}
			else if (dado < atual->getDado())
			{
				/// RECURSÃO DO NOH PARA ESQUERDA
				/// ACABANDO UMA RECURSÃO
				return this->excluirRecur(dado, atual->getEsquerda());
			}
			else
			{
				/// RECURSÃO DO NOH PARA DIREITA
				/// ACABANDO UMA RECURSÃO
				return this->excluirRecur(dado, atual->getDireita());
			}
		}

		// método recursivo que coloca no "maior" o valor do maior nó errado
		void maiorNivelRecur(NoArvore<Tipo>* no, const int &aux, int* maior)
		{
			if (no == nullptr)
			{
				/// ACABANDO UMA RECURSÃO
				return;
			}

			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
			{
				// coloca no maior se o nível da folha for maior que o maior antigo
				if (aux > *maior)
					*maior = aux;
				/// ACABANDO UMA RECURSÃO
				return;
			}

			/// RECURSÃO DO NOH PARA DIREITA
			maiorNivelRecur(no->getDireita(), aux+1, maior);
			/// RECURSÃO DO NOH PARA ESQUERDA
			maiorNivelRecur(no->getEsquerda(), aux+1, maior);
			/// ACABANDO UMA RECURSÃO
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
			{
				/// ACABANDO UMA RECURSÃO
				return true;
			}

			// se for folha, consideramos balanceado o nó
			if (no->getDireita() == nullptr && no->getEsquerda() == nullptr)
			{
				/// ACABANDO UMA RECURSÃO
				return true;
			}

			// retornamos se o nó está balanceado, juntamente com os seus filhos
			int countD = maiorNivel(no->getDireita());
			int countE = maiorNivel(no->getEsquerda());

			/// RECURSÂO PARA AMBOS OS LADOS (só armazenar os valores)
			/// ACABANDO UMA RECURSÃO
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
			if (no == nullptr)
				return nullptr;
			NoArvore<Tipo>* achado;
			structAcharNoErroneo* aStruct = new structAcharNoErroneo();
			fn comp = &Arvore<Tipo>::comparaParaEsvaziar;
			inicio:
			if (no == nullptr)
			{
				/// ACABANDO UMA RECURSÃO
				aStruct = dynamic_cast<structAcharNoErroneo*>(pilha.desempilhar());
				no = aStruct->no;
				achado = aStruct->achado;
				if (aStruct->indicacao == 1)
					goto retNoDireita;
				goto retNoEsquerda;
			}

			// procuramos o nó que está errado para a direita
			/// RECURSÃO PARA A DIREITA
			achado = acharNoErroneo(no->getDireita());
			retNoDireita:
			if (achado == nullptr)
			{
				// se não achamos, procuramos o nó que está errado para a esquerda
				/// RECURSÃO PARA A ESQUERDA
				achado = acharNoErroneo(no->getEsquerda());
				retNoEsquerda:
				if (achado != nullptr)
					// se achamos, retornamos ele
					/// ACABANDO UMA RECURSÃO
					pilha.esvaziar(comp, TIPO_STRUCT_ACHARNOERRONEO);
					return achado;
			}
			else
			{
				// se achamos, retornamos ele
				/// ACABANDO UMA RECURSÃO
				pilha.esvaziar(comp, TIPO_STRUCT_ACHARNOERRONEO);
				return achado;
			}

			// se o nó atual está desbalanceado, retornamos ele
			if (!verificaBalanceamentoSingle(no))
			{
				/// ACABANDO UMA RECURSÃO
				pilha.esvaziar(comp, TIPO_STRUCT_ACHARNOERRONEO);
				return no;
			}
			//retornamos nulo se nada está balanceado
			/// ACABANDO UMA RECURSÃO
			if (dynamic_cast<structAcharNoErroneo*>(pilha.getTopo()))
			{
				aStruct = dynamic_cast<structAcharNoErroneo*>(pilha.desempilhar());
				no = aStruct->no;
				achado = aStruct->achado;
				if (aStruct->indicacao == 1)
					goto retNoDireita;
				goto retNoEsquerda;
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
		Pilha<IEmpilhavel*> pilha;
	private:
		static bool comparaParaEsvaziar(int a, int b)
		{
			return (a == b);
		}
};