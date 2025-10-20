#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define QUEUE_SIZE 5
#define STACK_CAP 3

typedef struct { char tipo; int id; } Peca;

typedef struct {
    Peca v[QUEUE_SIZE];
    int head, tail, cnt;
} Fila;

typedef struct {
    Peca v[STACK_CAP];
    int top;
} Pilha;

int next_id = 0;

Peca gera() {
    Peca p;
    int r = rand() % 4;
    p.tipo = (r==0?'I': (r==1?'O': (r==2?'T':'L')));
    p.id = next_id++;
    return p;
}

void initFila(Fila *q){ q->head=0; q->tail=0; q->cnt=0; }
int filaVazia(Fila *q){ return q->cnt==0; }
int filaCheia(Fila *q){ return q->cnt==QUEUE_SIZE; }

void pushFilaOverwrite(Fila *q, Peca p){
    if(filaCheia(q)){
        q->head = (q->head+1)%QUEUE_SIZE;
        q->cnt--;
    }
    q->v[q->tail]=p;
    q->tail=(q->tail+1)%QUEUE_SIZE;
    q->cnt++;
}

int popFila(Fila *q, Peca *out){
    if(filaVazia(q)) return 0;
    *out = q->v[q->head];
    q->head = (q->head+1)%QUEUE_SIZE;
    q->cnt--;
    return 1;
}

void initPilha(Pilha *s){ s->top=-1; }
int pilhaVazia(Pilha *s){ return s->top==-1; }
int pilhaCheia(Pilha *s){ return s->top==STACK_CAP-1; }
int pushPilha(Pilha *s, Peca p){
    if(pilhaCheia(s)) return 0;
    s->v[++s->top]=p;
    return 1;
}
int popPilha(Pilha *s, Peca *out){
    if(pilhaVazia(s)) return 0;
    *out = s->v[s->top--];
    return 1;
}

void mostra(Fila *q, Pilha *s){
    printf("\nEstado atual:\n");
    printf("Fila: ");
    if(filaVazia(q)) printf("(vazia)");
    else{
        for(int i=0;i<q->cnt;i++){
            int idx = (q->head+i)%QUEUE_SIZE;
            printf("[%c %d] ", q->v[idx].tipo, q->v[idx].id);
        }
    }
    printf("\nPilha (Topo->Base): ");
    if(pilhaVazia(s)) printf("(vazia)");
    else{
        for(int i=s->top;i>=0;i--) printf("[%c %d] ", s->v[i].tipo, s->v[i].id);
    }
    printf("\n");
}

void flushin(){ int c; while((c=getchar())!='\n' && c!=EOF); }

int main(){
    srand((unsigned)time(NULL));
    Fila q; Pilha s;
    initFila(&q); initPilha(&s);
    for(int i=0;i<QUEUE_SIZE;i++) pushFilaOverwrite(&q, gera());

    int op;
    while(1){
        mostra(&q, &s);
        printf("\n1 Jogar  2 Reservar  3 Usar reserva  0 Sair\nOpção: ");
        if(scanf("%d",&op)!=1){ printf("Entrada errada\n"); flushin(); continue; }
        flushin();

        if(op==0) break;
        else if(op==1){
            Peca p;
            if(popFila(&q,&p)) printf("Jogou [%c %d]\n", p.tipo, p.id);
            else printf("Fila vazia\n");
            pushFilaOverwrite(&q, gera());
        }
        else if(op==2){
            if(pilhaCheia(&s)){ printf("Pilha cheia\n"); }
            else{
                Peca p;
                if(popFila(&q,&p)){
                    if(pushPilha(&s,p)) printf("Reservou [%c %d]\n", p.tipo, p.id);
                    else printf("Erro ao empilhar\n");
                } else printf("Fila vazia\n");
                pushFilaOverwrite(&q, gera());
            }
        }
        else if(op==3){
            Peca p;
            if(popPilha(&s,&p)) printf("Usou reserva [%c %d]\n", p.tipo, p.id);
            else printf("Pilha vazia\n");
            pushFilaOverwrite(&q, gera());
        }
        else printf("Opção inválida\n");
    }
    printf("Saiu\n");
    return 0;
}

