#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for a student
struct Student {
    int rollNumber;
    char name[50];
    float marks[3];
    float total;
};

// Function prototypes
void addRecord();
void displayRecords();
void generateRankList();

const char *FILENAME = "students.dat";

int main() {
    int choice;

    while (1) {
        printf("\n--- Student Marks Management System ---");
        printf("\n1. Add Student Record");
        printf("\n2. Display All Student Records");
        printf("\n3. Generate Rank List (Based on Total Marks)");
        printf("\n4. Exit");
        printf("\nSelect an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addRecord(); break;
            case 2: displayRecords(); break;
            case 3: generateRankList(); break;
            case 4: exit(0);
            default: printf("\nInvalid choice! Please try again.\n");
        }
    }
    return 0;
}

void addRecord() {
    FILE *fp = fopen(FILENAME, "ab"); // Open in append-binary mode
    struct Student s;

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Roll Number: ");
    scanf("%d", &s.rollNumber);
    printf("Enter Name: ");
    scanf(" %[^\n]s", s.name); // Space before % to capture strings with spaces
    
    s.total = 0;
    for (int i = 0; i < 3; i++) {
        printf("Enter marks for subject %d: ", i + 1);
        scanf("%f", &s.marks[i]);
        s.total += s.marks[i];
    }

    fwrite(&s, sizeof(struct Student), 1, fp);
    fclose(fp);
    printf("\nRecord added successfully!\n");
}

void displayRecords() {
    FILE *fp = fopen(FILENAME, "rb"); // Open in read-binary mode
    struct Student s;

    if (fp == NULL) {
        printf("\nNo records found. Please add a record first.\n");
        return;
    }

    printf("\n%-10s %-20s %-10s %-10s %-10s %-10s", "Roll No", "Name", "Sub 1", "Sub 2", "Sub 3", "Total");
    printf("\n----------------------------------------------------------------------\n");

    while (fread(&s, sizeof(struct Student), 1, fp)) {
        printf("%-10d %-20s %-10.2f %-10.2f %-10.2f %-10.2f\n", 
               s.rollNumber, s.name, s.marks[0], s.marks[1], s.marks[2], s.total);
    }

    fclose(fp);
}

void generateRankList() {
    FILE *fp = fopen(FILENAME, "rb");
    struct Student temp, *list = NULL;
    int count = 0;

    if (fp == NULL) {
        printf("\nNo records found.\n");
        return;
    }

    // Read all records into an array for sorting
    while (fread(&temp, sizeof(struct Student), 1, fp)) {
        list = realloc(list, (count + 1) * sizeof(struct Student));
        list[count++] = temp;
    }
    fclose(fp);

    // Simple Bubble Sort to rank students by total marks (descending)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (list[j].total < list[j + 1].total) {
                struct Student swap = list[j];
                list[j] = list[j + 1];
                list[j + 1] = swap;
            }
        }
    }

    printf("\n--- Rank List (Descending Order of Total Marks) ---\n");
    printf("%-5s %-10s %-20s %-10s\n", "Rank", "Roll No", "Name", "Total");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-10d %-20s %-10.2f\n", 
               i + 1, list[i].rollNumber, list[i].name, list[i].total);
    }

    free(list);
}
