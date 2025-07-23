// Copyright (c) 2025, London Sheard
// Licensed under the 3-Clause Sheard License
// See the LICENSE file in project root for details.
//
// Created by London Sheard on 22/07/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void echo() {
    char* line = NULL;
    size_t len = 0;
    ssize_t read = getline(&line, &len, stdin); // read full line, (dynamic realloc)

    if (read != -1) {
        printf("%s", line);
    } else {
        exit(0);
    }
    free(line); // must free
}

int print_contents(const char* inputfile) {
    FILE* file = fopen(inputfile, "r");
    /* Open file, display error if not found*/
    if (!file) {
        char errormsg[256];
        snprintf(errormsg, sizeof(errormsg), "%s%s%s", "Could not open file '", inputfile, "'");
        perror(errormsg);
        return 1;
    }

    /* Read each character from file and print to console immediately */
    int ci; // use int not char (match return type of fgetc)
    while ((ci = fgetc(file)) != EOF) {
        const char c = (char)ci; // cast to char AFTER eof found
        putchar(c);
    }
    fclose(file);
    return 0;
}

/* write cat result to external file */
int write_contents(char* inputfile, char* outputfile) {
    FILE* source = fopen(inputfile, "r");
    FILE* destination = fopen(outputfile, "w");
    if (!source) {
        char errormsg[256];
        snprintf(errormsg, sizeof(errormsg), "%s%s%s", "Could not open file '", inputfile, "'");
        perror(errormsg);
        return 1;
    } if (!outputfile) {
        char errormsg[256];
        snprintf(errormsg, sizeof(errormsg), "%s%s%s", "Could not open file '", outputfile, "'");
        perror(errormsg);
        return 1;
    }

    int ci;
    while ((ci = fgetc(source)) != EOF) {
        const char c = (char)ci;
        fputc(c, destination);
    }
    fclose(source);
    fclose(destination);
    return 0;
}

/* append cat result to external file */
int append_contents(char* inputfile, char* outputfile) {
    FILE* source = fopen(inputfile, "r");
    FILE* destination = fopen(outputfile, "a");
    if (!source) {
        char errormsg[256];
        snprintf(errormsg, sizeof(errormsg), "%s%s%s", "Could not open file '", inputfile, "'");
        perror(errormsg);
        return 1;
    } if (!destination) {
        char errormsg[256];
        snprintf(errormsg, sizeof(errormsg), "%s%s%s", "Could not open file '", outputfile, "'");
        perror(errormsg);
        return 1;
    }

    int ci;
    while ((ci = fgetc(source)) != EOF) {
        char c = (char)ci;
        fputc(c, destination);
    }
    fclose(source);
    fclose(destination);
    return 0;
}

void display_help() {
    printf("Usage:\n");
    printf("Display contents: dog 'filename'\n");
    printf("Write contents to new file: dog 'inputfile' --write 'outputfile'\n");
    printf("Append contents to new file: dog 'inputfile' --append 'outputfile'\n");
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        while (1) {
            echo();
        }
    } // defaults to echo loop, otherwise:
    if (argc == 2 ) {
        return print_contents(argv[1]);
    } else if (argc == 4) {
        if (strcmp(argv[2], "--write") == 0) {
            return write_contents(argv[1], argv[3]);
        } else if (strcmp(argv[2], "--append") == 0) {
            return append_contents(argv[1], argv[3]);
        }
    } else {
        display_help();
        return 1;
    }
}
