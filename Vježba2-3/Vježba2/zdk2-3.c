/*Definirati strukturu osoba(ime, prezime, godina rođenja) i napisati program koji :
A.dinamički dodaje novi element na početak liste,
B.ispisuje listu,
C.dinamički dodaje novi element na kraj liste,
D.pronalazi element u listi(po prezimenu),
E.briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.*/

/*3. Prethodnom zadatku dodati funkcije :
A.dinamički dodaje novi element iza određenog elementa,
B.dinamički dodaje novi element ispred određenog elementa,
C.sortira listu po prezimenima osoba,
D.upisuje listu u datoteku,
E.čita listu iz datoteke.*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 50
#define EXIT_SUCCESS 0
#define FILE_ERROR -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3


typedef struct _person {
    char name[MAX_SIZE];
    char surname[MAX_SIZE];
    int birthYear;
    struct _person* next; //next je pointer na iducu osobu na listi
} Person;

typedef Person* Position;

Position createPerson();
int addToFront(Position);
int addToEnd(Position);
int printPerson(Position);
int printList(Position);
Position findPerson(Position, char*);
Position findPrev(Position, char*);
int deletePerson(Position, char*);
int freemem(Position);
int addAfter(Position, char*);
int addBefore(Position, char*);
int sortList(Position);
int writeToFile(Position, FILE*);
int readFromFile(Position, FILE*);


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
    printf("6. - Add person after certain person\n");
    printf("7. - Add person before certain person\n");
    printf("8. - Sort list by surname\n");
    printf("9. - Write list to file\n");
    printf("10. - Read list from file\n");
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

        case 6:
            printf("Enter surname after which to add new person: ");
            if (scanf(" %s", surname) != 1) return SCANF_ERROR;
            addAfter(&head, surname);
            break;

        case 7:
            printf("Enter surname before which to add new person: ");
            if (scanf(" %s", surname) != 1) return SCANF_ERROR;
            addBefore(&head, surname);
            break;

        case 8:
            printf("Sorting list by surnames...\n");
            sortList(&head);
            break;

        case 9: {
            FILE* fp = fopen("persons.txt", "w"); // open in write mode each time
            if (!fp) {
                printf("\tError opening file for writing.\n");
                break;
            }
            writeToFile(head.next, fp);
            fclose(fp);
            printf("\tList written successfully to 'persons.txt'.\n");
            break;
        }
        case 10: {
            FILE* fp = fopen("persons.txt", "r"); // open in read mode
            if (!fp) {
                printf("\tError opening file for reading.\n");
                break;
            }
            readFromFile(&head, fp);
            fclose(fp);
            break;
        }
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

int addToFront(Position q)
{
    Position newPerson = createPerson(); //stvaramo osobu
    if (!newPerson) return MALLOC_ERROR;

    newPerson->next = q->next; // Povezujemo novi clanon s trenutnim prvim clanon u listi
    // newPerson->next pokazuje na stari prvi clan
    q->next = newPerson;     // Novi clan postaje prvi clan u listi


    return EXIT_SUCCESS;
}

int addToEnd(Position q)
{
    Position newPerson = createPerson();
    if (!newPerson) return MALLOC_ERROR;

    while (q->next != NULL) //petlja prolazi kroz lsitu dok nedode do onog clana koju pokazuje na NULL
        q = q->next; //prolazimo kroz listu

    newPerson->next = NULL; //novi clan je zadnji po mora pokazivat NULL
    q->next = newPerson; //aovaj prezhodni sad pokazuje na novi(zadnji)

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

Position findPrev(Position q, char* surname)
{
    while (q->next != NULL && strcmp(q->next->surname, surname) != 0)//petlja ide dok sljedeći clan nije onaj kojeg tražimo i da nije NULL
        q = q->next; //micanje kroz lsitu

    if (q->next == NULL)
        return NULL;

    return EXIT_SUCCESS;
}

int deletePerson(Position q, char* surname)
{
    Position prev = NULL;
    Position temp = NULL;

    prev = findPrev(q, surname); //saljemoga u funkciju findPreve da nade predhodnika
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

int freemem(Position q) //koristimo je za oslobadanje memorije
{
    Position temp = NULL;

    while (q->next != NULL) {
        temp = q->next;  // spremimo clan koji ćemo obrisati
        q->next = temp->next;    // preskočimo taj clan u listi
        //treba dodat temp->next=NULL da se oslobode pointeri
        free(temp);   // oslobodimo memoriju
    }

    return EXIT_SUCCESS;
}
int addAfter(Position q, char* surname)//dodajemo nakon nekog prezimena 
{
    Position temp = findPerson(q->next, surname);
    if (!temp) {
        printf("\tPerson with surname '%s' not found.\n", surname);
        return EXIT_SUCCESS;
    }

    Position newPerson = createPerson();//kreira mem za novu osobu
    if (!newPerson) return MALLOC_ERROR;

    newPerson->next = temp->next;
    temp->next = newPerson;

    printf("\tPerson added after '%s'.\n", surname);
    return EXIT_SUCCESS;
}
int addBefore(Position q, char* surname)//dodajemo prije
{
    Position prev = findPrev(q, surname); // trazimo prethodnika i zapisujemoga u prev
    if (!prev) {
        printf("\tPerson with surname '%s' not found.\n", surname);
        return EXIT_SUCCESS;
    }

    Position newPerson = createPerson();//alociramo mem za novu osobu koju dodajemo
    if (!newPerson) return MALLOC_ERROR;

    newPerson->next = prev->next;//dodajemo je u lsitu
    prev->next = newPerson;

    printf("\tPerson added before '%s'.\n", surname);
    return EXIT_SUCCESS;
}
int sortList(Position q)// sortitranje buuble sortom
{
    Position end = NULL;
    Position j = NULL;
    Position prev_j = NULL;
    Position temp = NULL;

    if (q->next == NULL || q->next->next == NULL)
        return EXIT_SUCCESS;

    while (q->next != end) {
        prev_j = q;
        j = q->next;

        while (j->next != end) {
            if (strcmp(j->surname, j->next->surname) > 0) {
                temp = j->next;
                prev_j->next = temp;
                j->next = temp->next;
                temp->next = j;

                j = temp;
            }
            prev_j = j;
            j = j->next;
        }
        end = j;
    }

    printf("\tList sorted successfully.\n");
    return EXIT_SUCCESS;
}

int writeToFile(Position first, FILE* fp)
{
    while (first != NULL) {
        fprintf(fp, "%s %s %d\n", first->name, first->surname, first->birthYear);
        first = first->next;
    }
    return EXIT_SUCCESS;
}

int readFromFile(Position q, FILE* fp)
{
    char name[MAX_SIZE] = { 0 };
    char surname[MAX_SIZE] = { 0 };
    int birthYear = 0;

    printf("\tReading list from file:\n");


    while (fscanf(fp, "%s %s %d", name, surname, &birthYear) == 3)
    {
        printf("\t%s %s, born in %d\n", name, surname, birthYear);
    }

    printf("\tEnd of file reached.\n");
    return EXIT_SUCCESS;

}

