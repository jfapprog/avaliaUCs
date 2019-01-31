/* 
 * e-folio B
 * 1301913
 * Jos� P�voa
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAXSTRING 256

/*estrutura atividade de avalia��o*/
typedef struct SAval
{
	double nota;
	char *nome;
} TAval;

/*cria uma atividade de avalia��o nova*/
TAval criarAval(double nota, char *nome)
{
	TAval novaAval;
	/*alocar mem�ria para o nome da atividade de avalia��o*/
	novaAval.nome = (char*) malloc(strlen(nome) + 1);
	strcpy(novaAval.nome, nome);
	novaAval.nota = nota;
	return novaAval;
}

/*escreve as informa��es de uma atividade de avalia��o*/
void mostrarAval(TAval *aval)
{
	printf("%.1lf %s\n", aval->nota, aval->nome);
}

/*remove uma atividade de avalia��o*/
void removerAval(TAval *aval)
{
	free(aval->nome);
}

/*estrutura lista de atividades de avalia��o*/
typedef struct SListaAval
{
	TAval *aval;
	struct SListaAval *seg;
} TListaAval;

/*adiciona uma atividade de avalia��o no topo da lista*/
TListaAval *adicionarTopoListaAval(TAval aval, TListaAval *topo)
{
	TListaAval *novoTopo;
	/*alocar mem�ria para um novo topo da lista*/
	novoTopo = (TListaAval*) malloc(sizeof(TListaAval));
	if (novoTopo == NULL)
	{
		printf("Nao foi possivel alocar memoria para um novo elemento da lista\n");
		return topo;
	}
	/*alocar mem�ria para a atividade de avalia��o do novo topo*/
	novoTopo->aval = (TAval*) malloc(sizeof(aval));
	if (novoTopo->aval == NULL)
	{
		printf("Nao foi possivel alocar memoria para uma at. de avaliacao\n");
		free(novoTopo);
		return topo;
	}
	/*apontar para a atividade de avalia��o*/
	*novoTopo->aval = aval;
	/*colocar no topo*/
	novoTopo->seg = topo;
	return novoTopo;
}

/*remove uma atividade de avalia��o do topo da lista*/
TListaAval *removerTopoListaAval(TListaAval *topo)
{
	TListaAval *aux;
	if (topo != NULL)
	{
		aux = topo->seg;
		/*libertar primeiro a mem�ria da atividade de avalia��o*/
		removerAval(topo->aval);
		free(topo->aval);
		free(topo);
		return aux;
	}
	return NULL;
}

/*remove a n-�sima atividade de avalia��o da lista*/
TListaAval *removerAvalNListaAval(int n, TListaAval *topo)
{
	TListaAval *aux;
	/*se � o primeiro elemento a eliminar o topo � alterado*/
	if (n == 1)
		topo = removerTopoListaAval(topo);
	/*se n�o, � necess�rio ligar o elemento anterior ao eliminado ao seu seguinte*/
	else
	{
		aux = topo;
		while (aux != NULL && n-- > 2)
			aux = aux->seg;
		aux->seg = removerTopoListaAval(aux->seg);
	}
	return topo;
}

/*remove todas as atividades de avalia��o da lista*/
void removerListaAval(TListaAval *topo)
{
	while (topo != NULL)
		topo = removerTopoListaAval(topo);
}

/*escreve as informa��es da lista de atividades de avalia��o*/
void mostrarListaAval(TListaAval *topo, int *cont)
{
	while (topo != NULL)
	{
		printf("<%d> ", (*cont)++);
		mostrarAval(topo->aval);
		topo = topo->seg;
	}
	putchar('\n');
}

/*contabiliza as atividades de avalia��o de uma lista*/
int contabilizarAval(TListaAval *topo)
{
	int cont = 0;
	while (topo != NULL)
	{
		cont++;
		topo = topo->seg;
	}
	return cont;
}

/*estrutura disciplina*/
typedef struct SDisc
{
	char *nome;
	double nota;
	TListaAval *listaAval;
} TDisc;

