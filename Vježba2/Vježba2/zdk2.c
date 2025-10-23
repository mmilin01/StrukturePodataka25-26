/*Definirati strukturu osoba(ime, prezime, godina rođenja) i napisati program koji :
A.dinamički dodaje novi element na početak liste,
B.ispisuje listu,
C.dinamički dodaje novi element na kraj liste,
D.pronalazi element u listi(po prezimenu),
E.briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 50
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

typedef struct _person {
    char name[MAX_SIZE];
    char surname[MAX_SIZE];
    int birthYear;
    struct _person* next; //next je pointer na iducu osobu na listi
} Person;

typedef Person* Position;

// Function prototypes
Position createPerson();
int addToFront(Position head);
int addToEnd(Position head);
int printPerson(Position p);
int printList(Position first);
Position findPerson(Position first, char* surname);
int deletePerson(Position head, char* surname);
int freemem(Position head);

int main()
{
    int in = 0;
    char surname[MAX_SIZE];
    Person head = { .next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 };//head elemet prvi ka elemtn na listi veznaoj
    printf("-----------------MENU-----------------\n");
    printf("1. - Add person to front of the list\n");
    printf("2. - Add person to end of the list\n");
    printf("3. - Print list\n");
    printf("4. - Find person by surname\n");
    printf("5. - Delete person by surname\n");
    printf("0 - Exit\n");
    printf("--------------------------------------\n");

    do {
        printf("\nChoose action: ");
        if (scanf(" %d", &in) != 1) return SCANF_ERROR;

        switch (in)
        {
        case 1:
            printf("Add to front of the list\n");
            addToFront(&head);
            break;

        case 2:
            printf("Add to end of the list\n");
            addToEnd(&head);
            break;

        case 3:
            printf("Printing list:\n");
            printList(head.next);
            break;

        case 4:
            printf("Enter surname to find: ");
            if (scanf(" %s", surname) != 1) return SCANF_ERROR;
            {
                Position found = findPerson(head.next, surname);
                if (found)
                    printPerson(found);
                else
                    printf("\tPerson with surname '%s' not found.\n", surname);
            }
            break;

        case 5:
            printf("Enter surname to delete: ");
            if (scanf(" %s", surname) != 1) return SCANF_ERROR;
            deletePerson(&head, surname);
            break;

        case 0:
            printf("Exiting program...\n");
            break;

        default:
            printf("\tInvalid input. Try again.\n");
            break;
        }

    } while (in != 0);

    freemem(&head); // oslobada mem

    return EXIT_SUCCESS;
}

Position createPerson()
{
    Position newPerson = NULL;
    newPerson = (Position)malloc(sizeof(Person)); //aloc memoriju savki put kase upisuje nova osoba treba se aloc
    if (!newPerson) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    printf("\tName: ");
    if (scanf(" %s", newPerson->name) != 1) return NULL;

    printf("\tSurname: ");
    if (scanf(" %s", newPerson->surname) != 1) return NULL;

    printf("\tBirth year: ");
    if (scanf(" %d", &newPerson->birthYear) != 1) return NULL;

    newPerson->next = NULL; //samo vako incicijaliziramo jel nije jos povezano snicin vracamo u funkciju addFront
    return newPerson;
}

int addToFront(Position head)
{
    Position newPerson = createPerson(); //stvaramo osobu
    if (!newPerson) return MALLOC_ERROR;

    newPerson->next = head->next; // Povezujemo novi clanon s trenutnim prvim clanon u listi
                                  // newPerson->next pokazuje na stari prvi clan
    head->next = newPerson;     // Novi clan postaje prvi clan u listi


    return EXIT_SUCCESS;
}

int addToEnd(Position head)
{ 
    Position newPerson = createPerson();
    if (!newPerson) return MALLOC_ERROR;

    while (head->next != NULL) //petlja prolazi kroz lsitu dok nedode do onog clana koju pokazuje na NULL
        head = head->next; //prolazimo kroz listu

    newPerson->next = NULL; //novi clan je zadnji po mora pokazivat NULL
    head->next = newPerson; //aovaj prezhodni sad pokazuje na novi(zadnji)

    return EXIT_SUCCESS;
}

int printPerson(Position p)
{
    printf("\t%s %s, born in %d\n", p->name, p->surname, p->birthYear); //isppisuje osobu
    return EXIT_SUCCESS;
}

int printList(Position first)
{
    if (first == NULL) {
        printf("\tThe list is empty.\n"); //ka provjer ada lista nije prazna
        return EXIT_SUCCESS;
    }

    while (first != NULL) { // ide dok nenaide na NULL ilitiga zadnji clan
        printPerson(first); //salje trenutacni clan u ispis
        first = first->next;  // mice pokazivac po listi
    }

    return EXIT_SUCCESS;
}

Position findPerson(Position first, char* surname) //funckija prima pokatzivac na prvi clan u listi i prezime koje zelimo nac
{
    while (first != NULL) { //krecemo se krozu lsitu
        if (strcmp(first->surname, surname) == 0)//strcmp usporeduje dva stringa ako su isti vrac 0
            return first;   //ako je prezime isto vraca pokazivac na taj cvor
        first = first->next;    //ako nije jednako trenutacno prezimenu idemo na slijedece
    }
    return NULL;
}

Position findPrev(Position head, char* surname)
{
    while (head->next != NULL && strcmp(head->next->surname, surname) != 0)//petlja ide dok sljedeći clan nije onaj kojeg tražimo i da nije NULL
        head = head->next; //micanje kroz lsitu

    if (head->next == NULL)
        return NULL;

    return head;
}

int deletePerson(Position head, char* surname)
{
    Position prev = NULL;
    Position temp = NULL;

    prev = findPrev(head, surname); //saljemoga u funkciju findPreve da nade predhodnika
    if (!prev) {
        printf("\tPerson with surname '%s' not found.\n", surname);
        return EXIT_SUCCESS;
    }

    temp = prev->next; //stavljamoga u temp da ga kasnije mozemo izbrisati
    prev->next = temp->next; //Prethodni sada pokazuje na sljedeci i preskcaemo onog kojeg zelimo brisat
    free(temp); //oslobadamo mem

    printf("\tPerson deleted successfully.\n");
    return EXIT_SUCCESS;
}

int freemem(Position head) //koristimo je za oslobadanje memorije
{
    Position temp = NULL;

    while (head->next != NULL) {
        temp = head->next;  // spremimo clan koji ćemo obrisati
        head->next = temp->next;    // preskočimo taj clan u listi
        free(temp);   // oslobodimo memoriju
    }

    return EXIT_SUCCESS;
}
