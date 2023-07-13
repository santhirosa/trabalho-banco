#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define NUM_CLIENTES 4
#define TAXA_POUPANCA 0.06 // taxa da poupanca por ano//

// declaracao das structs usadas //

typedef struct
{
        int dia;
        int mes;
        int ano;
}t_data; //struct que descreve uma data//

typedef struct
{
    t_data ultimo_acesso;
    double total;
}t_poupanca; // struct da poupança

typedef struct
    {
        char nome[80];
        char cpf[20];
        char senha[40];
        char codigo[10];
        double saldo;
        t_poupanca poupanca;
    } t_cliente;

struct tm *Data_atual; //struct para inserimento da data atual

// declaracao das funcoes e subrotinas usadas //
int menu_1(void);
int menu_2(void);
int realiza_cadastro(t_cliente*);
void recebe_arq_principal(t_cliente*);
void salva_principal(t_cliente*, int);
t_cliente* login(t_cliente*, int);
int encontra_conta_cod(char*, t_cliente*, int);
int encontra_conta_cpf(char*, t_cliente*, int);
void realiza_saque(t_cliente*);
void realiza_deposito(t_cliente*);
void realiza_pix(t_cliente*, t_cliente*, int);

// declaracao de funções relacionadas a poupanca

int menu_poupanca();
void receber_arqpoupanca(t_cliente*);
void salvar_arqpoupanca(t_cliente*);
double calcula_poupanca(t_cliente,double);
void colocar_na_poupanca(t_cliente*);
void receber_da_poupanca(t_cliente*);

//Subrrotinas auxiliares para limpeza de tela//
void cls(HANDLE);
void pausa(void);

//Subrrotinas auxiliares para trabalho com datas//
void define_data_atual(t_data*);
double diferenca_entre_datas(t_data,t_data);

// subrotina para limpar tela (criada pelo FOsorio) //
void cls(HANDLE hConsole)
{
    COORD coordScreen = { 0, 0 };    // home for the cursor
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire screen with blanks.
    if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer
                                    (TCHAR)' ',      // Character to write to the buffer
                                    dwConSize,       // Number of cells to write
                                    coordScreen,     // Coordinates of first cell
                                    &cCharsWritten)) // Receive number of characters written
    {
        return;
    }

    // Get the current text attribute.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    // Set the buffer's attributes accordingly.
    if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer
                                    csbi.wAttributes, // Character attributes to use
                                    dwConSize,        // Number of cells to set attribute
                                    coordScreen,      // Coordinates of first cell
                                    &cCharsWritten))  // Receive number of characters written
    {
        return;
    }

    // Put the cursor at its home coordinates.
    SetConsoleCursorPosition(hConsole, coordScreen);
}

// subrotina para pausar o programa por um instante //
void pausa()
{
    char plvr[5];
    fgets(plvr,3,stdin);
}

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

    cls(GetStdHandle(STD_OUTPUT_HANDLE));

    return atoi(word);
}

// funcao para o menu da conta //
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
    printf("6. Poupanca\n");
    printf("7. Sair da conta\n");
    printf("==============================\n");

    while(flag)
    {
        printf("Selecione a opcao desejada: ");
        fgets(word, 5, stdin);

        switch(word[0])
        case '1' ... '7' : flag=0;

        if(strlen(word) != 2) flag=1;

        if(flag) printf("Opcao invalida.\n");
    }

    cls(GetStdHandle(STD_OUTPUT_HANDLE));

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
    printf("Bem vindo(a) ao SuperBank!\n");
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
    cliente[i].poupanca.total=0.0;

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

    pausa();

    cls(GetStdHandle(STD_OUTPUT_HANDLE));
    i++;
    return i;
}
//subrotina para salvar no vetor lista de clientes os clientes ja cadastrados//

void recebe_arq_principal(cliente)
t_cliente *cliente;
{
    FILE *fp;
    int i; 
    fp=fopen("arq_principal.txt", "at");
    fclose(fp);

    fp=fopen("arq_principal.txt", "rt");
    if(fp==NULL)
    {
        printf("\n Impossivel ler o arquivo de clientes");
        exit(0);
    }
    i=0;
    while(!feof(fp))
    {
        fscanf(fp,"%s",cliente[i].nome);
        fscanf(fp,"%s",cliente[i].cpf);
        fscanf(fp,"%s",cliente[i].codigo);
        fscanf(fp,"%s",cliente[i].senha);
        fscanf(fp,"%lf",&cliente[i].saldo);
        i++;
    }
    fclose(fp);
}

