#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 100

// Function declarations
void loadClassifications();
int isKeyword(char buffer[]);
int isOperator(const char *buffer, int *opLength);
void storeToken(char token[], char type[]);
void printTokenTable();

// File pointers for storing identifiers and operators
FILE *identifiersFile, *operatorsFile;

// Token structure for storing token and type
typedef struct {
    char token[MAX_LENGTH];
    char type[MAX_LENGTH];
} Token;

Token tokenTable[MAX_LENGTH];
int tokenCount = 0;

// Classifications
char keywords[100][10];
char operators[100][3];
char specials[100][2];
int keywordCount = 0, operatorCount = 0, specialCount = 0;

int main() {
    char input[MAX_LENGTH];
    char token[MAX_LENGTH];
    char type[MAX_LENGTH];
    int index = 0;
    int totalTokens = 0;

    // Load classifications from file
    loadClassifications();

    // Open files for storing identifiers and operators
    identifiersFile = fopen("identifiers.txt", "w");
    operatorsFile = fopen("operators.txt", "w");

    if (identifiersFile == NULL || operatorsFile == NULL) {
        perror("Error opening files");
        return EXIT_FAILURE;
    }

    printf("Enter input (end with 'end'): \n");

    // Read input until "end" is entered
    while (1) {
        scanf(" %[^\n]", input);
        if (strcmp(input, "end") == 0) {
            break;
        }

        int length = strlen(input);

        // Check each character in the input
        for (int i = 0; i < length; i++) {
            if (isalpha(input[i])) {
                token[index++] = input[i];
                while (isalnum(input[i + 1])) {
                    token[index++] = input[++i];
                }
                token[index] = '\0';
                index = 0;
                if (isKeyword(token)) {
                    strcpy(type, "Keyword");
                } else {
                    strcpy(type, "Identifier");
                    fprintf(identifiersFile, "%s\n", token);
                }
                storeToken(token, type);
                totalTokens++;
            } else if (isdigit(input[i])) {
                token[index++] = input[i];
                while (isdigit(input[i + 1])) {
                    token[index++] = input[++i];
                }
                token[index] = '\0';
                index = 0;
                strcpy(type, "Numeric Literal");
                storeToken(token, type);
                totalTokens++;
            } else {
                int opLength = 0;
                if (isOperator(&input[i], &opLength)) {
                    strncpy(token, &input[i], opLength);
                    token[opLength] = '\0';
                    i += (opLength - 1); // Adjust the loop variable
                    index = 0;
                    strcpy(type, "Operator");
                    fprintf(operatorsFile, "%s\n", token);
                    storeToken(token, type);
                    totalTokens++;
                } else if (ispunct(input[i])) {
                    if (input[i] == '"' || input[i] == '\'') {
                        char quote = input[i];
                        token[index++] = input[i++];
                        while (input[i] != quote && i < length) {
                            token[index++] = input[i++];
                        }
                        token[index++] = input[i];
                        token[index] = '\0';
                        index = 0;
                        strcpy(type, "String Literal");
                        storeToken(token, type);
                    } else {
                        token[index++] = input[i];
                        token[index] = '\0';
                        index = 0;
                        strcpy(type, "Special Character");
                        storeToken(token, type);
                    }
                    totalTokens++;
                }
            }
        }
    }

    // Print total number of tokens
    printf("\nTotal number of tokens: %d\n", totalTokens);

    // Print token table
    printTokenTable();

    // Close files
    fclose(identifiersFile);
    fclose(operatorsFile);

    return EXIT_SUCCESS;
}

// Function to load classifications from file
void loadClassifications() {
    FILE *file = fopen("classifications.txt", "r");
    if (file == NULL) {
        perror("Error opening classifications file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char *tokenType = strtok(line, ": ");
        char *tokens = strtok(NULL, "\n");

        if (strcmp(tokenType, "KEYWORDS") == 0) {
            char *token = strtok(tokens, " ");
            while (token != NULL) {
                strcpy(keywords[keywordCount++], token);
                token = strtok(NULL, " ");
            }
        } else if (strcmp(tokenType, "OPERATORS") == 0) {
            char *token = strtok(tokens, " ");
            while (token != NULL) {
                strcpy(operators[operatorCount++], token);
                token = strtok(NULL, " ");
            }
        } else if (strcmp(tokenType, "SPECIALS") == 0) {
            char *token = strtok(tokens, " ");
            while (token != NULL) {
                strcpy(specials[specialCount++], token);
                token = strtok(NULL, " ");
            }
        }
    }

    fclose(file);
}

// Function to check if token is a keyword
int isKeyword(char buffer[]) {
    for (int i = 0; i < keywordCount; ++i) {
        if (strcmp(keywords[i], buffer) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to check if character is an operator
int isOperator(const char *buffer, int *opLength) {
    for (int i = 0; i < operatorCount; ++i) {
        int len = strlen(operators[i]);
        if (strncmp(operators[i], buffer, len) == 0) {
            *opLength = len;
            return 1;
        }
    }
    return 0;
}

// Function to store token and its type in the table
void storeToken(char token[], char type[]) {
    strcpy(tokenTable[tokenCount].token, token);
    strcpy(tokenTable[tokenCount].type, type);
    tokenCount++;
}

// Function to print the token table
void printTokenTable() {
    printf("\nToken Table:\n");
    printf("--------------------\n");
    printf("%-20s %-20s\n", "Token", "Type");
    printf("--------------------\n");
    for (int i = 0; i < tokenCount; i++) {
        printf("%-20s %-20s\n", tokenTable[i].token, tokenTable[i].type);
    }
}
