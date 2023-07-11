#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define NUM_CLIENTES 10

// declaracao das structs usadas //
typedef struct
    {
        char nome[80];
        char cpf[20];
        char senha[40];
        char codigo[10];
        double saldo;
    } t_cliente;
typedef struct 
    {
        char id[10];
        char tipo[30];
        double selic;
    }t_acao;

typedef struct
    {
        int dia;
        int mes;
        int ano;
    }t_data;

typedef struct 
    { char codigo[10];
        int numero;
        double entrada;
        t_data entrada;
        t_data retirada;
        double retorno;
        double lucro;
    }t_investimento;

// declaracao das funcoes usadas //
int menu_1(void);
int realiza_cadastro(t_cliente*);
void salva_principal(t_cliente*);
void atualiza_arquivo(t_cliente*,int);
void fazer_pix(t_cliente*,t_cliente*,int);
void fazer_saque(t_cliente*,t_cliente*,int);

// funcao para escolher entrar/cadastrar-se //
int menu_1()
{
    int flag=1;
    char word[5];

    printf("==============================\n");
    printf("1. Entrar\n");
    printf("2. Cadastrar-se\n");
    printf("3. Sair\n");
    printf("==============================\n");

    while(flag)
    {
        printf("Selecione a opcao desejada: ");
        fgets(word, 5, stdin);

        switch(word[0])
        case '1' ... '3' : flag=0;

        if(strlen(word) != 2) flag=1;

        if(flag) printf("Opcao invalida.\n");
    }

    return atoi(word);
}

// funcao para CADASTRAR-SE, devolve a qtd de clientes ja cadastrados //
int realiza_cadastro(cliente)
t_cliente *cliente;
{
    static int i=0;
    srand(time(NULL));
    int flag=1, flag_t=0;
    char cpf[20], senha[40];
    char cod_tmp[10];

    // pegar os dados do novo cliente //
    printf("Bem vindo ao SuperBank!\n");
    printf("Digite seu nome completo: ");
    fgets(cliente[i].nome, 80, stdin);
    while(flag)
    {
        printf("Digite seu CPF: ");
        fgets(cpf, 20, stdin);

        for(int k=0;k+1<strlen(cpf);k++)
        {
            switch(cpf[k])
            case '0' ... '9' : flag_t=1;
            if(flag_t == 1)
            {
                flag_t=0;
                flag=0;
                continue;
            }
            else
            {
                flag=1;
                break;
            };
        }
        if(strlen(cpf) != 12) flag=1;

        if(flag) printf("CPF invalido.\n");
    }
    strcpy(cliente[i].cpf, cpf);
    while(1)
    {
        printf("Digite uma senha para sua conta (max. 30 caracteres): ");
        fgets(senha,40,stdin);
        if(strlen(senha)>31)
        {
            printf("Senha invalida.\n");
            continue;
        }
        else
        {
            strcpy(cliente[i].senha, senha);
            break;
        }
    }
    cliente[i].saldo=0.0;

    printf("Sua conta foi criada com sucesso!\n");

    // criar um codigo para a nova conta //
    flag=1;
    while(flag)
    {
        flag_t=1;

        cod_tmp[0] = ((rand()%26)+97);
        cod_tmp[1] = ((rand()%26)+97);
        cod_tmp[2] = ((rand()%26)+97);
        cod_tmp[3] = ((rand()%10)+48);
        cod_tmp[4] = ((rand()%10)+48);
        cod_tmp[5] = '\0';

        // conferir se o codigo ja existe //
        flag_t=1;
        for(int k=0;k<i;k++)
        {
            if(!strcmp(cod_tmp,cliente[k].codigo))
            {
                flag_t=0;
                break;
            }
        }

            if(flag_t)
            {
                strcpy(cliente[i].codigo,cod_tmp);
                flag=0;
            }
    }

    printf("O codigo da sua conta e %s.\n", cliente[i].codigo);
    printf("Anote este codigo, pois e com ele que voce tera acesso a sua conta!\n");

    i++;
    return i;
}

// funcao para salvar os novos clientes na base de dados do banco //
void salva_principal(cliente)
t_cliente *cliente;
{
    int static j=0;
    FILE *fp;
    if(j==0)
        fp=fopen("arq_principal.txt", "wt");
    else
        fp=fopen("arq_principal.txt", "a");

    fprintf(fp,"%s",cliente->nome);
    fprintf(fp,"%s",cliente->cpf);
    fprintf(fp,"%s\n",cliente->codigo);
    fprintf(fp,"%s",cliente->senha);
    fprintf(fp,"%lf\n",cliente->saldo);

    fclose(fp);
    j++;
}

