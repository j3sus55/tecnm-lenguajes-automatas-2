#include "interno.h"

void *allocate(size_t n) {
    void *p=calloc(1,n);
    if (!p) { fputs("sin memoria\n",stderr); exit(1); }
    return p;
}
char *copy_text(const char *s, size_t n) {
    char *p=allocate(n+1); memcpy(p,s,n); return p;
}
void diagnostic(Context *c, Token t, const char *code, const char *fmt, ...) {
    if (c->errors>=20) return;
    c->errors++;
    printf("%s:%d:%d: error %s: ",c->path,t.line,t.col,code);
    va_list args; va_start(args,fmt); vprintf(fmt,args); va_end(args);
    putchar('\n');
}
Node *node(Kind kind, Token tok) {
    Node *n=allocate(sizeof *n); n->kind=kind; n->tok=tok; n->symbol=-1; return n;
}
void free_ast(Node *n) {
    if (!n) return;
    free_ast(n->a); free_ast(n->b); free_ast(n->c); free_ast(n->next); free(n);
}
static void children(Node *n) {
    for (;n;n=n->next) { putchar(' '); print_ast(n); }
}
void print_ast(Node *n) {
    switch (n->kind) {
    case PROGRAM: case BLOCK:
        printf("(%s",n->kind==PROGRAM?"programa":"bloque"); children(n->a); break;
    case DECL: case ASSIGN:
        printf("(%s %s",n->kind==DECL?"declarar":"asignar",n->tok.text);
        if(n->a) { putchar(' '); print_ast(n->a); } break;
    case PRINT: case NEG:
        printf("(%s ",n->kind==PRINT?"imprimir":"neg"); print_ast(n->a); break;
    case NUM: printf("(num %d",n->value); break;
    case VAR: printf("(var %s",n->tok.text); break;
    case BIN:
        printf("(%s ",n->tok.text); print_ast(n->a); putchar(' '); print_ast(n->b); break;
    case WHILE:
        printf("(mientras "); print_ast(n->a); putchar(' '); print_ast(n->b); break;
    case IF:
        printf("(si "); print_ast(n->a); printf(" (entonces "); print_ast(n->b); putchar(')');
        if(n->c) { printf(" (si_no "); print_ast(n->c); putchar(')'); } break;
    }
    putchar(')');
}
