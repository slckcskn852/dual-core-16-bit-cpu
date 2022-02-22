/**
 * MADE BY SELCUK COSKUN 041801079
 * ASSEMBLY TO MACHINE CODE ASSEMBLER
 *
 * COMP206 DUAL CORE CPU PROJECT
 * THIS CODE TAKES AN ASSEMBLY TEXT FILE AND CONVERTS IT INTO TWO SEPERATE TEXT
 * FILES TO BE UTILIZED BY THE DUAL CORE CPU MADE IN LOGISIM
 *
 * */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define NEG_FILTER 0x00000FFF

char *op_codes_str[16] = {
        "BRZ",
        "BRN",
        "LDI",
        "LDM",
        "STR",
        "XOR",
        "NOT",
        "AND",
        "ORR",
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "NEG",
        "LSL",
        "LSR"
};

int main()
{
    printf("COMP206 - Dual Core Assembler\n\n");
    FILE* fin;
    FILE* fcore0;
    FILE* fcore1;
    char buff[255];

    fin = fopen("ASM.txt", "r"); //INPUT
    fcore0 = fopen("ASCII0.txt", "w");	//OUTPUT FOR CORE0
    fcore1 = fopen("ASCII1.txt", "w");	//OUTPUT FOR CORE1

    fprintf(fcore0, "v2.0 raw\n");
    fprintf(fcore1, "v2.0 raw\n");

    printf("Reading assembly file as....\n\n");

    while (fgets(buff, 255, (FILE*)fin)) {
        int instruction = 0;
        char op[3];
        char file_con;
        int y, x;

        /*
         * CMU 1 YYYYYYYYYYYY XXXXXXXXXXXX
         * CPU 0 00000000PPPP YYYYYYYYYYYY
         * */

        if (strstr(buff, "MOV")) {
            sscanf(buff, "%s %s %d %d", op, &file_con, &y, &x);
            printf("%s %c %d %d\n", op, file_con, y, x);

            y &= NEG_FILTER;
            x &= NEG_FILTER;

            instruction = 0x1000000;
            instruction |= y << 12 | x;

            printf("\t %X\n", instruction);
        } else {
            sscanf(buff, "%s %s %d", op, &file_con, &y);
            printf("%s %c %d\n", op, file_con, y);

            int i = 0;
            for (; i < 16; i++) {
                if (strstr(op, op_codes_str[i])) {
                    break;
                }
            }

            y &= NEG_FILTER;

            instruction = i << 12;
            instruction |= y;

            printf("\t %X\n", instruction);
        }
        //Core0
        if (file_con == '0') {
            fprintf(fcore0, "%X\n", instruction);
        }

        //Core 1
        if (file_con == '1') {
            fprintf(fcore1, "%X\n", instruction);
        }

        //Operation is for Both Cores, write to both files.
        if (file_con == 'X' || file_con == 'x') {
            fprintf(fcore0, "%X\n", instruction);
            fprintf(fcore1, "%X\n", instruction);
        }
    }
    printf("\n\nASCII file for Core #0 created as...'ASCII0.txt'...\n");
    printf("\nASCII file for Core #1 created as...'ASCII1.txt'...\n\n");

    //Close file pointers
    fclose(fin);
    fclose(fcore0);
    fclose(fcore1);

    return 0;
}
