/* Aluno: Leonardo de Souza Gomes - Matrícula: 21.2.4054 - Turma 31*/
//----------------[Bibliotecas]
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//----------------[Cores e Tabelas]


#define ANSI_RESET            "\x1b[0m"  // desativa os efeitos anteriores
#define ANSI_BOLD             "\x1b[1m"  // coloca o texto em negrito
#define ANSI_COLOR_RED        "\x1b[31m"
#define ANSI_COLOR_GREEN      "\x1b[32m"
#define ANSI_COLOR_YELLOW     "\x1b[33m"
#define ANSI_COLOR_BLUE       "\x1b[34m"

#define ANSI_COLOR_CYAN       "\x1b[36m"

// macros para facilitar o uso
#define BOLD(string)       ANSI_BOLD             string ANSI_RESET
#define BLUE(string)       ANSI_COLOR_BLUE       string ANSI_RESET
#define RED(string)        ANSI_COLOR_RED        string ANSI_RESET
#define GREEN(string)      ANSI_COLOR_GREEN      string ANSI_RESET
#define YELLOW(string)     ANSI_COLOR_YELLOW     string ANSI_RESET
#define BLUE(string)       ANSI_COLOR_BLUE       string ANSI_RESET
#define CYAN(string)       ANSI_COLOR_CYAN       string ANSI_RESET


// caracteres uteis para tabelas
#define TAB_HOR "\u2501" // ━ (horizontal)
#define TAB_VER "\u2503" // ┃ (vertical)
#define TAB_TL  "\u250F" // ┏ (top-left)
#define TAB_ML  "\u2523" // ┣ (middle-left)
#define TAB_BL  "\u2517" // ┗ (bottom-left)
#define TAB_TJ  "\u2533" // ┳ (top-join)
#define TAB_MJ  "\u254B" // ╋ (middle-join)
#define TAB_BJ  "\u253B" // ┻ (bottom-join)
#define TAB_TR  "\u2513" // ┓ (top-right)
#define TAB_MR  "\u252B" // ┫ (middle-right)
#define TAB_BR  "\u251B" // ┛ (bottom-right)



//----------------[Constantes]

#define MAXIMO 100
#define FACIL 20                                      
#define MEDIO 40
#define DIFICIL 80
#define linCol 3

//----------------[Escopo de Funções Criadas]

typedef struct{
   int sair;
   int vezes;
   int msgErro;                 //<-------------------------> Mensagens de erro
   int vitoria;             
   int contador;                //<-------------------------> Contagem de movimentos
   int novoJogo;  
   int iniciaJogo; 
   int continuaJogo; 
   int jogoSalvo;
   int loopCarregarJogo;
   int carregarjogo;
   char nomeArquivo[MAXIMO];
} Config;

void instrucoes(Config *info);
void imprimeMenu(Config *info);
int  filtroComandos(Config *info);
void nivelDificuldade(Config *info);
void desalocaTabuleiro(int ***tabuleiro);
void criaTabuleiro(int ***tabuleiro);
void imprimeDificuldade(Config *info); 
int salvarJogo(Config *info,int **tabuleiro);
int  carregarJogo(Config *info, int ***tabuleiro);
void embaralhaTabuleiro(Config info, int ***tabuleiro);
void imprimeTabuleiroJogo(Config *info, int **tabuleiro);
int verificaVitoria(int **tabuleiro,int **tabuleiroReferencia);
void jogoPrincipal(Config *info, int ***tabuleiro, int ***tabuleiroReferencia);
//----------------[Função Principal]