/*cria uma disciplina nova*/
TDisc criarDisc(char *nome)
{
	TDisc novaDisc;
	/*alocar mem�ria para o nome da disciplina*/
	novaDisc.nome = (char*) malloc(strlen(nome) + 1);
	strcpy(novaDisc.nome, nome);
	/*inicializar a nota com valor 0 e a lista de atividades de avalia��o com NULL*/
	novaDisc.nota = 0.0;
	novaDisc.listaAval = NULL;
	return novaDisc;
}

/*calcula a nota de uma disciplina*/
double calcularNota(TDisc *disc)
{
	double nota = 0.0, decNota, intNota;
	TListaAval *aux = disc->listaAval;
	/*adicionar as notas de todas as atividades de avalia��o da disciplina*/
	while (aux != NULL)
	{
		nota += aux->aval->nota;
		aux = aux->seg;
	}
	/*arredondar a nota para o valor inteiro mais pr�ximo*/
	decNota = modf(nota, &intNota);
	if (decNota >= 0.49)
		intNota += 1.0;
	return intNota;
}

/*escreve as informa��es de uma disciplina*/
void mostrarDisc(TDisc *disc, int *cont)
{
	printf("%s", disc->nome);
	/*atualizar primeiro a nota*/
	disc->nota = calcularNota(disc);
	if (disc->nota > 9.49)
		printf(" - Nota: %.0lf valores", disc->nota);
	putchar('\n');
	/*mostrar lista de actividades de avalia��o da disciplina*/
	mostrarListaAval(disc->listaAval, cont);
	putchar('\n');
}

/*remove uma disciplina*/
void removerDisc(TDisc *disc)
{
	free(disc->nome);
	removerListaAval(disc->listaAval);
}

/*estrutura lista de disciplinas*/
typedef struct SListaDisc
{
	TDisc *disc;
	struct SListaDisc *seg;
} TListaDisc;

/*adiciona uma disciplina no topo da lista*/
TListaDisc *adicionarTopoListaDisc(TDisc disc, TListaDisc *topo)
{
	TListaDisc *novoTopo;
	/*alocar mem�ria para um novo elemento da lista*/
	novoTopo = (TListaDisc*) malloc(sizeof(TListaDisc));
	if (novoTopo == NULL)
	{
		printf("Nao foi possivel alocar memoria para um novo elemento da lista\n");
		return topo;
	}
	/*alocar mem�ria para a disciplina do novo topo*/
	novoTopo->disc = (TDisc*) malloc(sizeof(disc));
	if (novoTopo->disc == NULL)
	{
		printf("Nao foi possivel alocar memoria para uma disciplina\n");
		free(novoTopo);
		return topo;
	}
	/*apontar para a disciplina*/
	*novoTopo->disc = disc;
	/*colocar no topo*/
	novoTopo->seg = topo;
	return novoTopo;
}

TListaDisc *adicionarBaseListaDisc(TDisc disc, TListaDisc *topo)
{
	TListaDisc *aux;
	if (topo == NULL)
		topo = adicionarTopoListaDisc(disc, topo);
	else
	{
		aux = topo;
		while (aux->seg != NULL)
			aux = aux->seg;
		aux->seg = adicionarTopoListaDisc(disc, aux->seg);
	}
	return topo;
}

/*remove a disciplina do topo da lista*/
TListaDisc *removerTopoListaDisc(TListaDisc *topo)
{
	TListaDisc *aux;
	if (topo != NULL)
	{
		aux = topo->seg;
		/*libertar primeiro a mem�ria alocada para a disciplina*/
		removerDisc(topo->disc);
		free(topo->disc);
		free(topo);
		return aux;
	}
	return NULL;
}

/*remove a disciplina n da lista de disciplinas*/
TListaDisc *removerDiscNListaDisc(TListaDisc *topo, int n)
{
	TListaDisc *aux;
	/*se � o primeiro elemento a eliminar o topo � alterado*/
	if (n == 1)
		topo = removerTopoListaDisc(topo);
	/*se n�o � necess�rio ligar o elemento anterior ao eliminado ao seu seguinte*/
	else
	{
		aux = topo;
		while (aux != NULL && n-- > 2)
			aux = aux->seg;
		aux->seg = removerTopoListaDisc(aux->seg);
	}
	return topo;
}

