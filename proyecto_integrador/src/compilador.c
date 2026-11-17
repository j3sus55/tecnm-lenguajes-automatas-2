#include "compilador.h"
#include "interno.h"

int compilar(const char *ruta,const char *modo) {
    FILE *f=fopen(ruta,"rb");
    if(!f) { perror(ruta); return 1; }
    size_t size=0,capacity=4096;
    char *source=allocate(capacity);
    int ch;
    while((ch=fgetc(f))!=EOF) {
        if(size+1==capacity) {
            capacity*=2; char *p=realloc(source,capacity);
            if(!p) { fclose(f); free(source); fputs("sin memoria\n",stderr); return 1; } source=p;
        }
        source[size++]=(char)ch;
    }
    int failed=ferror(f); fclose(f); source[size]='\0';
    if(failed) { free(source); fputs("error leyendo archivo\n",stderr); return 1; }
    Context c={0}; c.path=ruta; lex(&c,source); free(source);
    Node *root=NULL; IR ir={0}; int result=0;
    if(!strcmp(modo,"--tokens")) {
        if(!c.errors)for(size_t i=0;i<c.count;i++) {
            Token t=c.tokens[i]; printf("%d:%d %s %s\n",t.line,t.col,t.kind,t.text);
        }
    } else if(!c.errors) {
        root=parse(&c);
        if(!c.errors) {
            /* --ast permite inspeccionar sintaxis antes de verificar nombres. */
            if(!strcmp(modo,"--ast")) { print_ast(root); putchar('\n'); }
            else {
                check(&c,root);
                if(!c.errors && strcmp(modo,"--check")) {
                    generate_ir(&ir,root,c.symbols);
                    if(!strcmp(modo,"--tac"))print_ir(&ir);
                    else if(!strcmp(modo,"--asm"))assembly(stdout,&ir);
                    else result=run_binary(&ir);
                }
            }
        }
    }
    if(c.errors)result=1;
    free(ir.items); free_ast(root);
    for(size_t i=0;i<c.count;i++)free(c.tokens[i].text);
    free(c.tokens); return result;
}
