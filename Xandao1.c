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

// declaracao das funcoes usadas //
int menu_1(void);
int menu_2(void);
int realiza_cadastro(t_cliente*);
void salva_principal(t_cliente*, int);
t_cliente* login(t_cliente*, int);
int encontra_conta(char*, t_cliente*, int);
void atualizar_arquivo(t_cliente*,int);
void realiza_saque(t_cliente*);
void realiza_deposito(t_cliente*);
void realizar_pix(t_cliente*,t_cliente*,int);

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

// menu da conta //
int menu_2()
{
    int flag=1;
    char word[5];

    printf("==============================\n");
    printf("1. Consultar saldo\n");
    printf("2. Imprimir extrato\n");
    printf("3. Fazer pagamento (PIX)\n");
    printf("4. Saque\n");
    printf("5. Deposito\n");
    printf("6. Sair da conta\n");
    printf("==============================\n");

    while(flag)
    {
        printf("Selecione a opcao desejada: ");
        fgets(word, 5, stdin);

        switch(word[0])
        case '1' ... '6' : flag=0;

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
    int flag=1, flag_t=0, k=0;
    char cpf[20], senha[40];
    char cod_tmp[10];

    // pegar os dados do novo cliente //
    printf("Bem vindo ao SuperBank!\n");
    printf("Digite seu nome completo: ");
    fgets(cliente[i].nome, 80, stdin);
    k = (strlen(cliente[i].nome)-1);
    if(cliente[i].nome[k]=='\n') cliente[i].nome[k]='\0';
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
    k = (strlen(cpf)-1);
    if(cliente[i].cpf[k]=='\n') cliente[i].cpf[k]='\0';
    while(1)
    {
        printf("Digite uma senha para sua conta (max. 30 caracteres): ");
        fgets(senha,40,stdin);
        k = (strlen(senha)-1);
        if(senha[k]=='\n') senha[k]='\0';

        if(strlen(senha)>30)
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
void salva_principal(cliente, qtd_clientes)
t_cliente *cliente;
int qtd_clientes;
{
    FILE *fp;
    fp=fopen("arq_principal.txt", "wt");

    for(int i=0;i<qtd_clientes;i++)
    {
        fprintf(fp,"%s\n",cliente[i].nome);
        fprintf(fp,"%s\n",cliente[i].cpf);
        fprintf(fp,"%s\n",cliente[i].codigo);
        fprintf(fp,"%s\n",cliente[i].senha);
        fprintf(fp,"%lf\n",cliente[i].saldo);
    }
    fclose(fp);
}

// funcao para ENTRAR //
t_cliente* login(cliente, qtd_clientes)
t_cliente *cliente;
int qtd_clientes;
{
    char cod_tmp[10], ver_senha[40];
    int ind_tmp=0, flag=1, k=0;
    while(flag)
    {
        printf("Digite o codigo de sua conta: ");
        fgets(cod_tmp,10,stdin);
        k = (strlen(cod_tmp)-1);
        if(cod_tmp[k]=='\n') cod_tmp[k]='\0';

        ind_tmp = encontra_conta(cod_tmp, cliente, qtd_clientes);
        if(ind_tmp == -1)
        {
            printf("Conta nao encontrada.\n");
        }
        else
        {
            flag=0;
        }
    }

    flag=1;
    while(flag)
    {
        printf("Digite sua senha: ");
        fgets(ver_senha,40,stdin);
        k = (strlen(ver_senha)-1);
        if(ver_senha[k]=='\n') ver_senha[k]='\0';

        if(strcmp(ver_senha,cliente[ind_tmp].senha))
        {
            printf("Senha incorreta.");
        }
        else
        {
            printf("Bem vindo, %s!\n", cliente[ind_tmp].nome);
            flag=0;
        }
    }

    return &cliente[ind_tmp];
}

// funcao para encontrar uma conta //
int encontra_conta(codigo, cliente, qtd_clientes)
char codigo[10];
t_cliente* cliente;
int qtd_clientes;
{
    for(int i=0;i<qtd_clientes;i++)
        {
            if(!strcmp(codigo,cliente[i].codigo))
            {
                return i;
            }
            if(i == qtd_clientes-1)
                break;
        }
    return -1;
}

//funcao para atualizar o arquivo com novos dados//

void atualizar_arquivo(cliente,contador)
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

// realizar um saque //
void realiza_saque(logado)
t_cliente* logado;
{
    int flag=1;
    char tmp_char[80];
    double tmp_lf;

    printf("Qual o valor a ser sacado? ");
    while(flag)
    {
        fgets(tmp_char,80,stdin);
        tmp_lf = atof(tmp_char);

        if(logado->saldo > tmp_lf)
        {
            logado->saldo-=tmp_lf;
            printf("Saque realizado com sucesso.\n");
            printf("Seu saldo atual e de %.2lf\n", logado->saldo);
            flag=0;
        }
        else
        {
            printf("Nao e possivel sacar essa quantia.\n");
            printf("Seu saldo atual e de %.2lf\n", logado->saldo);
            printf("Deseja inserir outro valor [s/n]? ");
            fgets(tmp_char,5,stdin);
            if(tmp_char[0] == 's') continue;
            else flag=0;
        }
    }
}

// realizar um deposito //
void realiza_deposito(logado)
t_cliente *logado;
{
    char tmp_char[80];
    char caracter;
    double tmp_lf;
    char senha[40];
    int flag=1;

    printf("Qual o valor a ser depositado? ");
    printf("\n Digite sua senha para confirmar o deposito"); fflush(stdin); fgets(senha,40,stdin);
    while(flag)
    {
        if(!strcmp(senha,logado->senha))
        {
            fgets(tmp_char,80,stdin);
            tmp_lf = atof(tmp_char);
            logado->saldo+=tmp_lf;
            printf("Deposito realizado com sucesso.\n");
            printf("Seu saldo atual e de %.2lf\n", logado->saldo);

        }
        else
        {
            printf("\n Senha incorreta!");
            printf("Deseja tentar novamente[s/n]:"); fflush(stdin); scanf("%c",&caracter);
            if(caracter=='n') flag=0;
            if(caracter=='s') continue;
        }
    }
}

// realizar pix //
void realizar_pix(logado, cliente,contador)
t_cliente *logado, *cliente; int contador;
{
    int flag=1;
    int k;
    char cpf[20],senha[40];
    char temp_char;
    double valor;
    printf("\n\t Digite a chave cpf para a qual deseja fazer o pix:"); fflush(stdin); fgets(cpf,20,stdin);
    while(flag)
    {
        printf("\n Digite o valor do pix:"); scanf("%lf",&valor);
        if(valor<=logado->saldo) flag=0;
        else printf("\nSaldo insuficiente![usuario possui R$%lf]",logado->saldo);
    }
    flag=1;
    for(k=0;k<contador;k++)
    {
        if(!strcpy(cliente[k].cpf,cpf))
        {
            while(flag)
            {
                printf("\n Digite sua senha para confirmar o pix"); fflush(stdin); fgets(senha,40,stdin);
                if(!strcmp(senha,logado->senha))
                {
                    printf("\nPix efetuado!");
                    logado->saldo=logado->saldo-valor;
                    printf("Seu saldo atual e de %.2lf\n", logado->saldo);
                    cliente[k].saldo= cliente[k].saldo + valor;
                    flag=0;

                }
                else
                {
                    printf("\n Senha incorreta! Deseja tentar novamente?[s/n]:"); fflush(stdin); scanf("%c",&temp_char);
                    if(temp_char=='n') flag=0;
                    if(temp_char=='s') continue;
                }
            }
            break;
        }
    }
    if(flag==1) printf("\nCPF não encontrado no sistema");
}



int main()
{
    // variaveis de acesso restrito ao banco //
    int retorno=0, continuar=1, cont=0;
    int continuar_1=0;
    t_cliente *lista, *logado;
    lista = (t_cliente*)calloc(sizeof(t_cliente),NUM_CLIENTES);

    while(continuar)
    {
        // apresenta o menu inicial //
        retorno = menu_1();
        if(retorno == 1)
        {
            logado = login(lista, cont);
            continuar_1=1;                  // ao logar, o cliente tem acesso ao menu da conta! //
        }
        if(retorno == 2)
        {
            cont=realiza_cadastro(lista);
        }
        if(retorno == 3) exit(-1);

        while(continuar_1)
        {
            // apresenta o menu da conta //
            retorno = menu_2();
            if(retorno == 1)        // Saldo //
            {
                printf("Seu saldo atual: %.2lf.\n", logado->saldo);
            }
            if(retorno == 2)        // Extrato //
            {

            }
            if(retorno == 3)        // PIX //
            {
                realizar_pix(logado,lista,cont);
                atualizar_arquivo(lista,cont);
            }
            if(retorno == 4)        // Saque //
            {
                realiza_saque(logado);
                atualizar_arquivo(lista,cont);
            }
            if(retorno == 5)        // Deposito //
            {
                realiza_deposito(logado);
                atualizar_arquivo(lista,cont);
            }
            if(retorno == 6)        // Sair da conta //
            {
                continuar_1=0;
            }
            salva_principal(lista, cont);
        }
        salva_principal(lista, cont);
    }

    return 0;
}
