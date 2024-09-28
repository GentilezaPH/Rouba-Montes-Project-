#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct Carta {
    int numero;
    int naipe;
} Carta;

typedef struct elemento_Pilha {
    Carta C;
    struct elemento_Pilha *anterior;
} elementoPilha;

typedef struct Pilha1 {
    elementoPilha *topo;
} Pilha;

typedef struct elemento_Lista {
    Carta C1;
    struct elemento_Lista *proximo;
} elementoLista;

typedef struct Lista1 {
    elementoLista *inicio;
} Lista;

typedef struct jogador1 {
    char nome[50];
    Pilha* jog;
    Carta c1;
    int pontuacao;
} jogador;

// Function prototypes
Lista* cria_lista();
int busca_descarte(Lista* L, int chave);//funcao para buscar as cartas no descarte
int Remove_Descarte(Lista *l, Carta *x, int posicao);// funcao para retirar a carta prga do Descarte
int Conta_Lista(Lista* l);//funcao auxiliar para contar o tamanho do descarte
void inserir_Descarte(Lista *l, Carta x);// funcao para descartar as cartas
void printLista(Lista* l); //funcao para imprimir o descarte

jogador* cria_jogador(int x);
int compara_Carta(jogador *j, int posicao1, int posicao2);// funcao para comparar a carta da mao com os topos dos jogadores
void empilha_do_descarte(jogador *j, Carta x, int posicao);// funcao para empilhar a carta pega no monte do jogador
void rouba_monte(jogador* j, int posicao1, int posicao2);// funcao para roubar os montes dos jogadores
int conta_Cartas(jogador* j, int posicao);//funcao auxiliar para contar as cartas dos montes do jogadores
void desempilha_jogador(jogador *j, int posicao); //funcao para desempilhar o monte roubado dos jogadores

Pilha* cria_Pilha();
void cria_baralho(Carta *x, int quantbar);
void embaralhar(Carta *baralho, int quantbar);//funcao para embaralhar
void empilha_monte2(Pilha* p, Carta v);//funcao auxiliar para passar o baralho embaralhado pra pilha do monte
void empilha_monte(Carta *baralho, int quantidade, Pilha *monte);//funcao para passar o baralho para a pilha do monte
void empilha_Carta_principal(jogador *j, Carta x, int posicao);// funcao para empilhar a carta tirada do baralho no monte do jogador
void desempilha_Carta_principal(Pilha *p, Carta *x);// funcao para desempilhar a carta do baralho
void ordena_ranking(jogador *j, int tam);// funcao para ordenar o ranking de ganhadores
void ordena_Cartas(Carta vetor[], jogador *j, int posicao);// funcao para ordenar as cartas do ganhador
void ordena_Quick(int esq, int dir, Carta *vetor); //funcao auxiliar para ordenar as cartas do ganhador
void particionar(int esq, int dir, int *i, int *j, Carta *vetor);// funcao auxiliar para ordenar as cartas do ganhador
void print_Ranking(jogador* j, int tam);// funcao para imprimir o ranking
void passa_vetor(jogador* j, Carta *vetor, int posicao); //funcao para passar as cartas do vencedor da pilha para o vetor 
void print_vetor(Carta *vetor, int tam);// funcao para imprimir as cartas do ganhador ordenadas
void printTopo(jogador* j, int posicao); //funcao para imprimir o topo da pilha dos jogadores

