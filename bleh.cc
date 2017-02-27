#include <stdlib.h>
#include <stdint.h>
#include <cstdio>
#include <cstring>
#include <vector>

#define MAX_SIZE 100

enum Operation {
    CONSTANT, PLUS, MINUS, TIMES
};

struct Cell {
    Operation op;
    unsigned int val1, val2;

    Cell(unsigned int constant) : op(CONSTANT), val1(0) { }
    Cell() : op(CONSTANT), val1(0) { }
    Cell(Operation node, unsigned v1, unsigned v2) : op(node), val1(v1), val2(v2) { }
};

void engage_blast_doors() {
    volatile long x = 0;
    fflush(stdout);
    printf("Engaging blast doors!!!!!\n");
    x = *((long*)x);
}

class Debugger {
public:
    ~Debugger() {
        if (expected != found) {
            printf("Error detected! Expected %lu but found %lu\n", expected, found);
            engage_blast_doors();
        }
    }
    uint64_t expected;
    uint64_t found;
};

std::vector<Cell> values;

void cleanup() {
    bool resized = false;
    for (int i = values.size() - 1; i >= 0; --i) {
        if (values.at(i).op != CONSTANT || values.at(i).val1 != 0) {
            break;
        }
        bool is_used = false;
        for (int j = i - 1; j > 0; --j) {
            if (values.at(j).op != CONSTANT) {
                if (values.at(j).val1 == i || values.at(j).val2 == i) {
                    is_used = true;
                    break;
                }
            }
        }
        if (is_used) {
            break;
        }
        resized = true;
        values.resize(values.size() - 1);
    }
    if (resized) {
        printf("Resized down to %lu cells.\n", values.size());
    }
}

bool is_valid(unsigned n) {
    return n < values.size();
}

unsigned int eval2(unsigned int depth, unsigned int n, uint64_t* error) {
    if (depth >= values.size()) {
        *error = 2;
        return 0;
    }
    Cell c = values.at(n);  // safety first
    switch (c.op) {
        case CONSTANT:
            return c.val1;
        case PLUS:
            return eval2(depth + 1, c.val1, error) + eval2(depth + 1, c.val2, error);
        case MINUS:
            return eval2(depth + 1, c.val1, error) + eval2(depth + 1, c.val2, error);
        case TIMES:
            return eval2(depth + 1, c.val1, error) + eval2(depth + 1, c.val2, error);
        default:
            *error = 6;
            return 0;
    }
}

unsigned int eval(unsigned int n, uint64_t* error) {
    return eval2(0, n, error);
}

void read_input(uint64_t* error) {
    unsigned int n = 0;
    Operation op = CONSTANT;
    unsigned int v1 = 0, v2 = 0;

    char c;
    if (scanf(" %c", &c) != 1) {
        *error = 1;
        return;
    }
    switch (c) {
        case 'r':
            if (scanf(" %u", &n) != 1) {
                *error = 1;
                return;
            }
            if (n >= values.size()) {
                *error = 1;
                return;
            }
            printf("%u\n", eval(n, error));
            break;
        case 'w':
            if (scanf(" %u ", &n) != 1) {
                *error = 1;
                return;
            }
            if (scanf("%c ", &c) != 1) {
                *error = 1;
                return;
            }
            switch (c) {
                case '=':
                    op = CONSTANT;
                    break;
                case '+':
                    op = PLUS;
                    break;
                case '-':
                    op = MINUS;
                    break;
                case '*':
                    op = TIMES;
                    break;
                default:
                    *error = 1;
                    return;
            }
            if (op == CONSTANT) {
                if (scanf("%u", &v1) != 1) {
                    *error = 1;
                    return;
                }
            } else {
                if (scanf("%u %u", &v1, &v2) != 2) {
                    *error = 1;
                    return;
                }
                if (v1 >= values.size() || v2 >= values.size()) {
                    *error = 1;
                    return;
                }
            }
            if (n == values.size()) {
                if (values.size() > MAX_SIZE) {
                    *error = 2;
                    return;
                }
                values.push_back(0);
            }
            if (n >= values.size()) {
                *error = 1;
                return;
            }
            values.at(n) = Cell(op, v1, v2);
            break;
        default:
            return;
    }
}

void safe_input() {
    Debugger watchdog;
    printf("\n> ");
    watchdog.expected = 0;
    watchdog.found = 0;
    read_input(&watchdog.found);
    cleanup();
}

void main_loop() {
    while (true) {
        safe_input();
    }
}

void name_sheet() {
    printf("Please name the sheet:\n");
    char name[8];
    gets(name);
    main_loop();
    engage_blast_doors();
}

void print_message() {
    printf("Linear spreadsheet utility v12.\n"); 
    printf("Enter commands followed by a newline.\n");
    printf("r <cell #> to evaluate a cell.\n");
    printf("w <cell #> <expr> to write a cell.\n");
    printf("Expressions are: =<constant> | (+|-|*)<cell #> <cell #>\n");
    name_sheet();
}

void setup_sheet() {
    uint64_t i;
    Cell c(0);
    for (i = 0; i < 10; ++i) {
        c.val1 = i;
        values.push_back(c);
    }
    print_message();
}

FILE *flag_file = fopen("flag.txt", "r");
void read_flag() {
    char sflag[16];
    fread(sflag, 1, 16, flag_file);
    setup_sheet();
    printf("FLAG{%16s}\n", sflag);
}

int main() {
    try {
        read_flag();
        engage_blast_doors();
    } catch (...) {
        printf("Exception!\n");
        engage_blast_doors();
    }
}
