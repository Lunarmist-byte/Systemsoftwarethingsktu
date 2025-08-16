#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYM 50
#define BUF 20

typedef struct { char lab[20]; int addr; } stab;
typedef struct { char opnd[20]; } optb;

int main() {
    FILE *src = fopen("input.txt", "r");
    FILE *symtab = fopen("symtab.txt", "w");
    FILE *optab = fopen("optab.txt", "r");
    FILE *inter = fopen("intermediate.txt", "w");
    if(!src || !symtab || !optab || !inter) { printf("File error\n"); return 1; }

    optb ot[MAX_SYM]; 
    int ot_cnt = 0; 
    char dummy[10];
    while(fscanf(optab, "%s %s", ot[ot_cnt].opnd, dummy) == 2) ot_cnt++;

    int start_addr = 0, locctr = 0;
    char label[BUF], opcode[BUF], operand[BUF];

    fprintf(inter,"Loc    Label     Opcode   Operand\n");
    fprintf(inter,"----------------------------------\n");

    fscanf(src,"%s %s %s",label,opcode,operand);
    if(strcmp(opcode,"START")==0) {
        start_addr = strtol(operand,NULL,16);
        locctr = start_addr;
        if(strcmp(label,"**")==0) strcpy(label,"-");
        fprintf(inter,"%04X   %-8s %-8s %-8s\n",locctr,label,opcode,operand);
        fscanf(src,"%s %s %s",label,opcode,operand);
    }

    while(strcmp(opcode,"END")!=0) {
        if(strcmp(label,"**")==0) strcpy(label,"-");
        fprintf(inter,"%04X   %-8s %-8s %-8s\n",locctr,label,opcode,operand);

        if(strcmp(label,"-")!=0) fprintf(symtab,"%s\t%04X\n",label,locctr);

        int found = 0;
        for(int i=0;i<ot_cnt;i++) {
            if(strcmp(opcode,ot[i].opnd)==0) { locctr+=3; found=1; break; }
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
        fscanf(src,"%s %s %s",label,opcode,operand);
    }

    if(strcmp(label,"**")==0) strcpy(label,"-");
    fprintf(inter,"       %-8s %-8s %-8s\n",label,opcode,operand);

    fclose(src); fclose(symtab); fclose(optab); fclose(inter);
    printf("Pass 1 complete.\n");
    return 0;
}