int main()
{  
    //-------Informações carregadas antes do menu
          
    int **tabJogo, **tabRef;
    criaTabuleiro(&tabJogo);
    criaTabuleiro(&tabRef);
    
    Config info;
    info.sair = 0;              //Saida do jogo
    info.novoJogo = 0;          //Informa se é um jogo começado do zero
    info.contador = 0;          //Contagem de movimentos
    info.msgErro = 0;           //Mensagens de erro
    info.continuaJogo = 0;      //Informa se é possivel continuar jogo
    info.vitoria = 0;           //Caso o player vença o jogo

    //Menu que controla jogo
    
    do{
        imprimeMenu(&info);
        
        switch(filtroComandos(&info)){       
            case 1://-------------------------//Novo Jogo
                info.iniciaJogo = 0;
                info.msgErro = 0;
                
                nivelDificuldade(&info);
                if(info.iniciaJogo != 0){
                    info.novoJogo = 1;
                    embaralhaTabuleiro(info, &tabJogo);
                    jogoPrincipal(&info, &tabJogo, &tabRef);
                    info.continuaJogo = 1;
                }
                info.novoJogo = 0;
            break;

            case 2:  //-------------------------//Continuar Jogo Atual
                info.msgErro = 0;
                if(info.continuaJogo != 0){     //permissão para continuar jogo
                    jogoPrincipal(&info, &tabJogo, &tabRef);
                }
                else
                    info.msgErro++;
            break;
            
            case 3:  //-------------------------//Continuar Jogo Salvo
                info.msgErro = 0;
                info.carregarjogo = 0;          //permissão para carregar jogo
                do{
                    carregarJogo(&info,&tabJogo);
                }while(info.loopCarregarJogo == 0);
                if(info.carregarjogo == 1){
                   info.continuaJogo = 1;
                   info.novoJogo = 0;
                   jogoPrincipal(&info, &tabJogo, &tabRef); 
                }
                info.msgErro = 0;
            break;
            case 4:   //-------------------------//Intruções de jogo
                info.msgErro = 0; 
                instrucoes(&info);
            break;
            case 0: //-------------------------//SAIR
                info.sair = 1;                    
            break;
            case -2://-------------------------//Salvar Jogo
                if(salvarJogo(&info, tabJogo) == 1)
                    info.jogoSalvo = 0;
                else{
                    info.jogoSalvo = 1;
                }
            break;
            default://-------------------------//Caso não entre em nenhuma condição
                info.msgErro++;
            break;
        }
    }while(info.sair == 0);
    
    system("clear"); //-------------------------//Limpa tela do terminal
    
    if(info.vitoria != 0)
        printf(BOLD(GREEN("\n O Reino de Nubreu agradece sua ajuda !\n")));
    else
        printf(BOLD(RED("\n O Reino de Nubreu aguarda seu retorno\n")));
    
    //----------Liberação de memória alocada
    
    desalocaTabuleiro(&tabJogo);
    desalocaTabuleiro(&tabRef);

    return 0;
}

//----------------[Estrutura das Funções Criadas]

void desalocaTabuleiro(int ***tabuleiro){
    for(int i = 0;i < linCol;i++)
        free((*tabuleiro)[i]);
    free((*tabuleiro));
}

int filtroComandos(Config *info)
{
    int peca;
    char comando[MAXIMO];
    char verifica[MAXIMO];
    // 01234567
    // 1 2 3 5\n
    // salvar 4.txt
    
    fgets(comando, MAXIMO, stdin);
    if(comando[1] == 10){                   // Caractere '\n'
        peca = comando[0] - 48;             //Converte char para inteiro
        if((peca < 9)&&(0 <= peca))
            return peca;
    }
    else if(strcmp(comando,"sair\n")== 0){  //Caso o queira sair
        info->sair = 1;
        return -4;
    }
    else if(strcmp(comando,"voltar\n")== 0){ //Caso queira voltar para o menu
        return -1;
        }
    else if(11 < strlen(comando)){          //Verifica se o usuário quer salvar
        strcpy(verifica,comando);
        verifica[6] = 0;
        if(strcmp(verifica,"salvar")== 0){ 
            strcpy(info->nomeArquivo,comando);
            return -2;
        }
    }
    strcpy(info->nomeArquivo,comando); 
    return -5;          //retorno caso não se encaixe em nenhuma condição
}

int verificaVitoria(int **tabuleiro,int **tabuleiroReferencia){
    int soma = 0;
    
    for(int i = 0; i < linCol; i++)
        for(int j = 0; j < linCol;j++){
            if(tabuleiro[i][j] == tabuleiroReferencia[i][j])
                soma++;
        }
    if(soma == 9)
        return 1; //-------------------------//Caso o tabuleiro esteja ordenado corretamente
    else
        return 0;
}

int salvarJogo(Config *info,int **tabuleiro)
{
    int tam = strlen(info->nomeArquivo);
    char *nomeSalvo = calloc(tam,sizeof(char));
    char *nome = calloc(tam, sizeof(char));
    
    strcpy(nomeSalvo,info->nomeArquivo);
    strcpy(nome,info->nomeArquivo);

    for(int i = 0; i < tam; i++)                      //Caractere barra
        if(nomeSalvo[i]== 47){
           free(nomeSalvo);
           free(nome);
           return 0;
        }
        else if(nomeSalvo[i]=='\n'){          //Maneira diferente de remover \n
            nomeSalvo[i] = 0; 
            nome[i] = 0;
        }
    
    for(int i = 0;i < tam; i++){
        nomeSalvo[i] = nome[7 + i];
    }

    FILE *salvar = fopen(nomeSalvo,"w");
    
    fprintf(salvar,"%d\n",linCol);                 //Imprime linhas 
    
    for(int i = 0;i < linCol;i++){                 //Imprime Tabuleiro no Arquivo
        for(int j = 0;j < linCol;j++)
            fprintf(salvar,"%d ",tabuleiro[i][j]);
    fprintf(salvar,"\n");    
    }

    fprintf(salvar,"%d\n",info->contador);          //Imprime contador
    
    fclose(salvar);

    free(nome);
    free(nomeSalvo);

    return 1;
}

