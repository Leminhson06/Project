#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CATEGORY 100
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
    int quantity;
    int price;
} Product;

// Category structure
typedef struct {
    char id[10];
    char name[MAX_NAME_LENGTH];
} Category;

// Order structure
typedef struct {
    char orderId[10];
    char customerId[20];
    Date date;
    Product product;
} Order;

// Global variables for category management
Category categories[MAX_CATEGORY];
int categoryCount = 0;

// Utility functions
void clearBuffer() {
    while (getchar() != '\n');
}

void displayCategory() {
    printf("\n%-10s %-20s\n", "ID", "Ten danh muc");
    printf("--------------------------------\n");
    for (int i = 0; i < categoryCount; i++) {
        printf("%-10s %-20s\n", categories[i].id, categories[i].name);
    }
}

int findCategory(char id[]) {
    for (int i = 0; i < categoryCount; i++) {
        if (strcmp(categories[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

int isUniqueCategory(char id[]) {
    return findCategory(id) == -1;
}

int isUniqueCategoryName(char name[]) {
    for (int i = 0; i < categoryCount; i++) {
        if (strcmp(categories[i].name, name) == 0) {
            return 0;
        }
    }
    return 1;
}

// Core functions
void addCategory() {
    if (categoryCount == MAX_CATEGORY) {
        printf("Danh sach danh muc da day, khong the them moi\n");
        return;
    }
    5
    Category newCategory;
    printf("Nhap ma danh muc: ");
    fgets(newCategory.id, sizeof(newCategory.id), stdin);
    newCategory.id[strcspn(newCategory.id, "\n")] = '\0';

    if (!isUniqueCategory(newCategory.id)) {
        printf("Ma danh muc da ton tai. Vui long nhap ma khac.\n");
        return;
    }

    printf("Nhap ten danh muc: ");
    fgets(newCategory.name, sizeof(newCategory.name), stdin);
    newCategory.name[strcspn(newCategory.name, "\n")] = '\0';

    if (!isUniqueCategoryName(newCategory.name)) {
        printf("Ten danh muc da ton tai. Vui long nhap ten khac.\n");
        return;
    }

    categories[categoryCount++] = newCategory;
    printf("Danh muc da duoc them thanh cong.\n");
}

void updateCategory() {
    char id[10];
    printf("Nhap ma danh muc can cap nhat: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';
    
    
    int index = findCategory(id);
    if (index == -1) {
        printf("Khong tim thay danh muc co ma la %s\n", id);
        return;
    }

    printf("Nhap ten danh muc moi: ");
    char newName[MAX_NAME_LENGTH];
    fgets(newName, sizeof(newName), stdin);
    newName[strcspn(newName, "\n")] = '\0';

    if (!isUniqueCategoryName(newName)) {
        printf("Ten danh muc da ton tai. Vui long nhap ten khac.\n");
        return;
    }

    strcpy(categories[index].name, newName);
    printf("Danh muc da duoc cap nhat thanh cong.\n");
}

void deleteCategory() {
    char id[10];
    printf("Nhap ma danh muc can xoa: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';
    
    int index = findCategory(id);
    if (index == -1) {
        printf("Khong tim thay danh muc co ma la %s\n", id);
        return;
    }

    for (int i = index; i < categoryCount - 1; i++) {
        categories[i] = categories[i + 1];
    }
    categoryCount--;
    printf("Danh muc da duoc xoa thanh cong.\n");
}

void searchCategory() {
    char name[MAX_NAME_LENGTH];
    printf("Nhap ten danh muc can tim kiem: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("\n%-10s %-20s\n", "ID", "Ten danh muc");
    printf("--------------------------------\n");
    
    int found = 0;
    for (int i = 0; i < categoryCount; i++) {
        if (strstr(categories[i].name, name) != NULL) {
            printf("%-10s %-20s\n", categories[i].id, categories[i].name);
            found = 1;
        }
    }
    
    if (!found) {
        printf("Khong tim thay danh muc nao.\n");
    }
}

void saveCategory() {
    FILE *file = fopen("category.txt", "w");
    if (file == NULL) {
        printf("Khong the mo file de ghi\n");
        return;
    }

    for (int i = 0; i < categoryCount; i++) {
        fprintf(file, "%s|%s\n", categories[i].id, categories[i].name);
    }
    
    fclose(file);
    printf("Da luu danh muc vao file thanh cong.\n");
}

void loadCategory() {
    FILE *file = fopen("category.txt", "r");
    if (file == NULL) {
        printf("Khong the mo file de doc\n");
        return;
    }

    categoryCount = 0;
    char line[MAX_NAME_LENGTH * 2];
    
    while (fgets(line, sizeof(line), file) != NULL && categoryCount < MAX_CATEGORY) {
        char *id = strtok(line, "|");
        char *name = strtok(NULL, "\n");
        
        if (id && name) {
            strcpy(categories[categoryCount].id, id);
            strcpy(categories[categoryCount].name, name);
            categoryCount++;
        }
    }
    
    fclose(file);
    printf("Da load danh muc tu file thanh cong.\n");
}

void displayMenu() {
    printf("\nQUAN LY DANH MUC\n");
    printf("1. Them moi danh muc\n");
    printf("2. Cap nhat danh muc\n");
    printf("3. Xoa danh muc\n");
    printf("4. Tim kiem danh muc\n");
    printf("5. Hien thi danh sach danh muc\n");
    printf("6. Luu danh muc vao file\n");
    printf("7. Load danh muc tu file\n");
    printf("8. Thoat\n");
    printf("Chon chuc nang: ");
}

int main() {
    int choice;
    
    do {
        displayMenu();
        scanf("%d", &choice);
        clearBuffer();
        
        switch (choice) {
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
                searchCategory();
                break;
            case 5:
                displayCategory();
                break;
            case 6:
                saveCategory();
                break;
            case 7:
                loadCategory();
                break;
            case 8:
                printf("Tam biet!\n");
                break;
            default:
                printf("Chuc nang khong hop le. Vui long chon lai.\n");
        }
    } while (choice != 8);

    return 0;
}