int main() {
    srand(time(NULL));

    int quantjog; //variavel para contabilizar a quantidade de jogadores
    int quantbar; //variavel para contabilizar a quantidade de baralhos
    int controle; //variavel para ter controle do fim do jogo
    Carta compra; //variavel da carta retirada do baralho
    int comparatopos; //variavel que verifica se ha topos iguais a carta
    int recebeDescarte;// variavel que recebe a posicao da carta no descarte caso ela exista
    int buscaDescarte; // variavel da que recebe a chave para buscar no descarte
    Carta ValorDescarte; //variavel para salvar a carta retirada do descarte
    Carta *recebeVetor; //variavel que recebe o vetor da ordenacao das cartas
    int quantcartas_ganhador; //variavel q recebe a quanrtidade de cartas do vencedor
    int repete; // variavel para regular as jogadas em que o jogador pode repetir
    int verifica_remocao; //variavel que recebe a verificacao se a carta foi removida do descarte ou nao


    printf("Bem vindo ao Rouba montes!!!\n\n");
    printf("Escolha a quantidade de jogadores: ");
    scanf("%d", &quantjog);
    printf("\nEscolha a quantidade de baralhos (quantidade de baralhos recomendados: 1 a cada 2 jogadores): ");
    scanf("%d", &quantbar);
    controle = 52 * quantbar;

    Pilha *Monte = cria_Pilha();
    jogador* j = cria_jogador(quantjog);
    Carta* bar = (Carta*)malloc((52 * quantbar) * sizeof(Carta));
    cria_baralho(bar, quantbar);
    embaralhar(bar, quantbar);
    empilha_monte(bar, quantbar, Monte);
    Lista *Descarte = cria_lista();

    int i=0;
    do{
        do{
            repete=1;

            if(i%quantjog==0)
                i=0;

                printf("jogador %d, aperte enter para comprar uma carta\n", i+1);
                setbuf(stdin, NULL);
                getchar();

                desempilha_Carta_principal(Monte, &compra);
                j[i].c1= compra;
                printf("Carta comprada: %d", compra.numero);
                if(compra.naipe== 1){
                    printf(" paus\n");
                    }
                else if(compra.naipe== 2){
                    printf(" copas\n");
                }
                else if(compra.naipe== 3){
                    printf(" espadas\n");
                }
                else if(compra.naipe== 4){
                    printf(" ouro\n");
                }


                for(int k=0; k<quantjog; k++){

                    comparatopos=-1;
                    if(j[k].jog->topo!=NULL)
                        comparatopos=compara_Carta(j, i, k);



                    if(comparatopos== 0){
                        if(i==k){

                            empilha_Carta_principal(j, compra, i);
                            printf("Carta empilhada no proprio monte.\n");
                            j[i].pontuacao= conta_Cartas(j, i);
                            printf("Agora voce tem %d cartas\n", j[i].pontuacao);
                            printTopo(j, i);
                            repete=0;

							break;


						}
                    }

                    else if(comparatopos== 1){

						rouba_monte(j, i, k );
                        desempilha_jogador(j, k);
						empilha_Carta_principal(j, compra, i);
						j[i].pontuacao= conta_Cartas(j, i);
						j[k].pontuacao= conta_Cartas(j, k);
						printf("Monte roubado do jogador %d\n", k);
						printf("Agora voce tem %d cartas\n", j[i].pontuacao);
						printTopo(j, i);
						repete=0;

                        break;

                    }

				}

                if(Descarte->inicio!=NULL){
                    buscaDescarte= compra.numero;
                }
                recebeDescarte=busca_descarte(Descarte, buscaDescarte );
                buscaDescarte=-1;


                if(recebeDescarte < 0 ){
                    inserir_Descarte(Descarte, compra);
                    printf("Carta descartada.\n");
                    printLista(Descarte);
                    printf("\n\n");
                    printf("Agora voce tem %d cartas\n", j[i].pontuacao);
                    printTopo(j, i);

                }


                else{
                    printLista(Descarte);
                    verifica_remocao=Remove_Descarte(Descarte, &ValorDescarte , recebeDescarte);
                    empilha_do_descarte(j, ValorDescarte , i);
                    empilha_Carta_principal(j, compra, i);
                    j[i].pontuacao= conta_Cartas(j, i);
                    printLista(Descarte);
                    printf("\nCarta igual a da compra pega do descarte.\n");
                    printf("Agora voce tem %d cartas\n", j[i].pontuacao);
                    printTopo(j, i);
                    repete=0;
                }


         } while(repete!=1);
            i++;
    } while(controle!=0);

    printf("\n\n\n\n");
    printf("END!!!\n\n\n\n");

    printf("SUMULA DO JOGO:\n");

    ordena_ranking(j, quantjog);
    print_Ranking(j, quantjog);
    passa_vetor(j, recebeVetor,  0);
    ordena_Cartas(recebeVetor, j, 0);
    quantcartas_ganhador= conta_Cartas(j, 0);
    print_vetor(recebeVetor, quantcartas_ganhador );


    printf("FIM DA SUMULA.....");

    return 0;
}


Pilha* cria_Pilha(){
    Pilha* p= (Pilha*)malloc(sizeof(Pilha));
    p->topo= NULL;
    return p;
}

Lista* cria_lista(){
    Lista *l= (Lista*)malloc(sizeof(Lista));
    l->inicio= NULL;
    return l;
}


jogador* cria_jogador(int x){
    jogador* a=(jogador*)malloc(x*sizeof(jogador));
    for(int i=0; i<x; i++){
        printf("Digite o nome do jogador %d\n", i+1);
        setbuf(stdin, NULL);
        fgets(a[i].nome, 50, stdin);
        a[i].nome[strcspn(a[i].nome, "\n")] = 0;
        a[i].pontuacao= 0;
        a[i].jog= cria_Pilha();
    }
    return a;
}

