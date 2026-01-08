/*
Napisati program koji èita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih država. Uz
ime države u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
države. Svaka datoteka koja predstavlja državu sadrži popis gradova u formatu naziv_grada,
broj_stanovnika.

a) Potrebno je formirati sortiranu vezanu listu država po nazivu. Svaki èvor vezane liste
sadrži stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.

b) Potrebno je formirati stablo država sortirano po nazivu. Svaki èvor stabla sadrži vezanu
listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.

Nakon formiranja podataka potrebno je ispisati države i gradove te omoguæiti korisniku putem
tastature pretragu gradova odreðene države koji imaju broj stanovnika veæi od unosa na
tastaturi.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -3

/* STRUKTURE */

struct cityTree;
typedef struct cityTree* CityTreePosition;
typedef struct cityTree {
    char name[20];
    int population;
    CityTreePosition left;
    CityTreePosition right;
} CityTree;

struct cityList;
typedef struct cityList* CityListPosition;
typedef struct cityList {
    char name[20];
    int population;
    CityListPosition next;
} CityList;

struct countryList;
typedef struct countryList* CountryListPosition;
typedef struct countryList {
    char name[20];
    CityTreePosition citiesTree;
    CountryListPosition next;
} CountryList;

struct countryTree;
typedef struct countryTree* CountryTreePosition;
typedef struct countryTree {
    char name[20];
    CityList cityList;
    CountryTreePosition left;
    CountryTreePosition right;
} CountryTree;

int addCountryToList(CountryListPosition current, CountryListPosition newCountry);
CountryTreePosition addCountryToTree(CountryTreePosition current, CountryTreePosition newCountry);
CityTreePosition addCityToTree(CityTreePosition current, CityTreePosition newCity);
int addCityToList(CityListPosition current, CityListPosition newCity);
int printCityTree(CityTreePosition current);
int printCityList(CityListPosition current);
int printCountryList(CountryListPosition current);
int printCountryTree(CountryTreePosition current);
CountryTreePosition findCountryByName(CountryTreePosition current, char* name);

int main()
{
    CountryList countryListHead = { .name = {0}, .next = NULL, .citiesTree = NULL };
    CountryTreePosition countryTree = NULL;

    char country[20], countryFile[24], city[20];
    int population;

    FILE* file = fopen("drzave.txt", "r");
    if (!file) {
        printf("Could not open drzave.txt\n");
        return FILE_OPEN_ERROR;
    }

    while (fscanf(file, " %19[^,], %23s", country, countryFile) == 2) {//citaj sve karaktere dok nedodes do ,
                                                                    //fscanf vraca broj uspjeno procitanih elemenata staej 2 ako je ispravno
        // NOVI ČVOR VEZANE LISTE DRŽAVA
        CountryListPosition newCountryList = malloc(sizeof(CountryList));
        if (!newCountryList) return MALLOC_ERROR;
        strcpy(newCountryList->name, country);//ime stavljamo isto kaop procitan county
        newCountryList->citiesTree = NULL;//je pokazivač na novi čvor vezane liste država
        newCountryList->next = NULL;
        addCountryToList(&countryListHead, newCountryList);

        // NOVI ČVOR STABLA DRŽAVA
        CountryTreePosition newCountryTree = malloc(sizeof(CountryTree));
        if (!newCountryTree) return MALLOC_ERROR;
        strcpy(newCountryTree->name, country);
        newCountryTree->left = NULL;
        newCountryTree->right = NULL;
        newCountryTree->cityList.next = NULL;
        newCountryTree->cityList.population = 0;
        newCountryTree->cityList.name[0] = '\0';

        countryTree = addCountryToTree(countryTree, newCountryTree);//newCountryTree Ovo je novi čvor stabla države koji smo upravo kreirali za državu
                                                                    //countryTree Ovo je pokazivač na korjenski čvor stabla država
        FILE* countryFilePtr = fopen(countryFile, "r");
        if (!countryFilePtr) {
            printf("Could not open %s\n", countryFile);
            continue;
        }

        while (fscanf(countryFilePtr, " %19[^,], %d", city, &population) == 2) {

            // DODAJ U STABLO GRADOVA UNUTAR LISTE DRŽAVA
            CityTreePosition newCityTree = malloc(sizeof(CityTree));
            if (!newCityTree) return MALLOC_ERROR;
            strcpy(newCityTree->name, city);
            newCityTree->population = population;
            newCityTree->left = newCityTree->right = NULL;
            newCountryList->citiesTree = addCityToTree(newCountryList->citiesTree, newCityTree);

            // DODAJ U VEZANU LISTU GRADOVA UNUTAR STABLA DRŽAVA
            CityListPosition newCityList = malloc(sizeof(CityList));
            if (!newCityList) return MALLOC_ERROR;
            strcpy(newCityList->name, city);
            newCityList->population = population;
            newCityList->next = NULL;
            addCityToList(&newCountryTree->cityList, newCityList);
        }

        fclose(countryFilePtr);
    }

    fclose(file);

    // ISPIS VEZANE LISTE DRŽAVA SA STABLIMA GRADOVA
    printf("LISTA DRZAVA SA GRADOVIMA (STABLO GRADOVA):\n");
    printCountryList(countryListHead.next);

    // ISPIS STABLA DRŽAVA SA VEZANIM LISTAMA GRADOVA
    printf("\nSTABLO DRZAVA SA VEZANIM LISTAMA GRADOVA:\n");
    printCountryTree(countryTree);

    // PRETRAGA PO DRŽAVI I MIN POPULACIJI
    char countryChoice[20];
    int minPopulation;
    while (1) {
        printf("\nEnter country name (0 to exit): ");
        scanf(" %19[^\n]", countryChoice);
        if (!strcmp(countryChoice, "0")) break;

        CountryTreePosition chosenCountry = findCountryByName(countryTree, countryChoice);
        if (!chosenCountry) {
            printf("No such country.\n");
            continue;
        }

        printf("Enter minimum population: ");
        scanf(" %d", &minPopulation);

        // Ispis gradova iz vezane liste u stablo države
        CityListPosition ptr = chosenCountry->cityList.next;
        int found = 0;
        while (ptr) {
            if (ptr->population > minPopulation) {
                printf("\t%s, %d\n", ptr->name, ptr->population);
                found = 1;
            }
            ptr = ptr->next;
        }
        if (!found) printf("No cities with population > %d\n", minPopulation);
    }

    return EXIT_SUCCESS;
}

