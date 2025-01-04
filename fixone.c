#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CATEGORY 100
#define MAX_PRODUCTS 500
#define MAX_NAME_LENGTH 50

// Date structure
typedef struct {
    int day;
    int month;
    int year;
} Date;

// Product structure
typedef struct {
    char productId[10];
    char productName[50];
    char categoryId[10];  // Reference to category
    int quantity;
    float price;
    int isActive;  // Flag for soft delete
} Product;

// Category structure
typedef struct {
    char id[10];
    char name[MAX_NAME_LENGTH];
    int isActive;  // Flag for soft delete
} Category;

// Global variables
Category categories[MAX_CATEGORY];
Product products[MAX_PRODUCTS];
int categoryCount = 0;
int productCount = 0;

// Utility functions
void clearBuffer() {
    while (getchar() != '\n');
}

// Category management functions
void displayCategory() {
    printf("\n%-10s %-20s %-15s\n", "ID", "Ten danh muc", "So san pham");
    printf("-------------------------------------------\n");
    for (int i = 0; i < categoryCount; i++) {
        if (categories[i].isActive) {
            // Count products in this category
            int productInCategory = 0;
            for (int j = 0; j < productCount; j++) {
                if (products[j].isActive && strcmp(products[j].categoryId, categories[i].id) == 0) {
                    productInCategory++;
                }
            }
            printf("%-10s %-20s %-15d\n", categories[i].id, categories[i].name, productInCategory);
        }
    }
}

void displayProductsInCategory(char categoryId[]) {
    printf("\n%-10s %-20s %-10s %-15s\n", "ID", "Ten san pham", "So luong", "Gia");
    printf("------------------------------------------------\n");
    for (int i = 0; i < productCount; i++) {
        if (products[i].isActive && strcmp(products[i].categoryId, categoryId) == 0) {
            printf("%-10s %-20s %-10d %-15.2f\n", 
                products[i].productId, 
                products[i].productName, 
                products[i].quantity, 
                products[i].price);
        }
    }
}

