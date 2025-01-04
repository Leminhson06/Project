#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_CATEGORY 100
#define MAX_PRODUCTS 500
#define MAX_ORDERS 1000
#define MAX_LENGTH 50
#define FILENAME_CATEGORY "categories.txt"
#define FILENAME_PRODUCT "products.txt"
#define FILENAME_ORDER "orders.txt"
#define FILENAME_ADMIN "admin.txt"

// Structures
typedef struct {
    char id[10];
    char name[MAX_LENGTH];
    char parentId[10];  // For hierarchical categories
} Category;

typedef struct {
    char id[10];
    char name[MAX_LENGTH];
    char categoryId[10];
    double price;
    int quantity;
} Product;

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char productId[10];
    int quantity;
    double price;
} OrderDetail;

typedef struct {
    char id[10];
    Date date;
    OrderDetail items[10];
    int itemCount;
    double totalAmount;
} Order;

// Global variables
Category categories[MAX_CATEGORY];
Product products[MAX_PRODUCTS];
Order orders[MAX_ORDERS];
int categoryCount = 0;
int productCount = 0;
int orderCount = 0;

// Utility functions
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pressEnterToContinue() {
    printf("\nNhan Enter de tiep tuc...");
    getchar();
}

// Validation functions
int isValidId(char *id) {
    return (strlen(id) > 0 && strlen(id) <= 9);
}

int isValidName(char *name) {
    return (strlen(name) > 0 && strlen(name) < MAX_LENGTH);
}

int isValidPrice(double price) {
    return price > 0;
}

int isValidQuantity(int quantity) {
    return quantity >= 0;
}

// Category Management Functions
void displayCategoryHeader() {
    printf("\n%-10s %-30s %-10s\n", "ID", "Ten Danh Muc", "Danh Muc Cha");
    printf("------------------------------------------------\n");
}

void displayCategory(Category cat) {
    printf("%-10s %-30s %-10s\n", cat.id, cat.name, cat.parentId);
}