/* FUNKCIJE */

int addCountryToList(CountryListPosition current, CountryListPosition newCountry)//current je trenutni pokazivac na cvor listee od kojeg pocinjem pretragu
{                                                                               //new countrty treba ubacit
    while (current->next && strcmp(newCountry->name, current->next->name) > 0)//gledamo jel postoji slijedecu u listi i usporedujemo imena drzava da ostanu abecedno sortirane
        current = current->next;
    newCountry->next = current->next;
    current->next = newCountry;
    return EXIT_SUCCESS;
}

CountryTreePosition addCountryToTree(CountryTreePosition current, CountryTreePosition newCountry)//current je trenutni pokazivac na cvor listee od kojeg pocinjem pretragu                                                                               //new countrty treba ubacit
{
    if (!current) return newCountry;
    if (strcmp(newCountry->name, current->name) < 0)
        current->left = addCountryToTree(current->left, newCountry);
    else if (strcmp(newCountry->name, current->name) > 0)
        current->right = addCountryToTree(current->right, newCountry);
    return current;
}

CityTreePosition addCityToTree(CityTreePosition current, CityTreePosition newCity)
{
    if (!current) return newCity;

    if (newCity->population < current->population)
        current->left = addCityToTree(current->left, newCity);
    else if (newCity->population > current->population)
        current->right = addCityToTree(current->right, newCity);
    else if (strcmp(newCity->name, current->name) < 0)
        current->left = addCityToTree(current->left, newCity);
    else
        current->right = addCityToTree(current->right, newCity);

    return current;
}

int addCityToList(CityListPosition current, CityListPosition newCity)
{
    while (current->next && newCity->population > current->next->population)
        current = current->next;
    while (current->next &&
        current->next->population == newCity->population &&
        strcmp(newCity->name, current->next->name) > 0)
        current = current->next;

    newCity->next = current->next;
    current->next = newCity;
    return EXIT_SUCCESS;
}

int printCityTree(CityTreePosition current)
{
    if (!current) return EXIT_SUCCESS;
    printCityTree(current->left);
    printf("\t%s, %d\n", current->name, current->population);
    printCityTree(current->right);
    return EXIT_SUCCESS;
}

int printCityList(CityListPosition current)
{
    while (current) {
        printf("\t%s, %d\n", current->name, current->population);
        current = current->next;
    }
    return EXIT_SUCCESS;
}

int printCountryList(CountryListPosition current)
{
    while (current) {
        printf("%s:\n", current->name);
        printCityTree(current->citiesTree);
        current = current->next;
    }
    return EXIT_SUCCESS;
}

int printCountryTree(CountryTreePosition current)
{
    if (!current) return EXIT_SUCCESS;
    printCountryTree(current->left);
    printf("%s:\n", current->name);
    printCityList(current->cityList.next);
    printCountryTree(current->right);
    return EXIT_SUCCESS;
}

CountryTreePosition findCountryByName(CountryTreePosition current, char* name)
{
    if (!current) return NULL;
    if (strcmp(name, current->name) < 0)
        return findCountryByName(current->left, name);
    else if (strcmp(name, current->name) > 0)
        return findCountryByName(current->right, name);
    return current;
}
