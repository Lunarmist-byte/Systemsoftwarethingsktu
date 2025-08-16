#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYM 50
#define BUF 20

typedef struct { char lab[20]; int addr; } stab;
typedef struct { char opnd[20]; char code[10]; } optb;

int main() {
    FILE *inter=fopen("intermediate.txt","r");
    FILE *symtab=fopen("symtab.txt","r");
    FILE *optab=fopen("optab.txt","r");
    FILE *out=fopen("pass2_output.txt","w");
    if(!inter||!symtab||!optab||!out){printf("File error\n");return 1;}

    optb ot[MAX_SYM]; int ot_cnt=0;
    while(fscanf(optab,"%s %s",ot[ot_cnt].opnd,ot[ot_cnt].code)==2) ot_cnt++;

    stab st[MAX_SYM]; int st_cnt=0;
    while(fscanf(symtab,"%s %x",st[st_cnt].lab,&st[st_cnt].addr)==2) st_cnt++;

    char line[100],label[BUF],opcode[BUF],operand[BUF],objcode[20];
    int loc;
    fprintf(out,"Loc    Label     Opcode   Operand   ObjectCode\n");
    fprintf(out,"-----------------------------------------------\n");

    fgets(line,sizeof(line),inter);
    fgets(line,sizeof(line),inter);

    while(fgets(line,sizeof(line),inter)){
        objcode[0]='\0'; label[0]=opcode[0]=operand[0]='\0';
        sscanf(line,"%x %s %s %s",&loc,label,opcode,operand);

        if(!strcmp(opcode,"START")||!strcmp(opcode,"END")||
           !strcmp(opcode,"RESW")||!strcmp(opcode,"RESB")){
        }
        else if(!strcmp(opcode,"WORD")){
            sprintf(objcode,"%06X",atoi(operand));
        }
        else if(!strcmp(opcode,"BYTE")){
            if(operand[0]=='C'){
                for(int i=2;i<strlen(operand)-1;i++)
                    sprintf(objcode+strlen(objcode),"%02X",operand[i]);
            }else if(operand[0]=='X'){
                strncpy(objcode,operand+2,strlen(operand)-3);
                objcode[strlen(operand)-3]='\0';
            }
        }
        else{
            char opcode_hex[10]=""; int addr=0,indexed=0;
            for(int i=0;i<ot_cnt;i++) if(!strcmp(opcode,ot[i].opnd)){strcpy(opcode_hex,ot[i].code);break;}
            if(strlen(opcode_hex)){
                char tempop[BUF]; strcpy(tempop,operand);
                char *p=strstr(tempop,",X"); if(p){*p='\0';indexed=1;}
                for(int i=0;i<st_cnt;i++) if(!strcmp(tempop,st[i].lab)){addr=st[i].addr;break;}
                if(indexed) addr|=0x8000;
                sprintf(objcode,"%s%04X",opcode_hex,addr);
            }
        }
        fprintf(out,"%04X   %-8s %-8s %-8s %-s\n",loc,label,opcode,operand,objcode);
    }
    fclose(inter);fclose(symtab);fclose(optab);fclose(out);
    printf("Pass 2 complete.\n");
    return 0;
}