void atualiza_arquivo(cliente,contador)
t_cliente *cliente; int contador;
{
    int j;
    FILE *fp;
    fp=fopen("arq_principal.txt","w");
    if(fp==NULL) printf("\nNao foi possivel salvar as alteracoes[erro de abrtura do arquivo]");
    else 
    {
        for(j=0;j<contador;j++)
        {
            fprintf(fp,"%s",cliente[j].nome);
            fprintf(fp,"%s",cliente[j].cpf);
            fprintf(fp,"%s\n",cliente[j].codigo);
            fprintf(fp,"%s",cliente[j].senha);
            fprintf(fp,"%lf\n",cliente[j].saldo);
        }
    }

}

void fazer_pix(logado, cliente,contador)
t_cliente *logado, *cliente; int contador;
{
    int cont=contador;
    int flag=1;
    int k;
    char cpf[20];
    double valor;
    printf("\n\t Digite a chave cpf para a qual deseja fazer o pix:"); fflush(stdin); fgets(cpf,20,stdin);
    for(k=0;k<cont;k++)
    {
        if(!strcpy(logado->cpf,cliente[k].cpf))
        {
            printf("\n Digite o valor do pix:"); scanf("%lf",&valor);
            cliente[k].saldo=cliente[k].saldo+valor;
            logado->saldo=logado->saldo-valor;
            flag=0;
            break;
        }
    }
    if(flag==1)
    {
        printf("\nCPF destinatário não encontrado no sistema");
    }
    atualiza_arquivo(cliente,cont);
}

void fazer_transferencia(logado, cliente, contador)
t_cliente *logado, *cliente; int contador;
{
    int cont=contador;
    int flag=1;
    int k;
    char codigo[20],senha[40];
    double valor;
    printf("\n\t Digite o codigo da conta para a qual deseja fazer o transferimento"); fflush(stdin); fgets(codigo,20,stdin);
    while(flag)
    {
        printf("\n Digite o valor do pix:"); scanf("%lf",&valor);
        if(valor<=logado->saldo) flag=0;
        else printf("\nSaldo insuficiente!");
    }
    flag=1;
    for(k=0;k<contador;k++)
    {
        if(!strcpy(logado->cpf,cliente[k].cpf))
        {
            while(flag)
            {
                printf("\n Digite sua senha para confirmar o saque"); fflush(stdin); fgets(senha,40,stdin);
                if(!strcmp(senha,logado->senha))
                {
                    printf("\nSaque efetuado!");
                    flag=0;
                    logado->saldo=logado->saldo-valor;
                    atualizar_arquivo(cliente,cont);
                }
                else printf("\nSenha incorreta![Tente Novamente]");
            }
        }
    }
}
void fazer_saque(logado,cliente,contador)
t_cliente *logado, *cliente; int contador;
{
    int cont=contador;
    int flag=1;
    char senha[40];
    double valor;
    while(flag)
    {
        printf("\n Digite o valor a ser sacado:"); fflush(stdin); scanf("%lf",valor);
        if(valor<=logado->saldo) flag=0;
        else printf("\nSaldo insuficiente!");
    }
    flag=1;
    while(flag)
    {
        printf("\n Digite sua senha para confirmar o saque"); fflush(stdin); fgets(senha,40,stdin);
        if(!strcmp(senha,logado->senha))
        {
            printf("\nSaque efetuado!");
            flag=0;
            logado->saldo=logado->saldo-valor;
            atualizar_arquivo(cliente,cont);
        }
        else printf("\nSenha incorreta![Tente Novamente]");
    }
}

int main()
{
    // variaveis de acesso restrito ao banco //
    int retorno=0, continuar=1, cont=0;
    t_cliente *lista, logado;
    lista = (t_cliente*)calloc(sizeof(t_cliente),NUM_CLIENTES);

    while(continuar)
    {
        // apresenta o menu inicial //
        retorno = menu_1();
        if(retorno == 1)
        {

        }
        if(retorno == 2)
        {
            cont=realiza_cadastro(lista);
            salva_principal(&lista[cont-1]);
        }
        if(retorno == 3) exit(-1);
    }

    return 0;
}