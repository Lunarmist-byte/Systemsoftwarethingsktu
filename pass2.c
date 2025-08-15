#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SYM 50
#define BUF 20

typedef struct symtab{
    char lab[20];
    int addr;
}stab;

typedef struct optab{
    char opnd[20];
    int code;
}optb;

int main(){
    FILE*inter=fopen("intermediate.txt","r");
    FILE*symtab=fopen("symtab","r");
    FILE*optab=fopen("optab","r");
    FILE*obj=fopen("object.txt","w");

    if(!inter||!symtab||!optab||!obj){
        printf("Could not open file...\n");
        return 1;
    }

    stab st[MAX_SYM];
    optb ot[MAX_SYM];
    int st_cnt=0,ot_cnt=0,start_addr=0,locctr=0,prog_len=0;
    char label[BUF],opcode[BUF],operand[BUF],addr[BUF];

    while(fscanf(symtab,"%s %x",st[st_cnt].lab,&st[st_cnt].addr)==2){
        st_cnt++;
    }

    while(fscanf(optab,"%s %d",ot[ot_cnt].opnd,&ot[ot_cnt].code)==2){
        ot_cnt++;
    }

    fscanf(inter,"%x %s %s %s",&start_addr,label,opcode,operand);
    prog_len=0;

    fprintf(obj,"H^%s^%06X^%06X\n",label,start_addr,prog_len);

    char text_rec[100]="";
    int text_len=0,text_start=start_addr;
    locctr=start_addr;

    printf("\nObject Code:\n");
    while(fscanf(inter,"%x %s %s %s",&locctr,label,opcode,operand)==4){
        if(!strcmp(opcode,"START")||!strcmp(opcode,"END"))continue;

        char obj_code[10]="";
        int opcode_val=0,sym_addr=0,found=0;

        for(int i=0;i<ot_cnt;i++){
            if(!strcmp(opcode,ot[i].opnd)){
                opcode_val=ot[i].code;
                found=1;
                break;
            }
        }

        if(found){
            for(int i=0;i<st_cnt;i++){
                if(!strcmp(operand,st[i].lab)){
                    sym_addr=st[i].addr;
                    sprintf(obj_code,"%02X%04X",opcode_val,sym_addr);
                    break;
                }
            }
        }else if(!strcmp(opcode,"BYTE")){
            if(operand[0]=='C'){
                for(int i=2;operand[i]!='\''&&operand[i]!='\0';i++){
                    sprintf(obj_code+strlen(obj_code),"%02X",operand[i]);
                }
            }else if(operand[0]=='X'){
                strncpy(obj_code,operand+2,strlen(operand)-3);
            }
        }else if(!strcmp(opcode,"WORD")){
            int val=atoi(operand);
            sprintf(obj_code,"%06X",val);
        }

        printf("%04X: %s\n",locctr,obj_code);

        if(strlen(text_rec)+strlen(obj_code)>60||strcmp(opcode,"RESB")==0||strcmp(opcode,"RESW")==0){
            if(text_len>0){
                fprintf(obj,"T^%06X^%02X^%s\n",text_start,text_len/2,text_rec);
            }
            text_rec[0]='\0';
            text_len=0;
            text_start=locctr;
        }

        if(strlen(obj_code)>0){
            strcat(text_rec,obj_code);
            text_len+=strlen(obj_code);
        }
    }

    if(text_len>0){
        fprintf(obj,"T^%06X^%02X^%s\n",text_start,text_len/2,text_rec);
    }

    fprintf(obj,"E^%06X\n",start_addr);

    fseek(inter,0,SEEK_SET);
    fscanf(inter,"%x %s %s %s",&start_addr,label,opcode,operand);
    int end_addr=locctr;
    prog_len=end_addr-start_addr;

    fseek(obj,0,SEEK_SET);
    fprintf(obj,"H^%s^%06X^%06X\n",label,start_addr,prog_len);

    fclose(inter);
    fclose(symtab);
    fclose(optab);
    fclose(obj);

    printf("Object code saved as \"object.txt\"\n");
    return 0;
}
