#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYM 50
#define BUF 20

typedef struct { char lab[20]; int addr; } stab;
typedef struct { char opnd[20]; int len; } optb;

int main() {
    FILE *src=fopen("source.txt","r");
    FILE *symtab=fopen("symtab","w");
    FILE *optab=fopen("optab","r");
    FILE *inter=fopen("intermediate.txt","w");
    if(!src||!symtab||!optab||!inter){ printf("File error\n"); return 1; }

    optb ot[MAX_SYM];
    int ot_cnt=0;
    while(fscanf(optab,"%s%d",ot[ot_cnt].opnd,&ot[ot_cnt].len)==2) ot_cnt++;

    int start_addr=0,locctr=0;
    char label[BUF],opcode[BUF],operand[BUF];
    fscanf(src,"%s%s%s",label,opcode,operand);

    if(strcmp(opcode,"START")==0) {
        start_addr=strtol(operand,NULL,16);
        locctr=start_addr;
        fprintf(inter,"%04X %s %s %s\n",locctr,label,opcode,operand);
        fscanf(src,"%s%s%s",label,opcode,operand);
    }

    while(strcmp(opcode,"END")!=0) {
        fprintf(inter,"%04X %s %s %s\n",locctr,label,opcode,operand);
        if(strcmp(label,"-")!=0) fprintf(symtab,"%s %04X\n",label,locctr);

        int found=0;
        for(int i=0;i<ot_cnt;i++) {
            if(strcmp(opcode,ot[i].opnd)==0) { locctr+=ot[i].len; found=1; break; }
        }

        if(!found) {
            if(strcmp(opcode,"WORD")==0) locctr+=3;
            else if(strcmp(opcode,"RESW")==0) locctr+=3*atoi(operand);
            else if(strcmp(opcode,"RESB")==0) locctr+=atoi(operand);
            else if(strcmp(opcode,"BYTE")==0) {
                if(operand[0]=='C') locctr+=strlen(operand)-3;
                else if(operand[0]=='X') locctr+=(strlen(operand)-3)/2;
            } else printf("Error: Invalid opcode %s\n",opcode);
        }

        fscanf(src,"%s%s%s",label,opcode,operand);
    }
    fprintf(inter,"    %s %s %s\n",label,opcode,operand);

    fclose(src); fclose(symtab); fclose(optab); fclose(inter);
    printf("Pass 1 complete.\n");
    return 0;
}