// subrotina para salvar os novos clientes na base de dados do banco //
void salva_principal(cliente, qtd_clientes)
t_cliente *cliente;
int qtd_clientes;
{
    FILE *fp;
    fp=fopen("arq_principal.txt", "wt");

    for(int i=0;i<NUM_CLIENTES;i++)
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

        ind_tmp = encontra_conta_cod(cod_tmp, cliente, qtd_clientes);
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
            printf("Senha incorreta.\n");
        }
        else
        {
            cls(GetStdHandle(STD_OUTPUT_HANDLE));
            printf("Bem vindo(a), %s!\n", cliente[ind_tmp].nome);
            flag=0;
        }
    }

    return &cliente[ind_tmp];
}

// funcao para encontrar uma conta pelo codigo //
int encontra_conta_cod(codigo, cliente, qtd_clientes)
char codigo[10];
t_cliente* cliente;
int qtd_clientes;
{
    for(int i=0;i<NUM_CLIENTES;i++)
        {
            if(!strcmp(codigo,cliente[i].codigo))
            {
                return i;
            }
            if(i == NUM_CLIENTES-1)
                break;
        }
    return -1;
}

// funcao para encontrar uma conta pelo CPF //
int encontra_conta_cpf(cpf, cliente, qtd_clientes)
char cpf[20];
t_cliente* cliente;
int qtd_clientes;
{
    for(int i=0;i<qtd_clientes;i++)
        {
            if(!strcmp(cpf,cliente[i].cpf))
            {
                return i;
            }
            if(i == qtd_clientes-1)
                break;
        }
    return -1;
}

// subrotina para realizar um saque //
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

        if(logado->saldo >= tmp_lf)
        {
            printf("Digite sua senha para confirmar: ");
            int flag_1=1;
            while(flag_1)
            {
                fgets(tmp_char,80,stdin);
                tmp_char[strlen(tmp_char)-1]='\0';
                if(!strcmp(tmp_char,logado->senha))
                {
                    logado->saldo-=tmp_lf;
                    printf("Saque realizado com sucesso.\n");
                    flag_1=0;
                }
                else
                {
                    printf("Senha incorreta, digite novamente.\n");
                }
            }

            printf("Seu saldo atual e de %.2lf\n", logado->saldo);
            flag=0;
        }
        else
        {
            printf("Nao e possivel sacar essa quantia.\n");
            printf("Seu saldo atual e de %.2lf\n", logado->saldo);
            printf("Deseja inserir outro valor (s/n)? ");
            fgets(tmp_char,5,stdin);
            if(tmp_char[0] == 's') continue;
            else flag=0;
        }
        pausa();
        cls(GetStdHandle(STD_OUTPUT_HANDLE));
    }
}

// subrotina para realizar um deposito //
void realiza_deposito(logado)
t_cliente *logado;
{
    char tmp_char[80];
    double tmp_lf;

    printf("Qual o valor a ser depositado? ");
    fgets(tmp_char,80,stdin);
    tmp_lf = atof(tmp_char);
    logado->saldo+=tmp_lf;
    printf("Deposito realizado com sucesso.\n");
    printf("Seu saldo atual e de %.2lf\n", logado->saldo);

    pausa();
    cls(GetStdHandle(STD_OUTPUT_HANDLE));
}