void cria_baralho(Carta *x, int quantbar ){
    int a;
    for(int i=0; i<quantbar; i++){
        for(int j=1; j<=4; j++){
            for(int k=1; k<=13; k++){
                x[a].naipe= j;
                x[a].numero= k;
                a++;
            }
        }

    }
}
void embaralhar(Carta *baralho,int quantbar){
    int a=0;
    int b=0;
    Carta aux;
    for(int i=0; i<100; i++){
        a= rand()% (52*quantbar);
        b= rand()% (52*quantbar);
        aux= baralho[a];
        baralho[a]=baralho[b];
        baralho[b]= aux;

    }
}

void empilha_monte2(Pilha* p, Carta v){

    elementoPilha* novaCarta= (elementoPilha*)malloc(sizeof(elementoPilha));

    novaCarta->C = v;

    novaCarta->anterior = p->topo;

    p->topo = novaCarta;

}

void empilha_monte(Carta *baralho,int quantidade, Pilha *monte){
    for(int i=0; i<quantidade*52; i++){
        empilha_monte2(monte, baralho[i]);
    }
}


int busca_descarte(Lista* L, int chave){
    elementoLista* aux;
    aux= L->inicio;
    int contador=0;

        while(aux!=NULL){
            if(aux->C1.numero== chave){
                return contador;
            }
            else{
                aux=aux->proximo;
                contador++;
            }
        }
        return -1;
}


int compara_Carta(jogador *j, int posicao1, int posicao2){
    if(j[posicao1].c1.numero == j[posicao2].jog->topo->C.numero){
        return 0;
    }
    else
        return 1;
}

void empilha_do_descarte(jogador *j, Carta x, int posicao){
    elementoPilha* Cartas= (elementoPilha*)malloc(sizeof(elementoPilha));
    Cartas->C= x;
    if(j[posicao].jog->topo==NULL){
        Cartas->anterior=NULL;
        j[posicao].jog->topo= Cartas;
    }
    else{
        Cartas->anterior= j[posicao].jog->topo;
        j[posicao].jog->topo= Cartas;
    }
}

void rouba_monte(jogador* j, int posicao1, int posicao2 ){
    elementoPilha* aux= j[posicao2].jog->topo;
    int x= conta_Cartas(j, posicao2);
    Carta* vetor=(Carta*)malloc(x*sizeof(Carta));
    for(int i=x; i>=0; i--){
        vetor[i]= aux->C;
        aux= aux->anterior;
    }
    elementoPilha* auxiliar2= j[posicao1].jog->topo;
    for(int j=0; j<x;j++){
        auxiliar2->C= vetor[j];
        auxiliar2->anterior= auxiliar2;
    }
    j[posicao1].jog->topo->C= auxiliar2->C;

}


int conta_Cartas(jogador* j, int posicao){
    elementoPilha* aux= j[posicao].jog->topo;
    int contador=0;
    while(aux!=NULL){
        aux=aux->anterior;
        contador++;
    }
    return contador;
}

void empilha_Carta_principal(jogador *j, Carta x, int posicao){
    elementoPilha *NovaCarta= (elementoPilha*)malloc(sizeof(elementoPilha));

    NovaCarta->C= x;
    NovaCarta->anterior= j[posicao].jog->topo;
    j[posicao].jog->topo= NovaCarta;

}

void desempilha_Carta_principal(Pilha *p, struct Carta *x){

    elementoPilha *aux;
    aux= p->topo;
    x->numero= p->topo->C.numero;
    x->naipe=p->topo->C.naipe;
    p->topo= aux->anterior;
    free(aux);
}

int Remove_Descarte(Lista *l, Carta *x, int posicao){
    int a= Conta_Lista(l);

    if(a==0 || posicao < 0 || posicao >= a){
        return 1;
    }

    else{
        elementoLista *aux= l->inicio;
        if(posicao== 0){
            *x= l->inicio->C1;
            l->inicio= l->inicio->proximo;

        }
        else{
            int count= 0;

            while(count< posicao- 1){
                aux= aux->proximo;
                count++;
            }

            elementoLista *auxiliar2= aux->proximo;
            *x= auxiliar2->C1;
            aux->proximo= auxiliar2->proximo;

        }
        return 0;
    }

}

int Conta_Lista (Lista* l){
    int count=0;
    elementoLista* aux;
    aux=l->inicio;
    while(aux->proximo!=NULL){
        aux=aux->proximo;
        count++;
    }
    return count;
}

void desempilha_jogador(jogador *j, int posicao){

    int tam= conta_Cartas(j, posicao);
    int count=0;


    while(count < tam){

        elementoPilha* aux= j[posicao].jog->topo;
        j[posicao].jog->topo= aux->anterior;

        free(aux);

    }
}

