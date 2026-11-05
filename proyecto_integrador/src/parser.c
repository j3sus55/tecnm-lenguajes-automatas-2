#include "interno.h"

static Token peek(Context *c) { return c->tokens[c->pos]; }
static int at(Context *c, const char *s) { return !strcmp(peek(c).text,s); }
static Token take(Context *c) { Token t=peek(c); if(c->pos+1<c->count)c->pos++; return t; }
static int accept(Context *c,const char *s) { if(!at(c,s))return 0; take(c); return 1; }
static void expect(Context *c,const char *s) {
    if(!accept(c,s)) diagnostic(c,peek(c),"S01","se esperaba '%s'",s);
}
static Node *expression(Context *c);
static Node *statement(Context *c);
static Node *primary(Context *c) {
    Token t=peek(c);
    if(!strcmp(t.kind,"NUM")) {
        take(c); Node *n=node(NUM,t);
        /* Conversion modular definida para enteros de 32 bits. */
        uint32_t v=0;
        for(const char *p=t.text;*p;p++)v=v*10u+(unsigned)(*p-'0');
        n->value=(int32_t)v; return n;
    }
    if(!strcmp(t.kind,"ID")) { take(c); return node(VAR,t); }
    if(accept(c,"(")) { Node *n=expression(c); expect(c,")"); return n; }
    diagnostic(c,t,"S01","se esperaba una expresion");
    if(!at(c,";")&&!at(c,"}")&&strcmp(t.kind,"EOF"))take(c);
    return node(NUM,t);
}
static Node *unary(Context *c) {
    Token t=peek(c);
    if(accept(c,"-")) { Node *n=node(NEG,t); n->a=primary(c); return n; }
    accept(c,"+"); return primary(c);
}
static Node *product(Context *c) {
    Node *n=unary(c);
    while(at(c,"*")||at(c,"/")) { Node *b=node(BIN,take(c)); b->a=n; b->b=unary(c); n=b; }
    return n;
}
static Node *sum(Context *c) {
    Node *n=product(c);
    while(at(c,"+")||at(c,"-")) { Node *b=node(BIN,take(c)); b->a=n; b->b=product(c); n=b; }
    return n;
}
static Node *expression(Context *c) {
    Node *n=sum(c);
    if(at(c,"<")||at(c,"<=")||at(c,">")||at(c,">=")||at(c,"==")||at(c,"!=")) {
        Node *b=node(BIN,take(c)); b->a=n; b->b=sum(c); n=b;
    }
    return n;
}
static void sequence(Context *c,Node *parent,int block) {
    Node **tail=&parent->a;
    while(strcmp(peek(c).kind,"EOF") && !(block&&at(c,"}"))) {
        size_t before=c->pos; int errors=c->errors;
        Node *s=statement(c);
        if(s) { *tail=s; tail=&s->next; }
        if(c->errors>errors) {
            while(strcmp(peek(c).kind,"EOF")&&!at(c,";")&&!at(c,"}"))take(c);
            accept(c,";");
        }
        if(c->pos==before)take(c);
    }
}
static Node *block(Context *c) {
    Node *n=node(BLOCK,peek(c));
    if(!accept(c,"{")) { diagnostic(c,peek(c),"S01","se esperaba '{'"); return n; }
    sequence(c,n,1); expect(c,"}"); return n;
}
static Node *statement(Context *c) {
    Token t=peek(c); Node *n;
    if(at(c,"{"))return block(c);
    if(accept(c,"int")) {
        Token id=peek(c);
        if(strcmp(id.kind,"ID")) { diagnostic(c,id,"S01","se esperaba identificador"); return NULL; }
        take(c); n=node(DECL,id);
        if(accept(c,"="))n->a=expression(c);
        expect(c,";"); return n;
    }
    if(accept(c,"if")) {
        n=node(IF,t); expect(c,"("); n->a=expression(c); expect(c,")"); n->b=block(c);
        if(accept(c,"else"))n->c=block(c);
        return n;
    }
    if(accept(c,"while")) {
        n=node(WHILE,t); expect(c,"("); n->a=expression(c); expect(c,")"); n->b=block(c); return n;
    }
    if(accept(c,"print")) {
        n=node(PRINT,t); expect(c,"("); n->a=expression(c); expect(c,")"); expect(c,";"); return n;
    }
    if(!strcmp(t.kind,"ID")) {
        take(c); n=node(ASSIGN,t); expect(c,"="); n->a=expression(c); expect(c,";"); return n;
    }
    diagnostic(c,t,"S01","se esperaba una sentencia"); return NULL;
}
Node *parse(Context *c) { Node *n=node(PROGRAM,peek(c)); sequence(c,n,0); return n; }
