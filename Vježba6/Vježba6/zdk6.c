#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 128
#define DATE_LEN 11

// -------------------------------
//          STRUCTOVI
// -------------------------------

// Cvor artikla
typedef struct _article {
    char name[MAX_SIZE];
    int qty;
    float price;
    struct _article* next;
} Article;

typedef Article* ArticlePosition;

// Cvor računa
typedef struct _receipt {
    int year, month, day;
    ArticlePosition articles;
    struct _receipt* next;
} Receipt;

typedef Receipt* ReceiptPosition;

// Lista za sumiranje artikala
typedef struct _articleSummary {
    char name[MAX_SIZE];
    int totalQty;
    float totalIncome;
    struct _articleSummary* next;
} ArticleSummary, * ArticleSummaryPosition; //pokazivac na sturkttur summary

// -------------------------------
//       DEKLARACIJE FUNKCIJA
// -------------------------------

int addArticle(ArticlePosition head, ArticlePosition article);
int addReceipt(ReceiptPosition head, ReceiptPosition receipt);
int readReceipt(char* fileName, ReceiptPosition headReceipt);
int printArticles(ArticlePosition article);
int printReceipts(ReceiptPosition receipt);
float getArticleInPeriod(ReceiptPosition currentReceipt, char* article, char* startDate, char* endDate, float* income, int* count);
int deleteArticles(ArticlePosition articleHead);
int deleteReceipts(ReceiptPosition receiptHead);

// Funkcije za analizu artikala
ArticleSummaryPosition summarizeArticles(ReceiptPosition receipts);
int mostSoldArticle(ReceiptPosition receipts);
int leastSoldArticle(ReceiptPosition receipts);
int highestIncomeArticle(ReceiptPosition receipts);
int lowestIncomeArticle(ReceiptPosition receipts);

// =================== MAIN ===================
int main()
{
    Receipt receipts = { 0,0,0, NULL, NULL };  // head za listu računa
    receipts.articles = NULL;

    // učitaj sve račune iz extras/racuni.txt
    FILE* fp = fopen("extras/racuni.txt", "r");
    if (!fp) {
        printf("Ne mogu otvoriti extras/racuni.txt\n");
        return -1;
    }

    char fileName[MAX_SIZE];
    while (fgets(fileName, sizeof(fileName), fp))//cita jedan reed datoteke u ovon slucaju ime fileqa izvrasasva se sve dok ima redova
    {
        fileName[strcspn(fileName, "\n")] = 0;  // ukloni \n
        readReceipt(fileName, &receipts);
    }
    fclose(fp);

    int option = 0;
    do {
        printf("\n======= MENU =======\n");
        printf("1 - Ispis svih racuna\n");
        printf("2 - Pretraga artikla u periodu\n");
        printf("3 - Najprodavaniji artikl (kolicina)\n");
        printf("4 - Najmanje prodavan artikl (kolicina)\n");
        printf("5 - Artikl sa najvecim prihodima\n");
        printf("6 - Artikl sa najmanjim prihodima\n");
        printf("0 - Izlaz\n");
        printf("====================\n");
        printf("Izbor: ");

        if (scanf("%d", &option) != 1) {
            printf("Neispravan unos.\n");
            break;
        }

        switch (option) {
        case 1: printReceipts(&receipts); break;
        case 2: {
            char item[MAX_SIZE], start[DATE_LEN], end[DATE_LEN];
            float income = 0;
            int count = 0;
            printf("Unesi artikl: ");
            scanf(" %s", item);
            printf("Unesi pocetni datum (YYYY-MM-DD): ");
            scanf(" %s", start);
            printf("Unesi krajnji datum (YYYY-MM-DD): ");
            scanf(" %s", end);
            getArticleInPeriod(&receipts, item, start, end, &income, &count);
            printf("\nUkupno potroseno: %.2f EUR\n", income);
            printf("Ukupna kolicina: %d kom\n", count);
            break;
        }
        case 3: mostSoldArticle(&receipts); break;
        case 4: leastSoldArticle(&receipts); break;
        case 5: highestIncomeArticle(&receipts); break;
        case 6: lowestIncomeArticle(&receipts); break;
        case 0: printf("Izlaz iz programa.\n"); break;
        default: printf("Nepoznata opcija.\n");
        }

    } while (option != 0);

    deleteReceipts(&receipts);
    return 0;
}

int parseDate(char* date, int* y, int* m, int* d)
{
    sscanf(date, "%d-%d-%d", y, m, d);
    
    return 0;
}

// pretvara Y, M, D u int za lako uspoređivanje
int dateToInt(int y, int m, int d)
{
    return y * 10000 + m * 100 + d;
}

//   SORTIRANI UNOS ARTIKALA PO NAZIVU
int addArticle(ArticlePosition head, ArticlePosition newA)
{
    ArticlePosition temp = head;
    while (temp->next != NULL && strcmp(temp->next->name, newA->name) < 0)
        temp = temp->next;

    newA->next = temp->next;
    temp->next = newA;

    return 0;
}

