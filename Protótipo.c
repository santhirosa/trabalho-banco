#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#define NMAX 100

typedef struct
{
    int num;
    char nome[30];
    float saldo;
    float investido;
}t_cliente;

t_cliente Cliente[NMAX];