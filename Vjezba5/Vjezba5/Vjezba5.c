#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct _postfix* Position;
typedef struct _postfix {
    double number;
    Position next;
} postfix;

// Deklaracije funkcija
int push(Position q, double number);
int pop(Position q, char operation);
int readAndCalculate(Position q, const char* filename);
int printResult(Position q);
int freeStack(Position q);

int main() {
    postfix head = { .number = 0, .next = NULL };
    const char* filename = "postfix.txt";

    printf("Citam postfiks izraz iz datoteke: %s\n\n", filename);

    if (readAndCalculate(&head, filename) == 0) {
        printResult(&head);
    }

    // Oslobadjanje memorije
    freeStack(&head);

    return 0;
}

// Funkcija za dodavanje elementa na stog (push)
int push(Position q, double number) {
    Position newEl = (Position)malloc(sizeof(postfix));
    if (!newEl) {
        printf("Greska pri alokaciji memorije!\n");
        return EXIT_FAILURE;
    }

    newEl->number = number;
    newEl->next = q->next;
    q->next = newEl;

    return 0;
}

// Funkcija za uklanjanje elementa sa stoga (pop) i izvodjenje operacije
int pop(Position q, char operation) {
    Position first = q->next;
    Position second = NULL;
    double result = 0;

    if (!first || !first->next) {
        printf("Greska: Nedovoljno operanada za operaciju %c\n", operation);
        return EXIT_FAILURE;
    }

    second = first->next;

    // Izvodjenje operacije
    switch (operation) {
    case '+':
        result = second->number + first->number;
        break;
    case '-':
        result = second->number - first->number;
        break;
    case '*':
        result = second->number * first->number;
        break;
    case '/':
        if (first->number == 0) {
            printf("Greska: Dijeljenje sa nulom!\n");
            return -1;
        }
        result = second->number / first->number;
        break;
    default:
        printf("Greska: Nepoznata operacija %c\n", operation);
        return EXIT_FAILURE;
    }

    // Spremanje rezultata u drugi element
    second->number = result;

    // Uklanjanje prvog elementa
    q->next = second;
    first->next = NULL;
    free(first);

    return 0;
}

// Funkcija za citanje i procesiranje postfiks izraza iz datoteke
int readAndCalculate(Position q, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Greska pri otvaranju datoteke %s\n", filename);
        return EXIT_FAILURE;
    }

    char c;
    double number;

    while ((c = fgetc(fp)) != EOF) {
        // Preskoci razmake i novi red
        if (isspace(c)) {
            continue;
        }

        // Provjera je li broj
        if (isdigit(c)) {
            ungetc(c, fp); // Vrati znak natrag u stream
            if (fscanf(fp, "%lf", &number) == 1) {
                printf("Broj: %.0lf\n", number);
                push(q, number);
            }
        }
        // Provjera je li operator
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            printf("Operator: %c\n", c);
            if (pop(q, c) != 0) {
                fclose(fp);
                return -1;
            }
        }
        // Nevaljan podatak
        else {
            printf("Upozorenje: Nepoznat znak '%c' - preskacemo\n", c);
        }
    }

    fclose(fp);
    return 0;
}

// Funkcija za ispis rezultata
int printResult(Position q) {
    Position first = q->next;

    if (!first) {
        printf("Greska: Stog je prazan!\n");
        return EXIT_FAILURE;
    }

    if (first->next != NULL) {
        printf("Greska: Vise od jednog elementa na stogu - nevazeći izraz!\n");
        return EXIT_FAILURE;
    }

    printf("\n===================\n");
    printf("REZULTAT: %.2lf\n", first->number);
    printf("===================\n");
    return EXIT_FAILURE;
}

// Funkcija za ciscenje memorije
int freeStack(Position q) {
    Position temp;
    while (q->next) {
        temp = q->next;
        q->next = temp->next;
        temp->next = NULL;
        free(temp);
    }
    return EXIT_FAILURE;
}