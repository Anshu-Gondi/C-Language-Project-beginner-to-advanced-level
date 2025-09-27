#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define Book Structure
struct Book {
    int id;
    char title[100];
    char author[50];
    char publisher[50];
    int year;
    int availablecopies; 
    int totalcopies;
    char category[30];
};

// ADD a new book
void addBook() {
    struct Book b;
    FILE *fp = fopen("books.dat", "ab");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &b.id);
    getchar(); // consume newline

    printf("Enter Title: ");
    fgets(b.title, sizeof(b.title), stdin);
    b.title[strcspn(b.title, "\n")] = 0; 

    printf("Enter Author: ");
    fgets(b.author, sizeof(b.author), stdin);
    b.author[strcspn(b.author, "\n")] = 0; 

    printf("Enter Publisher: ");
    fgets(b.publisher, sizeof(b.publisher), stdin);
    b.publisher[strcspn(b.publisher, "\n")] = 0; 

    printf("Enter Year of Publication: ");
    scanf("%d", &b.year);

    printf("Enter Total Copies: ");
    scanf("%d", &b.totalcopies);

    printf("Enter Category: ");
    getchar();
    fgets(b.category, sizeof(b.category), stdin);
    b.category[strcspn(b.category, "\n")] = 0; 

    b.availablecopies = b.totalcopies; // Initially all copies available
    fwrite(&b, sizeof(struct Book), 1, fp);
    fclose(fp);

    printf("✅ Book added successfully!\n");
}

// VIEW all books
void viewBooks() {
    struct Book b;
    FILE *fp = fopen("books.dat", "rb");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("\n--- Library Books ---\n");
    while (fread(&b, sizeof(struct Book), 1, fp)) {
        printf("ID: %d\nTitle: %s\nAuthor: %s\nPublisher: %s\nYear: %d\nAvailable Copies: %d\nTotal Copies: %d\nCategory: %s\n",
               b.id, b.title, b.author, b.publisher, b.year, b.availablecopies, b.totalcopies, b.category);
        printf("---------------------\n");
    }
    fclose(fp);
}

// SEARCH a book by ID
void searchBook() {
    int id, found = 0;
    struct Book b;
    FILE *fp = fopen("books.dat", "rb");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Book ID to search: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(struct Book), 1, fp)) {
        if (b.id == id) {
            printf("✅ Book Found:\nID: %d\nTitle: %s\nAuthor: %s\nPublisher: %s\nYear: %d\nAvailable Copies: %d\nTotal Copies: %d\nCategory: %s\n",
                   b.id, b.title, b.author, b.publisher, b.year, b.availablecopies, b.totalcopies, b.category);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("❌ Book with ID %d not found.\n", id);
    }
    fclose(fp);
}

// DELETE a book by ID
void deleteBook() {
    int id, found = 0;
    struct Book b;
    FILE *fp = fopen("books.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (!fp || !temp) { 
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Book ID to delete: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(struct Book), 1, fp)) {
        if (b.id != id) {
            fwrite(&b, sizeof(struct Book), 1, temp);
        } else {
            found = 1; 
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("books.dat");
        rename("temp.dat", "books.dat");
        printf("✅ Book with ID %d deleted successfully!\n", id);
    } else {
        remove("temp.dat");
        printf("❌ Book with ID %d not found.\n", id);
    }
}

// UPDATE book by ID
void updateBook() {
    int id, found = 0;
    struct Book b;
    FILE *fp = fopen("books.dat", "rb+");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Book ID to update: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(struct Book), 1, fp)) {
        if (b.id == id) {
            found = 1;
            printf("Updating Book with ID %d\n", id);

            getchar(); // consume newline
            printf("Enter New Title: ");
            fgets(b.title, sizeof(b.title), stdin);
            b.title[strcspn(b.title, "\n")] = 0;

            printf("Enter New Author: ");
            fgets(b.author, sizeof(b.author), stdin);
            b.author[strcspn(b.author, "\n")] = 0;

            printf("Enter New Publisher: ");
            fgets(b.publisher, sizeof(b.publisher), stdin);
            b.publisher[strcspn(b.publisher, "\n")] = 0;

            printf("Enter New Year of Publication: ");
            scanf("%d", &b.year);

            printf("Enter New Total Copies: ");
            scanf("%d", &b.totalcopies);

            getchar(); // consume newline
            printf("Enter New Category: ");
            fgets(b.category, sizeof(b.category), stdin);
            b.category[strcspn(b.category, "\n")] = 0;

            b.availablecopies = b.totalcopies;

            fseek(fp, -(long)sizeof(struct Book), SEEK_CUR); // ✅ fix overflow warning
            fwrite(&b, sizeof(struct Book), 1, fp);
            break;
        }
    }
    if (!found) {
        printf("❌ Book with ID %d not found.\n", id);
    }
    fclose(fp);
}

// Borrow book
void borrowBook() {
    int id, found = 0;
    struct Book b;
    FILE *fp = fopen("books.dat", "rb+");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Book ID to borrow: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(struct Book), 1, fp)) {
        if (b.id == id) {
            found = 1;
            if (b.availablecopies > 0) {
                b.availablecopies--;
                fseek(fp, -(long)sizeof(struct Book), SEEK_CUR); // ✅ fixed
                fwrite(&b, sizeof(struct Book), 1, fp);
                printf("✅ You have borrowed: %s\n", b.title);
            } else {
                printf("❌ No available copies.\n");
            }
            break;
        }
    }
    if (!found) printf("❌ Book with ID %d not found.\n", id);

    fclose(fp);
}

// Return book
void returnBook() {
    int id, found = 0;
    struct Book b;
    FILE *fp = fopen("books.dat", "rb+");
    if (!fp) {  
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Book ID to return: ");
    scanf("%d", &id);

    while (fread(&b, sizeof(struct Book), 1, fp)) {
        if (b.id == id) {
            found = 1;
            if (b.availablecopies < b.totalcopies) {
                b.availablecopies++;
                fseek(fp, -(long)sizeof(struct Book), SEEK_CUR); // ✅ fixed
                fwrite(&b, sizeof(struct Book), 1, fp);
                printf("✅ You have returned: %s\n", b.title);
            } else {
                printf("❌ All copies already in library.\n");
            }
            break;
        }
    }
    if (!found) printf("❌ Book with ID %d not found.\n", id);

    fclose(fp);
}

// MAIN MENU
int main() {
    int choice;
    while (1) {
        printf("\n--- Library Management System ---\n");
        printf("1. Add Book\n2. View Books\n3. Search Book\n4. Delete Book\n5. Update Book\n6. Borrow Book\n7. Return Book\n8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: searchBook(); break;
            case 4: deleteBook(); break;
            case 5: updateBook(); break;
            case 6: borrowBook(); break;
            case 7: returnBook(); break;
            case 8: exit(0);
            default: printf("❌ Invalid choice! Try again.\n");
        }
    }
    return 0;
}
