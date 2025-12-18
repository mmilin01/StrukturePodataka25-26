/*
Napisati program koji omogućava rad s binarnim stablom pretraživanja. Treba
omogućiti unošenje novog elementa u stablo, ispis elemenata (inorder, preorder, postorder i
level order), brisanje i pronalaženje nekog elementa.
*/
#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>

typedef struct tree {
    int value;
    struct tree* left;
    struct tree* right;
} Tree;

typedef Tree* TreePos;

TreePos insertTree(TreePos root, int value);
TreePos deleteTree(TreePos root, int value);
TreePos find(TreePos root, int value);
TreePos findMin(TreePos root);
TreePos freeTree(TreePos root);

int inorder(TreePos root);
int preorder(TreePos root);
int postorder(TreePos root);

int main()
{
    TreePos root = NULL;
    int choice, value;

    do {
        printf("\n--- IZBORNIK ---\n");
        printf("1 - Unos elementa\n");
        printf("2 - Ispis inorder\n");
        printf("3 - Ispis preorder\n");
        printf("4 - Ispis postorder\n");
        printf("5 - Trazenje elementa\n");
        printf("6 - Brisanje elementa\n");
        printf("7 - Brisanje cijelog stabla\n");
        printf("0 - Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        switch (choice) {

        case 1:
            printf("Unesi vrijednost: ");
            scanf("%d", &value);
            root = insertTree(root, value);
            break;

        case 2:
            printf("Inorder: ");
            inorder(root);
            printf("\n");
            break;

        case 3:
            printf("Preorder: ");
            preorder(root);
            printf("\n");
            break;

        case 4:
            printf("Postorder: ");
            postorder(root);
            printf("\n");
            break;

        case 5:
            printf("Vrijednost za trazenje: ");
            scanf("%d", &value);
            if (find(root, value))
                printf("Element postoji u stablu.\n");
            else
                printf("Element NE postoji.\n");
            break;

        case 6:
            printf("Vrijednost za brisanje: ");
            scanf("%d", &value);
            root = deleteTree(root, value);
            break;

        case 7:
            root = freeTree(root);
            printf("Stablo obrisano.\n");
            break;

        case 0:
            root = freeTree(root);
            printf("Izlaz iz programa.\n");
            break;

        default:
            printf("Pogresan odabir!\n");
        }

    } while (choice != 0);

    return 0;
}


TreePos insertTree(TreePos root, int value) //treePos je pokazivac na korjen trenutniog stabka
{
    if (root == NULL) {     //root ce bit NULL ako je stablo prazno
        root = (TreePos)malloc(sizeof(Tree));   //aloc
        root->value = value;    //dajemo vrjednost
        root->left = NULL;  //stavljamo da su i lijevo i desno dijete NULL
        root->right = NULL;
    }
    else if (value < root->value)   //gledamo je li vrjednost manja od rootaValuea jel manje vrjednosti idu lijevo
        root->left = insertTree(root->left, value); //rekurzivno radimo insert zaljemo lijevo djete i vrjednost koju zelimo unijet
    else if (value > root->value)   //gledamo je li vrijednost veca od root jel vece idu desno
        root->right = insertTree(root->right, value); //rekurzivno radimo insert saljemo desno djete i vrjednost koju zelimo unijet

    return root;
}

TreePos find(TreePos root, int value)
{
    if (root == NULL || root->value == value)   // ako root je NULL il ako je value jednak rootu
        return root;

    if (value < root->value)
        return find(root->left, value); //ako je trazena vrjednsot manja u lijevom je podstablu
    else
        return find(root->right, value);    //ako je veca trazimo je u desno podsttablu
}

TreePos findMin(TreePos root)
{
    if (root == NULL)   
        return NULL;

    while (root->left != NULL)  //trazimo min lijevo jesu manje vrjednosti lijevo
        root = root->left;

    return root;
}

TreePos deleteTree(TreePos root, int value)
{
    TreePos temp;

    if (root == NULL)
        return NULL;

    if (value < root->value)
        root->left = deleteTree(root->left, value);
    else if (value > root->value)
        root->right = deleteTree(root->right, value);
    else {
        if (root->left != NULL && root->right != NULL) {
            temp = findMin(root->right);
            root->value = temp->value;
            root->right = deleteTree(root->right, temp->value);
        }
        else {
            temp = root;
            if (root->left == NULL)
                root = root->right;
            else
                root = root->left;
            free(temp);
        }
    }
    return root;
}

int inorder(TreePos root)
{
    if (root == NULL)
        return 0;

    inorder(root->left);    //u inorderu prvo ipsijume manji elementi  pa onda veci znaci prvo lijevo pdostabvlo pa desno
    printf("%d ", root->value);
    inorder(root->right);
    return 0;
}

int preorder(TreePos root)
{
    if (root == NULL)
        return 0;

    printf("%d ", root->value);
    preorder(root->left);
    preorder(root->right);
    return 0;
}

int postorder(TreePos root)
{
    if (root == NULL)
        return 0;

    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->value);
    return 0;
}

TreePos freeTree(TreePos root)
{
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
    return NULL;
}
