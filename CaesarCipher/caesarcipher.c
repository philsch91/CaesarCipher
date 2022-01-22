//
//  caesarcipher.c
//  CaesarCipher
//
//  Created by philipp on 13.01.22.
//  Copyright © 2022 philipp. All rights reserved.
//

#include "caesarcipher.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int op = OP_NONE;
char str[BUFFERSIZE];
int offset = 3;

int a2i(char *ch) {
    char c = *ch;
    int ai = c + 0;
    return ai;
}

char itoa(int ai) {
    char ch = ai + '\0';
    return ch;
}

int encode(char *str, int buffersize, int offset) {
    int i = 0;
    while (*(str + i) != '\n') {
        char c = *(str + i);
        //printf("%c", c);
        //int ai = atoi((str + i));
        //int ai = a2i((str + i));
        int ai = a2i(&c);
        //printf("%d\n", ai);
        ai += offset;
        //printf("%d\n", ai);
        //w = 119 + 10 = 129 (>122)
        if (ai > (97 + 25)){
            int os = (ai % 97) - 26;
            c = 'a';
            ai = a2i(&c);   //97
            ai += os;       //97 + 6 = 103
        }
        //printf("%d\n", ai);
        c = itoa(ai);
        *(str + i) = c;
        //printf("%c", *(str + i));
        i++;
    }
    return 0;
}

int decode(char *str, int buffersize, int offset) {
    int i = 0;
    while (*(str + i) != '\n') {
        char c = *(str + i);
        //printf("%c", c);
        //int ai = atoi((str + i));
        //int ai = a2i((str + i));
        int ai = a2i(&c);
        //printf("%d\n", ai);
        ai -= offset;
        if (ai < (122 - 25)) {
            int os = offset - ((ai + offset) % 97);
            c = 'z';
            ai = a2i(&c);
            ai -= os;
            ai++;
        }
        c = itoa(ai);
        *(str + i) = c;
        //printf("%c", *(str + i));
        i++;
    }
    return 0;
}

int showValue(char *str, int buffersize) {
    int i = 0;
    while (*(str + i) != '\n') {
        printf("%c", *(str + i));
        i++;
    }
    printf("\n");
    //printf("i: %d", i);
    return i;
}

int readInput(int argc, const char * argv[]) {
    // option -d/-e
    printf("[options]\n");
    printf("d: decode\n");
    printf("e: encode\n");
    printf("option: ");

    char opc;
    int rc;
    do {
        rc = scanf("%c", &opc);
    } while (getchar() != '\n');

    if (rc == EOF) {
        printf("error with stdin - exit\n");
        return 1;
    }

    if (rc != 1) {
        printf("invalid option %c - exit\n", opc);
        return 1;
    }

    //int op = 0;     // global scope
    if (opc == 'd') {
        op = OP_DECODE;
    } else if (opc == 'e') {
        op = OP_ENCODE;
    } else {
        printf("invalid option %c - exit\n", opc);
        return 1;
    }

    printf("Option: %c\n", opc);

    // string
    printf("Value: ");
    char *res;
    //char ch;
    //char str[BUFFERSIZE];   // global scope
    res = fgets(str, BUFFERSIZE, stdin);
    //sscanf(str, "%c", ch);
    //getchar();
    
    if (res == NULL) {
        printf("error reading input value - exit\n");
        return 1;
    }

    printf("Value: %s", str);

    // offset -o
    printf("Offset: ");
    //int offset;       // global scope
    char buf[8];
    res = fgets(buf, 8, stdin);

    if (res == NULL) {
        printf("error reading input value - exit\n");
        return 1;
    }

    rc = sscanf(buf, "%d", &offset);
    //getchar();
    
    if (rc == EOF) {
        printf("error with stdin - exit\n");
        return 1;
    }
    
    if (rc != 1) {
        printf("invalid value %s - exit\n", buf);
        return 1;
    }
    
    if (offset < 0 || offset > 26) {
        printf("invalid offset: 0 <= offset <= 26\n");
        return 1;
    }

    printf("Offset: %d\n", offset);

    return 0;
}

int readInputValues(int argc, const char * argv[]) {
    int rc, charcount;
    char repeat = 'y';

    while (repeat == 'y' || repeat == 'Y') {
        rc = readInput(argc, argv);
        if (rc > 0) return rc;

        if (op == OP_DECODE) {
            //printf("OP_DECODE\n");
            rc = decode(str, BUFFERSIZE, offset);
            charcount = showValue(str, BUFFERSIZE);
            printf("character count: %d\n", charcount);
        } else if (op == OP_ENCODE) {
            //printf("OP_ENCODE\n");
            rc = encode(str, BUFFERSIZE, offset);
            charcount = showValue(str, BUFFERSIZE);
            printf("character count: %d\n", charcount);
        }

        printf("Continue? Yes(Y/y) No(N/n) ");
        char repeatbuffer[2];
        fgets(repeatbuffer, sizeof(repeatbuffer), stdin);
        int res = sscanf(repeatbuffer, "%c", &repeat);
        getchar();
    }

    return 0;
}

void usage(char *prog) {
    printf("%s [option]\n", prog);
    printf(" -d : decode\n");
    printf(" -e : encode\n");
    printf(" -o : offset\n");
    printf(" -h : help\n");
    printf(" -v : version\n");
    printf("\n");
}

int main(int argc, const char * argv[]) {
    printf("Caesar Cipher %s\n", VERSION);

    int c;
    //extern char *optarg;
    opterr = 0;             // optarg, opterr, optind, optopt

    //int op = OP_NONE;     // global scope
    char *optval = NULL;
    //char *offset = NULL;

    int osres;

    while((c = getopt(argc, argv, "d:e:o:hv")) != -1) {
        switch (c) {
            case 'd':
                op = OP_DECODE;
                optval = optarg;
                break;
            case 'e':
                op = OP_ENCODE;
                optval = optarg;
                break;
            case 'o':
                //offset = 3;
                osres = sscanf(optarg, "%d", &offset);

                if (osres == EOF) {
                    printf("error with stdin - exit\n");
                    return 1;
                }

                if (osres != 1) {
                    printf("invalid value %s - exit\n", optarg);
                    return 1;
                }
                break;
            case 'h':
                op = OP_HELP;
                break;
            case 'v':
                printf("%s\n", VERSION);
                return 0;
                break;
            case '?':
                op = OP_HELP;
                break;
            default:
                op = OP_NONE;
                break;
        }
    }

    if (op == OP_NONE) {
        int res = readInputValues(argc, argv);
        return res;
    }

    if (op == OP_HELP) {
        usage(argv[0]);
        return 0;
    }

    int rc, charcount;
    unsigned long strlength;
    switch (op) {
        case OP_DECODE:
            //printf("OP_DECODE\n");
            memset(str, '\n', sizeof(str));
            strcpy(str, optval);
            strlength = strlen(str);
            //printf("strlen(str): %lu\n", strlength);
            str[strlength] = '\n';

            rc = decode(str, BUFFERSIZE, offset);
            charcount = showValue(str, BUFFERSIZE);
            printf("character count: %d\n", charcount);
            break;
        case OP_ENCODE:
            //printf("OP_ENCODE\n");
            memset(str, '\n', sizeof(str));
            strcpy(str, optval);
            strlength = strlen(str);
            //printf("strlen(str): %lu\n", strlength);
            str[strlength] = '\n';

            rc = encode(str, BUFFERSIZE, offset);
            charcount = showValue(str, BUFFERSIZE);
            printf("character count: %d\n", charcount);
            break;
        default:
            printf("default");
            break;
    }

    return 0;
}
