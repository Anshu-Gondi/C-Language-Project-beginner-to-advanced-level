#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define student structure
struct Student {
    int roll;
    char name[50];
    float marks[5]; // 5 subjects
    float total;
    float average;
    char grade;
};

// Helper function to calculate total, average, grade
void assignGrade(struct Student *s) {
    s->total = 0;
    for (int i = 0; i < 5; i++)
        s->total += s->marks[i];
    s->average = s->total / 5.0;

    if (s->average >= 90) s->grade = 'A';
    else if (s->average >= 80) s->grade = 'B';
    else if (s->average >= 70) s->grade = 'C';
    else if (s->average >= 60) s->grade = 'D';
    else s->grade = 'F';
}

// Add a student record
void addStudent() {
    struct Student s;
    FILE *fp = fopen("students.dat", "ab");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Roll Number: ");
    scanf("%d", &s.roll);
    getchar(); // clear buffer

    printf("Enter Name: ");
    fgets(s.name, 50, stdin);
    s.name[strcspn(s.name, "\n")] = 0; // remove newline

    printf("Enter Marks for 5 Subjects: ");
    for (int i = 0; i < 5; i++)
        scanf("%f", &s.marks[i]);

    assignGrade(&s); // calculate total, average, grade

    fwrite(&s, sizeof(struct Student), 1, fp);
    fclose(fp);

    printf("✅ Student record added successfully!\n");
}

// Display all students
void displayStudents() {
    struct Student s;
    FILE *fp = fopen("students.dat", "rb");
    if (!fp) {
        printf("No records found!\n");
        return;
    }

    printf("\n--- Student Records ---\n");
    while (fread(&s, sizeof(struct Student), 1, fp)) {
        printf("Roll: %d | Name: %s\n", s.roll, s.name);
        printf("Marks: ");
        for (int i = 0; i < 5; i++)
            printf("%.2f ", s.marks[i]);
        printf("\nTotal: %.2f | Average: %.2f | Grade: %c\n", s.total, s.average, s.grade);
        printf("-------------------------\n");
    }
    fclose(fp);
}

// Search by roll number
void searchStudent() {
    int roll, found = 0;
    struct Student s;
    FILE *fp = fopen("students.dat", "rb");
    if (!fp) {
        printf("No records found!\n");
        return;
    }

    printf("Enter Roll Number to search: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(struct Student), 1, fp)) {
        if (s.roll == roll) {
            printf("✅ Student Found!\n");
            printf("Roll: %d | Name: %s\n", s.roll, s.name);
            printf("Marks: ");
            for (int i = 0; i < 5; i++)
                printf("%.2f ", s.marks[i]);
            printf("\nTotal: %.2f | Average: %.2f | Grade: %c\n", s.total, s.average, s.grade);
            found = 1;
            break;
        }
    }

    if (!found) printf("❌ No student found with Roll %d\n", roll);

    fclose(fp);
}

// Delete student by roll number
void deleteStudent() {
    int roll, found = 0;
    struct Student s;

    printf("Enter Roll Number to delete: ");
    scanf("%d", &roll);

    FILE *fp = fopen("students.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (!fp || !temp) {
        printf("Error opening file!\n");
        return;
    }

    while (fread(&s, sizeof(struct Student), 1, fp)) {
        if (s.roll != roll)
            fwrite(&s, sizeof(struct Student), 1, temp);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("students.dat");
        rename("temp.dat", "students.dat");
        printf("✅ Student record deleted successfully!\n");
    } else {
        remove("temp.dat");
        printf("❌ Student with Roll %d not found.\n", roll);
    }
}

// Modify student record
void modifyStudent() {
    struct Student s;
    int roll, found = 0;

    printf("Enter Roll Number to modify: ");
    scanf("%d", &roll);

    FILE *fp = fopen("students.dat", "rb+");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    while (fread(&s, sizeof(struct Student), 1, fp)) {
        if (s.roll == roll) {
            getchar(); // clear buffer
            printf("Enter new Name: ");
            fgets(s.name, 50, stdin);
            s.name[strcspn(s.name, "\n")] = 0;

            printf("Enter new Marks for 5 Subjects: ");
            for (int i = 0; i < 5; i++)
                scanf("%f", &s.marks[i]);

            assignGrade(&s); // recalculate total, average, grade

            fseek(fp, -sizeof(struct Student), SEEK_CUR);
            fwrite(&s, sizeof(struct Student), 1, fp);
            found = 1;
            printf("✅ Student record updated!\n");
            break;
        }
    }

    if (!found) printf("❌ Student with Roll %d not found.\n", roll);

    fclose(fp);
}

// Main menu
int main() {
    int choice;
    do {
        printf("\nStudent Report Card System\n");
        printf("1. Add Student Record\n");
        printf("2. Display All Student Records\n");
        printf("3. Search Student by Roll Number\n");
        printf("4. Delete Student Record\n");
        printf("5. Modify Student Record\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: deleteStudent(); break;
            case 5: modifyStudent(); break;
            case 6: printf("Exiting...\n"); break;
            default: printf("❌ Invalid choice! Try again.\n");
        }
    } while (choice != 6);

    return 0;
}