int carregarJogo(Config *info, int ***tabuleiro)
{
    int linha,nro;
    int contagem; 
    int **tabAux;
    system("clear");
    printf(BOLD(YELLOW("\n\n<<>><<>><<>><<>><<>><<>><<>><< CARREGAR JOGO >><<>><<>><<>><<>><<>><<>><<>>\n")));
    if(info->msgErro != 0){
        printf(BOLD(RED("\nO arquivo digitado não existe !")));
        info->msgErro = 0;
    }
    printf("\nExemplo:"BOLD(YELLOW("nubreu.txt\n")));
    printf("\nDigite o nome do arquivo salvo:");
    
    nro = filtroComandos(info);
    info->nomeArquivo[strlen(info->nomeArquivo)-1] = 0;
    FILE *carregar = fopen(info->nomeArquivo,"r");
    if(nro == -5){    
        if(carregar != NULL){                        //Caso o arquivo exista
            info->msgErro = 0;
            
            criaTabuleiro(&tabAux);
            fscanf(carregar,"%d",&linha);
            
            for(int i = 0; i < linha; i++)           // tabuleiro auxiliar recebe pecas
                fscanf(carregar,"%d %d %d",&tabAux[i][0],&tabAux[i][1],&tabAux[i][2]);
            fscanf(carregar,"%d",&contagem);
            
            for(int i = 0; i < linha; i++)          // Transfere o conteudo para o tabuleiro principal
                for(int j = 0; j < linha; j++)
                    (*tabuleiro)[i][j] = tabAux[i][j];

            info->contador = contagem;                   //Recebe o contador
            
            fclose(carregar);
            
            /*
            for(int i = 0;i < linCol;i++)         // libera tabuleiro auxiliar
                free(tabAux[i]);
            free(tabAux);
            */
            desalocaTabuleiro(&tabAux);
            
            info->msgErro = 0;
            info->loopCarregarJogo++;               
            info->carregarjogo = 1;             //Permissão para continuar jogo
            return 1;          
            }
        else{
            info->msgErro++;
        }
    }
    else if(nro == -4){     //-------------------------//Sair
            info->loopCarregarJogo++;
            info->sair++;
    }
    else if(nro == -1){     //-------------------------//Voltar
        info->msgErro = 0;
        info->loopCarregarJogo++;
    }
    else 
        info->msgErro = 0;
    return 0;
}

