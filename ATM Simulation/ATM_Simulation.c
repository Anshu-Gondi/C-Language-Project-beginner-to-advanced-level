#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================
// STRUCTURES
// ============================
struct Account {
    int id;
    char name[50];
    int pin;
    double balance;
    int isLocked; // 0 = active, 1 = locked
};

struct Transaction {
    int accId;
    char type[20]; // "Deposit" or "Withdraw" or "Balance Check"
    double amount;
    char timestamp[50];
};

// ============================
// HELPER FUNCTIONS
// ============================

// Get current timestamp
void getTimeStamp(char *buffer) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
}

// Write transaction record
void logTransaction(int accId, const char *type, double amount) {
    struct Transaction t;
    FILE *fp;
    char filename[30];

    t.accId = accId;
    strcpy(t.type, type);
    t.amount = amount;
    getTimeStamp(t.timestamp);

    sprintf(filename, "transactions_%d.dat", accId);
    fp = fopen(filename, "ab");
    if (fp != NULL) {
        fwrite(&t, sizeof(struct Transaction), 1, fp);
        fclose(fp);
    }
}

// Save account changes to file 
void updateAccount(struct Account *acc) {
    FILE *fp = fopen("accounts.dat", "r+b");
    struct Account temp;
    if (fp == NULL) return;

    while (fread(&temp, sizeof(struct Account), 1, fp) == 1) {
        if (temp.id == acc->id) {
            fseek(fp, -sizeof(struct Account), SEEK_CUR);
            fwrite(acc, sizeof(struct Account), 1, fp);
            break;
        }
    }
    fclose(fp);
}

// ============================
// ACCOUNT CREATION
// ============================
void createAccount() {
    struct Account acc;
    FILE *fp = fopen("accounts.dat", "ab");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Account ID: ");
    scanf("%d", &acc.id);
    getchar(); // consume newline

    printf("Enter Name: ");
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = 0; // remove newline

    printf("Set 4-digit PIN: ");
    scanf("%d", &acc.pin);

    acc.balance = 0.0;
    acc.isLocked = 0; // new account is active

    fwrite(&acc, sizeof(struct Account), 1, fp);
    fclose(fp);

    printf("✅ Account created successfully!\n");
}

// ============================
// LOGIN FUNCTION WITH ATTEMPTS
// ============================
int login(struct Account *acc) {
    int id, pin, found = 0, attempts = 0;
    struct Account temp;
    FILE *fp = fopen("accounts.dat", "rb+");
    if (fp == NULL) {
        printf("No accounts found. Create an account first.\n");
        return 0;
    }

    printf("Enter Account ID: ");
    scanf("%d", &id);

    while (fread(&temp, sizeof(struct Account), 1, fp) == 1) {
        if (temp.id == id) {
            found = 1;
            *acc = temp; // copy details
            break;
        }
    }

    if (!found) {
        fclose(fp);
        printf("❌ Account not found.\n");
        return 0;
    }

    if (acc->isLocked) {
        printf("🔒 Account is locked. Contact bank.\n");
        fclose(fp);
        return 0;
    }

    // PIN attempts
    while (attempts < 3) {
        printf("Enter PIN: ");
        scanf("%d", &pin);

        if (acc->pin == pin) {
            fclose(fp);
            return 1; // success
        } else {
            attempts++;
            printf("❌ Incorrect PIN. Attempts left: %d\n", 3 - attempts);
        }
    }

    // Lock account after 3 wrong attempts
    acc->isLocked = 1;
    updateAccount(acc);
    printf("🔒 Account locked due to multiple wrong attempts.\n");

    fclose(fp);
    return 0;
}

// ============================
// VIEW TRANSACTIONS
// ============================
void viewTransactions(int accId) {
    struct Transaction t;
    FILE *fp;
    char filename[30];

    sprintf(filename, "transactions_%d.dat", accId);
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("No transactions found.\n");
        return;
    }

    printf("\n--- Transaction History ---\n");
    printf("Type\t\tAmount\t\tTimestamp\n");
    while (fread(&t, sizeof(struct Transaction), 1, fp) == 1) {
        printf("%s\t%.2f\t%s\n", t.type, t.amount, t.timestamp);
    }
    fclose(fp);
}

// ============================
// ATM MENU
// ============================
void atmMenu(struct Account *acc) {
    int choice;
    double amt;

    while (1) {
        printf("\n--- ATM Menu ---\n");
        printf("1. Check Balance\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. View Transactions\n");
        printf("5. Change PIN\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("💰 Current Balance: %.2f\n", acc->balance);
                logTransaction(acc->id, "Balance Check", 0);
                break;
            case 2:
                printf("Enter amount to deposit: ");
                scanf("%lf", &amt);
                if (amt > 0) {
                    acc->balance += amt;
                    updateAccount(acc);
                    logTransaction(acc->id, "Deposit", amt);
                    printf("✅ Deposited: %.2f\n", amt);
                } else {
                    printf("Invalid amount.\n");
                }
                break;
            case 3:
                printf("Enter amount to withdraw: ");
                scanf("%lf", &amt);
                if (amt > 0 && amt <= acc->balance) {
                    acc->balance -= amt;
                    updateAccount(acc);
                    logTransaction(acc->id, "Withdraw", amt);
                    printf("✅ Withdrew: %.2f\n", amt);
                } else {
                    printf("❌ Invalid amount or insufficient balance.\n");
                }
                break;
            case 4:
                viewTransactions(acc->id);
                break;
            case 5:
                printf("Enter new 4-digit PIN: ");
                scanf("%d", &acc->pin);
                updateAccount(acc);
                logTransaction(acc->id, "PIN Change", 0);
                printf("✅ PIN changed successfully!\n");
                break;
            case 6:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}

// ============================
// MAIN FUNCTION
// ============================
int main() {
    int choice;
    struct Account acc;

    while (1) {
        printf("\n=== ATM Simulation ===\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            createAccount();
            break;
        case 2:
            if (login(&acc)) {
                printf("✅ Login successful! Welcome %s\n", acc.name);
                atmMenu(&acc);
            } else {
                printf("⚠️ Login failed!\n");
            }
            break;
        case 3:
            printf("Exiting program...\n");
            exit(0);
        default:
            printf("Invalid choice!\n");
        }
    }

    return 0;
}
