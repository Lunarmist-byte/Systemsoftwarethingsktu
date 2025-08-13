#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SYM 50
#define BUF 20

typedef struct symtab {
    char lab[20];
    int addr;
} stab;

typedef struct optab {
    char opnd[20];
    int addr;
} optb;

int st_cnt=0,otsize=0,start=0,locctr=0;
stab st[MAX_SYM];
optb otab[MAX_SYM];

void run(FILE* inter, char opcode[], char label[], char op[]) {
    if (strcmp(opcode, "START")==0) {
        start=locctr=(int)strtol(op, NULL, 16);
        fprintf(inter, "%X\t%s\t%s\t%s\n", start, label, opcode, op);
        return;
    }

    int inc;
    if (strcmp(label, "**") !=0) {
        int found=0;
        for (int i=0; i<st_cnt; i++) {
            if (!strcmp(st[i].lab, label)) {
                found=1;
                break;
            }
        }
        if (found) {
            printf("Symbol %s Already Declared!!!\n", label);
            exit(1);
        } else {
            strcpy(st[st_cnt].lab, label);
            st[st_cnt++].addr=locctr;
        }
    }

    if (!strcmp(opcode, "BYTE"))
        inc=strlen(op);
    else if (!strcmp(opcode, "WORD"))
        inc=3;
    else if (!strcmp(opcode, "RESB"))
        inc=(int)strtol(op, NULL, 10);
    else if (!strcmp(opcode, "RESW"))
        inc=3 *(int)strtol(op, NULL, 10);
    else {
        int found=0;
        for (int i=0; i < otsize; i++) {
            if (!strcmp(opcode, otab[i].opnd)) {
                found=1;
                break;
            }
        }
        if (found)
            inc=3;
        else if (!strcmp(opcode, "END"))
            inc=0;
        else {
            printf("OPCODE %s NOT FOUND!!!\n", opcode);
            exit(1);
        }
    }

    fprintf(inter, "%X\t%s\t%s\t%s\n", locctr, label, opcode, op);
    locctr+=inc;
}

int main() {
    FILE* src=fopen("input.txt", "r");
    FILE* optabFile=fopen("optab", "r");
    FILE* inter=fopen("intermediate.txt", "w");
    FILE* symtab=fopen("symtab", "w");

    if (!src || !optabFile || !inter || !symtab) {
        printf("Could not open file...\n");
        return 0;
    }

    char label[BUF], opcode[BUF], op[BUF];
    int opint;

    while (fscanf(optabFile, "%s\t%d", opcode, &opint) > 0) {
        strcpy(otab[otsize].opnd, opcode);
        otab[otsize++].addr=opint;
    }

    printf("\nIntermediate File...\n");
    printf("%10s%10s%10s%10s\n", "ADDRESS", "LABEL", "OPCODE", "OPERAND");

    do {
        if (fscanf(src, "%s\t%s\t%s", label, opcode, op) !=3) break;
        printf("%10X%10s%10s%10s\n", locctr, label, opcode, op);
        run(inter, opcode, label, op);
    } while (strcmp(opcode, "END") !=0);

    for (int i=0; i<st_cnt; i++) {
        fprintf(symtab, "%s\t%X\n", st[i].lab, st[i].addr);
    }

    printf("Intermediate file saved as \"intermediate.txt\"\n");
    printf("SYMTAB saved as \"symtab\"\n");

    return 0;
}

