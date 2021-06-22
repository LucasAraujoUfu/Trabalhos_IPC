/*
 *@autor: Lucas Gabriel, Yuri Algusto.
 *@data: 27 de outubro de 2019.
 *@edição: 4 de novenbro de 2019.
 *@bugs: Nenhum conhecido.
*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#define TAM 50

void frontend();
void seleciona(int opcao);
int str_int(char *num);
void invert(char *num, char *inv);
int bin_dec(char *num);
int hex_dec(char *num);
void dec_bin(int d, char *res);
void dec_hex(int d, char *res);
void bin_hex(char *num, char *res);
void hex_bin(char *num, char *res);
double fbin_dec(char *num);
void fdec_bin(double f, char *res);
void soma_bin(char *num, char *num1, char *res);
void repet(int opcao);
void ler(int o, char *num, double *f);
void imprimir(int o, char *num, char *res, int d, double f, double r);
void ler_9(char *num, char *num1);
void imprime_9(char *num, char *num1, char *res);

int main()
{
    setlocale(LC_ALL, "Portuguese");
    int opcao;
    while (1)
    {
        frontend();
        scanf("%d", &opcao);
        if (opcao == 0)
            break;
        else
            seleciona(opcao);
    } //while
    return 0;
} //main

void frontend()
{
    printf("****************************************\n");
    printf("*         C o n v e r s o r            *\n");
    printf("****************************************\n\n");
    printf("Opções:\n\n");
    printf("1 - converter de binário para decimal\n");
    printf("2 - converter de hexadecimal para decimal\n");
    printf("3 - converter de decimal para binário\n");
    printf("4 - converter de decimal para hexadecimal\n");
    printf("5 - converter de binário para hexadecimal\n");
    printf("6 - converter de hexadecimal para binário\n");
    printf("7 - converter de fração binária para decimal\n");
    printf("8 - converter de fração decimal para binário\n");
    printf("9 - somar dois binários sem sinal\n");
    printf("0 - sair\n\n");
    printf("Digite uma opção: ");

} // void frontend

void seleciona(int opcao) //função para selecionar outras funções
{
    int d = 0;
    double f = 0, r = 0;
    char num[TAM];
    char res[TAM];
    if (opcao != 9)
        ler(opcao, num, &f);
    if (opcao == 1)
    {
        d = bin_dec(num);
    } //if
    else if (opcao == 2)
    {
        d = hex_dec(num);
    } //else if
    else if (opcao == 3)
    {
        d = str_int(num);
        dec_bin(d, res);
    } //else if
    else if (opcao == 4)
    {
        d = str_int(num);
        dec_hex(d, res);
    } //else if
    else if (opcao == 5)
    {
        bin_hex(num, res);
    } //else if
    else if (opcao == 6)
    {
        hex_bin(num, res);
    } //else if
    else if (opcao == 7)
    {
        r = fbin_dec(num);
    }
    else if (opcao == 8)
	{
		fdec_bin(f, res);
	} //else if
    else if (opcao == 9)
    {
        char num1[TAM];
        ler_9(num, num1);
        soma_bin(num, num1, res);
        imprime_9(num, num1, res);
    } //else if
    if (opcao != 9)
        imprimir(opcao, num, res, d, f, r);
    repet(opcao);
    return;
} // void seleciona

int str_int(char *num) //função que converte string para decimal
{
    int d = 0;
    for (int i = 0; i < strlen(num); i++)
    {
        d += (int)(num[i] - 48) * (pow(10, strlen(num) - i - 1));
    } //for
    return d;
}

void invert(char *num, char *inv) //função que inverte string
{
    int i;
    int j = strlen(num) - 1;
    for (i = 0; i < strlen(num); i++)
    {
        inv[i] = num[j];
        j--;
    } //for
    inv[i] = '\0';
    return;
} //invert

int bin_dec(char *num) //função para converter binario para decimal
{
    int d = 0;
    for (int i = 0; i < strlen(num); i++)
    {
        if (num[i] == '1')
            d += pow(2, strlen(num) - (i + 1));
    } //for
    return d;
} //bin_dec

int hex_dec(char *num) //função para converter hexadecimal para decimal
{
    int d = 0;
    for (int i = 0; i < strlen(num); i++)
    {
        num[i] = toupper(num[i]);
        if (num[i] > 64)
        {
            d += (int)(num[i] - 55) * pow(16, strlen(num) - i - 1);
        } //if
        else
            d += (int)(num[i] - 48) * pow(16, strlen(num) - i - 1);
    } //for
    return d;
} //hex_dec

void dec_bin(int d, char *res) //função para converter decimal para binário
{
    char temp[TAM];
    int j = 0;
    while (1)
    {
        if (d == 1 || d == 0)
        {
            temp[j] = (char)d + 48;
            break;
        } //if
        temp[j] = (char)(d % 2) + 48;
        d /= 2;
        j++;
    } //while
    temp[j + 1] = '\0';
    invert(temp, res);
    return;
} //dec_bin

void dec_hex(int d, char *res) //função que converte decimal em hexadecimal
{
    char temp[TAM];
    int j = 0;
    while (1)
    {
        if (d < 16)
        {
            if (d >= 10)
                temp[j] = (char)d + 55;
            else
                temp[j] = (char)d + 48;
            break;
        } //if
        if (d % 16 >= 10)
        {
            temp[j] = (char)(d % 16) + 55;
        } //if
        else
        {
            temp[j] = (char)(d % 16) + 48;
        } //else
        d /= 16;
        j++;
    } //while
    temp[j + 1] = '\0';
    invert(temp, res);
} //dec_hex

void bin_hex(char *num, char *res) //função que converte binário para hexadecimal
{
    int d = bin_dec(num);
    dec_hex(d, res);
} //bin_hex

void hex_bin(char *num, char *res) //função que converte hexadecimal para binário
{
    int d = hex_dec(num);
    dec_bin(d, res);
} //hex_bin

double fbin_dec(char *num) //função que converte fração binaria em real
{
    int i;
    char numI[10];
    for (i = 0; i < strlen(num); i++)
    {
        if (num[i] == '.' || num[i] == ',')
            break;
        numI[i] = num[i];
    } //for
    int d = bin_dec(numI);
    double f = 0;
    numI[i] = '\0';
    i++;
    int p = -1;
    while (i < strlen(num))
    {
        if (num[i] == '1')
            f += pow(2,p);
        p--;
        i++;
    } //while
    return d+f;
} //fbin_dec

void fdec_bin(double f, char *res) //função que converte real em binario
{
	char resi[TAM], resd[42] = {','};
	dec_bin((int)floor(f), resi); 
	double r = f - floor(f);
	for(int i = 1; i <= 40; i++)
	{
		r*=2;
		resd[i] = (char)trunc(r) + 48;
		r -= floor(r);
	} //for
	resd[41] = '\0';
	strcat(resi,resd);
	strcpy(res,resi);
} //fdec_bin

void soma_bin(char *num, char *num1, char *res) //função que soma dois binários sem sinal
{
    dec_bin((bin_dec(num) + bin_dec(num1)), res);
}

void repet(int opcao) //função para converter novamente
{
    if (opcao == 1 || opcao == 5)
    {
        printf("Deseja converter outro binario (s/n)? ");
    } //if
    else if (opcao == 2 || opcao == 6)
    {
        printf("Deseja converter outro hexadecimal (s/n)? ");
    } //else if
    else if (opcao == 3 || opcao == 4)
    {
        printf("Deseja converter outro natural (s/n)? ");
    } //else if
    else if (opcao == 7)
    {
        printf("Deseja converter outro binário fracionário? (s/n)? ");
    } //else if
    else if (opcao == 8)
    {
        printf("Deseja converter outro real (s/n)? ");
    } //else if
    else if (opcao == 9)
    {
        printf("Deseja somar outros binários (s/n)? ");
    } //else if
    char op;
    setbuf(stdin, NULL);
    scanf(" %c", &op);
    if (op == 's' || op == 'S')
    {
        seleciona(opcao);
    } //if
    return;
} //repet

void ler(int o, char *num, double *f) //função para ler o número a ser convertido
{
    if (o == 1 || o == 5)
    {
        printf("\nDigite um binário: ");
    } //if
    else if (o == 2 || o == 6)
    {
        printf("\nDigite um hexadecimal: ");
    } //else if
    else if (o == 3 || o == 4)
    {
        printf("\nDigite um natural: ");
    } //else if
    else if (o == 7)
    {
        printf("\nDigite um binário fracionário: ");
    } //else if
    else if (o == 8)
    {
        printf("\nDigite um real: ");
        scanf("%lf",f);
    } //else if
    setbuf(stdin, NULL);
    if(o != 8)scanf(" %s", num);
} // void ler

void imprimir(int o, char *num, char *res, int d, double f, double r) //função para printar a resposta na tela
{
    printf("\n");
    if (o == 1)
    {
        printf("%s_(2) = %d_(10)\n", num, d);
    } //if
    else if (o == 2)
    {
        printf("%s_(16) = %d_(10)\n", num, d);
    } //else if
    else if (o == 3)
    {
        printf("%s_(10) = %s_(2)\n", num, res);
    } //else if
    else if (o == 4)
    {
        printf("%s_(10) = %s_(16)\n", num, res);
    } //else if
    else if (o == 5)
    {
        printf("%s_(2) = %s_(16)\n", num, res);
    } //else if
    else if (o == 6)
    {
        printf("%s_(16) = %s_(2)\n", num, res);
    } //else if
    else if (o == 7)
    {
        printf("%s_(2) = %lf_(10)\n", num, r);
    } //else if
    else if (o == 8)
    {
        printf("%lf_(10) = %s_(2)\n", f, res);
    } //else if
    printf("\n");
} // void imprimir

void ler_9(char *num, char *num1) //função para ler dois numeros binários
{
    printf("\nDigite o primeiro binário: ");
    scanf(" %s", num);
    printf("\n\nDigite o segundo binário: ");
    scanf(" %s", num1);
} //ler_9

void imprime_9(char *num, char *num1, char *res) //imprime a soma de binários com formatação
{
    printf("%10s\t\t%10d\n", num, bin_dec(num));
    printf("%10s\t\t%10d\n", num1, bin_dec(num1));
    printf("-----------\t\t-----------\n");
    printf("%10s\t\t%10d\n\n", res, bin_dec(num) + bin_dec(num1));
} //imprime_9
