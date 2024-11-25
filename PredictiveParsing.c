
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_RULES 10
#define MAX_RULE_LENGTH 100

char input[100];
int position = 0;
int backtrack_count = 0;

typedef struct {
    char non_terminal;
    char productions[MAX_RULE_LENGTH][MAX_RULE_LENGTH];
    int production_count;
} ProductionRule;

ProductionRule rules[MAX_RULES];
int num_rules = 0;

int parse(char non_terminal);
void backtrack() {
    backtrack_count++;
    printf("Backtracking... Step: %d at position: %d\n", backtrack_count, position);
}

int match(char terminal) {
    if (input[position] == terminal) {
        position++;
        return 1;
    }
    return 0;
}

int parse(char non_terminal) {
    int pos_backup = position;

    for (int i = 0; i < num_rules; i++) {
        if (rules[i].non_terminal == non_terminal) {
            for (int p = 0; p < rules[i].production_count; p++) {
                char* production = rules[i].productions[p];
                printf("Attempting %c -> %s at position: %d\n", non_terminal, production, position);
                int success = 1;

                for (int j = 0; j < strlen(production); j++) {
                    char symbol = production[j];

                    if (symbol == ' ') continue;  // Skip spaces

                    if (symbol >= 'A' && symbol <= 'Z') {  // Non-terminal
                        if (!parse(symbol)) {
                            success = 0;
                            break;
                        }
                    } else {  // Terminal
                        if (!match(symbol)) {
                            success = 0;
                            break;
                        }
                    }
                }

                if (success) {
                    printf("Matched %c -> %s at position: %d\n", non_terminal, production, position);
                    return 1;
                } else {
                    position = pos_backup;
                    printf("Failed %c -> %s, backtracking to position: %d\n", non_terminal, production, position);
                }
            }
        }
    }

    backtrack();
    return 0;
}

void split_productions(char *productions, ProductionRule *rule) {
    char *production = strtok(productions, "|");
    while (production != NULL) {
        strcpy(rule->productions[rule->production_count], production);
        rule->production_count++;
        production = strtok(NULL, "|");
    }
}

int main() {
    printf("Enter the number of production rules: ");
    scanf("%d", &num_rules);
    getchar();  // Consume the newline character

    for (int i = 0; i < num_rules; i++) {
        printf("Enter the production rule %d (e.g., E -> T E'): ", i + 1);
        char rule[MAX_RULE_LENGTH];
        fgets(rule, sizeof(rule), stdin);
        rule[strcspn(rule, "\n")] = '\0';

        rules[i].non_terminal = rule[0];
        split_productions(&rule[5], &rules[i]);
    }

    printf("Enter the input string to be parsed: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    printf("Parsing input: %s\n", input);

    if (parse(rules[0].non_terminal) && input[position] == '\0') {
        printf("Rejected\n");
    } else {
        printf("Accepted\n");
    }

    return 0;
}
