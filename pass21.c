#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUF 20
#define MAX 100
#define REC_BUFF 70
#define REC_BYTES 30

typedef struct tab {
    char symbol[BUF];
    int address;
} tab;

typedef struct inter {
    int addr;
    char label[BUF];
    char opcode[BUF];
    char opnd[BUF];
} inter;

typedef struct record {
    int start_addr;
    int len;
    char rec[REC_BUFF];
} rec;

int otsize = 0, stsize = 0, lcnt = 0;
tab optab[BUF];
tab symtab[BUF];
inter itr[MAX];
rec text;


void flush_text(FILE *out) {
    if (text.len > 0) {
        fprintf(out, "T %06X %02X %s\n", text.start_addr, text.len, text.rec);
        text.len = 0;
        text.rec[0] = '\0';
        text.start_addr = -1;
    }
}

void start_text_if_needed(int addr) {
    if (text.len == 0) {
        text.start_addr = addr;
        text.rec[0] = '\0';
    }
}


void text_write_bytes(const char *hex_no_spaces, int nbytes, int locctr, FILE *out) {
    if (text.len + nbytes > REC_BYTES) {
        flush_text(out);
    }
    start_text_if_needed(locctr);
    if (text.rec[0] != '\0') strcat(text.rec, " ");
    strcat(text.rec, hex_no_spaces);
    text.len += nbytes;
}


void int_to_hex(int value, int width_bytes, char *outhex) {
    char fmt[8];
    sprintf(fmt, "%%0%dX", width_bytes * 2);
    sprintf(outhex, fmt, value & ((1 << (width_bytes * 8)) - 1));
}


int parse_BYTE(const char *opnd, char *hex_out) {
    int nbytes = 0;
    if (opnd[0] == 'C' && opnd[1] == '\'' && opnd[strlen(opnd)-1] == '\'') {
        const char *p = opnd + 2;
        size_t len = strlen(opnd);
        size_t end = len - 1;
        hex_out[0] = '\0';
        for (; p < opnd + end; ++p) {
            char tmp[3*2];
            sprintf(tmp, "%02X", (unsigned char)*p);
            strcat(hex_out, tmp);
            nbytes++;
        }
    } else if (opnd[0] == 'X' && opnd[1] == '\'' && opnd[strlen(opnd)-1] == '\'') {
        size_t len = strlen(opnd);
        size_t datalen = len - 3;
        const char *p = opnd + 2;
        char buf[REC_BUFF];
        strncpy(buf, p, datalen);
        buf[datalen] = '\0';
        buf[datalen - 1] = '\0';
        for (char *q = buf; *q; ++q) {
            if (!isxdigit((unsigned char)*q)) return -1;
            *q = (char)toupper((unsigned char)*q);
        }
        if (strlen(buf) % 2 != 0) return -1;
        strcpy(hex_out, buf);
        nbytes = (int)(strlen(buf) / 2);
    } else {
        int val = atoi(opnd);
        if (val < 0 || val > 255) return -1;
        sprintf(hex_out, "%02X", val & 0xFF);
        nbytes = 1;
    }
    return nbytes;
}


void pass2(FILE* out, FILE* list) {
    text.len = 0;
    text.rec[0] = '\0';
    text.start_addr = -1;

    int start_addr = itr[0].addr;
    const char *progname = itr[0].label[0] ? itr[0].label : "NONAME";
    int proglen = (lcnt > 0) ? (itr[lcnt-1].addr - start_addr) : 0;

    fprintf(out,  "H %06s %06X %06X\n", progname, start_addr, proglen);
    fprintf(list, "%X\t%s\t%s\t%s\t**\n", itr[0].addr, itr[0].label, itr[0].opcode, itr[0].opnd);

    for (int i = 1; i < lcnt; i++) {
        char objhex[REC_BUFF]; objhex[0] = '\0';
        int objbytes = 0;

        int opi = -1;
        for (int j = 0; j < otsize; j++) {
            if (!strcmp(optab[j].symbol, itr[i].opcode)) { opi = j; break; }
        }

        if (opi != -1) {
            char opc[3]; int_to_hex(optab[opi].address, 1, opc);
            int addr_val = 0;
            if (strcmp(itr[i].opnd, "**")) {
                int stf = -1;
                for (int j = 0; j < stsize; j++) {
                    if (!strcmp(symtab[j].symbol, itr[i].opnd)) { stf = j; break; }
                }
                if (stf == -1) {
                    fprintf(stderr, "Undefined symbol: %s at %X\n", itr[i].opnd, itr[i].addr);
                    exit(1);
                }
                addr_val = symtab[stf].address;
            }
            char addrhex[5]; int_to_hex(addr_val, 2, addrhex);
            sprintf(objhex, "%s%s", opc, addrhex);
            objbytes = 3;
            text_write_bytes(objhex, objbytes, itr[i].addr, out);
        }
        else if (!strcmp(itr[i].opcode, "BYTE")) {
            int nb = parse_BYTE(itr[i].opnd, objhex);
            if (nb < 0) {
                fprintf(stderr, "Invalid BYTE operand: %s at %X\n", itr[i].opnd, itr[i].addr);
                exit(1);
            }
            objbytes = nb;
            text_write_bytes(objhex, objbytes, itr[i].addr, out);
        }
        else if (!strcmp(itr[i].opcode, "WORD")) {
            int val = atoi(itr[i].opnd);
            int_to_hex(val, 3, objhex);
            objbytes = 3;
            text_write_bytes(objhex, objbytes, itr[i].addr, out);
        }
        else if (!strcmp(itr[i].opcode, "RESB") || !strcmp(itr[i].opcode, "RESW")) {
            flush_text(out);
        } else if (!strcmp(itr[i].opcode, "END")) {
        } else {
            flush_text(out);
        }

        fprintf(list, "%X\t%s\t%s\t%s\t%s\n",
                itr[i].addr, itr[i].label, itr[i].opcode, itr[i].opnd,
                (objhex[0] ? objhex : "**"));
    }

    flush_text(out);
    fprintf(out, "E %06X\n", start_addr);
}

int main(void) {
    FILE *inter = fopen("ipop/intermediate.sic", "r");
    FILE *otab  = fopen("ipop/optab", "r");
    FILE *stab  = fopen("ipop/symtab", "r");
    FILE *out   = fopen("ipop/output", "w");
    FILE *list  = fopen("ipop/listfile.sic", "w");
    if (!inter || !otab || !stab || !out || !list) {
        printf("Could not open files...\n");
        return 1;
    }

    // read optab
    while (fscanf(otab, "%s %X", optab[otsize].symbol, &optab[otsize].address) == 2) otsize++;
    // read symtab
    while (fscanf(stab, "%s %X", symtab[stsize].symbol, &symtab[stsize].address) == 2) stsize++;
    // read intermediate
    while (fscanf(inter, "%X %s %s %s", &itr[lcnt].addr, itr[lcnt].label, itr[lcnt].opcode, itr[lcnt].opnd) == 4) lcnt++;

    pass2(out, list);
    printf("Object Code Saved in \"output\" in ipop\n");
    printf("Listing file Saved in \"listfile.sic\" in ipop\n");

    fclose(inter); fclose(otab); fclose(stab); fclose(out); fclose(list);
    return 0;
}