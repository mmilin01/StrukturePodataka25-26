#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50

#define EMPTY_STACK 1
#define MALLOC_ERROR -1
#define SCANF_ERROR -2

struct directory;
typedef struct directory* DirPosition;
typedef struct directory {
    char name[MAX_LENGTH];
    DirPosition subDirPosition;     //prvo dijete (ulaz u lsitu podirekotorija)
    DirPosition next;           //sljedeci direktoriji na istom nivou
} Directory;

struct levelStack;
typedef struct levelStack* LevelStackPosition;
typedef struct levelStack {         //kad uded u podirektoriji ->push
    DirPosition directoryLevel;     //kad ides cd ->popo
    LevelStackPosition next;
} LevelStack;

DirPosition createDirectory(char* directoryName);
int push(LevelStackPosition headLevelStack, DirPosition currentDirectory);
int pop(LevelStackPosition headLevelStack);
int addDirectory(DirPosition superdirectory, DirPosition subdirectory);
int changeDirectory(LevelStackPosition headLevelStack, char* directoryName);
int printDirectory(DirPosition current, int depth);
int deleteDirectory(DirPosition headDirectory);
int deleteStack(LevelStackPosition headLevelStack);

int freeTree(DirPosition node);

int main()
{
    char input = 0;
    char directoryName[MAX_LENGTH] = { 0 };
    Directory headDirectory = { .name = {0}, .subDirPosition = NULL, .next = NULL };    //head ellemtn
    DirPosition rootDirectory = NULL;
    LevelStack headLevelStack = { .directoryLevel = NULL, .next = NULL };   //head element

    rootDirectory = createDirectory("C:");      
    if (rootDirectory == NULL) return MALLOC_ERROR;

    headDirectory.next = rootDirectory;         //C: postaje korjeniu direktorij stavljamo ga na stack
    if (push(&headLevelStack, rootDirectory) != EXIT_SUCCESS) return MALLOC_ERROR;

    printf("1 - make directory\n2 - change directory to <dir>\n3 - go to parent directory\n4 - view directory content\n5 - end program\n");

    do {
        printf("\nEnter command: ");
        if (scanf(" %c", &input) != 1) return SCANF_ERROR;

        switch (input) {
        case '1':
            // md - make directory
            printf("md ");
            if (scanf(" %s", directoryName) != 1) return SCANF_ERROR;
            {
                DirPosition newDirectory = createDirectory(directoryName);
                if (newDirectory == NULL) return MALLOC_ERROR;
                addDirectory(headLevelStack.next->directoryLevel, newDirectory);    //Dodaje ga kao dijete trenutnog direktorija 
                printf("Created directory %s\n", directoryName);
            }
            break;
        case '2':
            // cd <dir> - change directory
            printf("cd ");
            if (scanf(" %s", directoryName) != 1) return SCANF_ERROR;
            changeDirectory(&headLevelStack, directoryName);
            break;
        case '3':
            // cd.. - go to parent directory
            if (pop(&headLevelStack)) 
                printf("Already in root directory.\n");
            else 
                printf("You are now in parent directory\n");
            break;
        case '4':
            // dir - view directory content
            printf("Printing content of %s:\n", headLevelStack.next->directoryLevel->name); //headLevelStack -> dummy node steka headLevelStack.next -> vrh steka (trenutni direktorij) 
            printDirectory(headLevelStack.next->directoryLevel, 0);                           //directoryLevel -> pokazuje na stvarni direktorij name -> ime tog direktorija
            break;
        case '5':
            // end of program
            printf("\nEnd of program.\n");
            break;
        default:
            printf("Invalid input.\n");
            break;
        }

    } while (input != '5');


    deleteDirectory(&headDirectory);
    deleteStack(&headLevelStack);

    return 0;
}

DirPosition createDirectory(char* directoryName) {

    DirPosition newDirectory = NULL;        //nepoakzuje nigdi
    newDirectory = (DirPosition)malloc(sizeof(Directory));  //aloc

    if (newDirectory == NULL) return NULL;

    strcpy(newDirectory->name, directoryName);  //psotavljamo name unutar strukture na directoy name
    newDirectory->subDirPosition = NULL;        //ovaj direktorij nema djecu ovo je početak liste poddirektorija
    newDirectory->next = NULL;  //nema sljedećeg direktorija u istom nivou

    return newDirectory;    //vraca pokazivac na kreirani direktorij
}

int push(LevelStackPosition headLevelStack, DirPosition currentDirectory) { //push koristimo kao nekakvu povijes direktorija
                                                                            //kad god udemo u novi direktoru zove se push i onda se novi stavlja na v rh
    LevelStackPosition newLevelStack = NULL;
    newLevelStack = (LevelStackPosition)malloc(sizeof(LevelStack));     //alociora memoriju za stack

    if (newLevelStack == NULL) return MALLOC_ERROR;

    newLevelStack->directoryLevel = currentDirectory;   //čvor na steku čuva pokazivač na direktorij u koji smo ušli

    newLevelStack->next = headLevelStack->next;     //Povezujemo novi vrh steka sa ostatkom liste
    headLevelStack->next = newLevelStack;   //Postavljamo novi čvor kao VRH steka

    return EXIT_SUCCESS;
}

