/*
 *@autor Lucas Gariel e Yuri Algusto
 *@data 07/12/2019
 *@edição 14/12/2019
 *@bugs nenhum conhecido
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <locale.h>

typedef struct aluno aluno;

void frontend();
void frontTurma();
void seleciona();
void lerTurma();
void lerArquivo(char *arq, aluno *al, int *t);
void imprimeTurma(aluno *al, int t);
void criarTurma(char *arq, aluno *al);
void selectTurma(int n, char *na, aluno *al, int *t);
void salvar(char *arq, aluno *al, int t);
void estatisticas(aluno *al, int t);
void editAluno(aluno *al, int t);
void frontEdit();
int selectEdit(int n,aluno *al,int t);
void frontOpicao();
void selectOp(int c, aluno *al, int t);
void calculaSituacao(aluno *al, int t);
int buscaNome(aluno *al, int t);
int buscaMatricula(aluno *al, int t);
void histograma(int n[]);
void frontCad(aluno *al, int *t);

struct aluno{
	char nome[50];
	char matricula[12];
	int p[3];
	int t[2];
	int faltas;
	int notaFinal;
	char situacao;
};

int menor(const void *aa, const void *ab){
	const double *a = aa, *b = ab;
	if(*a<*b)return 1;
	else return 0;
}

int main(void){
	setlocale(LC_ALL, "Portuguese");
	seleciona();
	return 0;
}

void frontend() //função que devolve as opções de ação
{
	system("clear");
    printf("===================================================\n");
    printf("                Menu Principal                     \n");
    printf("===================================================\n");
    printf("Opções:\n\n");
    printf("1 - Ler turma de estudantes do arquivo\n");
    printf("2 - Imprimir turma de estudantes\n");
    printf("3 - Imprimir estatísticas da turma\n");
    printf("4 - Cadastrar novo estudante\n");
    printf("5 - Editar informações de um estudante\n");
    printf("6 - Reler turma de estudante do arquivo\n");
    printf("7 - Salvar e sair\n\n");
    printf("Digite uma opção: ");
} //frontend

void frontTurma()
{
	system("clear");
    printf("===================================================\n");
    printf("                    Ler Turma                      \n");
    printf("===================================================\n");
    printf("Opções:\n\n");
    printf("1 - Ler turma existente\n");
    printf("2 - Criar nova turma\n");
    printf("3 - Voltar ao menu principal\n\n");
    printf("Digite uma opção: ");
}

void seleciona(){
	aluno al[80];
	char na[20];
	int n, t;
	inicio:
	frontend();
	scanf("%d",&n);
	switch(n){
		case 1:
			lerTurma(na, al, &t);
			break;
			
		case 2:
		    imprimeTurma(al, t);
			break;
			
		case 3:
			estatisticas(al, t);
			break;
		case 4:
			frontCad(al, &t);
			break;
			
		case 5:
			editAluno(al,t);
			break;
			
		case 6:
			lerArquivo(na, al, &t);
			break;
			
		case 7:
			salvar(na, al, t);
			printf("Até a próxima.\n");
			return;
			break;
		default:
			printf("Número fora da faixa\n");
	} //switch case
	goto inicio;
	return;
} //seleciona

void selectTurma(int n,char *na, aluno *al, int *t){
	switch(n){
		case 1:
			printf("\nDigite o nome do arquivo com a turma: ");
			setbuf(stdin, NULL);
			scanf(" %s",na);
			lerArquivo(na, al, t);
			break;
			
		case 2:
			criarTurma(na, al);
			break;
			
		case 3:
			break;
		default:
			printf("Número fora da faixa.\nDigite uma opção valida ");
			scanf("%d",&n);
			selectTurma(n, na, al, t);
		} //switch case
} //selectTurma

void lerTurma(char *na, aluno *al, int *t){
	int n;
	do{	
		frontTurma();
		scanf("%d",&n);
		selectTurma(n, na, al, t);
	}while(n != 3);
} //lerTurma

void lerArquivo(char * arq, aluno *al, int *t) // função que le uma arquivi de alunos
{
	FILE *f;
	f = fopen(arq, "r");
	if(f == NULL){
		printf("Arquivo %s não existe!\n",arq);
		return;
	} //if
	printf("\nArquivo %s lido com sucesso!\n",arq);
	int j;
	for(j = 0; j < 80; j++){
		fgets(al[j].nome, 50, f);
		for(int i = 0; i < 50; i++){
			if(al[j].nome[i] == '\n'){
				al[j].nome[i] = '\0';
				break;
			} //if
		} //for
		fscanf(f, " %s", al[j].matricula);
		for(int i = 0; i < 3; i++){
			fscanf(f, "%d", &al[j].p[i]);
		} //for
		for(int i = 0; i < 2; i++){
			fscanf(f, "%d", &al[j].t[i]);
		} //for
		fscanf(f, "%d", &al[j].faltas);
		fscanf(f, "%d", &al[j].notaFinal);
		fscanf(f, " %c", &al[j].situacao);
		fgetc(f);
		if (feof(f)) break;
	} //while
	fclose(f);
	*t = j;
	printf("\nPrecione Enter para voltar ao Menu\n");
	setbuf(stdin,NULL);
	getchar();
} //lerArquivo

void criarTurma(char *arq, aluno *al) //função que cria um arquivo de alunos
{
	FILE *f;
	char r;
	printf("\nDigite o nome do arquivo para a nova turma: ");
	setbuf(stdin, NULL);
	scanf("%s", arq);
	f = fopen(arq,"r");
	if(f != NULL){
		printf("Arquivo %s já existe. Deseja sobrescrevê-lo? (S/N)",arq);
		setbuf(stdin,NULL);
		r = getchar();
		if(r != 'S' || r != 's'){
			return;
		} //if
	} //if
	f = fopen(arq,"w+");
	fclose(f);
	printf("\nPrecione Enter para voltar ao Menu\n");
	setbuf(stdin,NULL);
	getchar();
} //criarTurma

void imprimeTurma(aluno *al, int t) //função que lista os alunos 
{
	system("clear");
    printf("                             Estudantes                              \n");
    printf("---------------------------------------------------------------------------\n");
    printf("Nome                  Matrícula \tN1  N2  N3  T1  T2  Fal  Final Sit\n");
    printf("---------------------------------------------------------------------------\n");
    for (int i = 0; i < t; i++){
     printf("%-20s ", al[i].nome);
     printf("%-12s \t", al[i].matricula);
     printf("%3d ", al[i].p[0]);
     printf("%3d ", al[i].p[1]);
     printf("%3d ", al[i].p[2]);
     printf("%3d ", al[i].t[0]);
     printf("%3d ", al[i].t[1]);
     printf("%3d ", al[i].faltas);
     printf(" %4d  ", al[i].notaFinal);
     printf("  %c\n", al[i].situacao);
    } //for
    printf("---------------------------------------------------------------------------\n");
    printf("\nPrecione Enter para voltar ao Menu\n");
    setbuf(stdin, NULL);
    getchar();
} //imprimeTurma

void salvar(char *arq, aluno *al, int t) //função para salvar a lista de alunos
{
	FILE *f = fopen(arq, "r");
	char r;
	if(f == NULL){
		printf("O arquivo %s não existe. Deseja criar um novo? (S/N) ",arq);
		setbuf(stdin, NULL);
		r = getchar();
		if(r != 'S' && r != 's') return;
	} //if
	else {
		printf("Arquivo %s já existe. Deseja sobrescrevê-lo? (S/N) ",arq);
		setbuf(stdin, NULL);
		r = getchar();
		if(r != 'S' && r != 's') return;
		} //else
	fclose(f);
	f = fopen(arq, "w");
	for(int i = 0; i < t; i++){
		fprintf(f,"%s\n",al[i].nome);
		fprintf(f,"%s\n",al[i].matricula);
		for(int j = 0; j < 3; j++)fprintf(f,"%d\n",al[i].p[j]);
		for(int j = 0; j < 3; j++)fprintf(f,"%d\n",al[i].t[j]);
		fprintf(f,"%d\n",al[i].faltas);
		fprintf(f,"%d\n",al[i].notaFinal);
		fprintf(f,"%c\n",al[i].situacao);
	} //for
} //salvar

void estatisticas(aluno *al, int t) //função que imprime desempenho da turma
{
	system("clear");	
	int mx[6] = {al[0].p[0], al[0].p[1], al[0].p[2], al[0].t[0],
		al[0].t[1], al[0].notaFinal};
	int mn[6] = {al[0].p[0], al[0].p[1], al[0].p[2], al[0].t[0],
		al[0].t[1], al[0].notaFinal};
	double med[6] = {al[0].p[0], al[0].p[1], al[0].p[2], al[0].t[0],
		al[0].t[1], al[0].notaFinal};
	double p1[80] = {al[0].p[0]}, p2[80] = {al[0].p[1]}, p3[80] = {al[0].p[2]};
	double t1[80] = {al[0].t[0]}, t2[80] = {al[0].t[1]}, fn[80] = {al[0].notaFinal};
	int ap = 0, rep = 0, rf = 0;
	int vn[10] = {0};
	if(al[0].situacao == 'A')ap++;
	else if(al[0].situacao == 'F')rf++;
	else rep++;
		
	if(al[0].notaFinal > 90)vn[9]++;
	else if(al[0].notaFinal > 80)vn[8]++;
	else if(al[0].notaFinal > 70)vn[7]++;
	else if(al[0].notaFinal > 60)vn[6]++;
	else if(al[0].notaFinal > 50)vn[5]++;
	else if(al[0].notaFinal > 40)vn[4]++;
	else if(al[0].notaFinal > 30)vn[3]++;
	else if(al[0].notaFinal > 20)vn[2]++;
	else if(al[0].notaFinal > 10)vn[1]++;
	else vn[0]++;
		
	for(int i = 1; i < t; i++){
		p1[i] = al[i].p[i]; p2[i] = al[i].p[1]; p3[i] = al[i].p[2];
		t1[i] = al[i].t[i]; t2[i] = al[i].t[1]; fn[i] = al[i].notaFinal;
		
		mx[0] = fmax(mx[0], al[i].p[0]);
		mx[1] = fmax(mx[1], al[i].p[1]);
		mx[2] = fmax(mx[2], al[i].p[2]);
		mx[3] = fmax(mx[3], al[i].t[0]);
		mx[4] = fmax(mx[4], al[i].t[1]);
		mx[5] = fmax(mx[5], al[i].notaFinal);
		
		mn[0] = fmin(mn[0], al[i].p[0]);
		mn[1] = fmin(mn[1], al[i].p[1]);
		mn[2] = fmin(mn[2], al[i].p[2]);
		mn[3] = fmin(mn[3], al[i].t[0]);
		mn[4] = fmin(mn[4], al[i].t[1]);
		mn[5] = fmin(mn[5], al[i].notaFinal);
		
		med[0] = (med[0] + al[i].p[0])/2.0;
		med[1] = (med[1] + al[i].p[1])/2.0;
		med[2] = (med[2] + al[i].p[2])/2.0;
		med[3] = (med[3] + al[i].t[0])/2.0;
		med[4] = (med[4] + al[i].t[1])/2.0;
		med[5] = (med[5] + al[i].notaFinal)/2.0;  
		
		if(al[i].notaFinal > 90)vn[9]++;
		else if(al[i].notaFinal > 80)vn[8]++;
		else if(al[i].notaFinal > 70)vn[7]++;
		else if(al[i].notaFinal > 60)vn[6]++;
		else if(al[i].notaFinal > 50)vn[5]++;
		else if(al[i].notaFinal > 40)vn[4]++;
		else if(al[i].notaFinal > 30)vn[3]++;
		else if(al[i].notaFinal > 20)vn[2]++;
		else if(al[i].notaFinal > 10)vn[1]++;
		else vn[0]++;
		
		if(al[i].situacao == 'A')ap++;
		else if(al[i].situacao == 'F')rf++;
		else rep++;
	} //for i
	qsort(p1,t,sizeof(double),menor);
	qsort(p2,t,sizeof(double),menor);
	qsort(p3,t,sizeof(double),menor);
	qsort(t1,t,sizeof(double),menor);
	qsort(t2,t,sizeof(double),menor);
	qsort(fn,t,sizeof(double),menor);
	printf("	                Estatı́sticas\n");
	printf("----------------------------------------------------------------\n");
	printf("                                   N1   N2   N3   T1   T2  Final\n");
	printf("----------------------------------------------------------------\n");
	printf("Maiores notas da turma           ");
	for(int i = 0; i < 6; i++){
		printf("%3d  ",mx[i]);
	} //for max
	printf("\n");
	printf("Menores notas da turma           ");
	for(int i = 0; i < 6; i++){
		printf("%3d  ",mn[i]);
	} //for min
	printf("\n");
	printf("Notas médias da turma           ");
	for(int i = 0; i < 6; i++){
		printf("%4.1lf ",med[i]);
	} //for med
	printf("\n");
	printf("Notas medianas da turma         ");
	if(t%2 != 0){
		printf("%4.1lf %4.1lf %4.1lf %4.1lf %4.1lf %4.1lf\n",
			p1[t/2+1],p2[t/2+1],p3[t/2+1],t1[t/2+1],t2[t/2+1],fn[t/2+1]);
	} //if
	else{
		printf("%4.1lf %4.1lf %4.1lf %4.1lf %4.1lf %4.1lf\n",
			(p1[t/2]+p1[t/2+1])/2.0,(p2[t/2]+p2[t/2+1])/2.0,(p3[t/2]+p3[t/2+1])/2.0,
			(t1[t/2]+t1[t/2+1])/2.0,(t2[t/2]+t2[t/2+1])/2.0,(fn[t/2]+fn[t/2+1])/2.0);
	} //else
	printf("----------------------------------------------------------------\n");
	printf("\nNúmero de estudantes aprovados:\t\t\t%d\n",ap);
	printf("Número de estudantes reprovados:\t\t%d\n",rep);
	printf("Número de estudantes reprovados por falta:\t%d\n",rf);
	histograma(vn);
	printf("\nPrecione Enter para voltar ao Menu\n");
	setbuf(stdin,NULL);
	getchar();
} // estatisticas


void frontCad(aluno *al, int *t){
    system("clear");
    printf("=========================================\n");
	printf("        Cadastrar Novo Estudante         \n");
	printf("=========================================\n\n");
    printf("Digite o nome : ");
    setbuf(stdin,NULL);
    fgets(al[*t].nome, 50, stdin);
    for(int i = 0; i < 50; i++){
		if(al[*t].nome[i] == '\n')al[*t].nome[i] = '\0';
	} //for
    printf("Digite a matrícula:");
    setbuf(stdin, NULL);
    scanf(" %s", al[*t].matricula);
    printf("Digite as três notas das provas: ");
    scanf("%d%d%d",&al[*t].p[0],&al[*t].p[1],&al[*t].p[2]);
    printf("Digite as duas notas dos trabalhos: ");
    scanf("%d%d",&al[*t].t[0],&al[*t].t[1]);
    printf("Digite o número de faltas: ");
    scanf("%d",&al[*t].faltas);
    (*t)++;
    printf("Nota final calculada: %d\n",al[(*t)-1].notaFinal);
	calculaSituacao(al, *t);
    printf("Situação final: %c\n", al[(*t)-1].situacao);
    printf("\nPrecione Enter para voltar para o Menu\n");
    setbuf(stdin, NULL);
    getchar();
}

void editAluno(aluno *al, int t) //função principal para editar um registro
{
	int n, c;
	do{
		frontEdit();
		scanf("%d", &n);
		if(n != 3){
			c = selectEdit(n, al, t);
			if(c == -1){
				printf("ERRO!\n");
				printf("Precione Enter para voltar para o Menu\n");
				setbuf(stdin,NULL);
				getchar();
				return;
			}
			selectOp(c, al, t);
		} //if
	}while(n != 3);
} //editAluno

void frontEdit() //menu de seleçao de busca
{
	system("clear");
	printf("================================================\n");
	printf("        Editar Informações do Estudante         \n");
	printf("================================================\n\n");
    printf("Opções:\n\n");
    printf("1 - Selecionar por número de matrícula\n");
    printf("2 - Selecionar por nome\n");
    printf("3 - Voltar ao menu principal\n\n");
    printf("Digite uma opção: ");
}//frontEdit

void frontOpicao() //menu de ediçao
{
	system("clear");
	printf("================================================\n");
	printf("        Editar Informações do Estudante         \n");
	printf("================================================\n\n");
    printf("Opções:\n\n");
    printf("1 - Alterar nome\n");
    printf("2 - Alterar matrícula\n");
    printf("3 - Alterar nota da prova 1\n");
    printf("4 - Alterar nota da prova 2\n");
    printf("5 - Alterar nota da prova 3\n");
    printf("6 - Alterar nota do trabalho 1\n");
    printf("7 - Alterar nota do trabalho 2\n");
    printf("8 - Alterar número de faltas\n");
    printf("9 - Voltar ao menu anterior\n\n");
    printf("Digite uma opção: ");
}

int selectEdit(int n,aluno *al,int t){
	switch(n){
		case 1:
			return buscaMatricula(al, t);
			break;
		case 2:
			return buscaNome(al, t);
			break;
		case 3:
			break;
	} //switch case
	return -1;
} //selectEdit

int buscaMatricula(aluno *al, int t) //busca por matriculas
{
	int p = -1;
	char mat[12];
	printf("\nDigite o número de matricula: \n");
	setbuf(stdin, NULL);
	scanf(" %s", mat);
	for(int i = 0; i < t; i++){
		if(strcmp(mat, al[i].matricula) == 0)p = i;
	} //for
	return p;
} //buscaMatricula

int buscaNome(aluno *al, int t) //busca por nome
{
	int p = -1;
	char nome[50];
	printf("\nDigite o nome do aluno:\n");
	setbuf(stdin, NULL);
	fgets(nome, 50, stdin);
	for(int i = 0; i < 50; i++){
				if(nome[i] == '\n')nome[i] = '\0';
			} //for
	for(int i = 0; i < t; i++){
		if(strcmp(nome, al[i].nome) == 0)p = i;
	} //for
	return p;
} //buscaNome

void selectOp(int c, aluno *al, int t){
	int m;
	inicio:
	frontOpicao();
	scanf("%d",&m);
	switch(m){
		case 9:
			goto inicio;
			break;
		case 1:
			printf("digite o nome:\n");
			setbuf(stdin,NULL);
			fgets(al[c].nome, 50, stdin);
			for(int i = 0; i < 50; i++){
				if(al[c].nome[i] == '\n')al[c].nome[i] = '\0';
			} //for
			break;
		case 2:
		  printf("Digite a matricula:\n");
		  setbuf(stdin,NULL);
		  scanf(" %s", al[c].matricula);
			break;
		case 3:
			printf("Digite a nota da prova 1\n");
			scanf("%d",&al[c].p[0]);
			break;
		case 4:
			printf("Digite a nota da prova 2\n");
			scanf("%d",&al[c].p[1]);
			break;
		case 5:
			printf("Digite a nota da prova 3\n");
			scanf("%d",&al[c].p[2]);
			break;
		case 6:
			printf("Digite a nota do trabalho 1\n");
			scanf("%d",&al[c].t[0]);
			break;
		case 7:
			printf("Digite a nota do trabalho 2\n");
			scanf("%d",&al[c].t[1]);
			break;
		case 8:
			printf("Digite o número de faltas\n");
			scanf("%d",&al[c].faltas);
			break;
	} //switch case
	calculaSituacao(al, t);
}  //selectOp

void calculaSituacao(aluno *al, int t) //função que calacula nota funal e aprovação dos um aluno
{
	for(int i = 0; i < t; i++){
		int nf = al[i].p[0]+al[i].p[1]+al[i].p[2]+al[i].t[0]+al[i].t[1];
		al[i].notaFinal = nf;
		if(al[i].faltas > 18)
			al[i].situacao = 'F';
		else if(nf >= 60)
			al[i].situacao = 'A';
		else 
			al[i].situacao = 'R';
	} //for
} //calculaSituação

void histograma(int n[]) //printa grafico  de desempenho
{
  printf("\n 0 - 10   %d", n[0]);
  for(int i = 0;i<n[0];i++)printf("*");
  printf("\n11 - 20   %d ", n[1]);
  for(int i = 0;i<n[1];i++)printf("*");
  printf("\n21 - 30   %d ",n[2]);
  for(int i = 0;i<n[2];i++)printf("*");
  printf("\n31 - 40   %d ",n[3]);
  for(int i = 0;i<n[3];i++)printf("*");
  printf("\n40 - 50   %d ",n[4]);
  for(int i = 0;i<n[4];i++)printf("*");
  printf("\n51 - 60   %d ",n[5]);
  for(int i = 0;i<n[5];i++)printf("*");
  printf("\n61 - 70   %d ",n[6]);
  for(int i = 0;i<n[6];i++)printf("*");
  printf("\n71 - 80   %d ",n[7]);
  for(int i = 0;i<n[7];i++)printf("*");
  printf("\n81 - 90   %d ",n[8]);
  for(int i = 0;i<n[8];i++)printf("*");
  printf("\n91 - 100  %d ",n[9]);
  for(int i = 0;i<n[9];i++)printf("*");
  printf("\n");
} //histograma
