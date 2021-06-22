/*@Autor: Lucas Gabriel e Yuri Augusto
 *@Data: 23/11/2019
 *@Ediçao: 01/12/2019
 *@Bugs: função de chromakey infuncional, se errar o caminho para a imagem a segunda tentativa irá falhar.
 */

#include <stdio.h>
#include <stdlib.h>

#include "pmn.h"

#define TAM 50
#define MAX 1000
#define volta() printf("\nDigite uma tecla para voltar para o menu principal\n"); setbuf(stdin,NULL); getchar();

void frontend();
void carregaimg(unsigned char mg[MAX][MAX][3], unsigned int *lin,
	unsigned int *col, unsigned int *m);
void cortaImg(unsigned int l,unsigned int c,
	unsigned int m, unsigned char img[l][c][3]);
void invertimg(int l, int c, int m, unsigned char img[l][c][3]);
void printbrilho(int l, int c, unsigned char img[l][c][3]);
void chromakey(unsigned int l,unsigned int c,
	unsigned int m, const unsigned char img[l][c][3]);
/*
void ocultarimg();
void desocultarimg();
*/
void ocultartxt(unsigned int l,unsigned int c,
	unsigned int m, const unsigned char img[l][c][3]);
void desocultartxt();
void seleciona(int n);
void lerString(char* str);

void escreveResp(unsigned int l,unsigned int c,
	unsigned int m, unsigned char img[l][c][3]);

int main(void)
{
	int n;
	while(1){
		frontend();
		scanf("%d",&n);
		if(n==0)break;
		seleciona(n);
	} //while
	return 0;
} //main

void frontend()
{
	system("clear");
	printf("*********************************************************\n");
	printf("*       P R O C E S S A D O R  D E  I M A G E N S       *\n");
	printf("*********************************************************\n\n");
	printf("Opções:\n\n");
	printf("1 - carregar uma imagem de fundo\n");
	printf("2 - recortar parte da imagem de fundo\n");
	printf("3 - inverter a imagem\n");
	printf("4 - imprimir o histograma de brilho médio\n");
	printf("5 - realizar um chromakey com a imagem de fundo\n");
	printf("6 - ocultar uma imagem na imagem de fundo\n");
	printf("7 - revelar a imagem oculta inserida em uma imagem\n");
	printf("8 - inserir texto na imagem de fundo\n");
	printf("9 - recuperar texto inserido em uma imagem\n");
	printf("0 - sair\n\n");
	printf("Digite uma opção: ");
} //frontend

void seleciona(int n) //função para selecionar outras funções
{
	unsigned char img[MAX][MAX][3];
	unsigned int l, c, m;
	switch(n){
		case 1:
			printf("\nDigite o caminho para a imagem: ");
			carregaimg(img, &l, &c, &m);
			volta();
			break;
		case 2:
			cortaImg(l, c, m, img);
			volta();
			break;
		case 3:
			invertimg(l, c, m, img);
			volta();
			break;
		case 4:
			printbrilho(l, c, img);
			volta();
			break;
		case 5:
			chromakey(l, c, m, img);
			volta();
			break;
		case 6:
			volta();
			break;
		case 7:
			volta();
			break;
		case 8:
			ocultartxt(l, c, m, img);
			volta();
			break;
		case 9:
			desocultartxt();
			volta();
			break;
		default:
			printf("\nNúmero fora da faixa!\n");
	} //switch case
} //select

void carregaimg(unsigned char img[MAX][MAX][3],unsigned int *lin, 
	unsigned int *col,unsigned int *m) // função para carregar uma imagem
{
	FILE *f;
	char raiz[TAM];
	unsigned int l,c, max;
	lerString(raiz);
	f = fopen(raiz,"rt");
	if(f == NULL){
		printf("A imagem %s não pode ser aberta.\n", raiz);
		printf("\nDigite novamente o caminho da imagem desejada: ");
		carregaimg(img, lin, col, m);
	} //if
	else printf("\nA imagem %s foi carregada com sucesso.\n", raiz);
	leia_cabecalho(f, &l, &c, &max);
	leia_imagem(f, l, c, max, img);
	fclose(f);
	*col = c;
	*lin = l;
	*m = max;
} //carregaimg

void lerString(char *str) //Função para ler uma String.
{
	setbuf(stdin, NULL);
	if(fgets(str,TAM,stdin) == NULL){
		fprintf(stderr,"Erro ao ler a origem da imagem\n");
		exit(1);
	} //if
	for(int i = 0; i<TAM;i++){
		if(str[i] == '\n')str[i] = '\0';
	} //for
} //lerString

void cortaImg(unsigned int l, unsigned int c, unsigned int m,
	unsigned char img[l][c][3]) //função para cortar uma imagem
	{
	unsigned int x = 0, y = 0, largura = 0, altura = 0;
    printf("Digite a coordenada x do ponto de corte: ");
    scanf("%ud", &x);
    printf("Digite a coordenada y do ponto de corte: ");
    scanf("%ud", &y);
    printf("Digite a altura do retângulo de corte: ");
    scanf("%ud", &altura);
    printf("Digite a largura do retângulo de corte: ");
    scanf("%ud", &largura);
    unsigned char recorte[altura+1][largura+1][3];
    unsigned int i, j;
    if(altura - x > l || largura - y > c)
      printf("ERRO\n");
    else{
      for(i = 0; i<=altura; i++)
	for(j = 0; j<=largura; j++){
	  recorte[i][j][0] = img[i+altura][j+largura][0]; 
	  recorte[i][j][1] = img[i+altura][j+largura][1];
	  recorte[i][j][2] = img[i+altura][j+largura][2];
	}
      escreveResp(altura+1, largura+1, m, recorte);
	} //else
} //cortaImg

