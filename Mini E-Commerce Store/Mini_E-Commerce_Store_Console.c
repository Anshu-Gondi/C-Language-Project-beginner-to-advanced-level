#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTS 50
#define MAX_CART 50
#define MAX_CUSTOMERS 50

// ============================
// STRUCTURES
// ============================
struct Product {
    int id;
    char name[50];
    double price;
    int stock;
};

struct CartItem {
    int productId;
    char name[50];
    double price;
    int quantity;
};

struct Customer {
    int id;
    char name[50];
};

struct Order {
    int orderId;
    int customerId;
    struct CartItem items[MAX_CART];
    int itemCount;
    double totalAmount;
};

// ============================
// HELPER FUNCTIONS
// ============================
int loadProducts(struct Product products[]) {
    FILE *fp = fopen("products.txt", "r");
    if (!fp) {
        printf("Could not open products file.\n");
        return 0;
    }
    int count = 0;
    while (fread(&products[count], sizeof(struct Product), 1, fp) && count < MAX_PRODUCTS) {
        count++;
    }
    fclose(fp);
    return count;
}

void saveProducts(struct Product products[], int count) {
    FILE *fp = fopen("products.txt", "w");
    if (!fp) {
        printf("Could not open products file for writing.\n");
        return;
    }
    fwrite(products, sizeof(struct Product), count, fp);
    fclose(fp);
}

void displayProducts(struct Product products[], int count) {
    printf("Available Products:\n");
    printf("ID\tName\t\tPrice\tStock\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%s\t%.2f\t%d\n", products[i].id, products[i].name, products[i].price, products[i].stock);
        if (products[i].stock == 0) {
            printf("  [Out of Stock]\n");
        } else if (products[i].stock < 5) {
            printf("  [Low Stock]\n");
        }
    }
}

int loadCustomers(struct Customer customers[]) {
    FILE *fp = fopen("customers.txt", "r");
    if (!fp) {
        printf("Could not open customers file.\n");
        return 0;
    }
    int count = 0;
    while (fread(&customers[count], sizeof(struct Customer), 1, fp) && count < MAX_CUSTOMERS) {
        count++;
    }
    fclose(fp);
    return count;
}

void addCustomer() {
    struct Customer c;
    FILE *fp = fopen("customers.txt", "a");
    if (!fp) {
        printf("Could not open customers file for writing.\n");
        return;
    }
    printf("Enter customer ID: ");
    scanf("%d", &c.id);
    printf("Enter customer name: ");
    scanf("%s", c.name);
    fwrite(&c, sizeof(struct Customer), 1, fp);
    fclose(fp);
    printf("Customer added successfully.\n");
}

int loginCustomer(struct Customer *c) {
    struct Customer customers[MAX_CUSTOMERS];
    int count = loadCustomers(customers);
    if (count == 0) { printf("No customers found.\n"); return 0; }

    int id, found = 0;
    printf("Enter Customer ID: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (customers[i].id == id) {
            *c = customers[i];
            found = 1;
            break;
        }
    }

    if (!found) { printf("Customer not found!\n"); return 0; }
    printf("✅ Welcome %s!\n", c->name);
    return 1;
}

// Generate unique order ID
int nextOrderId() {
    static int id = 1;
    return id++;
}

// Save complete order
void saveOrder(struct Customer c, struct CartItem cart[], int cartCount, double total) {
    struct Order o;
    o.orderId = nextOrderId();
    o.customerId = c.id;
    o.itemCount = cartCount;
    o.totalAmount = total;
    for (int i = 0; i < cartCount; i++) o.items[i] = cart[i];

    FILE *fp = fopen("orders.txt", "a");
    if (!fp) { printf("Error saving order!\n"); return; }
    fwrite(&o, sizeof(struct Order), 1, fp);
    fclose(fp);
}

