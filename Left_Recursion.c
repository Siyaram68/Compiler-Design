#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10
#define MAX_PROD_LEN 100


void Set(char set[MAX][MAX_PROD_LEN], int *count, const char *symbol) {
    for (int i = 0; i < *count; i++) {
        if (strcmp(set[i], symbol) == 0) {
            return;
        }
    }
    strcpy(set[*count], symbol);
    (*count)++;
}


void extractSymbols(char productions[MAX][MAX_PROD_LEN], int n, char nonTerminals[MAX][MAX_PROD_LEN], int *nonTerminalCount,
	char terminals[MAX][MAX_PROD_LEN], int *terminalCount) {
    for (int i = 0; i < n; i++) {

        char nonTerminal[2] = {productions[i][0], '\0'};
        Set(nonTerminals, nonTerminalCount, nonTerminal);

        char *production = strchr(productions[i], '>') + 1;

        for (int j = 0; j < strlen(production); j++) {
            if (isupper(production[j])) {

                char nonTerm[2] = {production[j], '\0'};
                Set(nonTerminals, nonTerminalCount, nonTerm);
            } else if (islower(production[j]) || isdigit(production[j]) || production[j] == '('
		|| production[j] == ')'|| production[j] == '+' || production[j] == ',' ||  production[j] == '>') {

                char terminal[2] = {production[j], '\0'};
                Set(terminals, terminalCount, terminal);
            }
        }
    }
}


void removeLeftRecursion(char productions[MAX][MAX_PROD_LEN], int n) {
    for (int i = 0; i < n; i++) {
        char nonTerminal = productions[i][0];
        char alpha[MAX][MAX_PROD_LEN];
        char beta[MAX][MAX_PROD_LEN];
        int alphaCount = 0, betaCount = 0;

        char *production = strchr(productions[i], '>') + 1;
        char *token = strtok(production, "|");

        while (token != NULL) {
            if (token[0] == nonTerminal) {
                strcpy(alpha[alphaCount++], token + 1);
            } else {
                strcpy(beta[betaCount++], token);
            }
            token = strtok(NULL, "|");
        }

        if (alphaCount > 0) {
            char newNonTerminal[MAX_PROD_LEN];
            sprintf(newNonTerminal, "%c'", nonTerminal);

            printf("%c -> ", nonTerminal);
            for (int j = 0; j < betaCount; j++) {
                printf("%s%s", beta[j], newNonTerminal);
                if (j < betaCount - 1) {
                    printf(" | ");
                }
            }
            printf("\n");

            printf("%s -> ", newNonTerminal);
            for (int j = 0; j < alphaCount; j++) {
                printf("%s%s", alpha[j], newNonTerminal);
                if (j < alphaCount - 1) {
                    printf(" | ");
                }
            }
            printf(" | e\n");
        } else {

            printf("%s\n", productions[i]);
        }
    }
}

int main() {
    char productions[MAX][MAX_PROD_LEN];
    int n;

    char nonTerminals[MAX][MAX_PROD_LEN];
    char terminals[MAX][MAX_PROD_LEN];
    int nonTerminalCount = 0, terminalCount = 0;

    printf("Enter the number of productions: ");
    scanf("%d", &n);
    printf("Enter the productions:\n");
    for (int i = 0; i < n; i++) {
        scanf("%s", productions[i]);
    }

    extractSymbols(productions, n, nonTerminals, &nonTerminalCount, terminals, &terminalCount);

    printf("\nVariables: ");
    for (int i = 0; i < nonTerminalCount; i++) {
        printf("%s ", nonTerminals[i]);
    }

    printf("\nTerminals: ");
    for (int i = 0; i < terminalCount; i++) {
        printf("%s ", terminals[i]);
    }

    printf("\n\nRemoving left recursion:\n");
    removeLeftRecursion(productions, n);

    return 0;
}