/*remove todos as disciplinas da lista*/
void removerListaDisc(TListaDisc *topo)
{
	while (topo != NULL)
		topo = removerTopoListaDisc(topo);
}

/*escreve as informa��es da lista de disciplinas*/
void mostrarListaDisc(TListaDisc *topo)
{
	int cont = 1;
	TListaDisc *aux;
	aux = topo;
	while (aux != NULL)
	{
		printf("<%d> ", cont++);
		mostrarDisc(aux->disc, &cont);
		aux = aux->seg;
	}
}

/*determina o n�mero de elementos da lista*/
int contabilizarDisc(TListaDisc *topo)
{
	int cont = 0;
	while (topo != NULL)
	{
		cont++;
		topo = topo->seg;
	}
	return cont;
}

/*adiciona uma actividade de avalia��o na lista da n-�sima disciplina*/
void adicionarAvalDiscN(TAval aval, int nDisc, TListaDisc *topo)
{
	/*percorrer a lista de disciplinas at� � n-�sima*/
	while (topo != NULL && nDisc-- > 1)
		topo = topo->seg;
	/*verificar se a nota fica superior a 20.0 - nesse caso nao adiciona*/
	topo->disc->nota = calcularNota(topo->disc);
	if (topo->disc->nota + aval.nota > 20.0)
	{
		printf("*** A nota da UC ultrapassa os 20 valores - AA nao adicionada ***\n");
		return;
	}
	/*adicionar a actividade de avalia��o*/
	topo->disc->listaAval = adicionarTopoListaAval(aval, topo->disc->listaAval);
}

/*remove a m-�sima actividade de avalia��o na lista da n-�sima disciplina*/
void removerAvalMDiscN(int nAval, int nDisc, TListaDisc *topo)
{
	/*percorrer a lista de disciplinas at� � n-�sima*/
	while (topo != NULL && nDisc-- > 1)
		topo = topo->seg;
	/*remover a m-�sima actividade de avalia��o*/
	topo->disc->listaAval = removerAvalNListaAval(nAval, topo->disc->listaAval);
}

/*determina a posi��o da disciplina ou act. aval. correspondente ao id*/
void determinarNumDiscAval(int id, int *nDisc, int *nAval, TListaDisc *topo)
{
	TListaDisc *auxD;
	TListaAval *auxA;
	int somaAval = 0;
	/*contabilizar todas as avalia��es*/
	auxD = topo;
	while (auxD != NULL)
	{
		somaAval += contabilizarAval(auxD->disc->listaAval);
		auxD = auxD->seg;
	}
	/*se o id for inferior a 0 ou superior ao n�mero de id existentes*/
	if (id < 0 || id > contabilizarDisc(topo) + somaAval)
	{
		(*nDisc) = 0;
		(*nAval) = 0;
		return;
	}
	/*percorrer, contabilizando, as disc e suas act de aval, at� chegar id 1*/
	(*nDisc) = 0;
	auxD = topo;
	while (auxD != NULL)
	{
		if (id < 1)
			break;
		(*nDisc)++;
		id--;
		(*nAval) = 0;
		auxA = auxD->disc->listaAval;
		while (auxA != NULL)
		{
			if (id < 1)
				break;
			(*nAval)++;
			id--;
			auxA = auxA->seg;
		}
		auxD = auxD->seg;
	}
}

/*calcula o n�mero de UCs terminadas*/
int calcularDiscTerminadas(TListaDisc *topo)
{
	int discTer = 0;
	/*n�o atualizar notas porque foram atualizadas na fun��o que chama esta*/
	while (topo != NULL)
	{
		/*considerar >=0.9, porque as notas das disciplinas s�o � unidade*/
		if (topo->disc->nota > 9.9)
			discTer++;
		topo = topo->seg;
	}
	return discTer;
}

