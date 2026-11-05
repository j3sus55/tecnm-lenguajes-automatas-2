#define _POSIX_C_SOURCE 200809L
#include "interno.h"
#ifndef _WIN32
#include <unistd.h>
#include <sys/wait.h>
#endif

static void load(FILE *out,Operand o,const char *reg) {
    if(o.immediate)fprintf(out,"\tmovl $%d, %s\n",o.value,reg);
    else fprintf(out,"\tmovl -%d(%%rbp), %s\n",4*(o.slot+1),reg);
}
void assembly(FILE *out,IR *ir) {
    fprintf(out,".section .rodata\n.Lformat:\n\t.string \"%%d\\n\"\n.text\n.globl main\n.type main, @function\nmain:\n\tpushq %%rbp\n\tmovq %%rsp, %%rbp\n");
    /* SysV: pila alineada a 16 bytes antes de llamar printf. */
    if(ir->slots)fprintf(out,"\tsubq $%d, %%rsp\n",((ir->slots*4+15)/16)*16);
    for(size_t i=0;i<ir->count;i++) {
        Instruction *p=&ir->items[i]; if(p->removed)continue;
        switch(p->op) {
        case LABEL: fprintf(out,".L%d:\n",p->label); break;
        case JUMP: fprintf(out,"\tjmp .L%d\n",p->label); break;
        case JFALSE:
            load(out,p->a,"%eax"); fprintf(out,"\ttestl %%eax, %%eax\n\tje .L%d\n",p->label); break;
        case OUTPUT:
            load(out,p->a,"%esi");
            fprintf(out,"\tleaq .Lformat(%%rip), %%rdi\n\txorl %%eax, %%eax\n\tcall printf@PLT\n"); break;
        case COPY: case UNARY: case BINARY:
            load(out,p->a,"%eax");
            if(p->op==UNARY)fprintf(out,"\tnegl %%eax\n");
            if(p->op==BINARY) {
                load(out,p->b,"%ecx"); const char *op=p->operator;
                if(!strcmp(op,"+"))fprintf(out,"\taddl %%ecx, %%eax\n");
                else if(!strcmp(op,"-"))fprintf(out,"\tsubl %%ecx, %%eax\n");
                else if(!strcmp(op,"*"))fprintf(out,"\timull %%ecx, %%eax\n");
                else if(!strcmp(op,"/"))
                    fprintf(out,"\tmovslq %%eax, %%rax\n\tmovslq %%ecx, %%rcx\n\tcqto\n\tidivq %%rcx\n");
                else {
                    const char *cc=!strcmp(op,"<")?"l":!strcmp(op,"<=")?"le":
                        !strcmp(op,">")?"g":!strcmp(op,">=")?"ge":!strcmp(op,"==")?"e":"ne";
                    fprintf(out,"\tcmpl %%ecx, %%eax\n\tset%s %%al\n\tmovzbl %%al, %%eax\n",cc);
                }
            }
            fprintf(out,"\tmovl %%eax, -%d(%%rbp)\n",4*(p->dst.slot+1)); break;
        }
    }
    fprintf(out,"\txorl %%eax, %%eax\n\tleave\n\tret\n.size main, .-main\n.section .note.GNU-stack,\"\",@progbits\n");
}
#ifndef _WIN32
/* Sin shell: las rutas y el fuente nunca se interpretan como comandos. */
static int process(char *const args[]) {
    fflush(NULL); pid_t pid=fork();
    if(pid<0) { perror("fork"); return 1; }
    if(!pid) { execvp(args[0],args); perror(args[0]); _exit(1); }
    int status;
    if(waitpid(pid,&status,0)<0) { perror("waitpid"); return 1; }
    return WIFEXITED(status)?WEXITSTATUS(status):1;
}
#endif
int run_binary(IR *ir) {
#ifdef _WIN32
    (void)ir;
    fputs("--run requiere Linux x86-64 con as y gcc (Colab o WSL).\n",stderr); return 1;
#else
    char directory[]="/tmp/lenguajito-XXXXXX";
    if(!mkdtemp(directory)) { perror("mkdtemp"); return 1; }
    char source[128],object[128],binary[128];
    snprintf(source,sizeof source,"%s/program.s",directory);
    snprintf(object,sizeof object,"%s/program.o",directory);
    snprintf(binary,sizeof binary,"%s/program",directory);
    FILE *f=fopen(source,"w"); int result=1;
    if(f) {
        assembly(f,ir); int failed=ferror(f); failed|=fclose(f)!=0;
        if(!failed) {
            char *as[]={"as","--64",source,"-o",object,NULL};
            char *gcc[]={"gcc",object,"-o",binary,NULL}; char *run[]={binary,NULL};
            if(!process(as)&&!process(gcc))result=process(run);
        }
    } else perror(source);
    unlink(source); unlink(object); unlink(binary); rmdir(directory); return result;
#endif
}