void jogoPrincipal(Config *info, int ***tabuleiro, int ***tabuleiroReferencia)
{
    int zeroColuna, zeroLinha, nro, troca;    
    int loopPrincipal = 0;        //Controla a saída
    int bloqTroca = 0;            //Caso seja '1' bloqueia a troca de peca
    int jogoSalvo = 0;
    info->vitoria = 0;

    if(info->novoJogo == 1){      //zera contador
        info->contador = 0;       //zera vitória             
        info->vitoria = 0;
    }
//---------------------------------------------//Verifica se o Jogo carregado já foi Vencido

    if((*tabuleiro)[linCol - 1][linCol - 1] == 0)                
        if(verificaVitoria((*tabuleiro), (*tabuleiroReferencia)) == 1){ 
            info->vitoria = 1;
            bloqTroca = 1;
            loopPrincipal = 1;
        }

//---------------------------------------------//Inicia o Jogo
    while(loopPrincipal == 0){ 
        troca = 0;          
        //Permissão trocar peça
        
        //---------------//Imprime tela de jogo//---------------//         
        
        imprimeTabuleiroJogo(info,(*tabuleiro));
        if(jogoSalvo == 1)
            printf("\njogo salvo com sucesso\n");                     
        else if(jogoSalvo == 2)
            printf("\nO jogo não foi salvo\n");

        jogoSalvo = 0;
        //---------------//Busca posição do zero//---------------//
        
        for(int i = 0; i < linCol; i++)                       
            for(int j = 0; j < linCol; j++)
                if((*tabuleiro)[i][j] == 0){
                    zeroLinha = i;
                    zeroColuna = j;
                }
        //---------------//Imprime possibilidades//---------------//                                                                       
        
        for(int i = 0 ; i < linCol; i++)                             
            for(int j = 0; j < linCol; j++){
                if(zeroColuna == j){//---------------//caso estejam na mesma coluna
                    if((zeroLinha - i) == -1)
                        printf(BOLD(RED("%d ")),(*tabuleiro)[i][j]);  
                    else if((zeroLinha - i) == 1)
                        printf(BOLD(RED("%d ")),(*tabuleiro)[i][j]);
                }
                else if(zeroLinha == i){//---------------//caso estejam na mesma linha
                    if((zeroColuna - j) == 1)
                        printf(BOLD(RED("%d ")),(*tabuleiro)[i][j]);
                    else if((zeroColuna - j) == -1)
                        printf(BOLD(RED("%d ")),(*tabuleiro)[i][j]);
                }      
            }
        
        printf("\nPara sair digite “sair”\n\nEscolha uma peça:");  //---------------//entrada de digito
        nro = filtroComandos(info);                                    

        //---------------// Caso a troca seja possivel//---------------//       
        
        if((nro < 9)&&(0 < nro)&&(bloqTroca == 0)){                               
            for(int i = 0; i < linCol; i++) 
                for(int j = 0; j < linCol; j++){
                    if((*tabuleiro)[i][j] == nro){                           
                        if(zeroColuna == j){
                            if((zeroLinha - i) == -1){              //Cima    
                                troca++;
                            }
                            else if((zeroLinha - i) == 1)           //Baixo
                                troca++;
                        }
                        else if(zeroLinha == i){                    //Esquerda   
                            if((zeroColuna - j) == -1)
                                troca++;
                            else if((zeroColuna - j) == 1)          //Direita
                                troca++;
                        }
                    }
                }
            //---------------//Armazena peça//---------------//
            if(troca == 0){
                info->nomeArquivo[0] = nro;
                info->msgErro = 1;
                info->nomeArquivo[1] = 10;
            }
            //---------------//Faz a troca do número//---------------//            
            
            if((troca != 0)&&(bloqTroca == 0)){                                            
                for(int i = 0; i < linCol; i++) 
                    for(int j = 0; j < linCol; j++)
                        if((*tabuleiro)[i][j] == nro){
                            (*tabuleiro)[i][j] = 0;
                            info->msgErro = 0;
                        }
                        else if((*tabuleiro)[i][j] == 0){
                            (*tabuleiro)[zeroLinha][zeroColuna] = nro;
                            info->contador++;
                        }                     
                }
            
            //---------------//Verica se o usuário venceu //---------------//           
            
            if((*tabuleiro)[linCol - 1][linCol - 1] == 0)                
                if(verificaVitoria((*tabuleiro), (*tabuleiroReferencia)) == 1){ 
                    info->vitoria = 1;
                    bloqTroca = 1;        //Bloqueia troca de posição
                }
        }
        else if(nro == -1){          //Voltar
            loopPrincipal = 1;
            info->msgErro = 0;
        }
        else if(nro == -4){         //Sair
            loopPrincipal = 1;
            info->sair = 1;
        }                           // Salvar
        else if(nro == -2){
            if (salvarJogo(info,(*tabuleiro))== 1)
                jogoSalvo = 1;
            else
                jogoSalvo = 2;
            info->msgErro = 0;
        }
        else 
            info->msgErro = 1;
    }
}

