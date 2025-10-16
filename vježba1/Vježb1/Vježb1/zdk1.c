#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char Name[50];
    char Surname[50];
    int points;
} Student;

// Broj linije u dat
int countLines(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file.\n");
        return -1;
    }
    int count = 0;
    char buffer[256]; // privremeni niz za pohranu linija
    //ide i cita red po red dok nedode do kraja dat
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        count++;
    }
    fclose(fp);
    return count;
}

// Alocira mem i cita imeprez...
int loadStudents(const char* filename, Student** students, int numStudents) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    // alociranej mem
    *students = malloc(numStudents * sizeof(Student));
    if (*students == NULL) {
        printf("Memory allocation error.\n");
        fclose(fp);
        return -1;
    }

    // cita iz filea i zapisuje
    for (int i = 0; i < numStudents; i++) {
        fscanf(fp, "%49s %49s %d", (*students)[i].Name, (*students)[i].Surname, &(*students)[i].points);
    }

    fclose(fp);
    return 0;
}

// Funkcija za ispis
int printStudents(Student* students, int numStudents, int maxPoints) {
    for (int i = 0; i < numStudents; i++) {
        double relative = ((double)students[i].points / maxPoints) * 100;
        printf("%s\t\t%s\t\t%d\t%.2f\n", students[i].Name, students[i].Surname, students[i].points, relative);
    }
    return 0;
}

int main() {
    const char* filename = "Students.txt";
    Student* students = NULL;
    int MAX_POINTS = 50; // ukuono ka koliko moze bit bodova
    int numStudents;

    numStudents = countLines(filename);
    if (numStudents <= 0) {
        printf("error reading file.\n");
        return 1;
    }

    if (loadStudents(filename, &students, numStudents) != 0) {
        return 1;
    }

    printStudents(students, numStudents, MAX_POINTS);

    free(students);
    return 0;
}