/*calcula a m�dia*/
double calcularMedia(TListaDisc *topo)
{
	TListaDisc *aux;
	double soma = 0.0;
	/*n�o atualizar notas porque foram atualizadas na fun��o que chama esta*/
	aux = topo;
	while (aux != NULL)
	{
		if (aux->disc->nota >= 10.0)
			soma += aux->disc->nota;
		aux = aux->seg;
	}
	return soma / calcularDiscTerminadas(topo);
}

/*calcula a distribui��o das classifica��es*/
void calcularDistribuicao(TListaDisc *topo, int *discNT, int *s, int *b,
		int *mB, int *e)
{
	/*n�o atualizar notas porque foram atualizadas na fun��o que chama esta*/
	while (topo != NULL)
	{
		if (topo->disc->nota <= 9.9)
			(*discNT)++;
		else if (topo->disc->nota < 13.9)
			(*s)++;
		else if (topo->disc->nota <= 15.9)
			(*b)++;
		else if (topo->disc->nota <= 17.9)
			(*mB)++;
		else
			(*e)++;
		topo = topo->seg;
	}
}

/*escreve informa��es sobre UCs terminadas, m�dia e se final = 1 dist. classif.*/
void mostrarRelatorio(TListaDisc *topo, int final)
{
	TListaDisc *aux;
	int discNT = 0, s = 0, b = 0, mB = 0, e = 0;
	/*atualizar primeiro as notas das disciplinas*/
	aux = topo;
	while (aux != NULL)
	{
		aux->disc->nota = calcularNota(aux->disc);
		aux = aux->seg;
	}
	/*se final = 0, escrever UCs terminadas e m�dia*/
	if (!final)
	{
		printf("UCs terminadas: %d\nMedia: %.0lf\n",
				calcularDiscTerminadas(topo), calcularMedia(topo));
	}
	/*se final = 1, escrever distibui��o de classifica��es*/
	else
	{
		calcularDistribuicao(topo, &discNT, &s, &b, &mB, &e);
		printf("Relatorio de Classificacoes:\n"
				"- Satisfaz: %d\n"
				"- Bom: %d\n"
				"- Muito Bom: %d\n"
				"- Excelente: %d\n", s, b, mB, e);
	}
	putchar('\n');
}