// View customer's order history
void viewOrderHistory(struct Customer c) {
    FILE *fp = fopen("orders.txt", "r");
    if (!fp) { printf("No orders found.\n"); return; }

    struct Order o;
    int found = 0;

    printf("\n--- Your Order History ---\n");
    while (fread(&o, sizeof(struct Order), 1, fp)) {
        if (o.customerId == c.id) {
            printf("Order ID: %d | Total: %.2f\n", o.orderId, o.totalAmount);
            printf("Items:\n");
            for (int i = 0; i < o.itemCount; i++)
                printf("%s x%d = %.2f\n", o.items[i].name, o.items[i].quantity, o.items[i].price * o.items[i].quantity);
            found = 1;
            printf("---------------------------\n");
        }
    }

    if (!found) printf("No past orders.\n");
    fclose(fp);
}

// ============================
// CUSTOMER SHOP MENU
// ============================
void placeOrder(struct Customer c) {
    struct Product products[MAX_PRODUCTS];
    int productCount = loadProducts(products);

    struct CartItem cart[MAX_CART];
    int cartCount = 0;
    int choice, qty;

    while (1) {
        displayProducts(products, productCount);
        printf("\n1. Add to Cart\n2. Remove from Cart\n3. View Cart\n4. Checkout\n5. View Order History\n6. Logout\nEnter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter Product ID to add: ");
            int pid;
            scanf("%d", &pid);
            printf("Enter Quantity: ");
            scanf("%d", &qty);

            int found = 0;
            for (int i = 0; i < productCount; i++) {
                if (products[i].id == pid) {
                    if (products[i].stock >= qty) {
                        strcpy(cart[cartCount].name, products[i].name);
                        cart[cartCount].productId = pid;
                        cart[cartCount].price = products[i].price;
                        cart[cartCount].quantity = qty;
                        cartCount++;
                        printf("Added %d x %s to cart.\n", qty, products[i].name);
                    } else {
                        printf("❌ Not enough stock.\n");
                    }
                    found = 1;
                    break;
                }
            }
            if (!found) printf("Product ID not found.\n");

        } else if (choice == 2) {
            printf("Enter Product ID to remove: ");
            int pid, idx = -1;
            scanf("%d", &pid);
            for (int i = 0; i < cartCount; i++) if (cart[i].productId == pid) { idx = i; break; }
            if (idx != -1) {
                for (int i = idx; i < cartCount - 1; i++) cart[i] = cart[i + 1];
                cartCount--;
                printf("Item removed from cart.\n");
            } else printf("Item not found in cart.\n");

        } else if (choice == 3) {
            printf("\n--- Cart ---\n");
            double total = 0;
            for (int i = 0; i < cartCount; i++) {
                printf("%d x %s = %.2f\n", cart[i].quantity, cart[i].name, cart[i].quantity * cart[i].price);
                total += cart[i].quantity * cart[i].price;
            }
            printf("Total: %.2f\n", total);

        } else if (choice == 4) {
            if (cartCount == 0) { printf("Cart is empty!\n"); continue; }

            double total = 0;
            for (int i = 0; i < cartCount; i++) total += cart[i].quantity * cart[i].price;

            char code[20];
            printf("Enter discount code (or NONE): ");
            scanf("%s", code);
            if (strcmp(code, "SAVE10") == 0) {
                total *= 0.9;
                printf("✅ Discount applied! 10%% off.\n");
            }

            printf("Total after discount: %.2f\n", total);

            int payChoice;
            printf("Select payment method: 1. Cash 2. Card: ");
            scanf("%d", &payChoice);
            if (payChoice == 1) printf("Payment received in Cash.\n");
            else if (payChoice == 2) printf("Payment processed via Card.\n");
            else printf("Invalid payment method. Proceeding as Cash.\n");

            saveOrder(c, cart, cartCount, total);

            for (int i = 0; i < cartCount; i++)
                for (int j = 0; j < productCount; j++)
                    if (products[j].id == cart[i].productId) products[j].stock -= cart[i].quantity;

            saveProducts(products, productCount);
            printf("✅ Order placed successfully!\n");
            cartCount = 0;

        } else if (choice == 5) viewOrderHistory(c);
        else if (choice == 6) { printf("Logging out...\n"); break; }
        else printf("Invalid choice.\n");
    }
}