//   SORTIRANI UNOS RAČUNA PO DATUMU
int addReceipt(ReceiptPosition head, ReceiptPosition newR)
{
    int newDate = dateToInt(newR->year, newR->month, newR->day);
    ReceiptPosition temp = head;
    while (temp->next != NULL) {
        int curDate = dateToInt(temp->next->year, temp->next->month, temp->next->day);
        if (newDate < curDate) break;
        temp = temp->next;
    }

    newR->next = temp->next;
    temp->next = newR;
    return 0;
}


//      UČITAVANJE JEDNOG RAČUNA
int readReceipt(char* fileName, ReceiptPosition headReceipt)
{
    FILE* fp = fopen(fileName, "r");
    if (!fp) {
        printf("Ne mogu otvoriti datoteku: %s\n", fileName);
        return -1;
    }

    char date[DATE_LEN];
    if (!fgets(date, DATE_LEN, fp)) {   //cita prvi red i uzimaga kao string
        printf("Datoteka %s je prazna!\n", fileName);
        fclose(fp);
        return -1;
    }
    date[strcspn(date, "\n")] = 0;  // ukloni \n

    int y, m, d;
    parseDate(date, &y, &m, &d);

    ReceiptPosition r = (ReceiptPosition)malloc(sizeof(Receipt));//lsita za datum
    if (!r) { fclose(fp); return -1; }

    r->year = y;
    r->month = m;
    r->day = d;
    r->next = NULL;

    r->articles = (ArticlePosition)malloc(sizeof(Article));//lsita za artikle
    strcpy(r->articles->name, "");//prazan stirng
    r->articles->qty = 0;
    r->articles->price = 0;
    r->articles->next = NULL;

    char line[MAX_SIZE];
    while (fgets(line, sizeof(line), fp)) { //cita red datoteke i sprema ga u lin e
        if (line[0] == '\n' || line[0] == '\0') continue;    // presakce linije ako je prazna il nista nema u njoj

        ArticlePosition a = (ArticlePosition)malloc(sizeof(Article));   //stavara novi artikal i alloc mem
        sscanf(line, "%[^,], %d, %f", a->name, &a->qty, &a->price); //%[^,] čitaj sve znakove dok ne dođeš do zareza
        a->next = NULL;
        addArticle(r->articles, a); //r->actukles he dumm head a=novi artikl kjoji ubacujemo
    }

    fclose(fp);
    addReceipt(headReceipt, r);

    return 0;
}

int printArticles(ArticlePosition a)
{
    a = a->next;
    while (a) {
        printf("  %s x%d (%.2f EUR)\n", a->name, a->qty, a->price);
        a = a->next;
    }
    return 0;
}
int printReceipts(ReceiptPosition r)
{
    r = r->next;
    while (r) {
        printf("\nDatum: %04d-%02d-%02d\n", r->year, r->month, r->day);
        printf("------------------------\n");
        printArticles(r->articles);
        r = r->next;
    }
    return 0;
}

//   RAČUNANJE POTROŠNJE ZA ARTIKL U VREMENSKOM PERIODU
float getArticleInPeriod(ReceiptPosition current, char* article, char* startDate, char* endDate, float* income, int* count)
{
    int sy, sm, sd, ey, em, ed;
    parseDate(startDate, &sy, &sm, &sd);
    parseDate(endDate, &ey, &em, &ed);
    int startInt = dateToInt(sy, sm, sd);
    int endInt = dateToInt(ey, em, ed);

    current = current->next;
    while (current) {
        int curDate = dateToInt(current->year, current->month, current->day);
        if (curDate >= startInt && curDate <= endInt) { //gledamo jese datum nalazi u trazenom periodu
            ArticlePosition a = current->articles->next;        //pokazuje na porvi pravi artikal u racunu
            while (a) {
                if (strcmp(a->name, article) == 0) {        //gledamo jel na racunu imam artikal kojg trazimo
                    *income += (float)(a->qty * a->price);        //racunanje sume
                    *count += a->qty;       //glkedamo koliko ihhe
                }
                a = a->next;
            }
        }
        current = current->next;
    }
    return 0;
}

int deleteArticles(ArticlePosition head)
{
    ArticlePosition temp;
    while (head->next) {
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }
    return 0;
}

int deleteReceipts(ReceiptPosition head)
{
    ReceiptPosition temp;
    while (head->next) {
        temp = head->next;
        head->next = temp->next;
        deleteArticles(temp->articles);
        free(temp->articles);
        free(temp);
    }
    return 0;
}

