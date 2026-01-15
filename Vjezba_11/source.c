#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 11
#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2

typedef struct cityTree* CityTreePosition;//pokazivac na cvor stabla
typedef struct cityTree {
    char name[20];
    int population;
    CityTreePosition left;
    CityTreePosition right;
} CityTree;

typedef struct countryHash* CountryHashPosition;
typedef struct countryHash {
    char name[20];
    CityTreePosition citiesTree;    //pokazivac na stablop gradova te drzave
    CountryHashPosition next;   //sljedeca drzava ka u istopm hashu
} CountryHash;

int hashFunction(char* countryName);
int addCountryToHash(CountryHashPosition* table, CountryHashPosition newCountry);
CityTreePosition addCityToTree(CityTreePosition root, CityTreePosition newCity);
int printCityTree(CityTreePosition root);
int printHashTable(CountryHashPosition* table);
CountryHashPosition findCountry(CountryHashPosition* table, char* name);
int printCityTreeAbovePopulation(CityTreePosition, int);
int freeCityTree(CityTreePosition root);
int freeHashTable(CountryHashPosition* table);

int main()
{
    CountryHashPosition hashTable[TABLE_SIZE] = { NULL };//stvaramo hash velicine 11 i stavljamo pokazivace na NULL

    FILE* file = fopen("drzave.txt", "r");
    if (!file) {
        printf("Greska pri otvaranju drzave.txt\n");
        return FILE_OPEN_ERROR;
    }

    char country[20], countryFile[30];
    char city[20];
    int population;

    while (fscanf(file, " %19[^,], %29s", country, countryFile) == 2) {

        CountryHashPosition newCountry = malloc(sizeof(CountryHash));
        if (!newCountry) return MALLOC_ERROR;

        strcpy(newCountry->name, country);
        newCountry->citiesTree = NULL;//inicijaliziramo prazno stablo gradova
        newCountry->next = NULL;//pokazivac na sljedecu drzavu

        addCountryToHash(hashTable, newCountry);//dodajemo drzavu u hash

        FILE* f = fopen(countryFile, "r");
        if (!f) {
            printf("Greska pri otvaranju %s\n", countryFile);
            return FILE_OPEN_ERROR;
        }

        while (fscanf(f, " %19[^,], %d", city, &population) == 2) {

            CityTreePosition newCity = malloc(sizeof(CityTree));
            if (!newCity) return MALLOC_ERROR;

            strcpy(newCity->name, city);
            newCity->population = population;
            newCity->left = newCity->right = NULL;

            newCountry->citiesTree =addCityToTree(newCountry->citiesTree, newCity);
        }

        fclose(f);
    }
    fclose(file);

    printHashTable(hashTable);

    char searchCountry[20];
    int minPopulation;

    while (1) {
        printf("\nUnesi ime drzave (0 za izlaz): ");
        scanf(" %19[^\n]", searchCountry);
        if (!strcmp(searchCountry, "0")) break;

        CountryHashPosition found = findCountry(hashTable, searchCountry);
        if (!found) {
            printf("Drzava ne postoji.\n");
            continue;
        }

        printf("Unesi minimalan broj stanovnika: ");
        scanf("%d", &minPopulation);

        printf("Gradovi s vise od %d stanovnika:\n", minPopulation);
        printCityTreeAbovePopulation(found->citiesTree, minPopulation);
    }

    freeHashTable(hashTable);
    return EXIT_SUCCESS;
}

int hashFunction(char* countryName)//uzima ime drzave i vraca broj izmedu 0 i 9
{
    int sum = 0;//vraijabla u kojoj zbrajamo ASCII vrjednosti
    for (int i = 0; i < 5 && countryName[i] != '\0'; i++)
        sum += (int)countryName[i];//zubraja ih u sumu

    return sum % TABLE_SIZE;//vraca ostatak
}

