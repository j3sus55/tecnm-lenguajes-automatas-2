#include "interno.h"
#include <ctype.h>

static void token(Context *c, const char *s, size_t n, int line, int col, const char *kind) {
    if (c->count == c->capacity) {
        c->capacity = c->capacity ? c->capacity * 2 : 128;
        Token *p = realloc(c->tokens, c->capacity * sizeof *p);
        if (!p) { fputs("sin memoria\n", stderr); exit(1); }
        c->tokens = p;
    }
    c->tokens[c->count++] = (Token){copy_text(s,n),line,col,kind};
}

void lex(Context *c, const char *s) {
    int line=1, col=1;
    while (*s) {
        if (isspace((unsigned char)*s)) {
            if (*s=='\n') { line++; col=1; } else col++;
            s++; continue;
        }
        if (s[0]=='/' && s[1]=='/') {
            while (*s && *s!='\n') { s++; col++; }
            continue;
        }
        const char *start=s;
        int first=col;
        if (isalpha((unsigned char)*s) || *s=='_') {
            do { s++; col++; } while (isalnum((unsigned char)*s) || *s=='_');
            size_t n=(size_t)(s-start);
            const char *kind="ID";
            const char *keywords[]={"int","if","else","while","print"};
            for (size_t i=0;i<5;i++)
                if (strlen(keywords[i])==n && !strncmp(start,keywords[i],n)) kind="KW";
            token(c,start,n,line,first,kind);
        } else if (isdigit((unsigned char)*s)) {
            do { s++; col++; } while (isdigit((unsigned char)*s));
            token(c,start,(size_t)(s-start),line,first,"NUM");
        } else if (strchr("(){};",*s)) {
            token(c,s++,1,line,col++,"DELIM");
        } else if (strchr("+-*/<>=!",*s)) {
            size_t n=(strchr("<>=!",s[0]) && s[1]=='=') ? 2:1;
            if (*s=='!' && n==1) {
                Token t={(char *)"!",line,col,"OP"};
                diagnostic(c,t,"S01","caracter invalido '!'");
            } else token(c,s,n,line,col,"OP");
            s+=n; col+=(int)n;
        } else {
            Token t={(char *)"",line,col,""};
            diagnostic(c,t,"S01","caracter invalido '%c'",*s);
            s++; col++;
        }
    }
    token(c,"",0,line,col,"EOF");
}