int findCategoryById(char *id) {
    for (int i = 0; i < categoryCount; i++) {
        if (strcmp(categories[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

void addCategory() {
    clearScreen();
    printf("=== THEM DANH MUC MOI ===\n\n");
    
    if (categoryCount >= MAX_CATEGORY) {
        printf("Danh sach danh muc da day!\n");
        return;
    }

    Category newCat;
    
    do {
        printf("Nhap ID danh muc (toi da 9 ky tu): ");
        scanf("%s", newCat.id);
        clearBuffer();
        
        if (!isValidId(newCat.id)) {
            printf("ID khong hop le! Vui long nhap lai.\n");
            continue;
        }
        
        if (findCategoryById(newCat.id) != -1) {
            printf("ID da ton tai! Vui long nhap lai.\n");
            continue;
        }
        break;
    } while (1);

    do {
        printf("Nhap ten danh muc: ");
        fgets(newCat.name, MAX_LENGTH, stdin);
        newCat.name[strcspn(newCat.name, "\n")] = 0;
        
        if (!isValidName(newCat.name)) {
            printf("Ten khong hop le! Vui long nhap lai.\n");
            continue;
        }
        break;
    } while (1);

    printf("Nhap ID danh muc cha (Enter neu la danh muc goc): ");
    fgets(newCat.parentId, sizeof(newCat.parentId), stdin);
    newCat.parentId[strcspn(newCat.parentId, "\n")] = 0;
    
    if (strlen(newCat.parentId) > 0 && findCategoryById(newCat.parentId) == -1) {
        printf("Danh muc cha khong ton tai!\n");
        return;
    }

    categories[categoryCount++] = newCat;
    printf("\nThem danh muc thanh cong!\n");
}

void updateCategory() {
    clearScreen();
    printf("=== CAP NHAT DANH MUC ===\n\n");
    
    char id[10];
    printf("Nhap ID danh muc can cap nhat: ");
    scanf("%s", id);
    clearBuffer();
    
    int index = findCategoryById(id);
    if (index == -1) {
        printf("Khong tim thay danh muc!\n");
        return;
    }
    
    printf("\nThong tin hien tai:\n");
    displayCategoryHeader();
    displayCategory(categories[index]);
    
    Category updatedCat = categories[index];
    
    printf("\nNhap thong tin moi (Enter de giu nguyen):\n");
    
    char input[MAX_LENGTH];
    printf("Ten moi: ");
    fgets(input, MAX_LENGTH, stdin);
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) > 0) {
        if (!isValidName(input)) {
            printf("Ten khong hop le!\n");
            return;
        }
        strcpy(updatedCat.name, input);
    }
    
    printf("ID danh muc cha moi: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    
    if (strlen(input) > 0) {
        if (findCategoryById(input) == -1) {
            printf("Danh muc cha khong ton tai!\n");
            return;
        }
        strcpy(updatedCat.parentId, input);
    }
    
    categories[index] = updatedCat;
    printf("\nCap nhat thanh cong!\n");
}

void deleteCategory() {
    clearScreen();
    printf("=== XOA DANH MUC ===\n\n");
    
    char id[10];
    printf("Nhap ID danh muc can xoa: ");
    scanf("%s", id);
    clearBuffer();
    
    int index = findCategoryById(id);
    if (index == -1) {
        printf("Khong tim thay danh muc!\n");
        return;
    }
    
    printf("\nThong tin danh muc:\n");
    displayCategoryHeader();
    displayCategory(categories[index]);
    
    printf("\nBan co chac chan muon xoa? (Y/N): ");
    char confirm;
    scanf(" %c", &confirm);
    clearBuffer();
    
    if (toupper(confirm) == 'Y') {
        // Check if category has products
        for (int i = 0; i < productCount; i++) {
            if (strcmp(products[i].categoryId, id) == 0) {
                printf("Khong the xoa danh muc vi con san pham!\n");
                return;
            }
        }
        
        // Check if category has children
        for (int i = 0; i < categoryCount; i++) {
            if (strcmp(categories[i].parentId, id) == 0) {
                printf("Khong the xoa danh muc vi con danh muc con!\n");
                return;
            }
        }
        
        // Shift array to remove category
        for (int i = index; i < categoryCount - 1; i++) {
            categories[i] = categories[i + 1];
        }
        categoryCount--;
        printf("Xoa danh muc thanh cong!\n");
    }
}

// Product Management Functions
void displayProductHeader() {
    printf("\n%-10s %-30s %-10s %-10s %-10s\n", 
           "ID", "Ten San Pham", "Danh Muc", "Gia", "So Luong");
    printf("--------------------------------------------------------\n");
}

void displayProduct(Product prod) {
    printf("%-10s %-30s %-10s %-10.2f %-10d\n",
           prod.id, prod.name, prod.categoryId, prod.price, prod.quantity);
}

int findProductById(char *id) {
    for (int i = 0; i < productCount; i++) {
        if (strcmp(products[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

// File Operations
void saveCategories() {
    FILE *file = fopen(FILENAME_CATEGORY, "w");
    if (file == NULL) {
        printf("Loi luu file danh muc!\n");
        return;
    }
    
    for (int i = 0; i < categoryCount; i++) {
        fprintf(file, "%s|%s|%s\n", 
                categories[i].id, 
                categories[i].name, 
                categories[i].parentId);
    }
    
    fclose(file);
}

void loadCategories() {
    FILE *file = fopen(FILENAME_CATEGORY, "r");
    if (file == NULL) {
        return;
    }
    
    categoryCount = 0;
    char line[MAX_LENGTH * 3];
    
    while (fgets(line, sizeof(line), file) && categoryCount < MAX_CATEGORY) {
        char *id = strtok(line, "|");
        char *name = strtok(NULL, "|");
        char *parentId = strtok(NULL, "\n");
        
        if (id && name && parentId) {
            strcpy(categories[categoryCount].id, id);
            strcpy(categories[categoryCount].name, name);
            strcpy(categories[categoryCount].parentId, parentId);
            categoryCount++;
        }
    }
    
    fclose(file);
}

// Main menu functions
void displayMainMenu() {
    clearScreen();
    printf("=== HE THONG QUAN LY CUA HANG ===\n\n");
    printf("1. Quan ly danh muc\n");
    printf("2. Quan ly san pham\n");
    printf("3. Ban hang\n");
    printf("4. Quan ly hoa don\n");
    printf("5. Bao cao doanh thu\n");
    printf("0. Thoat\n\n");
    printf("Lua chon cua ban: ");
}

void displayCategoryMenu() {
    clearScreen();
    printf("=== QUAN LY DANH MUC ===\n\n");
    printf("1. Them danh muc\n");
    printf("2. Cap nhat danh muc\n");
    printf("3. Xoa danh muc\n");
    printf("4. Xem danh sach danh muc\n");
    printf("5. Tim kiem danh muc\n");
    printf("0. Quay lai\n\n");
    printf("Lua chon cua ban: ");
}

int main() {
    loadCategories();
    
    int choice;
    do {
        displayMainMenu();
        scanf("%d", &choice);
        clearBuffer();
        
        switch (choice) {
            case 1: {
                int catChoice;
                do {
                    displayCategoryMenu();
                    scanf("%d", &catChoice);
                    clearBuffer();
                    
                    switch (catChoice) {
                        case 1:
                            addCategory();
                            break;
                        case 2:
                            updateCategory();
                            break;
                        case 3:
                            deleteCategory();
                            break;
                        case 4:
                            clearScreen();
                            printf("=== DANH SACH DANH MUC ===\n");
                            displayCategoryHeader();
                            for (int i = 0; i < categoryCount; i++) {
                                displayCategory(categories[i]);
                            }
                            break;
                        case 0:
                            break;
                        default:
                            printf("Lua chon khong hop le!\n");
                    }
                    if (catChoice != 0) {
                        pressEnterToContinue();
                    }
                } while (catChoice != 0);
                break;
            }
            case 0:
                saveCategories();
                printf("\nCam on ban da su dung chuong trinh!\n");
                break;
            default:
                printf("Lua chon khong hop le!\n");
                pressEnterToContinue();
        }
    } while (choice != 0);
    
    return 0;
}