ArticleSummaryPosition summarizeArticles(ReceiptPosition receipts)
{
    ArticleSummary head = { "", 0, 0.0, NULL };     //head cvor 
    ReceiptPosition r = receipts->next;     //racuni micemo ih na prvbi realan element

    while (r) {
        ArticlePosition a = r->articles->next;      //artikli lsita presakcemo head i idemo na prvbi realni
        while (a) {
            if (a->qty <= 0) { a = a->next; continue; } //ako je nula presakacemo nema smisal zbrajat
            ArticleSummaryPosition curr = head.next;    //postavlja pokazivač na početak stvarnih elemenata liste
            while (curr) {      //cur pokazuje na prvi stvarin element
                if (strcmp(curr->name, a->name) == 0) { //glead jel postoji artitkal istog imena vec u listi ako postoji vraca 0
                    curr->totalQty += a->qty;   //povecamo kolicinu za taj aritkl
                    curr->totalIncome += a->qty * a->price;     //isto samo sa cjenom
                    break;
                }
                curr = curr->next;  //ako ime nije isto pomicemo curr na slijedeci cvor u listi
            }
            if (!curr) {
                ArticleSummaryPosition newNode = (ArticleSummaryPosition)malloc(sizeof(ArticleSummary));        //stvaramo novi artikal u lsoti
                strcpy(newNode->name, a->name);     //ime se kopira u novu lsitu
                newNode->totalQty = a->qty;     //pocetna kolicina ona iz trbutnog racuna
                newNode->totalIncome = a->qty * a->price;   //pocentni prihod
                newNode->next = head.next;  //insertamoga na pocetak
                head.next = newNode;
            }
            a = a->next;
        }
        r = r->next;
    }

    return head.next;      //vracamo pokazivac na prvi stavrni cvor
}


int mostSoldArticle(ReceiptPosition receipts)
{
    ArticleSummaryPosition head = summarizeArticles(receipts);  //rad listu svih artikala
    if (!head) { printf("Nema artikala.\n"); return; }

    ArticleSummaryPosition maxQty = head;       //trenutni najprofddavaniji
    ArticleSummaryPosition curr = head->next;   //iduci snjin usporedujemop
    while (curr){
        if (curr->totalQty > maxQty->totalQty) //akoje trneutni veci od max minjamo ih
            maxQty = curr; 
        curr = curr->next; 
    }
    
    printf("Najprodavaniji artikl (kolicina): %s x%d\n", maxQty->name, maxQty->totalQty);

    curr = head;
    while (curr){ 
        ArticleSummaryPosition temp = curr; 
        curr = curr->next; 
        free(temp); 
    }
    
    return 0;
}

int leastSoldArticle(ReceiptPosition receipts)
{
    ArticleSummaryPosition head = summarizeArticles(receipts);  //kreira listu svih artikala sa svih racuna
    if (!head) { printf("Nema artikala.\n"); return; }

    ArticleSummaryPosition minQty = NULL;
    ArticleSummaryPosition curr = head;
   
    curr = head->next;
    while (curr) {
        if (curr->totalQty < minQty->totalQty)  //akoje trenutni manji od namnajeg zamjena
            minQty = curr;
        curr = curr->next;
    }

    printf("Najmanje prodavan artikl (kolicina): %s x%d\n", minQty->name, minQty->totalQty);

    curr = head;
    while (curr){ 
        ArticleSummaryPosition temp = curr; 
        curr = curr->next; 
        free(temp); 
    }
    
    return 0;
}

int highestIncomeArticle(ReceiptPosition receipts)
{
    ArticleSummaryPosition head = summarizeArticles(receipts);      //prolazi kroz sve racune i stabra listu 
    if (!head) { printf("Nema artikala.\n"); return; }

    ArticleSummaryPosition maxIncome = head;    //trneutni najveci
    ArticleSummaryPosition curr = head->next;   //sljedeci
    while (curr){ 
        if (curr->totalIncome > maxIncome->totalIncome) //ako je trenutni veci od najveceg minjamio ih
            maxIncome = curr; 
        curr = curr->next; 
    }
    printf("Artikl sa najvecim prihodima: %s (%.2f EUR)\n", maxIncome->name, maxIncome->totalIncome);

    curr = head;
    while (curr){ 
        ArticleSummaryPosition temp = curr; 
        curr = curr->next; 
        free(temp); 
    }
    
    return 0;
}

int lowestIncomeArticle(ReceiptPosition receipts)
{
    ArticleSummaryPosition head = summarizeArticles(receipts);
    if (!head) { printf("Nema artikala.\n"); return; }

    ArticleSummaryPosition minIncome = head; // trenutni namjanji
    ArticleSummaryPosition curr = head->next;   //slijedeci

    while (curr) {
        if (curr->totalIncome < minIncome->totalIncome)     //usporedujemo kao je trenutni manji od najmanjeng zamjena
            minIncome = curr;
        curr = curr->next;
    }

    printf("Artikl sa najmanjim prihodima: %s (%.2f EUR)\n", minIncome->name, minIncome->totalIncome);

    
    curr = head;
    while (curr){ 
        ArticleSummaryPosition temp = curr; 
        curr = curr->next; 
        free(temp); 
    }

    return 0;
}


