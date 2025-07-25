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
    memset(line, 0, sizeof(line));
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
    if (!source) {
        char errormsg[256];
        snprintf(errormsg, sizeof(errormsg), "%s%s%s", "Could not open file '", inputfile, "'");
        perror(errormsg);
        return 1;
    }

    FILE* check = fopen(outputfile, "r");
    if (check) {
        printf("File '%s' already exists, overwrite it? (Y/n): ", outputfile);
        const char option = getchar();
        switch (option) {
            case 'y': break;
            case 'Y': break;
            case '\n': break;
            case 'n': return 1;
            case 'N': return 1;
            default:
                printf("Unexpected option, quitting");
                return 1;
        }
    }

    FILE* destination = fopen(outputfile, "w");
    if (!destination) {
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
    fclose(check);
    return 0;
}

/* append cat result to external file */
int append_contents(char* inputfile, char* outputfile) {
    FILE* source = fopen(inputfile, "r");
    if (!source) {
        char errormsg[256];
        snprintf(errormsg, sizeof(errormsg), "%s%s%s", "Could not open file '", inputfile, "'");
        perror(errormsg);
        return 1;
    }
    // Check if output file exists
    FILE* check = fopen(outputfile, "r");
    if (!check) {
        char errormsg[256];
        snprintf(errormsg, sizeof(errormsg), "%s%s%s", "Could not append to file: '", outputfile, "'");
        perror(errormsg);
        fclose(source);
        return 1;
    }
    FILE* destination = fopen(outputfile, "a");
    if (!destination) {
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
    fclose(check);
    return 0;
}

int display_help() {
    printf("Usage:\n");
    printf("Display contents: dog 'filename'\n");
    printf("Write contents to new file: dog 'inputfile' --write 'outputfile'\n");
    printf("Append contents to new file: dog 'inputfile' --append 'outputfile'\n");
    return 0;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        while (1) {
            echo();
        }
    } // defaults to echo loop, otherwise:
    if (argc == 2 ) {
        if ((strcmp(argv[1], "--help") == 0)) {
            return display_help();
        }
        if (print_contents(argv[1]) == 1) {
            printf("dog --help for help\n"); // prompt help if file not found, for possible mistype of --help
        }
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
