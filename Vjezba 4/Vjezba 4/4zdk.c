/*
Napisati program za zbrajanje i množenje polinoma.
Koeficijenti i eksponenti se čitaju iz datoteke "poly.txt".
Napomena: Eksponenti u datoteci nisu nužno sortirani.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct _poly* Position;

typedef struct _poly {
    int coeff;      // koeficijent
    int pow;        // eksponent
    Position next;  // pokazivač na sljedeći član
} Poly;

Position createNode(int coeff, int pow);
int insertSorted(Position head, int coeff, int pow);
int readPolyFromLine(const char* line, Position head);
int readPolynomials(const char* filename, Position first, Position second);
void printPoly(Position head);
Position addPolynomials(Position first, Position second);
Position multiplyPolynomials(Position first, Position second);
int freePoly(Position head);

int main()
{
    // Glave polinoma (prazni čvorovi)
    Poly firstHead = { .coeff = 0, .pow = 0, .next = NULL };
    Poly secondHead = { .coeff = 0, .pow = 0, .next = NULL };

    printf("Ucitavanje polinoma iz datoteke\n");

    // Učitavanje iz datoteke
    if (readPolynomials("poly.txt", &firstHead, &secondHead) == -1) {
        printf("File error\n");
        return -1;
    }

    // Ispis učitanih polinoma
    printf("\nPrvi polinom: ");
    printPoly(&firstHead);

    printf("Drugi polinom: ");
    printPoly(&secondHead);

    // Zbrajanje
    Position sum = addPolynomials(&firstHead, &secondHead);
    printf("\nZbroj: ");
    printPoly(sum);

    // Množenje
    Position product = multiplyPolynomials(&firstHead, &secondHead);
    printf("Umnozak: ");
    printPoly(product);

    // Oslobađanje memorije
    freePoly(sum);
    freePoly(product);
    freePoly(firstHead.next);
    freePoly(secondHead.next);

    return 0;
}

// Kreira novi čvor
Position createNode(int coeff, int pow) {
    Position newNode = (Position)malloc(sizeof(Poly)); //Alocira memoriju za novi čvor polinoma
    if (!newNode) {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }
    newNode->coeff = coeff;  // postavljanje koeficijenta
    newNode->pow = pow;     // postavljanje eksponenta
    newNode->next = NULL;   // novi čvor ne pokazuje na ništa
    return newNode;
}

// Umeće element sortiran po eksponentu (silazno)
int insertSorted(Position q, int coeff, int pow) {
    if (coeff == 0) return;     // ignoriramo nule

    Position newEl = createNode(coeff, pow);     // kreiramo novi čvor
    if (!newEl) return;

    Position prev = q;  // prethodni čvor 
    Position current = q->next;    // trenutni čvor

    // Traženje mjesta za umetanje tako da lista ostane sortirana po eksponentu
    while (current != NULL && current->pow > pow) {
        prev = current;
        current = current->next;
    }

    // ako postoji isti eksponent zbrajamo
    if (current != NULL && current->pow == pow) {
        current->coeff += coeff;
        free(newEl);

        // ako je koeficijent postao 0 — briši čvor
        if (current->coeff == 0) {
            prev->next = current->next;
            free(current);
        }
    }
    else {
        // umetni novi element
        newEl->next = current;
        prev->next = newEl;
    }
    return 0;
}

// Čita jedan red iz datoteke i parsira parove (koef, pow)
int readPolyFromLine(const char* line, Position q) {
    int coeff, pow, bytesRead;      //bytesRead koliko znakvba da scanf procita
    const char* ptr = line; //pokazivac ide kroz linije

    while (sscanf(ptr, "%d %d%n", &coeff, &pow, &bytesRead) == 2) {
        insertSorted(q, coeff, pow);
        ptr += bytesRead;       //pomak kroz liniju ka mice se za toliko koliko je procita ka skace na iduzci polinom
    }

    return 0;
}

// Čita oba polinoma iz datoteke (prvi i drugi red)
int readPolynomials(const char* filename, Position first, Position second) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Greska: datoteka '%s' se ne moze otvoriti!\n", filename);
        return -1;
    }

    char line[1024];    //buffer za citanje reda
    int lineNum = 0;

    while (fgets(line, sizeof(line), file) && lineNum < 2) { //cita do dva znaci dvije linije
        if (lineNum == 0)
            readPolyFromLine(line, first);
        else
            readPolyFromLine(line, second);
        lineNum++;  //kad prociat liniu inkrementira
    }

    fclose(file);

    if (lineNum < 2) {
        printf("Greska datoteka bnesmi imat vise od 2 read\n");
        return -1;
    }

    return 0;
}

// Ispis polinoma
void printPoly(Position q) {
    Position p = q->next;   //preskacemo head
    if (!p) {
        printf("0\n");
        return;
    }

    int first = 1;
    while (p) {
        if (!first) {
            if (p->coeff > 0) printf(" + ");
            else printf(" - ");
        }
        else {
            if (p->coeff < 0) printf("-");
        }

        int absCoeff = abs(p->coeff);   //Spremamo apsolutnu vrijednost koeficijenta, jer znak smo već ispisali.
        if (absCoeff != 1 || p->pow == 0)
            printf("%d", absCoeff);         //Ako je apsolutna vrijednost koeficijenta različita od 1 ili je eksponent 0 → ispiši broj
                                                //Ako je 1 i pow != 0 → preskačemo broj, npr. 1x^3 se ispisuje samo kao x^3.
        if (p->pow > 0) {   //akoje je eksponent veci od 0 ispusjuemo x
            printf("x");
            if (p->pow > 1)     //ako je eksponent veci od jedan dodajemo pow
                printf("^%d", p->pow);
        }

        p = p->next;    //slijedeci cvor
        first = 0;      //kad prodemo clan vracamoga na 0
    }
    printf("\n");
}

// Zbrajanje polinoma
Position addPolynomials(Position first, Position second) {
    Position result = createNode(0, 0);     //Kreiramo dummy head za rezultat
    if (!result) return NULL;

    for (Position p = first->next; p; p = p->next)  //Petlja prolazi kroz sve čvorove prvog polinoma
        insertSorted(result, p->coeff, p->pow);     //kopiramnoi monom u result u insertSoretedu se sortitranju i zbrajaju

    for (Position q = second->next; q; q = q->next)     // Isto kao i za prvi polinom, prolazimo kroz drugi polinom
        insertSorted(result, q->coeff, q->pow);     //Dodajemo svaki monom drugog polinoma u rezultat. Ako već postoji isti eksponent → koeficijenti se zbrajaju(u insertSorted).

    return result;
}

// Množenje polinoma
Position multiplyPolynomials(Position first, Position second) {
    Position result = createNode(0, 0); // prazna lista za rezultat
    if (!result) return NULL;

    for (Position p = first->next; p; p = p->next)  //Vanjska petlja prolazi kroz sve članove prvog polinoma
        for (Position q = second->next; q; q = q->next)     //Unutarnja petlja prolazi kroz sve članove drugog polinoma
            insertSorted(result, p->coeff * q->coeff, p->pow + q->pow); //Množimo koeficijente: p->coeff * q->coeff Zbrajamo eksponente : p->pow + q->pow
                                                                        //Ubacujemo rezultat u result pomoću insertSorted.Ako već postoji član s istim eksponentom → koeficijenti se zbrajaju

    return result;
}

// Brisanje memorije
int freePoly(Position q) {
    Position temp;
    while (q->next != NULL) {
        temp = q->next;
        q->next = temp->next;
        temp->next = NULL;
        free(temp);
    }
    return 0;
}

