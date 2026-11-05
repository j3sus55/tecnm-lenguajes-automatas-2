#include "interno.h"

static Operand immediate(int32_t v) { return (Operand){1,v,-1,NULL}; }
static Operand variable(Node *n) { return (Operand){0,0,n->symbol,n->tok.text}; }
static Operand temporary(IR *ir) { return (Operand){0,ir->temps++,ir->slots++,NULL}; }
static void emit(IR *ir,Instruction i) {
    if(ir->count==ir->capacity) {
        ir->capacity=ir->capacity?ir->capacity*2:128;
        Instruction *p=realloc(ir->items,ir->capacity*sizeof *p);
        if(!p) { fputs("sin memoria\n",stderr); exit(1); } ir->items=p;
    }
    ir->items[ir->count++]=i;
}
static Operand expression(IR *ir,Node *n) {
    int32_t v;
    if(constant(n,&v))return immediate(v);
    if(n->kind==VAR)return variable(n);
    Operand a=expression(ir,n->a), b=immediate(0), dst=temporary(ir);
    if(n->kind==BIN)b=expression(ir,n->b);
    emit(ir,(Instruction){.op=n->kind==NEG?UNARY:BINARY,.dst=dst,.a=a,.b=b,.operator=n->tok.text});
    return dst;
}
static void statements(IR *ir,Node *n) {
    for(;n;n=n->next) {
        switch(n->kind) {
        case PROGRAM: case BLOCK: statements(ir,n->a); break;
        case DECL: case ASSIGN:
            if(n->a) { Operand a=expression(ir,n->a); emit(ir,(Instruction){.op=COPY,.dst=variable(n),.a=a}); }
            break;
        case PRINT: {
            Operand a=expression(ir,n->a); emit(ir,(Instruction){.op=OUTPUT,.a=a}); break;
        }
        case IF: {
            int no=++ir->labels, end=n->c?++ir->labels:no;
            Operand a=expression(ir,n->a);
            emit(ir,(Instruction){.op=JFALSE,.a=a,.label=no}); statements(ir,n->b);
            if(n->c)emit(ir,(Instruction){.op=JUMP,.label=end});
            emit(ir,(Instruction){.op=LABEL,.label=no});
            if(n->c) { statements(ir,n->c); emit(ir,(Instruction){.op=LABEL,.label=end}); } break;
        }
        case WHILE: {
            int start=++ir->labels,end=++ir->labels;
            emit(ir,(Instruction){.op=LABEL,.label=start}); Operand a=expression(ir,n->a);
            emit(ir,(Instruction){.op=JFALSE,.a=a,.label=end}); statements(ir,n->b);
            emit(ir,(Instruction){.op=JUMP,.label=start}); emit(ir,(Instruction){.op=LABEL,.label=end}); break;
        }
        default: break;
        }
    }
}
/* Elimina definiciones de temporales sin consumidores hasta punto fijo.
 * Las variables se conservan: sus valores pueden vivir a traves de saltos. */
static void dead_temporaries(IR *ir) {
    int changed;
    do {
        changed=0; int *used=allocate(((size_t)ir->slots+1)*sizeof *used);
        for(size_t i=0;i<ir->count;i++) {
            Instruction *p=&ir->items[i]; if(p->removed)continue;
            if(p->op==COPY||p->op==BINARY||p->op==UNARY||p->op==JFALSE||p->op==OUTPUT)
                if(!p->a.immediate)used[p->a.slot]=1;
            if(p->op==BINARY && !p->b.immediate)used[p->b.slot]=1;
        }
        for(size_t i=0;i<ir->count;i++) {
            Instruction *p=&ir->items[i];
            if(!p->removed && (p->op==COPY||p->op==BINARY||p->op==UNARY) &&
               !p->dst.name && !used[p->dst.slot]) { p->removed=1; changed=1; }
        }
        free(used);
    } while(changed);
}
void generate_ir(IR *ir,Node *n,int symbols) {
    ir->slots=symbols; statements(ir,n); dead_temporaries(ir);
}
static void operand(Operand o) {
    if(o.immediate)printf("%d",o.value);
    else if(o.name)printf("%s",o.name);
    else printf("_t%d",o.value);
}
void print_ir(IR *ir) {
    for(size_t i=0;i<ir->count;i++) {
        Instruction *p=&ir->items[i]; if(p->removed)continue;
        switch(p->op) {
        case COPY: case BINARY: case UNARY:
            operand(p->dst); printf(" = ");
            if(p->op==UNARY)putchar('-');
            operand(p->a);
            if(p->op==BINARY) { printf(" %s ",p->operator); operand(p->b); } break;
        case LABEL: printf("L%d:",p->label); break;
        case JUMP: printf("goto L%d",p->label); break;
        case JFALSE: printf("ifFalso "); operand(p->a); printf(" goto L%d",p->label); break;
        case OUTPUT: printf("print "); operand(p->a); break;
        }
        putchar('\n');
    }
}
