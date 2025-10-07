#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BUF 1000


int main() {
    FILE* obj = fopen("../ipop/objcode", "r");
    if (!obj) {
        printf("Couldnot open file...\n");
        return 1;
    }
    int locctr = 0;
    char ch;
    while ((ch = fgetc(obj)) != EOF) {
        if (ch == 'H') {
            while ((ch = fgetc(obj)) == ' ');
            char pname[6];
            int start, length;
            fscanf(obj, "%s %X %X\n", pname, &start, &length);
            locctr = start;
            printf("Program Name: %c%s\n", ch, pname);
            printf("Starting Address: %X\n", start);
            printf("Program Length: %X\n", length);
        }
        else if (ch == 'T') {
            while ((ch = fgetc(obj)) == ' ');
            int start, length;
            fscanf(obj, "%X %X ", &start, &length);
            for (int i = 0; i < length; i++) {
                ch = fgetc(obj);
                if (ch == ' ') {
                    i--;
                    continue;
                }
                char ch1 = fgetc(obj);
                printf("%X: %c%c\n", locctr++, ch, ch1);
            }
        }
    }
    return 0;
}











/*
I/P
----
H   COPY 001000 000012
T 001000 0F 001009 18100C 0C100F 000005 000003
E 001000


O/P
----
Program Name: COPY
Starting Address: 1000
Program Length: 12
1000: 00
1001: 10
1002: 09
1003: 18
1004: 10
1005: 0C
1006: 0C
1007: 10
1008: 0F
1009: 00
100A: 00
100B: 05
100C: 00
100D: 00
100E: 03

*/