int pop(LevelStackPosition headLevelStack) {        //po koristimo ka za izldaz iz dirkeotoriaj

    LevelStackPosition temp;       //spremammo cvor koji skidamo da bismo ga kasniej lakse oslobodili

    if (headLevelStack->next == NULL) return EMPTY_STACK; // provbjera je li stack prazan
                                                           //ovo je provjera je li stack prazan dogodicew se ako smo u rootu C
    temp = headLevelStack->next;    //micemo svrha stoga pa ka spremammo kasniej za brisanje
    headLevelStack->next = headLevelStack->next->next;  //preskacemo onaj sa vrhga stogha POP
    free(temp); //freemem

    return 0;
}

int addDirectory(DirPosition superdirectory, DirPosition subdirectory) {    //superdirectory onaj u kojem se nalazimo
                                                                            //subdiretory novi dirkeotoriji koji ubacujemo u njega
    if (superdirectory == NULL || subdirectory == NULL) return MALLOC_ERROR;

    if (superdirectory->subDirPosition == NULL) {   //ako supedirectory nema djece
        superdirectory->subDirPosition = subdirectory;      //stavlajmo subdirectory kao prvo djete
    }
    else {      //ako vec ima djecu
        DirPosition temp = superdirectory->subDirPosition;
        while (temp->next != NULL)  //idemo do kraja liste
            temp = temp->next;
        
        temp->next = subdirectory;  //i kad dodemo do kraja stavimo novo djete
    }

    return EXIT_SUCCESS;
}

/* Goes to child directory with directoryName */
int changeDirectory(LevelStackPosition headLevelStack, char* directoryName) {       //pretrazuje sve poddirektorije nkeog diirekotoiaj da vidi ima li trazeni poddirektoprij ako postoji pusha ga na stack

    DirPosition current = headLevelStack->next->directoryLevel->subDirPosition; //headLevelStack->next -> vrh steka (trenutni direktorij) 
                                                              //directoryLevel -> pokazivač na stvarni direktorijsubDirPosition -> pokazuje na prvo dijete(lista poddirektorija)
    while (current != NULL && strcmp(current->name, directoryName) != 0) //strcmpprovjerava je lio ime odogovara trazenom
        current = current->next;    //ako ne odogovara idmeo na sljedeci

    if (current != NULL) {      //ako nije NULL nasli smo ga
        printf("Changed directory to %s\n", directoryName); //promjenili smo direktoriji
        push(headLevelStack, current);  //pusha ga na vrh stacka
    }
    else printf("Directory not found.\n");

    return EXIT_SUCCESS;
}

int printDirectory(DirPosition current, int depth) {       //current direktoriji kojeg zelimo ispitat
                                                          //deopth koliko je direktorijui u stablu
    int i = 0;
    DirPosition currentSubdirectory = NULL;     //privremeni pokazivač za prolazak 

    if (current->subDirPosition != NULL) {      //ako je jednako NULL neam poddirektorij
        currentSubdirectory = current->subDirPosition;  //psotavljamo currentSubdirectoy na prvo dijete
        while (currentSubdirectory != NULL) {   //dok god postoji dijete
            for (i = 0; i < depth; i++)    //kao u ovisjnoisti u depth uvlaci ka da bolje izgleda
                printf("  "); 
            printf("%s\n", currentSubdirectory->name);  //ispisujemo ime
            printDirectory(currentSubdirectory, depth + 1); //rekurzivini poziv funkcija se poziva za svalpo dijete depth +1 idemo jedan nivo nize 
            currentSubdirectory = currentSubdirectory->next;    //idemo na slijedeci podirektoriji na istom nivou
        }
    }
    else {
        for (i = 0; i < depth; i++)
            printf("  ");
        printf("(empty)\n");
    }

    return EXIT_SUCCESS;
}

int deleteDirectory(DirPosition headDirectory) {

    DirPosition temp = NULL;    //preivremeni pokazivac na direkotriji koji cemo obrisat
    DirPosition next = NULL;    //pamti sljedeći direktorij u listi jer nakon brisanja temp ne smijemo izgubiti vezu

    while (headDirectory->next != NULL) {   //izvodi se dok postoji barem jedan direkotorij
        temp = headDirectory->next; //temp pokazuje na prvi stvarni
        next = temp->next;  //Trebamo sačuvati pokazivač na sljedeći direktorij Poslije brisanja temp, ne smijemo izgubiti listu
        freeTree(temp); //oslobada mem
        headDirectory->next = next; //nakon stoi smo obrisali temo dad je prvi direktorij next petlaj ide do kraja dok svi nisu obrisnai
    }

    return EXIT_SUCCESS;
}

int freeTree(DirPosition headDirectory) {    //rekurzivnio brise sve direktorije o headDirectory
    if (headDirectory == NULL) return;  

    DirPosition child = headDirectory->subDirPosition;  //child pokazuje na prvo djete trenutnog cvora
    DirPosition nextChild = NULL;   //privremeni pokazivač da sačuvamo sljedece dijete jer ćemo trenutno dijete brisati
    while (child != NULL) {
        nextChild = child->next;    //pamti sljedece dijete prije nego što obrisemo trenutno
        freeTree(child);    //rekurzivno brise poddirektorij i sve njegove poddirektorije
        child = nextChild;  //Idemo na sljedece dijete
    }

   
    free(headDirectory);    //na kraju brisemo i direktorij
    return EXIT_SUCCESS;
}

int deleteStack(LevelStackPosition headLevelStack) {

    LevelStackPosition temp = NULL; //privremni cvor

    while (headLevelStack->next != NULL) {  //ide dok postoji barem jedan element na stacku
        temp = headLevelStack->next;    //temp pokazuej na vrh stacka
        headLevelStack->next = temp->next;  //vrh steka sada postaje sljedeci cvor preskacemo temp
        free(temp);
    }

    return EXIT_SUCCESS;
}
