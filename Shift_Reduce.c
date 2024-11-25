#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STACK 100
#define MAX_PROD 50
#define MAX_RHS 20
#define MAX_SYMBOL 10
#define EPSILON '#'

// Structure to store production rules
typedef struct {
    char lhs;
    char rhs[MAX_RHS];
    int rhs_length;
} Production;

// Structure to represent the parser stack
struct Parser {
    char stack[MAX_STACK];
    int top;
    char* input;
    int input_pos;
};

// Structure to track handlers
typedef struct {
    char handle[MAX_RHS];
    int start_pos;
    int length;
    char reduces_to;
} Handler;

// Global variables
Production productions[MAX_PROD];
int num_productions = 0;
Handler handlers[MAX_STACK];
int num_handlers = 0;

// Initialize the parser
void init_parser(struct Parser* p, char* input) {
    p->top = 0;
    p->stack[p->top++] = '$';  // Initialize with $
    p->input = input;
    p->input_pos = 0;
}

// Stack operations
void push(struct Parser* p, char c) {
    if (p->top < MAX_STACK) {
        p->stack[p->top++] = c;
    }
}

char pop(struct Parser* p) {
    if (p->top > 0) {
        return p->stack[--p->top];
    }
    return '\0';
}

// Function to remove whitespace
void remove_whitespace(char* str) {
    int i = 0, j = 0;
    while (str[i]) {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
            str[j] = str[i];
            j++;
        }
        i++;
    }
    str[j] = '\0';
}

// Function to read grammar from file
void read_grammar(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening grammar file\n");
        exit(1);
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        remove_whitespace(line);
        if (strlen(line) == 0) continue;

        char* arrow = strstr(line, "->");
        if (arrow == NULL) {
            arrow = strchr(line, '=');
            if (arrow == NULL) continue;
        }

        // Split multiple productions (separated by |)
        char* lhs = line;
        char* rhs = arrow + (arrow[1] == '>' ? 2 : 1);
        char* production = strtok(rhs, "|");
        
        while (production != NULL) {
            productions[num_productions].lhs = line[0];
            strcpy(productions[num_productions].rhs, production);
            productions[num_productions].rhs_length = strlen(production);
            num_productions++;
            production = strtok(NULL, "|");
        }
    }
    fclose(file);
}

// Print current parser state with action and handler
void print_parser_state(struct Parser* p, const char* action, const char* handle) {
    printf("%-20s", "Stack: ");
    for(int i = 0; i < p->top; i++) {
        printf("%c", p->stack[i]);
    }
    printf("%-20s", "        Input: ");
    printf("%-15s", &p->input[p->input_pos]);
    printf("%-25s", action);
    if (handle != NULL) {
        printf("Handler: %s", handle);
    }
    printf("\n");
}

// Add handler to the list
void add_handler(const char* handle, int start_pos, int length, char reduces_to) {
    strcpy(handlers[num_handlers].handle, handle);
    handlers[num_handlers].start_pos = start_pos;
    handlers[num_handlers].length = length;
    handlers[num_handlers].reduces_to = reduces_to;
    num_handlers++;
}

// Check if a handle exists in the stack
int check_handle(struct Parser* p) {
    for (int i = 0; i < num_productions; i++) {
        int rhs_len = productions[i].rhs_length;
        
        // Special handling for epsilon productions
        if (rhs_len == 1 && productions[i].rhs[0] == EPSILON) {
            push(p, productions[i].lhs);
            char action[50];
            sprintf(action, "Reduce: %c->%c", productions[i].lhs, EPSILON);
            print_parser_state(p, action, "ε");
            return 1;
        }

        if (p->top - 1 >= rhs_len) {
            int match = 1;
            for (int j = 0; j < rhs_len; j++) {
                if (p->stack[p->top - rhs_len + j] != productions[i].rhs[j]) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                // Found a handle - record it
                char handle[MAX_RHS + 1];
                strncpy(handle, &p->stack[p->top - rhs_len], rhs_len);
                handle[rhs_len] = '\0';
                add_handler(handle, p->top - rhs_len, rhs_len, productions[i].lhs);

                // Perform reduction
                for (int j = 0; j < rhs_len; j++) {
                    pop(p);
                }
                push(p, productions[i].lhs);
                
                char action[50];
                sprintf(action, "Reduce: %c->%s", productions[i].lhs, productions[i].rhs);
                print_parser_state(p, action, handle);
                return 1;
            }
        }
    }
    return 0;
}

// Main parsing function
int parse(struct Parser* p) {
    print_parser_state(p, "Initial state", NULL);
    num_handlers = 0;  // Reset handlers count
    
    while (1) {
        // Check if parsing is complete
        if (p->top == 2 && p->stack[1] == productions[0].lhs && p->input[p->input_pos] == '$') {
            print_parser_state(p, "Accept", NULL);
            printf("\nParsing completed successfully!\n");
            return 1;
        }
        
        // Try to reduce
        if (check_handle(p)) {
            continue;
        }
        
        // If no reduction possible, try to shift
        if (p->input[p->input_pos] != '$') {
            char action[20];
            sprintf(action, "Shift %c", p->input[p->input_pos]);
            push(p, p->input[p->input_pos]);
            print_parser_state(p, action, NULL);
            p->input_pos++;
        } else {
            printf("\nError: Unable to parse the input\n");
            return 0;
        }
    }
}

void print_grammar() {
    printf("\nGrammar Productions:\n");
    printf("-------------------\n");
    for (int i = 0; i < num_productions; i++) {
        printf("%c -> %s\n", productions[i].lhs, productions[i].rhs);
    }
    printf("\n");
}

void print_handlers() {
    printf("\nHandlers found during parsing:\n");
    printf("----------------------------\n");
    for (int i = 0; i < num_handlers; i++) {
        printf("Handler %d: %s (reduces to %c)\n", 
               i + 1, 
               handlers[i].handle, 
               handlers[i].reduces_to);
    }
    printf("\n");
}

int main() {
    // Read grammar from file
    read_grammar("grammar.txt");
    print_grammar();

    // Get input string from user
    char input[100];
    printf("Enter input string: ");
    scanf("%s", input);
    
    // Append $ if not present
    if (input[strlen(input)-1] != '$') {
        strcat(input, "$");
    }

    printf("\nParsing string: %s\n\n", input);
    printf("%-20s%-35s%-25s%-15s\n", "STACK", "INPUT", "ACTION", "HANDLER");
    printf("--------------------------------------------------------------------------------\n");

    struct Parser parser;
    init_parser(&parser, input);
    parse(&parser);
    print_handlers();
    
    return 0;
}