// ============================
// ADMIN FUNCTIONS
// ============================
int adminLogin() {
    char username[50], password[50];
    printf("Enter Admin username: ");
    scanf("%s", username);
    printf("Enter Admin password: ");
    scanf("%s", password);
    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) return 1;
    printf("❌ Invalid credentials.\n"); return 0;
}

void adminMenu() {
    struct Product products[MAX_PRODUCTS];
    int productCount = loadProducts(products);
    int choice;

    while (1) {
        printf("\n--- Admin Menu ---\n");
        printf("1. Add Product\n2. Remove Product\n3. Update Product\n4. View Orders\n5. Logout\nEnter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            struct Product p;
            printf("Enter Product ID: "); scanf("%d", &p.id); getchar();
            printf("Enter Name: "); fgets(p.name, sizeof(p.name), stdin); p.name[strcspn(p.name, "\n")] = 0;
            printf("Enter Price: "); scanf("%lf", &p.price);
            printf("Enter Stock: "); scanf("%d", &p.stock);
            products[productCount++] = p;
            saveProducts(products, productCount);
            printf("✅ Product added.\n");

        } else if (choice == 2) {
            int pid, idx = -1; printf("Enter Product ID to remove: "); scanf("%d", &pid);
            for (int i = 0; i < productCount; i++) if (products[i].id == pid) { idx = i; break; }
            if (idx != -1) { for (int i = idx; i < productCount-1; i++) products[i] = products[i+1]; productCount--; saveProducts(products, productCount); printf("✅ Product removed.\n"); }
            else printf("Product not found.\n");

        } else if (choice == 3) {
            int pid, found = 0; printf("Enter Product ID to update: "); scanf("%d", &pid);
            for (int i = 0; i < productCount; i++) {
                if (products[i].id == pid) {
                    found = 1;
                    printf("Enter new Name: "); getchar(); fgets(products[i].name, sizeof(products[i].name), stdin); products[i].name[strcspn(products[i].name, "\n")] = 0;
                    printf("Enter new Price: "); scanf("%lf", &products[i].price);
                    printf("Enter new Stock: "); scanf("%d", &products[i].stock);
                    saveProducts(products, productCount);
                    printf("✅ Product updated.\n");
                    break;
                }
            }
            if (!found) printf("Product not found.\n");

        } else if (choice == 4) {
            FILE *fp = fopen("orders.dat", "rb");
            if (!fp) { printf("No orders found.\n"); continue; }
            struct Order o;
            printf("\n--- All Orders ---\n");
            while (fread(&o, sizeof(struct Order), 1, fp)) {
                printf("Order ID: %d | Customer ID: %d | Total: %.2f\n", o.orderId, o.customerId, o.totalAmount);
                for (int i = 0; i < o.itemCount; i++)
                    printf("%s x%d = %.2f\n", o.items[i].name, o.items[i].quantity, o.items[i].price*o.items[i].quantity);
                printf("---------------------\n");
            }
            fclose(fp);

        } else if (choice == 5) break;
        else printf("Invalid choice.\n");
    }
}

// ============================
// MAIN FUNCTION
// ============================
int main() {
    struct Customer c;
    struct Product products[MAX_PRODUCTS];
    int productCount = loadProducts(products);

    if (productCount == 0) {
        FILE *fp = fopen("products.dat", "wb");
        struct Product p[] = {{1,"Laptop",50000,10},{2,"Phone",20000,20},{3,"Headphones",2000,50}};
        fwrite(p, sizeof(struct Product), 3, fp); fclose(fp);
    }

    int choice;
    while (1) {
        printf("\n=== Mini E-Commerce Store ===\n");
        printf("1. Register Customer\n2. Customer Login\n3. Admin Login\n4. Exit\nEnter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addCustomer(); break;
            case 2: if (loginCustomer(&c)) placeOrder(c); break;
            case 3: if (adminLogin()) adminMenu(); break;
            case 4: printf("Exiting...\n"); exit(0);
            default: printf("Invalid choice!\n"); break;
        }
    }
    return 0;
}
