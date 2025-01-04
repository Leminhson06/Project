#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CATEGORIES 100
#define MAX_PRODUCTS 100
#define MAX_NAME_LEN 50
#define MAX_INVOICES 1000
#define MAX_ITEMS_PER_INVOICE 20

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
} Category;

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    int categoryId;
    float price;
    int quantity;
} Product;

typedef struct {
    int productId;
    int quantity;
    float unitPrice;
    float totalPrice;
} InvoiceItem;

typedef struct {
    int id;
    time_t date;
    InvoiceItem items[MAX_ITEMS_PER_INVOICE];
    int itemCount;
    float totalAmount;
} Invoice;

// Global variables
Category categories[MAX_CATEGORIES];
Product products[MAX_PRODUCTS];
Invoice invoices[MAX_INVOICES];
int categoryCount = 0;
int productCount = 0;
int invoiceCount = 0;

// Utility functions
void clearScreen() {
    printf("\033[H\033[J");
}

void pauseScreen() {
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

void printLine() {
    printf("--------------------------------------------------\n");
}

void flushInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Safe input function
void safeInput(char *buffer, int maxLength) {
    fgets(buffer, maxLength, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

// Product management functions
void addProduct() {
    if (productCount >= MAX_PRODUCTS) {
        printf("\nCannot add more products.\n");
        return;
    }

    if (categoryCount == 0) {
        printf("\nPlease add at least one category first.\n");
        return;
    }

    Product newProduct;
    newProduct.id = productCount + 1;

    printf("\nEnter product details:\n");
    printf("Name: ");
    flushInput();
    safeInput(newProduct.name, MAX_NAME_LEN);

    // Display available categories
    displayCategories();
    printf("Enter category ID: ");
    scanf("%d", &newProduct.categoryId);

    // Validate category ID
    int validCategory = 0;
    for (int i = 0; i < categoryCount; i++) {
        if (categories[i].id == newProduct.categoryId) {
            validCategory = 1;
            break;
        }
    }

    if (!validCategory) {
        printf("\nInvalid category ID.\n");
        return;
    }

    printf("Price: ");
    scanf("%f", &newProduct.price);
    printf("Quantity: ");
    scanf("%d", &newProduct.quantity);

    if (newProduct.price <= 0 || newProduct.quantity < 0) {
        printf("\nInvalid price or quantity.\n");
        return;
    }

    products[productCount++] = newProduct;
    printf("\nProduct added successfully!\n");
    saveProductsToFile();
}

void displayProducts() {
    if (productCount == 0) {
        printf("\nNo products available.\n");
        return;
    }

    printf("\n%-5s %-20s %-15s %-10s %-10s\n", 
           "ID", "Name", "Category", "Price", "Quantity");
    printLine();

    for (int i = 0; i < productCount; i++) {
        // Find category name
        char categoryName[MAX_NAME_LEN] = "Unknown";
        for (int j = 0; j < categoryCount; j++) {
            if (categories[j].id == products[i].categoryId) {
                strcpy(categoryName, categories[j].name);
                break;
            }
        }

        printf("%-5d %-20s %-15s %-10.2f %-10d\n",
               products[i].id,
               products[i].name,
               categoryName,
               products[i].price,
               products[i].quantity);
    }
}

void updateProduct() {
    if (productCount == 0) {
        printf("\nNo products available to update.\n");
        return;
    }

    displayProducts();
    int productId;
    printf("\nEnter product ID to update: ");
    scanf("%d", &productId);

    int index = -1;
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == productId) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nProduct not found.\n");
        return;
    }

    printf("\nCurrent values:\n");
    printf("1. Name: %s\n", products[index].name);
    printf("2. Price: %.2f\n", products[index].price);
    printf("3. Quantity: %d\n", products[index].quantity);
    printf("4. Category ID: %d\n", products[index].categoryId);

    int choice;
    printf("\nWhat would you like to update?\n");
    printf("Enter choice (1-4): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter new name: ");
            flushInput();
            safeInput(products[index].name, MAX_NAME_LEN);
            break;
        case 2:
            printf("Enter new price: ");
            scanf("%f", &products[index].price);
            break;
        case 3:
            printf("Enter new quantity: ");
            scanf("%d", &products[index].quantity);
            break;
        case 4:
            displayCategories();
            printf("Enter new category ID: ");
            scanf("%d", &products[index].categoryId);
            break;
        default:
            printf("\nInvalid choice.\n");
            return;
    }

    printf("\nProduct updated successfully!\n");
    saveProductsToFile();
}

