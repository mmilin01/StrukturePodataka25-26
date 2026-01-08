#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Node {
    int data;
    struct Node* left, * right;
};
struct Node* insert(struct Node* , int );
int replace(struct Node* );
void inorderToFile(struct Node* , FILE* );

int main() {
    struct Node* root = NULL;
    srand(time(NULL));

    // c) Generiranje 10 slučajnih brojeva u rasponu <10, 90> (od 11 do 89)
    printf("Generirani brojevi: ");
    for (int i = 0; i < 10; i++) {
        int num = (rand() % 79) + 11;
        printf("%d ", num);
        root = insert(root, num);
    }

    FILE* fp = fopen("stablo.txt", "w");
    if (fp == NULL) {
        printf("\nGreška pri otvaranju datoteke!");
        return 1;
    }

    // Upis stanja nakon a) dijela
    fprintf(fp, "Inorder nakon insert funkcije (a):\n");
    inorderToFile(root, fp);
    fprintf(fp, "\n\n");

    // Izvršavanje b) dijela
    replace(root);

    // Upis stanja nakon b) dijela
    fprintf(fp, "Inorder nakon replace funkcije (b):\n");
    inorderToFile(root, fp);

    fclose(fp);
    printf("\nPodaci su uspjesno zapisani u 'stablo.txt'.\n");

    return 0;
}

// a) Funkcija za umetanje elementa u stablo
struct Node* insert(struct Node* root, int value) {
    if (root == NULL) {
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->data = value;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (value < root->data) {
        root->right = insert(root->right, value);
    }
    else {
        root->left = insert(root->left, value);
    }
    return root;
}

// b) Funkcija koja zamjenjuje vrijednost čvora sumom svih njegovih potomaka
int replace(struct Node* root) {
    if (root == NULL) return 0;

    // Pohrani trenutnu vrijednost (prije zamjene) da je možemo vratiti roditelju
    int oldValue = root->data;

    // Rekurzivno izračunaj sume lijevog i desnog podstabla
    int sumLeft = replace(root->left);
    int sumRight = replace(root->right);

    // Postavi novu vrijednost čvora kao sumu potomaka
    root->data = sumLeft + sumRight;

    // Vrati ukupnu sumu ovog podstabla (nova vrijednost + stara vrijednost čvora)
    return root->data + oldValue;
}

// Funkcija za inorder ispis u datoteku
void inorderToFile(struct Node* root, FILE* fp) {
    if (root != NULL) {
        inorderToFile(root->left, fp);
        fprintf(fp, "%d ", root->data);
        inorderToFile(root->right, fp);
    }
}