int addCountryToHash(CountryHashPosition* table, CountryHashPosition newCountry)//dodaje drzavu u has tablicu
{
    int index = hashFunction(newCountry->name);//vraca index koji odlucuje gdje ce drazava ic u hash tablicu
    CountryHashPosition current = table[index];//current pokazuej na prvu

    if (!current || strcmp(newCountry->name, current->name) < 0) {//uspordivanje abecedno ako je manje ide na pocetak
        newCountry->next = current;//nova pokazuje na predthodnui prvu
        table[index] = newCountry;//sad nova postaje prva u hashu
        return 0;
    }

    while (current->next && strcmp(newCountry->name, current->next->name) > 0)//ide dalje dok je ime nove vece od slijedece u listi
        current = current->next;

    newCountry->next = current->next;
    current->next = newCountry;
    return 0;
}


CityTreePosition addCityToTree(CityTreePosition root, CityTreePosition newCity)//dodaje grad u binarno stablo gradova
{                       //root trenutni cvor stabla newCity zelimo umetnuti u stablo
    if (!root) return newCity;//root je NULL mozemo ubacit na pocetak

    if (newCity->population < root->population)
        root->left = addCityToTree(root->left, newCity);//ako novi grad ima manje stanovnika od trenutnog ide lijevo u stablo
    else if (newCity->population > root->population)//ako novi ima vise od trenutnog ide desno
        root->right = addCityToTree(root->right, newCity);
    else if (strcmp(newCity->name, root->name) < 0)//ako je ime novcog abecedno manje od trenutnog ide lijevo
        root->left = addCityToTree(root->left, newCity);
    else
        root->right = addCityToTree(root->right, newCity);//sve ostalo ide desno

    return root;
}

int printCityTree(CityTreePosition root)
{   //funckijon ispoiusujemo sve gradove u binarno stablu INORDER jel ce tako bit po velicini poredani
    if (!root) return;//ako dodoemo do NULL
    printCityTree(root->left);
    printf("\t%s, %d\n", root->name, root->population);
    printCityTree(root->right);

    return EXIT_SUCCESS;
}

int printHashTable(CountryHashPosition* table)
{
    printf("\n========== HASH TABLICA DRZAVA ==========\n");

    for (int i = 0; i < TABLE_SIZE; i++) {

        if (table[i] == NULL)//ako hash ne sadrzi nijedu drzavu preskace ga
            continue;

        printf("\n[%d]\n", i);

        CountryHashPosition current = table[i];//current ona trenutna prva
        while (current) {//prolazimo kroz has do NULLA
            printf("%s:\n", current->name);
            printCityTree(current->citiesTree);
            current = current->next;
        }
    }
    return EXIT_SUCCESS;
}

CountryHashPosition findCountry(CountryHashPosition* table, char* name)//trazi drzavu po imenu u hash tablici
{
    int index = hashFunction(name);//vraca index gdje bi drzava trebala bit u hash tablici
    CountryHashPosition current = table[index];

    while (current) {
        if (strcmp(current->name, name) == 0)//usporeduje dok nenande iste po imenu 
            return current;//vraca ga ako smo nasli
        current = current->next;//kretanje po listi
    }
    return NULL;//vraca NULL ako nismo nasli trazenu drzavu
}


int printCityTreeAbovePopulation(CityTreePosition root, int minPopulation)
{
    if (!root) return;

    //idmeo u lijevo pod stablo jel ispisujemo rastuc
    if (root->population > minPopulation)
        printCityTreeAbovePopulation(root->left, minPopulation);

    //ako grad zadovoljava uvjet ispisujemo
    if (root->population > minPopulation)
        printf("\t%s, %d\n", root->name, root->population);

    //esno podstablo ima vece ili jednake populacije
    printCityTreeAbovePopulation(root->right, minPopulation);

    return EXIT_SUCCESS;
}

int freeCityTree(CityTreePosition root)
{
    if (!root) return;
    freeCityTree(root->left);//oslkobadoamo livo
    freeCityTree(root->right);//pa desno
    free(root);

    return EXIT_SUCCESS;
}

int freeHashTable(CountryHashPosition* table)
{
    for (int i = 0; i < TABLE_SIZE; i++) {
        CountryHashPosition current = table[i];
        while (current) {
            CountryHashPosition temp = current;//spremamo trenutni da ga kasnije mozemo osloboditi
            freeCityTree(current->citiesTree);//mroamo osloboditi stablo gradova do te drzave
            current = current->next;//idemo na slijedecu drzavu u listi
            free(temp);//oslobodamoi trenutni cvor drzave
        }
    }
    return EXIT_SUCCESS;
}
