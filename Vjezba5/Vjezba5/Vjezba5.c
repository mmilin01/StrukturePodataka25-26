#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct _postfix* Position; //pokaziovac na strucuturu _postfix
typedef struct _postfix {
    double number;
    Position next; //pokazivac na sljedeci element
} postfix;


int push(Position q, double number);
int pop(Position q, char operation);
int readAndCalculate(Position q, const char* filename);
int printResult(Position q);
int freeStack(Position q);

int main() {
    postfix head = { .number = 0, .next = NULL }; //head element
    const char* filename = "postfix.txt";

    printf("Citam izraze iz datoteke: %s\n\n", filename);

    if (readAndCalculate(&head, filename) == 0) {
        printResult(&head);
    }

    // Oslobadjanje memorije
    freeStack(&head);

    return 0;
}

// Funkcija za dodavanje elementa na stog (push)
int push(Position q, double number) {
    Position newEl = (Position)malloc(sizeof(postfix)); //aloc memorije
    if (!newEl) {
        printf("Greska pri alokaciji memorije!\n");
        return EXIT_FAILURE;
    }

    newEl->number = number;    //Spremamo broj u novi element
    newEl->next = q->next;    //novi element pokazuje na stari vrh stoga
    q->next = newEl;        // head sada pokazuje na novi element (postaje elment na vrhu stoga)

    return 0;
}

// Funkcija za uklanjanje elementa sa stoga (pop) i izvodjenje operacije
int pop(Position q, char operation) {
    Position first = q->next; //first pokazuje na prvielement
    Position second = NULL;
    double result = 0;    //spremamo rezultat operacije
    
    second = first->next; //sada pokazuje na drugi element


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
            return EXIT_FAILURE;
        }
        result = second->number / first->number;
        break;
    default:
        printf("Greska: Nepoznata operacija %c\n", operation);
        return EXIT_FAILURE;
    }
    
    second->number = result; // Spremanje rezultata u drugi element ka usteda memorije jel nan vise netriba

    q->next = second;    //head pokazuej na second preskacemo prvi
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

    char c;    //za citanje znakova
    double number;    //za citanje brojeva

    while ((c = fgetc(fp)) != EOF) {
        if (isspace(c)) {     //ako je znak razma tab novi red preskacemmo ga
            continue;
        }

        if (isdigit(c)) {    // Provjera je li broj
            ungetc(c, fp);    //vracamo znak natrag u stream
            if (fscanf(fp, "%lf", &number) == 1) {     //cita broj
                printf("Broj: %.0lf\n", number);    
                push(q, number);    //dodajemo broj na stog
            }
        }
        // Provjera je li operator
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            if (pop(q, c) != 0) {    //ako imamo operator pozivamo pop i racunamo operaciju
                fclose(fp);
                return -1;
            }
        }
        // Nevaljan podatak
        else {
            printf("Nepoznat znak '%c' \n", c);
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
    
    printf("REZULTAT: %.2lf\n", first->number);
    
    return 0;
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
