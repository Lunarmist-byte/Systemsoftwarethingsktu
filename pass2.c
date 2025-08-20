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
    FILE *out=fopen("pass2_output.txt","w");
    if(!inter||!symtab||!optab||!out){printf("File error\n");return 1;}

    optb ot[MAX_SYM]; int ot_cnt=0;
    while(fscanf(optab,"%s %s",ot[ot_cnt].opnd,ot[ot_cnt].code)==2) ot_cnt++;

    stab st[MAX_SYM]; int st_cnt=0;
    while(fscanf(symtab,"%s %x",st[st_cnt].lab,&st[st_cnt].addr)==2) st_cnt++;

    char line[100],label[BUF],opcode[BUF],operand[BUF],objcode[20];
    int loc,base=-1;
    fprintf(out,"Loc    Label     Opcode   Operand   ObjectCode\n");
    fprintf(out,"-----------------------------------------------\n");

    fgets(line,sizeof(line),inter); /* header skip */
    fgets(line,sizeof(line),inter);

    while(fgets(line,sizeof(line),inter)){
        objcode[0]='\0'; label[0]=opcode[0]=operand[0]='\0';
        sscanf(line,"%x %s %s %s",&loc,label,opcode,operand);

        if(!strcmp(opcode,"START")||!strcmp(opcode,"END")||
           !strcmp(opcode,"RESW")||!strcmp(opcode,"RESB")){}
        else if(!strcmp(opcode,"BASE")){
            int val=find_sym(st,st_cnt,operand);
            if(val!=-1) base=val;
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
        else{
            int fmt4=0; char op_print[BUF]; strcpy(op_print,opcode);
            if(opcode[0]=='+'){ fmt4=1; memmove(opcode,opcode+1,strlen(opcode)); }

            char *ophex=find_op(ot,ot_cnt,opcode);
            if(ophex){
                int n=1,i=1,x=0,b=0,p=0,e=fmt4,addr=-1,disp=0;
                char temp[BUF]; strcpy(temp,operand);

                if(temp[0]=='#'){ n=0;i=1; memmove(temp,temp+1,strlen(temp)); }
                else if(temp[0]=='@'){ n=1;i=0; memmove(temp,temp+1,strlen(temp)); }
                char *px=strstr(temp,",X"); if(px){*px='\0'; x=1;}

                addr=find_sym(st,st_cnt,temp);
                if(addr==-1 && n==0 && i==1) disp=atoi(temp); 
                else if(addr!=-1) disp=addr;

                int op; sscanf(ophex,"%X",&op);
                op&=0xFC; op|=(n<<1)|i;

                if(!fmt4){
                    int next=loc+3;
                    if(addr==-1 && n==0 && i==1){ b=0;p=0; }
                    else{
                        int d=addr-next;
                        if(d>=-2048&&d<=2047){ p=1; disp=d&0xFFF; }
                        else if(base!=-1){ int db=addr-base;
                            if(db>=0&&db<=4095){ b=1; disp=db; }
                            else { fmt4=1; e=1; }
                        }else { fmt4=1; e=1; }
                    }
                }
                if(!fmt4){
                    int xbpe=(x<<3)|(b<<2)|(p<<1)|e;
                    int obj=(op<<16)|(xbpe<<12)|(disp&0xFFF);
                    sprintf(objcode,"%06X",obj);
                }else{
                    int xbpe=(x<<3)|(0<<2)|(0<<1)|1;
                    int obj=(op<<24)|(xbpe<<20)|(disp&0xFFFFF);
                    sprintf(objcode,"%08X",obj);
                }
                strcpy(opcode,op_print);
            }
        }
        fprintf(out,"%04X   %-8s %-8s %-8s %-s\n",loc,label,opcode,operand,objcode);
    }
    fclose(inter);fclose(symtab);fclose(optab);fclose(out);
    printf("Pass 2 complete.\n");
    return 0;
}