void manageProducts() {
    int choice;
    do {
        clearScreen();
        printf("--- Manage Products ---\n");
        printf("1. Add Product\n");
        printf("2. Display Products\n");
        printf("3. Update Product\n");
        printf("4. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addProduct();
                pauseScreen();
                break;
            case 2:
                displayProducts();
                pauseScreen();
                break;
            case 3:
                updateProduct();
                pauseScreen();
                break;
            case 4:
                break;
            default:
                printf("\nInvalid choice. Try again.\n");
                pauseScreen();
        }
    } while (choice != 4);
}

// Sales and Invoice functions
void sellProduct() {
    if (productCount == 0) {
        printf("\nNo products available to sell.\n");
        pauseScreen();
        return;
    }

    Invoice newInvoice;
    newInvoice.id = invoiceCount + 1;
    newInvoice.date = time(NULL);
    newInvoice.itemCount = 0;
    newInvoice.totalAmount = 0;

    char continueSelling;
    do {
        displayProducts();
        
        int productId;
        printf("\nEnter product ID to sell: ");
        scanf("%d", &productId);

        int productIndex = -1;
        for (int i = 0; i < productCount; i++) {
            if (products[i].id == productId) {
                productIndex = i;
                break;
            }
        }

        if (productIndex == -1) {
            printf("\nProduct not found.\n");
            continue;
        }

        int quantity;
        printf("Enter quantity: ");
        scanf("%d", &quantity);

        if (quantity <= 0 || quantity > products[productIndex].quantity) {
            printf("\nInvalid quantity.\n");
            continue;
        }

        // Add item to invoice
        InvoiceItem item;
        item.productId = productId;
        item.quantity = quantity;
        item.unitPrice = products[productIndex].price;
        item.totalPrice = quantity * products[productIndex].price;

        newInvoice.items[newInvoice.itemCount++] = item;
        newInvoice.totalAmount += item.totalPrice;

        // Update product quantity
        products[productIndex].quantity -= quantity;

        printf("\nAdd another item? (y/n): ");
        flushInput();
        scanf("%c", &continueSelling);

    } while (continueSelling == 'y' || continueSelling == 'Y');

    // Save invoice
    invoices[invoiceCount++] = newInvoice;
    saveProductsToFile();

    // Print invoice
    printf("\n=== Invoice #%d ===\n", newInvoice.id);
    printf("Date: %s", ctime(&newInvoice.date));
    printLine();
    printf("%-20s %-10s %-10s %-10s\n", "Product", "Quantity", "Unit Price", "Total");
    printLine();

    for (int i = 0; i < newInvoice.itemCount; i++) {
        char productName[MAX_NAME_LEN] = "Unknown";
        for (int j = 0; j < productCount; j++) {
            if (products[j].id == newInvoice.items[i].productId) {
                strcpy(productName, products[j].name);
                break;
            }
        }

        printf("%-20s %-10d %-10.2f %-10.2f\n",
               productName,
               newInvoice.items[i].quantity,
               newInvoice.items[i].unitPrice,
               newInvoice.items[i].totalPrice);
    }
    printLine();
    printf("Total Amount: %.2f\n", newInvoice.totalAmount);
    pauseScreen();
}

void viewInvoices() {
    if (invoiceCount == 0) {
        printf("\nNo invoices available.\n");
        pauseScreen();
        return;
    }

    printf("\n=== Invoice History ===\n");
    for (int i = 0; i < invoiceCount; i++) {
        printf("\nInvoice #%d\n", invoices[i].id);
        printf("Date: %s", ctime(&invoices[i].date));
        printf("Total Amount: %.2f\n", invoices[i].totalAmount);
        printLine();
    }
    pauseScreen();
}

// File handling for products
void saveProductsToFile() {
    FILE *file = fopen("products.txt", "w");
    if (file == NULL) {
        printf("\nError saving products to file.\n");
        return;
    }
    for (int i = 0; i < productCount; i++) {
        fprintf(file, "%d|%s|%d|%.2f|%d\n",
               products[i].id,
               products[i].name,
               products[i].categoryId,
               products[i].price,
               products[i].quantity);
    }
    fclose(file);
}

void loadProductsFromFile() {
    FILE *file = fopen("products.txt", "r");
    if (file == NULL) {
        return;
    }
    while (fscanf(file, "%d|%[^|]|%d|%f|%d\n",
                  &products[productCount].id,
                  products[productCount].name,
                  &products[productCount].categoryId,
                  &products[productCount].price,
                  &products[productCount].quantity) != EOF) {
        productCount++;
    }
    fclose(file);
}