void imprimeTabuleiroJogo(Config *info, int **tabuleiro)
{
    int nro;
    if(info->vitoria == 0){
        system("clear");
        printf(BOLD(RED("\n\n\t\t\t.:]MALLOC PUZZLE[:.\n")));
        printf(BOLD(RED("\t\t\t  "TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_TJ TAB_HOR TAB_HOR TAB_HOR TAB_TJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR)) "\n");
        printf(BOLD(RED("\t\t\t   "TAB_VER " %d " TAB_VER " %d " TAB_VER " %d " TAB_VER)) "\n",tabuleiro[0][0],tabuleiro[0][1],tabuleiro[0][2]);
        printf(BOLD(RED("\t\t\t   "TAB_ML TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MR)) "\n");
        printf(BOLD(RED("\t\t\t   "TAB_VER " %d " TAB_VER " %d " TAB_VER " %d " TAB_VER)) "\n",tabuleiro[1][0],tabuleiro[1][1],tabuleiro[1][2]);
        printf(BOLD(RED("\t\t\t   "TAB_ML TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MR)) "\n");
        printf(BOLD(RED("\t\t\t   "TAB_VER " %d " TAB_VER " %d " TAB_VER " %d " TAB_VER)) "\n",tabuleiro[2][0],tabuleiro[2][1],tabuleiro[2][2]);
        printf(BOLD(RED("\t\t\t  "TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_BJ TAB_HOR TAB_HOR TAB_HOR TAB_BJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR)) "\n\n");
        
        if((info->msgErro != 0)&&(info->nomeArquivo[1] == 10)){ //Enter
            nro = info->nomeArquivo[0];  //RECEBE O NUMERO QUE NÃO PODE SER MOVIDO
            if((nro < 9)&&(0 < nro)){
                printf("\nA peca "BOLD(YELLOW("%d "))"não está disponivel\n",info->nomeArquivo[0]);
                info->msgErro = 0;
            }
            else{
                printf(BOLD(RED("\nVoce digitou algo errado"))"\n");
                info->msgErro = 0;
            }
        }
        else if(info->msgErro != 0){
                printf(BOLD(RED("Voce digitou algo errado"))"\n");
                info->msgErro = 0;
            }
        printf("Jogadas >>"BOLD(GREEN(" %d")), info->contador);
        printf("\nPeças disponíveis >> ");
        
    }
    else if(info->vitoria == 1){
        system("clear");
        printf(BOLD(GREEN("\n\n\t\t\t.:]MALLOC PUZZLE[:.\n")));
        printf(BOLD(GREEN("\t\t\t  "TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_TJ TAB_HOR TAB_HOR TAB_HOR TAB_TJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR)) "\n");
        printf(BOLD(GREEN("\t\t\t   "TAB_VER " %d " TAB_VER " %d " TAB_VER " %d " TAB_VER)) "\n",tabuleiro[0][0],tabuleiro[0][1],tabuleiro[0][2]);
        printf(BOLD(GREEN("\t\t\t   "TAB_ML TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MR)) "\n");
        printf(BOLD(GREEN("\t\t\t   "TAB_VER " %d " TAB_VER " %d " TAB_VER " %d " TAB_VER)) "\n",tabuleiro[1][0],tabuleiro[1][1],tabuleiro[1][2]);
        printf(BOLD(GREEN("\t\t\t   "TAB_ML TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MR)) "\n");
        printf(BOLD(GREEN("\t\t\t   "TAB_VER " %d " TAB_VER " %d " TAB_VER " %d " TAB_VER)) "\n",tabuleiro[2][0],tabuleiro[2][1],tabuleiro[2][2]);
        printf(BOLD(GREEN("\t\t\t  "TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_BJ TAB_HOR TAB_HOR TAB_HOR TAB_BJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR)) "\n\n");
        printf(BOLD(GREEN("\t\tPARABÉNS O REINO DE NUBREU FOI SALVO !\n\n\t\t\t  JOGADAS FEITAS: %d")), info->contador);
        printf("\n\nPeças disponíveis >> ");
    }
}

void nivelDificuldade(Config *info)
{
    char comando[MAXIMO];
    int loop = 0;
    info->vezes = 0;
    do{    
        imprimeDificuldade(info);
        fgets(comando, MAXIMO, stdin);
        if(strcmp(comando,"1\n")== 0){     //Novo Jogo
            loop++;
            info->vezes = FACIL;
            info->iniciaJogo++;             //Permissão para começar jogo
            }    
        else if(strcmp(comando,"2\n")== 0){//Continuar Jogo
                loop++;
                info->vezes = MEDIO;
                info->iniciaJogo++;
            }    
        else if(strcmp(comando,"3\n")== 0){//Continuar Jogo Salvo
                loop++;
                info->vezes = DIFICIL;
                info->iniciaJogo++;
            }
        else if(strcmp(comando,"voltar\n")== 0){
                loop++;
            }
        else if((strcmp(comando,"sair\n")== 0)){
                info->sair = 1;
                loop++;
            }
        else{ 
            info->msgErro++;
        }  
    }while(loop == 0);
}

