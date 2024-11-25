#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LENGTH 100

char input[MAX_INPUT_LENGTH];
int pos = 0;
char lookahead;

// Function prototypes
void update_lookahead();
int S();
int A();
int A_prime();
int match(char terminal);

// Function to update the lookahead token
void update_lookahead() {
    if (input[pos] != '\0') {
        lookahead = input[pos];
    } else {
        lookahead = '\0'; // End of input
    }
}

// Function to match terminal symbols and update lookahead
int match(char terminal) {
    if (lookahead == terminal) {
        pos++;
        update_lookahead();
        return 1;
    }
    return 0;
}

// Function for non-terminal S -> (A) | a
int S() {
    if (lookahead == '(') {
        if (match('(') && A() && match(')')) {
            return 1; // Successfully parsed (A)
        }
    } else if (lookahead == 'a') {
        if (match('a')) {
            return 1; // Successfully parsed a
        }
    }
    return 0; // Parsing failed
}

// Function for non-terminal A -> SA'
int A() {
    if (S() && A_prime()) {
        return 1; // Successfully parsed SA'
    }
    return 0; // Parsing failed
}

// Function for non-terminal A' -> bSA' | ε
int A_prime() {
    if (lookahead == 'b') {
        if (match('b') && S() && A_prime()) {
            return 1; // Successfully parsed bSA'
        }
        return 0; // Parsing failed
    }
    // ε case: A' can be empty
    return 1; // Successfully parsed ε (do nothing)
}

// Function to start the parsing process
int start_parsing() {
    update_lookahead(); // Initialize lookahead
    if (S()) { // Start parsing from S
        if (lookahead == '\0') { // Ensure entire input is consumed
            return 1; // Successfully parsed entire input
        }
    }
    return 0; // Parsing failed
}

int main() {
    // Get the input string
    printf("Enter the input string: ");
    scanf("%s", input);

    // Start parsing
    if (start_parsing()) {
        printf("String successfully parsed.\n");
    } else {
        printf("Parsing failed.\n");
    }

    return 0;
}