// subrotina para realizar um PIX //
void realiza_pix(logado, cliente, qtd_clientes)
t_cliente *logado, *cliente;
int qtd_clientes;
{
    char cpf[20], tmp_char[80];
    int indice, flag=1, k=0;
    double tmp_lf;

    printf("Digite a chave CPF para a qual deseja fazer o pix: ");
    while(flag)
    {
        fgets(cpf,20,stdin);
        k = (strlen(cpf)-1);
        if(cpf[k]=='\n') cpf[k]='\0';

        indice = encontra_conta_cpf(cpf, cliente, qtd_clientes);
        if(indice == -1)
        {
            printf("Conta nao encontrada.\n");
            printf("Digite novamente: ");
        }
        else
        {
            flag=0;
        }
    }

    flag=1;
    printf("Digite o valor do PIX: ");
    while(flag)
    {
        fgets(tmp_char,80,stdin);
        tmp_lf = atof(tmp_char);

        if(logado->saldo >= tmp_lf && tmp_lf>0.0)
        {
            printf("Digite sua senha para confirmar: ");
            int flag_1=1;
            while(flag_1)
            {
                fgets(tmp_char,80,stdin);
                tmp_char[strlen(tmp_char)-1]='\0';
                if(!strcmp(tmp_char,logado->senha))
                {
                    logado->saldo-=tmp_lf;
                    cliente[indice].saldo+=tmp_lf;

                    printf("PIX efetuado com sucesso.\n");
                    flag_1=0;
                }
                else
                {
                    printf("Senha incorreta, digite novamente.\n");
                }
            }

            printf("Seu saldo atual e de %.2lf\n", logado->saldo);
            flag=0;
            pausa();
        }
        else
        {
            printf("Nao e possivel enviar essa quantia.\n");
            printf("Seu saldo atual e de %.2lf\n", logado->saldo);
            printf("Deseja inserir outro valor (s/n)? ");
            fgets(tmp_char,5,stdin);
            if(tmp_char[0] == 's') continue;
            else flag=0;
        }
        cls(GetStdHandle(STD_OUTPUT_HANDLE));
    }

}
//subrotina que define a data atual no momento de acesso do sistema//
void define_data_atual(data)
t_data *data;
{
    time_t segundos;
    time(&segundos);
    Data_atual = localtime(&segundos);
    data->dia=Data_atual->tm_mday;
    data->mes=Data_atual->tm_mon+1;
    data->ano=Data_atual->tm_year+1900;
}
//calcula diferença entre datas em anos//
double diferenca_entre_datas(data1,data2)
t_data data1, data2;
{
    struct tm data_1,data_2;
    time_t diff;
    memset( &data_1, 0 , sizeof(data_1));
    memset( &data_2, 0 , sizeof(data_2));

    data_1.tm_mday=data1.dia;
    data_1.tm_mon=data1.mes-1;
    data_1.tm_year=data1.ano-1900;
    data_2.tm_mday=data2.dia;
    data_2.tm_mon=data2.mes-1;
    data_2.tm_year=data2.ano-1900;

    diff=difftime(mktime(&data_2),mktime(&data_2));
    return(diff/(365*24*60*60));
}

// Menu relacionado a poupanca
int menu_poupanca()
{
    int comando;
    printf("\n==============================\n");
    printf("1. Depositar quantia na poupanca \n2. Sacar investimento\n3. Voltar ao menu principal\n");
    printf("Comando:"); scanf("%d",&comando);
    printf("\n==============================\n");
    cls(GetStdHandle(STD_OUTPUT_HANDLE));
    return(comando);
}
//Recebe poupancas do arquivo//
void receber_arqpoupanca(cliente)
t_cliente *cliente;
{
    FILE *fp;
    int i;
    char tmp_char[30];
    fp=fopen("poupanca.txt","at");
    fclose(fp);

    fp=fopen("poupanca.txt","rt");
    if(fp==NULL)
    {
        printf("\n IMPOSSIVEL ABRIR ARQUIVO DE POUPANCAS!");
        exit(0);
    }
    i=0;
    while(!feof(fp))
    {
        fscanf(fp,"%s",tmp_char);
        fscanf(fp,"%lf",&cliente[i].poupanca.total);
        fscanf(fp,"%d/%d/%d",&cliente[i].poupanca.ultimo_acesso.dia,&cliente[i].poupanca.ultimo_acesso.mes,&cliente[i].poupanca.ultimo_acesso.ano);
        i++;
    }
    fclose(fp);
}
//Salva poupanca de cada cliente em um arquivo//
void salva_arqpoupanca(cliente)
t_cliente *cliente;
{
    FILE *fp;
    int i;
    fp=fopen("poupanca.txt","wt");
    if(fp==NULL)
    {
        printf("\n IMPOSSIVEL ABRIR ARQUIVO DE POUPANCAS!");
        exit(0);
    }
    for(i=0;i<NUM_CLIENTES;i++)
    {
        fprintf(fp,"%s\n",cliente[i].codigo);
        fprintf(fp,"%lf\n",cliente[i].poupanca.total);
        fprintf(fp,"%d/%d/%d\n",cliente[i].poupanca.ultimo_acesso.dia,cliente[i].poupanca.ultimo_acesso.mes,cliente[i].poupanca.ultimo_acesso.ano);
    }
    fclose(fp);
}

