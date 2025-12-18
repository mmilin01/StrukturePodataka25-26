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

typedef struct queue {
    TreePos node;
    struct queue* next;
} Queue;

typedef Tree* TreePos;

TreePos insertTree(TreePos root, int value);
TreePos deleteTree(TreePos root, int value);
TreePos find(TreePos root, int value);
TreePos findMin(TreePos root);
TreePos freeTree(TreePos root);

int inorder(TreePos root);
int preorder(TreePos root);
int postorder(TreePos root);

int enqueue(Queue** front, Queue** rear, TreePos node);
TreePos dequeue(Queue** front, Queue** rear);
int levelorder(TreePos root);

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
        printf("8 - Ispis level order\n");
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
        
        case 8:
            printf("Level order: ");
            levelorder(root);
            printf("\n");
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

    if (value < root->value)//ako je vrijednsot manja od trenutne
        root->left = deleteTree(root->left, value);    //brisemoga u liejom stablu rekurzivno ga trazimo u lijevom stzalbu
    else if (value > root->value)    //ako je veca od trenutne trazimo ga u desnom podstablu i tu brisemo
        root->right = deleteTree(root->right, value);
    else {//pronsali smo cvor
        if (root->left != NULL && root->right != NULL) { //slucaj cvor ima dva djeteta
            temp = findMin(root->right);    //nadi najmanji element u desnom stablu
            root->value = temp->value;    //stavljamop njegovu vrijendost u trenutni cvor
            root->right = deleteTree(root->right, temp->value);    //obrsii pomocni cvor
        }
        else {    //cvor ima jedno dijete ili nijedno
            temp = root;
            if (root->left == NULL)//ako nema lijevog djeteta
                root = root->right;//desno dijete postaje root
            else
                root = root->left;//ili lijevo uzima mjesto roditelja
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
    printf("%d ", root->value);    //ispisuje se vrijednost trenutnog čvora
    inorder(root->right);    //zatim se ispisuju svi veći elementi
    return 0;
}

int preorder(TreePos root)
{
    if (root == NULL)
        return 0;

    printf("%d ", root->value);    //prvo se ispisuje vrijednost trenutnog čvora
    preorder(root->left);    //rekurzivno se ispisuje lijevo dijete
    preorder(root->right);    //rekurzivno se ispisuje desno dijete
    return 0;
}

int postorder(TreePos root)
{
    if (root == NULL)
        return 0;

    postorder(root->left);    //prvo idemo do najlijevijeg potomka
    postorder(root->right);        //zatim se na onog desnijeg
    printf("%d ", root->value);    //tek na kraju se ispisuje vrijednost trenutnog čvora
    return 0;
}

TreePos freeTree(TreePos root)
{
    if (root != NULL) {
        freeTree(root->left);    //brisnaje lijevog podstabla
        freeTree(root->right);    //brisanje desnog podstabla
        free(root);    //brsianje roota
    }
    return NULL;
}
int levelorder(TreePos root)
{
    Queue *front = NULL, *rear = NULL;    //front pokazuje na prvi elemetn u redu, a rear na zadnije element u redu
    TreePos current;

    if (root == NULL)
        return 0;

    enqueue(&front, &rear, root); //pocetniu korijen dodajemo u red

    while (front != NULL) {//do god ima cvora u redu skidamo prvi cvor ispisujemo njegovu vrjendost i stavljamu djecu u red
        current = dequeue(&front, &rear);//curent je onaj koji upravu koristimo deque ga uklanja iz reda
        printf("%d ", current->value);//ispisuje se vrijednsot cvora

        if (current->left != NULL)//lijevo dijete ide prvo stavljamo ga u red
            enqueue(&front, &rear, current->left);

        if (current->right != NULL)//desno dijete nakon njega uz red i ispisuju se
            enqueue(&front, &rear, current->right);
    }
    return 0;
}
int enqueue(Queue** front, Queue** rear, TreePos node)
{
    Queue* newNode = (Queue*)malloc(sizeof(Queue));//alčoc memmorije jer stavljmo novi element u red
    if (newNode == NULL)
        return -1;

    newNode->node = node;//pridodajemo vrijednsot
    newNode->next = NULL;

    if (*rear == NULL) {//ako je red rpazan noviu element pstoaje i front i rear
        *front = newNode;
        *rear = newNode;
    }
    else {//ako vec ima elementata
        (*rear)->next = newNode;//stari rear sad pokazuje na novi elkement
        *rear = newNode;//novi element posatje rear
    }
    return 0;
}
TreePos dequeue(Queue** front, Queue** rear)
{
    Queue* temp;
    TreePos node;

    if (*front == NULL)
        return NULL;

    temp = *front;//temp pokazuje na prvi element u niz(njega uklanjamo)
    node = temp->node;//node cuzva poklazvac na cvor stabla koje zelimo vratit
    *front = temp->next;//front sad pokazuej na sljedeci element u redu
                        //odvjamo temnp od reda
    if (*front == NULL)//ako vise nema elementata i rear psotavljamo na NULL
        *rear = NULL;

    free(temp);//oslobadamo mem
    return node;
}