int main(int argc, char **argv)
{
	FILE *ficheiro;
	TListaDisc *topo = NULL, *auxD;
	TListaAval *auxA;
	TDisc disc;
	TAval aval;
	char str[MAXSTRING], *ptChar;
	double nota;
	int id, nDisc, nAval;
	/*verificar se o nome do ficheiro foi indicado*/
	if (argc != 2)
	{
		printf("Sintaxe: 1301913 <nome do ficheiro>\n");
		return EXIT_FAILURE;
	}
	/*abrir o ficheiro para leitura*/
	ficheiro = fopen(argv[1], "rt");
	if (ficheiro == NULL)
	{
		printf("Nao foi possivel abrir ficheiro para leitura\n");
		return EXIT_FAILURE;
	}
	/*ler dados do ficheiro*/
	while (!feof(ficheiro))
	{
		/*ler disciplinas at� a 1� linha em branco ou at� chegar ao fim*/
		while (fgets(str, MAXSTRING, ficheiro) != NULL)
		{
			if (!isalnum(str[0]))
				break;
			/*limpar o carater \n*/
			str[strlen(str) - 1] = '\0';
			disc = criarDisc(str);
			topo = adicionarBaseListaDisc(disc, topo);
		}
		auxD = topo;
		/*ler avalia��es*/
		while (fgets(str, MAXSTRING, ficheiro) != NULL)
		{
			if (!isalnum(str[0]))
			{
				auxD = auxD->seg;
				continue;
			}
			/*limpar o carater \n*/
			str[strlen(str) - 1] = '\0';
			nota = atof(str);
			ptChar = strchr(str, ' ');
			aval = criarAval(nota, &ptChar[1]);
			auxD->disc->listaAval = adicionarTopoListaAval(aval,
					auxD->disc->listaAval);
		}
	}
	fclose(ficheiro);
	/*mostrar dados ao utilizador*/
	mostrarListaDisc(topo);
	mostrarRelatorio(topo, 0);
	/*realizar opera��es*/
	do
	{
		printf("Indique uma de entre as tres seguintes operacoes\n"
				"Adicionar UC: + <nome>\n"
				"Adicionar AA a UC com <id>: +<id> <nota> <nome>\n"
				"Remover UC ou AA com id: -<id>: ");
		gets(str);
		if (str[0] == '+')
		{
			id = atoi(&str[1]);
			/*adicionar uma disciplina*/
			if (id == 0)
			{
				ptChar = strchr(str, ' ');
				if (ptChar == NULL)
					printf("\n*** Erro: Introduza uma sintaxe correta ***\n");
				else
				{
					disc = criarDisc(&ptChar[1]);
					topo = adicionarTopoListaDisc(disc, topo);
				}
			}
			else
			{
				determinarNumDiscAval(id, &nDisc, &nAval, topo);
				if ((nDisc == 0 && nAval == 0) || nAval > 0)
					printf("\n*** Erro: Introduza um id valido ***\n");
				/*adicionar atividade de avalia��o*/
				else if (nAval == 0)
				{
					ptChar = strchr(str, ' ');
					if (ptChar == NULL)
						printf("\n*** Erro: Introduza uma sintaxe correta ***\n");
					else
					{
						nota = atof(&ptChar[1]);
						ptChar = strchr(&ptChar[1], ' ');
						if (ptChar == NULL || nota <= 0.0 || nota > 20.0)
							printf("\n*** Erro: Introduza uma sintaxe correta ***\n");
						else
						{
							aval = criarAval(nota, &ptChar[1]);
							adicionarAvalDiscN(aval, nDisc, topo);
						}
					}
				}
			}
		}
		else if (str[0] == '-')
		{
			id = atoi(&str[1]);
			if (id == 0)
				printf("\n*** Erro: Introduza um id valido ***\n");
			else
			{
				determinarNumDiscAval(id, &nDisc, &nAval, topo);
				if (nDisc == 0 && nAval == 0)
					printf("\n*** Erro: Introduza um id valido ***\n");
				/*remover uma disciplina*/
				if (nDisc > 0 && nAval == 0)
					topo = removerDiscNListaDisc(topo, nDisc);
				/*remover uma atividade de avalia��o*/
				if (nDisc > 0 && nAval > 0)
					removerAvalMDiscN(nAval, nDisc, topo);
			}
		}
		else
			break;
		/*mostrar dados ao utilizador*/
		mostrarListaDisc(topo);
		mostrarRelatorio(topo, 0);
	} while (1);
	/*mostar relatorio final ao utilizador*/
	putchar('\n');
	mostrarRelatorio(topo, 1);
	/*abrir o ficheiro para escrita*/
	ficheiro = fopen(argv[1], "wt");
	if (ficheiro == NULL)
	{
		printf("Nao foi possivel abrir ficheiro para escrita\n");
		return EXIT_FAILURE;
	}
	/*gravar dados no ficheiro*/
	auxD = topo;
	/*gravar primeiro disciplinas*/
	while (auxD != NULL)
	{
		fprintf(ficheiro, "%s\n", auxD->disc->nome);
		auxD = auxD->seg;
	}
	auxD = topo;
	fprintf(ficheiro, "\n");
	/*depois de uma linha em branco gravar at. aval. de cada disciplina*/
	while (auxD != NULL)
	{
		auxA = auxD->disc->listaAval;
		while (auxA != NULL)
		{
			fprintf(ficheiro, "%.1lf %s\n", auxA->aval->nota, auxA->aval->nome);
			auxA = auxA->seg;
		}
		fprintf(ficheiro, "\n");
		auxD = auxD->seg;
	}
	fclose(ficheiro);
	removerListaDisc(topo);
	return EXIT_SUCCESS;
}

