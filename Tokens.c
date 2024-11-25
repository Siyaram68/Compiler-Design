#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// List of keywords
const char *keywords[] = {"if", "else", "while", "do", "int", "float", "return", "char", "double", "void"};
int keywordCount = 10;

// Function to check if a string is a keyword
int isKeyword(const char *str) {
    for(int i = 0; i < keywordCount; i++) {
        if(strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

// Function to check if a character is an operator
const char operators[] = {"+","-","","/","%","=","?","==","++","--","&&"};
int operators1 = 11;
int isOperator(char *str) {
    for(int i = 0; i < operators1; i++) {
        if(strcmp(str, operators[i]) == 0)
            return 1;
    }
    return 0;
}

// Function to check if a character is a special character (sdc)
int isSpecialCharacter(char ch) {
    char sdc[] = "(),;{}[]";
    for(int i = 0; i < strlen(sdc); i++) {
        if(ch == sdc[i])
            return 1;
    }
    return 0;
}

// Function to check if a string is a number
int isNumber(const char *str) {
    for(int i = 0; i < strlen(str); i++) {
        if(!isdigit(str[i]))
            return 0;
    }
    return 1;
}

// Function to check if a string is a literal
int isLiteral(const char *str) {
    int len = strlen(str);
    return (len >= 2 && str[0] == '"' && str[len - 1] == '"');
}

// Structure to hold token information
typedef struct {
    char lexeme[256];
    char tokenName[20];
    char token[20];
} Token;

// Function to set token information
void setToken(Token tokens[], int *count, const char *lexeme, const char *tokenName, const char *token) {
    strcpy(tokens[*count].lexeme, lexeme);
    strcpy(tokens[*count].tokenName, tokenName);
    sprintf(tokens[*count].token, "< %s , %s >", tokenName, token);
    (*count)++;
}

// Function to separate tokens from the input string
void identifyTokens(char *str, Token tokens[], int *count) {
    char *token;
    int inLiteral = 0;
    char literal[256] = "";

    token = strtok(str, " ");
    while (token != NULL) {
        if (inLiteral) {
            strcat(literal, " ");
            strcat(literal, token);
            if (token[strlen(token) - 1] == '"') {
                inLiteral = 0;
                setToken(tokens, count, literal, "literal", literal);
            }
        } else if (token[0] == '"' && token[strlen(token) - 1] != '"') {
            inLiteral = 1;
            strcpy(literal, token);
        } else if (isLiteral(token)) {
            setToken(tokens, count, token, "literal", token);
        } else if (isKeyword(token)) {
            setToken(tokens, count, token, "Keyword", token);
        } else if (isNumber(token)) {
            setToken(tokens, count, token, "Num", token);
        } else if (isOperator(token)) {
            setToken(tokens, count, token, "Op", token);
        } else if (strlen(token) == 1 && isSpecialCharacter(token[0])) {
            setToken(tokens, count, token, "STC", token);
        } else {
            setToken(tokens, count, token, "Id", token);
        }
        
        token = strtok(NULL, " ");
    }
}

// Main function to read file and print token table
int main() {
    FILE *file = fopen("sample.c", "r");
    if (file == NULL) {
        printf("Error: Could not open file sample.c\n");
        return 1;
    }

    char line[256];
    Token tokens[1000];
    int tokenCount = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Remove the newline character
        identifyTokens(line, tokens, &tokenCount);
    }

    fclose(file);

    // Print the table of tokens
    printf("%-20s %-20s %-20s\n", "Lexeme", "Token Name", "Token");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < tokenCount; i++) {
        printf("%-20s %-20s %-20s\n", tokens[i].lexeme, tokens[i].tokenName, tokens[i].token);
    }
    printf("--------------------------------------------------------\n");
    printf("Total Tokens: %d\n", tokenCount);

    return 0;
}
