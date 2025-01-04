#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_CATEGORY 1000
#define MAX_PRODUCTS 5000
#define MAX_ORDERS 10000
#define MAX_LENGTH 100
#define MAX_ORDER_ITEMS 50
#define MAX_SEARCH_RESULTS 100
#define FILENAME_CATEGORY "categories.txt"
#define FILENAME_PRODUCT "products.txt"
#define FILENAME_ORDER "orders.txt"
#define FILENAME_ADMIN "admin.txt"

// Enhanced structures with additional fields
typedef struct {
    char id[10];
    char name[MAX_LENGTH];
    char parentId[10];
    char description[MAX_LENGTH * 2];
    time_t createdAt;
    time_t updatedAt;
    int status;  // 1: active, 0: inactive
} Category;

typedef struct {
    char id[10];
    char name[MAX_LENGTH];
    char categoryId[10];
    char description[MAX_LENGTH * 2];
    double price;
    int quantity;
    double costPrice;
    time_t createdAt;
    time_t updatedAt;
    int status;  // 1: active, 0: inactive
    char barcode[20];
    char location[50];
    int minStockLevel;
} Product;

typedef struct {
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
} DateTime;

typedef struct {
    char productId[10];
    int quantity;
    double price;
    double discount;
    char notes[MAX_LENGTH];
} OrderDetail;

typedef struct {
    char id[10];
    DateTime dateTime;
    char customerId[10];
    char customerName[MAX_LENGTH];
    char customerPhone[15];
    char customerEmail[50];
    OrderDetail items[MAX_ORDER_ITEMS];
    int itemCount;
    double subtotal;
    double discount;
    double tax;
    double totalAmount;
    char paymentMethod[20];
    int status;  // 1: pending, 2: confirmed, 3: completed, 4: cancelled
    char notes[MAX_LENGTH * 2];
} Order;

// Global variables
Category categories[MAX_CATEGORY];
Product products[MAX_PRODUCTS];
Order orders[MAX_ORDERS];
int categoryCount = 0;
int productCount = 0;
int orderCount = 0;

// Enhanced utility functions
void getCurrentDateTime(DateTime *dt) {
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    
    dt->year = tm_now->tm_year + 1900;
    dt->month = tm_now->tm_mon + 1;
    dt->day = tm_now->tm_mday;
    dt->hour = tm_now->tm_hour;
    dt->minute = tm_now->tm_min;
    dt->second = tm_now->tm_sec;
}

char* generateId(char prefix, int count) {
    static char id[10];
    sprintf(id, "%c%06d", prefix, count + 1);
    return id;
}

int searchCategories(char* keyword, Category* results, int maxResults) {
    int found = 0;
    char lowerKeyword[MAX_LENGTH];
    strcpy(lowerKeyword, keyword);
    for (int i = 0; i < strlen(lowerKeyword); i++) {
        lowerKeyword[i] = tolower(lowerKeyword[i]);
    }
    
    for (int i = 0; i < categoryCount && found < maxResults; i++) {
        char lowerName[MAX_LENGTH];
        strcpy(lowerName, categories[i].name);
        for (int j = 0; j < strlen(lowerName); j++) {
            lowerName[j] = tolower(lowerName[j]);
        }
        
        if (strstr(lowerName, lowerKeyword) || strstr(categories[i].id, keyword)) {
            results[found++] = categories[i];
        }
    }
    return found;
}

// Enhanced category management
void addCategory() {
    clearScreen();
    printf("=== THEM DANH MUC MOI ===\n\n");
    
    if (categoryCount >= MAX_CATEGORY) {
        printf("Danh sach danh muc da day!\n");
        return;
    }

    Category newCat;
    strcpy(newCat.id, generateId('C', categoryCount));
    newCat.status = 1;
    newCat.createdAt = time(NULL);
    newCat.updatedAt = time(NULL);
    
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

    printf("Nhap mo ta danh muc: ");
    fgets(newCat.description, MAX_LENGTH * 2, stdin);
    newCat.description[strcspn(newCat.description, "\n")] = 0;

    printf("Nhap ID danh muc cha (Enter neu la danh muc goc): ");
    fgets(newCat.parentId, sizeof(newCat.parentId), stdin);
    newCat.parentId[strcspn(newCat.parentId, "\n")] = 0;
    
    if (strlen(newCat.parentId) > 0 && findCategoryById(newCat.parentId) == -1) {
        printf("Danh muc cha khong ton tai!\n");
        return;
    }

    categories[categoryCount++] = newCat;
    saveCategories();
    printf("\nThem danh muc thanh cong! ID moi: %s\n", newCat.id);
}