//Calcular o saldo total atual da poupanca//
double calcula_poupanca(logado,tempo)
t_cliente logado; double tempo;
{
    double novo_total;
    printf("\nSaldo anterior da poupanca em [%d/%d/%d]:%lf",logado.poupanca.ultimo_acesso.dia,logado.poupanca.ultimo_acesso.mes,logado.poupanca.ultimo_acesso.ano,logado.poupanca.total);
    novo_total=(double)(logado.poupanca.total*(pow((1+TAXA_POUPANCA),tempo)));
    return(novo_total);
}
//Depositar quantia na poupança//
void colocar_na_poupanca(logado)
t_cliente*logado;
{
    int continuar=1;
    char tmp_char[10];
    char c;
    double valor;
    while(continuar)
    {
        printf("\n Valor a ser investido:"); fflush(stdin); fgets(tmp_char,10,stdin);
        valor=atof(tmp_char);
        if(valor<=logado->saldo) continuar=0;
        else printf("\n Seu saldo[R$%lf] é insuficiente, digite um valor disponivel em sua conta!",logado->saldo);
    }
    logado->saldo-=valor;
    logado->poupanca.total+=valor;
    printf("\nSaldo atual:%lf\nValor total na poupanca:%lf",logado->saldo,logado->poupanca.total);
    printf("\nPressione uma tecla para continuar:"); scanf("%c",&c);
    cls(GetStdHandle(STD_OUTPUT_HANDLE));
}
//Receber um valor da poupanca//
void receber_da_poupanca(logado)
t_cliente*logado;
{
    int continuar=1;
    char tmp_char[10];
    char c;
    double valor;
    while(continuar)
    {
        printf("\n Valor a ser recebido:"); fflush(stdin); fgets(tmp_char,10,stdin);
        valor=atof(tmp_char);
        if(valor<=logado->poupanca.total) continuar=0;
        else printf("\n Valor total da poupancaq [R$%lf] insuficiente, digite um valor disponivel na poupanca!",logado->poupanca.total);
    }
    logado->saldo+=valor;
    logado->poupanca.total-=valor;
    printf("\nSaldo atual:%lf\nValor total na poupanca:%lf",logado->saldo,logado->poupanca.total);
    printf("\nPressione uma tecla para continuar:"); scanf("%c",&c);
    cls(GetStdHandle(STD_OUTPUT_HANDLE));
}


int main()
{
    // variaveis de acesso restrito ao banco //
    int retorno=0, continuar=1, cont=0;
    int continuar_1=0;
    t_cliente *lista, *logado;
    lista = (t_cliente*)calloc(sizeof(t_cliente),NUM_CLIENTES);

    //poupanca//
    int prosseguir;
    int comando;
    t_data data_hoje;
    double tempo; //em anos//
    
    recebe_arq_principal(lista);
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
                pausa();
            }
            if(retorno == 2)        // Extrato //
            {

            }
            if(retorno == 3)        // PIX //
            {
                realiza_pix(logado, lista, cont);
            }
            if(retorno == 4)        // Saque //
            {
                realiza_saque(logado);
            }
            if(retorno == 5)        // Deposito //
            {
                realiza_deposito(logado);
            }
            if(retorno == 6)
            {
                prosseguir=1;
                define_data_atual(&data_hoje);
                receber_arqpoupanca(lista);
                tempo=diferenca_entre_datas(logado->poupanca.ultimo_acesso,data_hoje);
                logado->poupanca.total=calcula_poupanca(*logado,tempo);
                logado->poupanca.ultimo_acesso=data_hoje;
                while(prosseguir)
                {
                    printf("\n\tSenhor[a] %s tem %lf acumulado em sua poupanca hoje[%d/%d/%d]",logado->nome,logado->poupanca.total,logado->poupanca.ultimo_acesso.dia,logado->poupanca.ultimo_acesso.mes,logado->poupanca.ultimo_acesso.ano);
                    comando=menu_poupanca();
                    if(comando==1)
                    {
                        colocar_na_poupanca(logado);
                    }
                    if(comando==2)
                    {
                        receber_da_poupanca(logado);
                    }
                    if(comando==3)
                    {
                        prosseguir=0;
                    }
                    salva_arqpoupanca(lista);
                }
            }
            if(retorno == 7)        // Sair da conta //
            {
                continuar_1=0;
            }
            salva_principal(lista, cont);
        }
        salva_principal(lista, cont);
    }

    return 0;
}
