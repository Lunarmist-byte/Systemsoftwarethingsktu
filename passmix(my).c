#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *fin = fopen("input.txt", "r");
    FILE *opt = fopen("optab.txt", "r");
    FILE *sym = fopen("symtab.txt", "w+");
    FILE *out = fopen("output.txt", "w+");
    if (!fin || !opt || !sym || !out) return 1;

    char la[10], mne[10], opnd[10], m1[10], o1[10], s[10];
    char name[10], obj1[10], obj2[10];
    int lc = 0, sa = 0, m[10], i = 0, j = 0;

    fscanf(fin, "%s%s%s", la, mne, opnd);
    if (!strcmp(mne, "START")) { sa = atoi(opnd); strcpy(name, la); lc = sa; }
    fscanf(fin, "%s%s%s", la, mne, opnd);

    while (strcmp(mne, "END")) {
        rewind(opt);
        if (!strcmp(la, "-")) {
            while (fscanf(opt, "%s%s", m1, o1) == 2)
                if (!strcmp(m1, mne)) {
                    m[i++] = lc + 1;
                    fprintf(sym, "%s\t*\n", opnd);
                    fprintf(out, "%s\t0000\n", o1);
                    lc += 3; break;
                }
        } else {
            if (!strcmp(mne, "WORD")) { lc += 3; fprintf(out, "%s\t#\n", opnd); }
            else if (!strcmp(mne, "RESW")) lc += 3 * atoi(opnd);
            else if (!strcmp(mne, "RESB")) lc += atoi(opnd);
            else if (!strcmp(mne, "BYTE")) {
                lc += strlen(opnd) - 2;
                for (int k = 2; opnd[k] != '\0' && opnd[k] != '\''; k++) putc(opnd[k], out);
                fprintf(out, "\t-\n");
            }
        }
        fscanf(fin, "%s%s%s", la, mne, opnd);
    }

    int pgmlen = lc - sa;
    printf("H^%s^%d^0%x\nT^00%d^0%x", name, sa, pgmlen, sa, pgmlen);

    rewind(out);
    while (fscanf(out, "%s%s", obj1, obj2) == 2) {
        if (!strcmp(obj2, "0000")) printf("^%s%s", obj1, obj2);
        else if (!strcmp(obj2, "-")) { printf("^"); for (int k = 0; obj1[k]; k++) printf("%d", obj1[k]); }
        else if (!strcmp(obj2, "#")) printf("^%s", obj1);
    }

    rewind(out);
    while (fscanf(out, "%s%s", obj1, obj2) == 2)
        if (strcmp(obj2, "0000") && strcmp(obj2, "-") && strcmp(obj2, "#"))
            printf("\nT^%s^02^%s", obj1, obj2);

    printf("\nE^00%d\n", sa);
    fclose(fin); fclose(opt); fclose(sym); fclose(out);
}