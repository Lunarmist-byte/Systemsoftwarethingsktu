#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {
    FILE* obj = fopen("../ipop/objcode-rel", "r");
    if (!obj) {
        printf("Couldnot Open File...\n");
        return 1;
    }
    int locctr = 0, start_addr = 0, new_start, addr_diff = 0;
    char ch;
    printf("Enter New Starting Address: ");
    scanf("%X", &new_start);
    locctr = new_start;
    while ((ch = fgetc(obj)) != EOF) {
        if (ch == 'H') {
            while ((ch = fgetc(obj)) == ' ');
            char pname[6];
            int length;
            fscanf(obj, "%s %X %X\n", pname, &start_addr, &length);
            printf("Program Name: %c%s\n", ch, pname);
            printf("Starting Address: %X\n", start_addr);
            printf("Program Length: %X\n", length);
            addr_diff = new_start - start_addr;
        }
        else if (ch == 'T') {
            while ((ch = fgetc(obj)) == ' ');
            int start, length, rel;
            fscanf(obj, "%X %X %X ", &start, &length, &rel);

            char rel_bits[12];
            int code;
            char ocode[6];
            sprintf(rel_bits, "%012b", rel);
            for (int i = 0; i < length/3; i++) {
                fscanf(obj, "%X ", &code);
                if (rel_bits[i] == '1')
                    code += addr_diff;

                sprintf(ocode, "%06X", code);
                for (int i = 0; i < 6; i += 2)
                    printf("%X: %c%c\n", locctr++, ocode[i], ocode[i+1]);
            }
        }
    }
    return 0;
}






/*
I/P
H   COPY 001000 000012
T 001000 0F E00 001009 18100C 0C100F 000005 000003
E 001000

O/P
Enter New Starting Address: 3000
Program Name: COPY
Starting Address: 1000
Program Length: 12
3000: 00
3001: 30
3002: 09
3003: 18
3004: 30
3005: 0C
3006: 0C
3007: 30
3008: 0F
3009: 00
300A: 00
300B: 05
300C: 00
300D: 00
300E: 03

*/