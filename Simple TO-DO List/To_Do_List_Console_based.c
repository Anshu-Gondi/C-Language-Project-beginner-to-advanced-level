// 👉 Concepts: arrays, loops, functions, CRUD (Create, Read, Update, Delete)
#include <stdio.h>
#include <string.h>

#define MAX 100

char tasks[MAX][100]; // Array to store tasks
int count = 0;        // Number of tasks

void addTask()
{
    if (count >= MAX)
    {
        printf("Task list is full!\n");
        return;
    }
    printf("Enter the task: ");
    getchar(); // Clear newline character from buffer
    fgets(tasks[count], 100, stdin);
    tasks[count][strcspn(tasks[count], "\n")] = 0; // Remove newline character
    count++;
}

void showTasks()
{
    if (count == 0)
    {
        printf("NO TASKS AVAILABLE\n");
        return;
    }
    for (int i = 0; i < count; i++)
    {
        printf("%d. %s\n", i + 1, tasks[i]);
    }
}

void updateTask()
{
    int index;
    printf("Enter the task number to update: ");
    scanf("%d", &index);
    if (index < 1 || index > count)
    {
        printf("Invalid task number!\n");
        return;
    }
    printf("Enter the new task: ");
    getchar(); // Clear newline character from buffer
    fgets(tasks[index - 1], 100, stdin);
    tasks[index - 1][strcspn(tasks[index - 1], "\n")] = 0; // Remove newline character
}

void deleteTask()
{
    int index;
    printf("Enter the task number to delete: ");
    scanf("%d", &index);
    if (index < 1 || index > count)
    {
        printf("Invalid task number!\n");
        return;
    }
    for (int i = index - 1; i < count - 1; i++)
    {
        strcpy(tasks[i], tasks[i + 1]);
    }
    count--;
}

int main()
{
    int choice;
    do
    {
        printf("\nTo-Do List Menu:\n");
        printf("1. Add Task\n");
        printf("2. Show Tasks\n");
        printf("3. Update Task\n");
        printf("4. Delete Task\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addTask();
            break;
        case 2:
            showTasks();
            break;
        case 3:
            updateTask();
            break;
        case 4:
            deleteTask();
            break;
        case 5:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 5);
    return 0;
}