#include <stdio.h>
#include <stdbool.h>

#define MAX_STATES 100
#define MAX_SYMBOLS 26

typedef struct {
    int num_states;
    int num_symbols;
    char symbols[MAX_SYMBOLS];
    int start_state;
    int num_final_states;
    int final_states[MAX_STATES];
    int transitions[MAX_STATES][MAX_SYMBOLS];
} Automaton;

int get_symbol_index(Automaton *automaton, char symbol) {
    for (int i = 0; i < automaton->num_symbols; i++) {
        if (automaton->symbols[i] == symbol) {
            return i;
        }
    }
    return -1;
}

bool load_automaton(const char *filename, Automaton *automaton) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        char choice;
        printf("Error opening file %s\n", filename);
        do {
            printf("Do you want to try again (y/n)? ");
            scanf(" %c", &choice);

            if (choice == 'y' || choice == 'Y') {
                file = fopen(filename, "r");
                if (file) {
                    break;
                } else {
                    printf("Error reopening file %s\n", filename);
                }
            } else if (choice == 'n' || choice == 'N') {
                return false;
            }
        } while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N' || !file);
    }


    if (fscanf(file, "%d", &automaton->num_symbols) != 1) {
        printf("Error reading num_symbols\n");
        fclose(file);
        return false;
    }
    for (int i = 0; i < automaton->num_symbols; i++) {
        fscanf(file, " %c", &automaton->symbols[i]);
    }

    if (fscanf(file, "%d", &automaton->num_states) != 1) {
        printf("Error reading num_states\n");
        fclose(file);
        return false;
    }
    if (fscanf(file, "%d", &automaton->start_state) != 1) {
        printf("Error reading start_state\n");
        fclose(file);
        return false;
    }
    if (fscanf(file, "%d", &automaton->num_final_states) != 1) {
        printf("Error reading num_final_states\n");
        fclose(file);
        return false;
    }
    for (int i = 0; i < automaton->num_final_states; i++) {
        fscanf(file, "%d", &automaton->final_states[i]);
    }

    for (int i = 0; i < automaton->num_states; i++) {
        for (int j = 0; j < automaton->num_symbols; j++) {
            automaton->transitions[i][j] = -1;
        }
    }

    int s, s_prime;
    char a;
    while (fscanf(file, "%d %c %d", &s, &a, &s_prime) == 3) {
        int symbol_index = get_symbol_index(automaton, a);
        automaton->transitions[s][symbol_index] = s_prime;
    }

    fclose(file);
    return true;
}

void dfs(Automaton *automaton, int state, bool *visited, bool *processed_symbols) {
    if (visited[state]) {
        return;
    }

    visited[state] = true;

    for (int i = 0; i < automaton->num_symbols; i++) {
        int next_state = automaton->transitions[state][i];
        if (next_state != -1) {
            processed_symbols[i] = true;
            dfs(automaton, next_state, visited, processed_symbols);
        }
    }
}

bool unprocessed_symbols(Automaton *automaton) {
    bool visited[MAX_STATES] = {false};
    bool processed_symbols[MAX_SYMBOLS] = {false};

    dfs(automaton, automaton->start_state, visited, processed_symbols);

    bool allProcessed = true;
    for (int i = 0; i < automaton->num_symbols; i++) {
        if (!processed_symbols[i]) {
            if (allProcessed) {
                printf("Symbols that are not processed by the automaton: ");
                allProcessed = false;
            }
            printf("%c ", automaton->symbols[i]);
        }
    }

    if (allProcessed) {
        printf("All symbols are processed.\n");
    } else {
        printf("\n");
    }

    return allProcessed;
}


int main() {
    Automaton automaton;
    if (!load_automaton("automaton.txt", &automaton)) {
        return 1;
    }

    printf("Loaded Automaton:\n");
    printf("Symbols: ");
    for (int i = 0; i < automaton.num_symbols; i++) {
        printf("%c ", automaton.symbols[i]);
    }
    printf("\nStates: %d, Start: %d, Final: ", automaton.num_states, automaton.start_state);
    for (int i = 0; i < automaton.num_final_states; i++) {
        printf("%d ", automaton.final_states[i]);
    }
    printf("\n");
    unprocessed_symbols(&automaton);
    return 0;
}