void imprimeMenu(Config *info)
{
    //---------------------------------Cabeçalho do menu
    // Menu com Imagem do Dragão
    system("clear");
    printf("\t\t\t"BOLD(GREEN("         ,     \\    /      ,        \n")));
    printf("\t\t\t"BOLD(GREEN("        / \\    )\\__/(     / \\       \n")));
    printf("\t\t\t"BOLD(GREEN("       /   \\  (_\\  /_)   /   \\      \n")));
    printf("\t\t\t"YELLOW(TAB_TL TAB_HOR TAB_HOR TAB_HOR TAB_HOR BOLD(GREEN("/"))YELLOW(TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR)));
    printf(BOLD(GREEN("\\"))YELLOW(TAB_HOR TAB_HOR ) BOLD(GREEN("\\"))BOLD(RED("◉   ◉ "))BOLD(GREEN("/")));
    printf(YELLOW( TAB_HOR TAB_HOR) BOLD(GREEN("\\")));
    printf(YELLOW(TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR BOLD(GREEN("\\"))YELLOW(TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_TR))"\n");
    printf("\t\t\t"YELLOW(TAB_VER)"  .   .     . "BOLD(GREEN("|\\../|"))"  .    .     . "YELLOW(TAB_VER)"\n");
    printf("\t\t\t"YELLOW(TAB_VER)" .   . .   .   "BOLD(GREEN("\\VV/"))"  .   .   .   . "YELLOW(TAB_VER)"\n");
    printf("\t\t\t"YELLOW(TAB_VER)"  .  . "BOLD(RED(".:]MALLOC PUZZLE[:."))" .    .  "YELLOW(TAB_VER)"\n");
    printf("\t\t\t"YELLOW(TAB_BL TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR));
    printf(YELLOW(TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR));
    printf(YELLOW(TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR));
    printf(YELLOW(TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_BR));
    printf("\n\t\t\t"BOLD(GREEN("  |    /\\ /      \\\\       \\ /\\    | ")));
    printf("\n\t\t\t"BOLD(GREEN("  |  /   V        ))       V   \\  | \n")));
    printf("\t\t\t"BOLD(GREEN("  |/     `       //        '     \\| \n")));
    printf("\t\t\t"BOLD(GREEN("  `              V                ' \n")));
    
    printf("\n\t  O terrível "BOLD(RED("Dragão Malloc"))" está aterrorizando o Reino de Nubreu!\n");
    printf("Nobre Guerreiro(a) você deve libertar o reino resolvendo o Quebra-Cabeça de Malloc.\n");
    
    printf(BOLD(GREEN("\n » 0 - Sair da Jornada\n » 1 - Começar nova jornada\n")));
    printf(BOLD(GREEN(" » 2 - Continuar Jornada atual\n » 3 - Continuar jornada salva\n")));
    printf(BOLD(GREEN(" » 4 - Manual do guerreiro\n\n")));
    if(info->msgErro != 0){
        printf(BOLD(RED("Voce digitou algo errado"))"\n");
        info->msgErro = 0;
    }
    printf(BOLD(YELLOW("Escolha uma opção ou digite \"sair\" em qualquer lugar:")));
}

void imprimeDificuldade(Config *info)
{
    //Cavaleiro
    system("clear");
    printf(BLUE("   .\n  / \\\n  | |")BOLD(YELLOW("\t\t\t<<>> DIFICULDADE <<>>")));
    printf(BLUE("\n  |.|\n  |.|")BOLD(GREEN("\t\t  ")));
    printf(BLUE("\n  |:|")BOLD(RED("      __"))BOLD(GREEN("\t\t[1] FACIL - Camponês\n")));
    printf(BOLD(YELLOW(",_")) BLUE("|:|") BOLD(YELLOW("_,"))BOLD(RED("   /  )")));
    printf(BOLD(BLUE("\t      [2] MEDIO - Aventureiro \n")));
    printf(BOLD("  (Oo    ") BOLD(RED("/ "))BOLD("_I_")BOLD(RED("\t [3] DIFICIL - Guerreiro Experiente\n")));
    printf(BOLD(YELLOW("   +"))BOLD("\\ \\")BOLD(RED("  |"))BOLD("| __|\n"));
    printf(BOLD("      \\ \\")BOLD(RED("|"))BOLD("|___|\n"));
    printf(BOLD("\t\\ /")BOLD(RED(".:."))BOLD("\\-\\"));
    printf(BOLD("\n\t |  ")BOLD(RED(":  "))BOLD(YELLOW("/-----\\")));
    printf(BOLD("\n         |____")BOLD(YELLOW("|::oOo::|\n")));
    printf(BOLD("         /   ")BOLD(YELLOW(" |:<_T_>:|\n")));
    printf(BOLD("        |______")BOLD(YELLOW("\\ ::: /\n")));
    printf(BOLD("         | |  \\ \\")BOLD(YELLOW("\\:/\n")));
    printf(BOLD("         | |   | |\n"));
    printf(BOLD("         \\ /   | \\_\n"));
    printf(BOLD("         / |   \\___\\\n"));
    printf(BOLD("         `-'\n"));

    if(info->msgErro != 0){
        printf(BOLD(RED("Voce digitou algo errado"))"\n");
        info->msgErro = 0;
    }
    printf(BOLD(YELLOW("Escolha uma opção ou digite \"sair\" em qualquer lugar:\a")));
}     

