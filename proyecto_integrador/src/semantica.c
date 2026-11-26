#include "interno.h"

typedef struct { const char *name; int depth, initialized, active; } Symbol;
typedef struct { Context *c; Symbol *items; size_t count, capacity; int depth; } Scope;

int32_t calculate(const char *op,int32_t a,int32_t b) {
    if(!strcmp(op,"+"))return (int32_t)((uint32_t)a+(uint32_t)b);
    if(!strcmp(op,"-"))return (int32_t)((uint32_t)a-(uint32_t)b);
    if(!strcmp(op,"*"))return (int32_t)((uint32_t)a*(uint32_t)b);
    if(!strcmp(op,"/"))return b ? (int32_t)((int64_t)a/b):0;
    if(!strcmp(op,"<"))return a<b;
    if(!strcmp(op,"<="))return a<=b;
    if(!strcmp(op,">"))return a>b;
    if(!strcmp(op,">="))return a>=b;
    if(!strcmp(op,"=="))return a==b;
    return a!=b;
}
int constant(Node *n,int32_t *v) {
    if(!n)return 0;
    if(n->kind==NUM) { *v=n->value; return 1; }
    int32_t a,b;
    if(n->kind==NEG && constant(n->a,&a)) { *v=(int32_t)(0u-(uint32_t)a); return 1; }
    if(n->kind==BIN && constant(n->a,&a) && constant(n->b,&b)) {
        if(!strcmp(n->tok.text,"/") && !b)return 0;
        *v=calculate(n->tok.text,a,b); return 1;
    }
    return 0;
}
static int lookup(Scope *s,const char *name) {
    for(size_t i=s->count;i>0;i--)
        if(s->items[i-1].active && !strcmp(s->items[i-1].name,name))return (int)i-1;
    return -1;
}
static void expr(Scope *s,Node *n) {
    if(!n)return;
    if(n->kind==VAR) {
        n->symbol=lookup(s,n->tok.text);
        if(n->symbol<0)diagnostic(s->c,n->tok,"E01","variable no declarada '%s'",n->tok.text);
        else if(!s->items[n->symbol].initialized)
            diagnostic(s->c,n->tok,"E01","variable sin inicializar '%s'",n->tok.text);
    } else {
        expr(s,n->a); expr(s,n->b);
        int32_t v;
        if(n->kind==BIN && !strcmp(n->tok.text,"/") && constant(n->b,&v) && !v)
            diagnostic(s->c,n->tok,"E04","division entre cero constante");
    }
}
static void visit(Scope *s,Node *n);
static int *snapshot(Scope *s,size_t count) {
    int *state=allocate((count+1)*sizeof *state);
    for(size_t i=0;i<count;i++)state[i]=s->items[i].initialized;
    return state;
}
static void restore(Scope *s,int *state,size_t count) {
    for(size_t i=0;i<count;i++)s->items[i].initialized=state[i];
}
static void visit(Scope *s,Node *n) {
    for(;n;n=n->next) {
        switch(n->kind) {
        case PROGRAM: visit(s,n->a); break;
        case BLOCK: {
            size_t start=s->count; s->depth++; visit(s,n->a);
            for(size_t i=start;i<s->count;i++)s->items[i].active=0;
            s->depth--; break;
        }
        case DECL: {
            int old=lookup(s,n->tok.text);
            if(old>=0 && s->items[old].depth==s->depth) {
                diagnostic(s->c,n->tok,"E02","redeclaracion de '%s' en el mismo ambito",n->tok.text);
                expr(s,n->a); break;
            }
            if(s->count==s->capacity) {
                s->capacity=s->capacity?s->capacity*2:64;
                Symbol *p=realloc(s->items,s->capacity*sizeof *p);
                if(!p) { fputs("sin memoria\n",stderr); exit(1); } s->items=p;
            }
            n->symbol=(int)s->count;
            s->items[s->count++]=(Symbol){n->tok.text,s->depth,0,1};
            expr(s,n->a); s->items[n->symbol].initialized=n->a!=NULL; break;
        }
        case ASSIGN:
            n->symbol=lookup(s,n->tok.text); expr(s,n->a);
            if(n->symbol<0)diagnostic(s->c,n->tok,"E01","variable no declarada '%s'",n->tok.text);
            else s->items[n->symbol].initialized=1;
            break;
        case PRINT: expr(s,n->a); break;
        case IF: {
            expr(s,n->a); size_t count=s->count; int *before=snapshot(s,count);
            visit(s,n->b); int *yes=snapshot(s,count); restore(s,before,count);
            if(n->c)visit(s,n->c);
            for(size_t i=0;i<count;i++)s->items[i].initialized=yes[i]&&s->items[i].initialized;
            free(before); free(yes); break;
        }
        case WHILE: {
            expr(s,n->a); size_t count=s->count; int *before=snapshot(s,count);
            visit(s,n->b); restore(s,before,count); free(before); break;
        }
        default: break;
        }
    }
}
void check(Context *c,Node *n) {
    Scope s={0}; s.c=c; visit(&s,n); c->symbols=(int)s.count; free(s.items);
}
