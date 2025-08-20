#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYM 50
#define BUF 20

typedef struct { char lab[20]; int addr; } stab;
typedef struct { char opnd[20]; char code[10]; } optb;

int find_sym(stab *st,int st_cnt,char *lab){
    for(int i=0;i<st_cnt;i++) if(!strcmp(st[i].lab,lab)) return st[i].addr;
    return -1;
}
char* find_op(optb *ot,int ot_cnt,char *op){
    for(int i=0;i<ot_cnt;i++) if(!strcmp(ot[i].opnd,op)) return ot[i].code;
    return NULL;
}

int main(){
    FILE *inter=fopen("intermediate.txt","r");
    FILE *symtab=fopen("symtab.txt","r");
    FILE *optab=fopen("optab.txt","r");
    FILE *list=fopen("pass2_output.txt","w");
    FILE *obj=fopen("object_program.txt","w");
    if(!inter||!symtab||!optab||!list||!obj){printf("File error\n");return 1;}

    optb ot[MAX_SYM]; int ot_cnt=0;
    while(fscanf(optab,"%s %s",ot[ot_cnt].opnd,ot[ot_cnt].code)==2) ot_cnt++;

    stab st[MAX_SYM]; int st_cnt=0;
    while(fscanf(symtab,"%s %x",st[st_cnt].lab,&st[st_cnt].addr)==2) st_cnt++;

    char line[100],label[BUF],opcode[BUF],operand[BUF],objcode[20];
    char program_name[20]=""; int startAddr=0,progLen=0,loc,base=-1,firstExec=0;
    char textrec[1000]="",temp[100]; int textlen=0,textStart=0,textInit=0;

    fprintf(list,"Loc    Label     Opcode   Operand   ObjectCode\n");
    fprintf(list,"-----------------------------------------------\n");

    fgets(line,sizeof(line),inter); /* header skip */
    fgets(line,sizeof(line),inter);

    /* First line START */
    fgets(line,sizeof(line),inter);
    sscanf(line,"%x %s %s %s",&loc,label,opcode,operand);
    strcpy(program_name,label);
    if(!strcmp(opcode,"START")){
        sscanf(operand,"%x",&startAddr);
        firstExec=startAddr;
        textStart=startAddr;
    }
    fprintf(list,"%04X   %-8s %-8s %-8s\n",loc,label,opcode,operand);

    /* Now read rest */
    while(fgets(line,sizeof(line),inter)){
        objcode[0]='\0'; label[0]=opcode[0]=operand[0]='\0';
        sscanf(line,"%x %s %s %s",&loc,label,opcode,operand);

        if(!strcmp(opcode,"END")){
            progLen=loc-startAddr;
            break;
        }
        else if(!strcmp(opcode,"WORD")){
            sprintf(objcode,"%06X",atoi(operand));
        }
        else if(!strcmp(opcode,"BYTE")){
            if(operand[0]=='C'){
                for(int i=2;i<(int)strlen(operand)-1;i++)
                    sprintf(objcode+strlen(objcode),"%02X",operand[i]);
            }else if(operand[0]=='X'){
                strncpy(objcode,operand+2,strlen(operand)-3);
                objcode[strlen(operand)-3]='\0';
            }
        }
        else if(!strcmp(opcode,"RESW")||!strcmp(opcode,"RESB")){
            /* flush current text record */
            if(textlen>0){
                sprintf(temp,"T^%06X^%02X%s\n",textStart,textlen,textrec);
                fputs(temp,obj);
                textrec[0]='\0'; textlen=0; textInit=0;
            }
        }
        else{
            /* normal instruction */
            char *ophex=find_op(ot,ot_cnt,opcode);
            if(ophex){
                int addr=find_sym(st,st_cnt,operand);
                int op; sscanf(ophex,"%X",&op);
                sprintf(objcode,"%02X%04X",op,addr==-1?0:addr);
            }
        }

        fprintf(list,"%04X   %-8s %-8s %-8s %-s\n",loc,label,opcode,operand,objcode);

        if(strlen(objcode)>0){
            if(!textInit){ textStart=loc; textInit=1; }
            sprintf(temp,"^%s",objcode);
            strcat(textrec,temp);
            textlen+=strlen(objcode)/2;
            if(textlen>=30){ /* flush */
                sprintf(temp,"T^%06X^%02X%s\n",textStart,textlen,textrec);
                fputs(temp,obj);
                textrec[0]='\0'; textlen=0; textInit=0;
            }
        }
    }

    /* flush last text record */
    if(textlen>0){
        sprintf(temp,"T^%06X^%02X%s\n",textStart,textlen,textrec);
        fputs(temp,obj);
    }

    /* header & end */
    fprintf(obj,"H^%-6s^%06X^%06X\n",program_name,startAddr,progLen);
    fprintf(obj,"E^%06X\n",firstExec);

    fclose(inter);fclose(symtab);fclose(optab);fclose(list);fclose(obj);
    printf("Pass 2 complete with object program.\n");
    return 0;
}