void instrucoes(Config *info)
{
    char comando[MAXIMO];
    int loop = 0;
    do{
        system("clear");

        printf(BOLD(YELLOW("\t\t\t<<>><<>> MANUAL DO GUERREIRO <<>><<>>\n")));

        printf("\n\t  O terrível "BOLD(RED("Dragão Malloc"))" está aterrorizando o Reino de Nubreu!\a\n");
        printf("Nobre Guerreiro(a) você deve libertar o reino resolvendo o Quebra-Cabeça de Malloc.\n");
        printf("\n\nAlgumas informações úteis para vencer o desafio:\n\n");
        
        printf(BOLD(YELLOW("\n<<>><<>><<>><<>><<>><<>><<>>{ NIVEL }<<>><<>><<>><<>><<>><<>><<>><<>>\n\n")));
        printf(BOLD(GREEN("\t\t    <<>> FÁCIL <<>> \n No nível fácil as peças do tabuleiro são movidas 20 vezes \n\n")));
        printf(BOLD(BLUE("\t\t    <<>> MEDIO <<>> \n No nível medio as peças do tabuleiro são movidas 40 vezes \n\n")));
        printf(BOLD(RED("\t\t   <<>> DIFÍCIL <<>> \n No nível difícil as peças do tabuleiro são movidas 80 vezes \n\n")));

        printf(BOLD(RED("\n<<>><<>><<>>{VOLTAR\n")));
        printf("\n Para voltar ao menu inicial digite 'voltar', em qualquer lugar do jogo.");
        printf("\n\nEscolha a opção (digite “sair” em qualquer lugar para sair do jogo):");
        printf(BOLD(RED(" voltar\n\n")));

        
        printf(BOLD(RED("\n<<>><<>><<>>{SALVAR\n")));
        printf("\n  Para salvar seu jogo digite 'salvar', na area de jogo "); 
        printf("ou no menu principal,\n digite \"salvar meujogo.txt\".\n\n");
        printf("Para sair digite “sair”\n\nEscolha uma peça:"BOLD(RED(" salvar")));
        printf(BOLD(RED(" jornada1.txt\n\n")));
        
        
        printf(BOLD(RED("\n<<>><<>><<>>{SAIR\n"))); 
        printf("\n Para sair do jogo digite sair em qualquer lugar do jogo.");
        printf("\n\nPara sair digite “sair”\n\nEscolha uma peça:"BOLD(RED(" sair\n\n")));
        
        printf(BOLD(YELLOW("\n\n<<>><<>><<>><<>><<>><<>><<>>{ COMO JOGAR }<<>><<>><<>><<>><<>><<>><<>><<>>\n\n")));
        
        printf("Sua jornada encerra quando o tabuleiro está organizado desta forma:\n");
            
        //Tabuleiro 
        printf(BOLD(RED("\t\t\t    "TAB_TL TAB_HOR TAB_HOR TAB_HOR TAB_TJ TAB_HOR TAB_HOR TAB_HOR TAB_TJ TAB_HOR TAB_HOR TAB_HOR TAB_TR)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_VER " 1 " TAB_VER " 2 " TAB_VER " 3 " TAB_VER)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_ML TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MR)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_VER " 4 " TAB_VER " 5 " TAB_VER " 6 " TAB_VER)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_ML TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MR)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_VER " 7 " TAB_VER " 8 " TAB_VER " 0 " TAB_VER)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_BL TAB_HOR TAB_HOR TAB_HOR TAB_BJ TAB_HOR TAB_HOR TAB_HOR TAB_BJ TAB_HOR TAB_HOR TAB_HOR TAB_BR)) "\n\n");
        
        printf("Apenas peças ao lado do zero podem ser movidas. Exemplo:\n");
        printf(BOLD(RED("\t\t\t    "TAB_TL TAB_HOR TAB_HOR TAB_HOR TAB_TJ TAB_HOR TAB_HOR TAB_HOR TAB_TJ TAB_HOR TAB_HOR TAB_HOR TAB_TR)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_VER " 1 " TAB_VER " 4 " TAB_VER " 3 " TAB_VER)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_ML TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MR)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_VER " 2 " TAB_VER " 5 " TAB_VER " 0 " TAB_VER)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_ML TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MR)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_VER " 7 " TAB_VER " 6 " TAB_VER " 8 " TAB_VER)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_BL TAB_HOR TAB_HOR TAB_HOR TAB_BJ TAB_HOR TAB_HOR TAB_HOR TAB_BJ TAB_HOR TAB_HOR TAB_HOR TAB_BR)) "\n\n");

        printf("Peças disponíveis: "BOLD(RED("3 5 8\n\n")));
        printf("Escolha a peça (digite “sair” em qualquer lugar para sair do jogo):" BOLD(RED(" 5\n\n")));

        printf("A peça escolhida" BOLD(RED(" 5\n\n")));

        printf(BOLD(RED("\t\t\t    "TAB_TL TAB_HOR TAB_HOR TAB_HOR TAB_TJ TAB_HOR TAB_HOR TAB_HOR TAB_TJ TAB_HOR TAB_HOR TAB_HOR TAB_TR)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_VER " 1 " TAB_VER " 4 " TAB_VER " 3 " TAB_VER)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_ML TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MR)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_VER " 2 " TAB_VER " 0 " TAB_VER " 5 " TAB_VER)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_ML TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MR)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_VER " 7 " TAB_VER " 6 " TAB_VER " 8 " TAB_VER)) "\n");
        printf(BOLD(RED("\t\t\t    "TAB_BL TAB_HOR TAB_HOR TAB_HOR TAB_BJ TAB_HOR TAB_HOR TAB_HOR TAB_BJ TAB_HOR TAB_HOR TAB_HOR TAB_BR)) "\n\n");
        
        if(info->msgErro != 0){
            printf(BOLD(RED("Aparentemente você digitou algo errado\n")));
            info->msgErro = 0;
        }
        printf("\nEscolha a opção (digite “sair” em qualquer lugar para sair do jogo):\a");
        fgets(comando,MAXIMO,stdin);
        if((strcmp(comando,"voltar\n") == 0))
            loop++;
        else if(strcmp(comando,"sair\n") == 0){
            loop++;
            info->msgErro++;
            info->sair = 1;
        }
        else{
            info->msgErro++;
        }
    }while(loop == 0);
}