void displayCategoryHierarchy(char* parentId, int level) {
    for (int i = 0; i < categoryCount; i++) {
        if (strcmp(categories[i].parentId, parentId) == 0) {
            for (int j = 0; j < level; j++) printf("  ");
            printf("- %s (%s)\n", categories[i].name, categories[i].id);
            displayCategoryHierarchy(categories[i].id, level + 1);
        }
    }
}

// Enhanced file operations
void saveCategories() {
    FILE *file = fopen(FILENAME_CATEGORY, "w");
    if (file == NULL) {
        printf("Loi luu file danh muc!\n");
        return;
    }
    
    fprintf(file, "ID|Name|ParentID|Description|Status|CreatedAt|UpdatedAt\n");
    for (int i = 0; i < categoryCount; i++) {
        fprintf(file, "%s|%s|%s|%s|%d|%ld|%ld\n", 
                categories[i].id, 
                categories[i].name, 
                categories[i].parentId,
                categories[i].description,
                categories[i].status,
                categories[i].createdAt,
                categories[i].updatedAt);
    }
    
    fclose(file);
}

void loadCategories() {
    FILE *file = fopen(FILENAME_CATEGORY, "r");
    if (file == NULL) {
        return;
    }
    
    categoryCount = 0;
    char line[MAX_LENGTH * 4];
    
    // Skip header line
    fgets(line, sizeof(line), file);
    
    while (fgets(line, sizeof(line), file) && categoryCount < MAX_CATEGORY) {
        Category *cat = &categories[categoryCount];
        
        char *token = strtok(line, "|");
        if (!token) continue;
        strcpy(cat->id, token);
        
        token = strtok(NULL, "|");
        if (!token) continue;
        strcpy(cat->name, token);
        
        token = strtok(NULL, "|");
        if (!token) continue;
        strcpy(cat->parentId, token);
        
        token = strtok(NULL, "|");
        if (!token) continue;
        strcpy(cat->description, token);
        
        token = strtok(NULL, "|");
        if (!token) continue;
        cat->status = atoi(token);
        
        token = strtok(NULL, "|");
        if (!token) continue;
        cat->createdAt = atol(token);
        
        token = strtok(NULL, "|");
        if (!token) continue;
        cat->updatedAt = atol(token);
        
        categoryCount++;
    }
    
    fclose(file);
}

// Enhanced menu system
void displayMainMenu() {
    clearScreen();
    printf("=== HE THONG QUAN LY CUA HANG ===\n");
    time_t now = time(NULL);
    printf("Thoi gian: %s\n", ctime(&now));
    printf("\n1. Quan ly danh muc");
    printf("\n2. Quan ly san pham");
    printf("\n3. Ban hang");
    printf("\n4. Quan ly hoa don");
    printf("\n5. Bao cao doanh thu");
    printf("\n6. Quan ly ton kho");
    printf("\n7. Cai dat he thong");
    printf("\n0. Thoat\n\n");
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
                            printf("=== DANH SACH DANH MUC ===\n\n");
                            printf("Cau truc phan cap:\n\n");
                            displayCategoryHierarchy("", 0);
                            break;
                        case 5: {
                            clearScreen();
                            printf("=== TIM KIEM DANH MUC ===\n\n");
                            char keyword[MAX_LENGTH];
                            printf("Nhap tu khoa: ");
                            fgets(keyword, MAX_LENGTH, stdin);
                            keyword[strcspn(keyword, "\n")] = 0;
                            
                            Category results[MAX_SEARCH_RESULTS];
                            int count = searchCategories(keyword, results, MAX_SEARCH_RESULTS);
                            
                            if (count > 0) {
                                printf("\nTim thay %d ket qua:\n", count);
                                displayCategoryHeader();
                                for (int i = 0; i < count; i++) {
                                    displayCategory(results[i]);
                                }
                            } else {
                                printf("Khong tim thay ket qua nao!\n");
                            }
                            break;
                        }
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