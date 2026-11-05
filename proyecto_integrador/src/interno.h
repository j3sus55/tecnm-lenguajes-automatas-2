#ifndef INTERNO_H
#define INTERNO_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>

typedef struct { char *text; int line, col; const char *kind; } Token;
typedef enum { PROGRAM, BLOCK, DECL, ASSIGN, IF, WHILE, PRINT, NUM, VAR, BIN, NEG } Kind;
typedef struct Node Node;
struct Node {
    Kind kind;
    Token tok;
    Node *a, *b, *c, *next;
    int32_t value;
    int symbol;
};
typedef struct {
    const char *path;
    Token *tokens;
    size_t count, capacity, pos;
    int errors, symbols;
} Context;
void *allocate(size_t n);
char *copy_text(const char *s, size_t n);
void diagnostic(Context *c, Token t, const char *code, const char *fmt, ...);
void lex(Context *c, const char *source);
Node *parse(Context *c);
Node *node(Kind kind, Token tok);
void free_ast(Node *n);
void print_ast(Node *n);
void check(Context *c, Node *n);
int constant(Node *n, int32_t *value);
int32_t calculate(const char *op, int32_t a, int32_t b);

typedef enum { COPY, BINARY, UNARY, LABEL, JUMP, JFALSE, OUTPUT } Op;
typedef struct { int immediate; int32_t value; int slot; const char *name; } Operand;
typedef struct { Op op; Operand dst, a, b; const char *operator; int label, removed; } Instruction;
typedef struct { Instruction *items; size_t count, capacity; int slots, temps, labels; } IR;
void generate_ir(IR *ir, Node *n, int symbols);
void print_ir(IR *ir);
void assembly(FILE *out, IR *ir);
int run_binary(IR *ir);
#endif