void inserir_Descarte(Lista *l, Carta x){
    elementoLista *NovaCarta = (elementoLista*) malloc(sizeof(elementoLista));
    elementoLista *aux;

    aux = l->inicio;
    NovaCarta->C1 = x;
    NovaCarta->proximo = aux;
    l->inicio = NovaCarta;

}

void ordena_ranking(jogador*j, int tam){
    int i, k;
    jogador aux;
    for(i=1; i<tam; i++){
        aux=j[i];
        k=i -1;
        while(aux.pontuacao> j[k].pontuacao && k>=0){
            j[k+1] = j[k];
            k--;
        }
        j[k+1]= aux;
    }
}


void particionar(int esq, int dir, int *i, int *j, Carta *vetor){
    Carta x, temp;
    *i=esq;
    *j=dir;
    x= vetor[((*i)+(*j))/ 2];
    do{
        while(x.numero> vetor[*i].numero){
            (*i)++;
        }

        while(x.numero< vetor[*j].numero){
            (*j)--;
        }
        if((*i)<=(*j)){
            temp=vetor[*i];
            vetor[*i]= vetor[*j];
            vetor[*j]= temp;
            (*i)++;
            (*j)--;
        }
    }while((*i)<=(*j));
}


void ordena_Quick(int esq, int dir, Carta *vetor){
    int i, j;
    particionar(esq, dir, &i, &j, vetor);
    if(esq< j)
        ordena_Quick(esq, j, vetor);
    if(i<dir)
        ordena_Quick(i, dir, vetor);
}

void ordena_Cartas(Carta *vetor, jogador *j, int posicao){
    int x= conta_Cartas(j, posicao);
    ordena_Quick(0, x-1, vetor);
}


 void printTopo(jogador* j, int posicao) {
    if (j[posicao].jog->topo== NULL) {
        printf("A pilha esta vazia\n");
    } else {
        printf("Topo da pilha, Carta: %d", j[posicao].jog->topo->C.numero);
            if(j[posicao].jog->topo->C.naipe== 1){
               printf(" paus \n");
            }
            else if(j[posicao].jog->topo->C.naipe== 2){
                 printf(" copas \n");
            }
            else if(j[posicao].jog->topo->C.naipe== 3){
                 printf(" espadas \n");
            }
            else if(j[posicao].jog->topo->C.naipe== 4){
                 printf(" ouro \n");
            }
    }
}

void printLista(Lista* l){
    elementoLista* aux;
    aux=l->inicio;
    int count=0;
    printf("Descarte\n");
    printf("   |    \n");
    printf("   V    \n");
    while(aux->proximo!=NULL){
            if(aux->C1.naipe== 1){
                printf("  [%d paus]  ", aux->C1.numero);
            }
            else if(aux->C1.naipe== 2){
                printf("  [%d copas]  ", aux->C1.numero);
            }
            else if(aux->C1.naipe== 3){
                printf("  [%d espadas]  ", aux->C1.numero);
            }
            else if(aux->C1.naipe== 4){
                printf("  [%d ouro] ", aux->C1.numero);

            }

            aux=aux->proximo;
            count++;
    }
    printf("\n\n");
}
void print_Ranking(jogador* j, int tam){

    for(int i=0; i<tam; i++){

        printf("%d° colocado: ", i+1);
        printf("%s\n", j[i].nome);
        printf("Com a pontuacao: %d", j[i].pontuacao);
    }
}


void passa_vetor(jogador* j, Carta *vetor, int posicao){

    elementoPilha* aux= j[posicao].jog->topo;
    int x= conta_Cartas(j, posicao);
    vetor=(Carta*)malloc(x*sizeof(Carta));

    for(int i=0; i<x; i++){

        vetor[i]= aux->C;
        aux= aux->anterior;

    }

}

void print_vetor(Carta *vetor, int tam){
    printf("As cartas do ganhador ordenadas sao: \n\n");
    printf("Cartas\n");
    printf("   |    \n");
    printf("   V    \n");
    for(int i=0; i<tam; i++){

            if(vetor[i].naipe== 1){
                printf("   [%d paus]\n  ", vetor[i]);
            }
            else if(vetor[i].naipe== 2){
                printf("   [%d copas]\n  ", vetor[i]);
            }
            else if(vetor[i].naipe== 3){
                printf("   [%d espadas]\n  ", vetor[i]);
            }
            else if(vetor[i].naipe== 4){
                printf("   [%d ouro]\n  ", vetor[i]);
            }
    }
}