int findCategory(char id[]) {
    for (int i = 0; i < categoryCount; i++) {
        if (categories[i].isActive && strcmp(categories[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

int isUniqueCategory(char id[]) {
    return findCategory(id) == -1;
}

void addCategory() {
    if (categoryCount == MAX_CATEGORY) {
        printf("Danh sach danh muc da day!\n");
        return;
    }

    Category newCategory;
    newCategory.isActive = 1;

    printf("Nhap ma danh muc: ");
    fgets(newCategory.id, sizeof(newCategory.id), stdin);
    newCategory.id[strcspn(newCategory.id, "\n")] = '\0';

    if (!isUniqueCategory(newCategory.id)) {
        printf("Ma danh muc da ton tai!\n");
        return;
    }

    printf("Nhap ten danh muc: ");
    fgets(newCategory.name, sizeof(newCategory.name), stdin);
    newCategory.name[strcspn(newCategory.name, "\n")] = '\0';

    categories[categoryCount++] = newCategory;
    printf("Them danh muc thanh cong!\n");
}

// Product management functions
int findProduct(char id[]) {
    for (int i = 0; i < productCount; i++) {
        if (products[i].isActive && strcmp(products[i].productId, id) == 0) {
            return i;
        }
    }
    return -1;
}

void addProduct() {
    if (productCount == MAX_PRODUCTS) {
        printf("Danh sach san pham da day!\n");
        return;
    }

    Product newProduct;
    newProduct.isActive = 1;

    printf("Nhap ma san pham: ");
    fgets(newProduct.productId, sizeof(newProduct.productId), stdin);
    newProduct.productId[strcspn(newProduct.productId, "\n")] = '\0';

    if (findProduct(newProduct.productId) != -1) {
        printf("Ma san pham da ton tai!\n");
        return;
    }

    printf("Nhap ma danh muc: ");
    fgets(newProduct.categoryId, sizeof(newProduct.categoryId), stdin);
    newProduct.categoryId[strcspn(newProduct.categoryId, "\n")] = '\0';

    if (findCategory(newProduct.categoryId) == -1) {
        printf("Danh muc khong ton tai!\n");
        return;
    }

    printf("Nhap ten san pham: ");
    fgets(newProduct.productName, sizeof(newProduct.productName), stdin);
    newProduct.productName[strcspn(newProduct.productName, "\n")] = '\0';

    printf("Nhap so luong: ");
    scanf("%d", &newProduct.quantity);
    clearBuffer();

    printf("Nhap gia: ");
    scanf("%f", &newProduct.price);
    clearBuffer();

    products[productCount++] = newProduct;
    printf("Them san pham thanh cong!\n");
}

void updateProduct() {
    char id[10];
    printf("Nhap ma san pham can cap nhat: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    int index = findProduct(id);
    if (index == -1) {
        printf("Khong tim thay san pham!\n");
        return;
    }

    printf("Nhap so luong moi: ");
    scanf("%d", &products[index].quantity);
    clearBuffer();

    printf("Nhap gia moi: ");
    scanf("%f", &products[index].price);
    clearBuffer();

    printf("Cap nhat san pham thanh cong!\n");
}

void deleteProduct() {
    char id[10];
    printf("Nhap ma san pham can xoa: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    int index = findProduct(id);
    if (index == -1) {
        printf("Khong tim thay san pham!\n");
        return;
    }

    products[index].isActive = 0;
    printf("Xoa san pham thanh cong!\n");
}

// File operations
void saveData() {
    // Save categories
    FILE *categoryFile = fopen("category.txt", "w");
    if (categoryFile != NULL) {
        for (int i = 0; i < categoryCount; i++) {
            if (categories[i].isActive) {
                fprintf(categoryFile, "%s|%s\n", categories[i].id, categories[i].name);
            }
        }
        fclose(categoryFile);
    }

    // Save products
    FILE *productFile = fopen("products.txt", "w");
    if (productFile != NULL) {
        for (int i = 0; i < productCount; i++) {
            if (products[i].isActive) {
                fprintf(productFile, "%s|%s|%s|%d|%.2f\n", 
                    products[i].productId,
                    products[i].categoryId,
                    products[i].productName,
                    products[i].quantity,
                    products[i].price);
            }
        }
        fclose(productFile);
    }
    printf("Du lieu da duoc luu thanh cong!\n");
}

void loadData() {
    // Load categories
    FILE *categoryFile = fopen("category.txt", "r");
    if (categoryFile != NULL) {
        categoryCount = 0;
        char line[100];
        while (fgets(line, sizeof(line), categoryFile) && categoryCount < MAX_CATEGORY) {
            char *id = strtok(line, "|");
            char *name = strtok(NULL, "\n");
            if (id && name) {
                strcpy(categories[categoryCount].id, id);
                strcpy(categories[categoryCount].name, name);
                categories[categoryCount].isActive = 1;
                categoryCount++;
            }
        }
        fclose(categoryFile);
    }

    // Load products
    FILE *productFile = fopen("products.txt", "r");
    if (productFile != NULL) {
        productCount = 0;
        char line[100];
        while (fgets(line, sizeof(line), productFile) && productCount < MAX_PRODUCTS) {
            char *productId = strtok(line, "|");
            char *categoryId = strtok(NULL, "|");
            char *name = strtok(NULL, "|");
            char *quantity = strtok(NULL, "|");
            char *price = strtok(NULL, "\n");
            
            if (productId && categoryId && name && quantity && price) {
                strcpy(products[productCount].productId, productId);
                strcpy(products[productCount].categoryId, categoryId);
                strcpy(products[productCount].productName, name);
                products[productCount].quantity = atoi(quantity);
                products[productCount].price = atof(price);
                products[productCount].isActive = 1;
                productCount++;
            }
        }
        fclose(productFile);
    }
    printf("Du lieu da duoc tai thanh cong!\n");
}

void displayMenu() {
    printf("\nQUAN LY DANH MUC VA SAN PHAM\n");
    printf("1. Hien thi danh sach danh muc\n");
    printf("2. Them danh muc\n");
    printf("3. Xem san pham trong danh muc\n");
    printf("4. Them san pham\n");
    printf("5. Cap nhat san pham\n");
    printf("6. Xoa san pham\n");
    printf("7. Luu du lieu\n");
    printf("8. Tai du lieu\n");
    printf("9. Thoat\n");
    printf("Chon chuc nang: ");
}

int main() {
    int choice;
    loadData();  // Load existing data at startup
    
    do {
        displayMenu();
        scanf("%d", &choice);
        clearBuffer();
        
        switch (choice) {
            case 1:
                displayCategory();
                break;
            case 2:
                addCategory();
                break;
            case 3: {
                char categoryId[10];
                printf("Nhap ma danh muc can xem: ");
                fgets(categoryId, sizeof(categoryId), stdin);
                categoryId[strcspn(categoryId, "\n")] = '\0';
                if (findCategory(categoryId) != -1) {
                    displayProductsInCategory(categoryId);
                } else {
                    printf("Khong tim thay danh muc!\n");
                }
                break;
            }
            case 4:
                addProduct();
                break;
            case 5:
                updateProduct();
                break;
            case 6:
                deleteProduct();
                break;
            case 7:
                saveData();
                break;
            case 8:
                loadData();
                break;
            case 9:
                printf("Tam biet!\n");
                break;
            default:
                printf("Chuc nang khong hop le!\n");
        }
    } while (choice != 9);

    return 0;
}