void criaTabuleiro(int ***tabuleiro)
{
    int peca = 1;
    //Aloca Matriz
    
    *tabuleiro = malloc(linCol * sizeof(int*));
    for(int i = 0; i < linCol; i++)
        (*tabuleiro)[i] = malloc(linCol * sizeof(int));
    
    //---------------//Ordena o tabuleiro
    
    for(int i = 0; i < linCol; i++){
        for(int j = 0; j < linCol;j++){
                (*tabuleiro)[i][j] = peca++;
        }
    }
    (*tabuleiro)[linCol - 1][linCol - 1] = 0;//Ultima posição recebe 0
}

void embaralhaTabuleiro(Config info, int ***tabuleiro)
{
    srand(time(NULL));
    
    int zeroColuna, zeroLinha, nro, troca;
    int ultimoNro = 0;
    int contador = 0;

    int **tabAux;                   //Tabuleiro usado para embaralhar 
    
    criaTabuleiro(&tabAux);         //---------------//Cria tabuleiro 
    
    do{ 
        troca = 0;
        nro = 1 + rand()%8;//---------------//Gera um número de 1 a 8
        
        if(nro != ultimoNro){
            //---------------//Busca posição do zero
            for(int i = 0; i < linCol; i++) 
                for(int j = 0; j < linCol; j++)
                    if(tabAux[i][j] == 0){
                        zeroLinha = i;
                        zeroColuna = j;
                    }
            //---------------// Testa se é possivel mover//---------------//
            for(int i = 0; i < linCol; i++) 
                for(int j = 0; j < linCol; j++)
                    if(tabAux[i][j] == nro){
                        if(zeroColuna == j){
                            if((zeroLinha - i) == 1)
                                troca++;
                            else if((zeroLinha - i) == -1)
                                troca++;
                        }
                        else if(zeroLinha == i){
                            if((zeroColuna - j) == 1)
                                troca++;
                            else if((zeroColuna - j) == -1)
                                troca++;
                        }
                    }   

            if(troca != 0){//---------------//Substitui o número
                for(int i = 0; i < linCol; i++) 
                    for(int j = 0; j < linCol; j++)
                        if(tabAux[i][j] == nro)
                            tabAux[i][j] = 0;
                        else if(tabAux[i][j] == 0){
                            tabAux[zeroLinha][zeroColuna] = nro;
                            contador++;
                        }
            }
        }         
    }while(contador < info.vezes); //---------------//Enquanto não embaralhar A quantidade de vezes

    for(int i = 0; i < linCol; i++) //---------------//Transfere conteúdo para a matriz principal
        for(int j = 0; j < linCol; j++){
            (*tabuleiro)[i][j] = tabAux[i][j];
        }
    /*
    for(int i = 0;i < linCol;i++)
        free(tabAux[i]);
    free(tabAux);
    */
   desalocaTabuleiro(&tabAux); //---------------//Desaloca o tabuleiro auxiliar
}