void invertimg(int l, int c, int m, unsigned char img[l][c][3]) //função que inverte as cores das imagens
{
	unsigned char neg[l][c][3];
	int i, j, k;

	for (i = 0; i < l; i++)
	{
		for (j = 0; j < c; j++)
		{
			for (k = 0; k < 3; k++)
				neg[i][j][k] = 255 - img[i][j][k];
		} //for j
	} //for i
	escreveResp(l, c, m, neg);
} //invertimg

void printbrilho(int l, int c, unsigned char img[l][c][3]) // função que printa estograma de brilho
{
	int hist[256] = {0};
	int brilho;
	int i, j;

	for (i = 0; i < l; i++)
	{
		for (j = 0; j < c; j++)
		{
			brilho = (img[i][j][0] + img[i][j][1] + img[i][j][2]) / 3; 
			hist[brilho]++;
		} // for j
	} // for i
	for (i = 0; i < 256; i++)
	{
		printf("%3d ", i);
		printf("%3d\n", hist[i]);
	}
} //printbrilho

void chromakey(unsigned int l,unsigned int c,
	       unsigned int m, const unsigned char img[l][c][3]) //Função que faz chromakey de imagem (infuncional)
{
  unsigned char fImg[MAX][MAX][3], cImg[l][c][3];
  unsigned int fl, fc, fm;
  printf("Digite o caminho da imagem da frente: ");
  carregaimg(fImg, &fl, &fc, &fm);
  for(unsigned int i = 0; i < l; i++){ //carregar imagem de fundo
    for(unsigned int j = 0; j < c; j++){
      for(int k = 0; k < 3; k++){
		cImg[i][j][k] = img[i][j][k];
      } //for k
    } //for j < c
  } //for i < l*/
  int ctr = (fImg[0][0][0] + fImg[0][0][1] + fImg[0][0][2])/3;
	for(unsigned int i = 0; i < fl || i < l; i++){ //carrega img da frente 
		for(unsigned int j = 0; j < fc || j < c; j++){
			if(((fImg[i][j][0] + fImg[i][j][1] + fImg[i][j][2]) / 3 )!= ctr){
				for(int k = 0; k < 3; k++){
					cImg[i][j][k] = fImg[i][j][k];
				} //for k
			} //if
		} //for j
	  } //for i
  escreveResp(l, c, m, cImg);
} //chromakey

void ocultartxt(unsigned int l,unsigned int c,
				unsigned int m, const unsigned char img[l][c][3]){
	unsigned char cImg[l][c][3];
	char msg[TAM];
	printf("Digite o texto a ser ocultado:\n");
	setbuf(stdin,NULL);
	fgets(msg,TAM,stdin);
	for(int i = 0;i < TAM; i++){
		if(msg[i]=='\n'){
			msg[i]='\0';
			break;
		} //if
	}//for i
	int ct = 0, p = 0, f = 1;
	for(unsigned int i = 0; i < l; i++){
		for(unsigned int j = 0; j < c; j++){
			for(int k = 0; k < 3; k++){
				cImg[i][j][k] = img[i][j][k];
				cImg[i][j][k] = (cImg[i][j][k]>>4)<<4;
				if(msg[p]=='\0' && f == 1){ 
					f = 0;
					if(ct % 2 == 0){
						cImg[i][j][k] += (msg[p]>>4);
						ct++;
					} //if ct
					else{
						cImg[i][j][k] += (msg[p]<<4)>>4;
						p++;
						ct++;
					} //else
				} //if msg == \0
				if(f == 1){ 
					if(ct % 2 == 0){
						cImg[i][j][k] += (msg[p]>>4);
						ct++;
					} // if ct
					else{
						cImg[i][j][k] += (msg[p]<<4)>>4;
						p++;
						ct++;
					} // else
				} // if f==1
			} //for k
		} //for j < c
	} //for i < l
	escreveResp(l, c, m, cImg);
} //ocultartxt

void desocultartxt(){ //função pra remover texto da imagem
	printf("Digite um caminho para a imagem com o texto ocultado nela: ");
	unsigned char img[MAX][MAX][3];
	unsigned int l, c, m;
	char msg[TAM] = {'\0'};
	int ct = 0, p = 0;
	carregaimg(img, &l, &c, &m);
	for(unsigned int i = 0; i < l; i++){
		for(unsigned int j = 0; j < c; j++){
			if(ct % 2 == 0){
				msg[p] = (img[i][j][0]<<4) + (img[i][j][1]<<4)>>4;
				if(msg[p] == '\0')goto sai;
				p++;
				msg[p] = img[i][j][2]<<4;
			} //if
			else {
				msg[p] += (img[i][j][0]<<4)>>4;
				if(msg[p] == '\0')goto sai;
				p++;
				msg[p] = (img[i][j][1]<<4) + (img[i][j][2]<<4)>>4;
				if(msg[p] == '\0')goto sai;
				p++;
			} //else
		} //for j < c
	} //for i < l
	sai:
	printf("%s\n",msg);
 } // desocultatxt

void escreveResp(unsigned int l,unsigned int c,
		 unsigned int m, unsigned char img[l][c][3]){
  char nm[20];
  printf("Digite um caminho para a imagem resultante: ");
  setbuf(stdin, NULL);
  scanf("%s",nm);
  if (escreve_imagem(nm, l, c, m, img) == false)
    printf("A imagem %s não está no formato PPM P3\n", nm);
  else
    printf("A imagem %s foi escrita.\n\n", nm);
} //escreveResp
