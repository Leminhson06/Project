#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CATEGORY 100
#define MAX_NAME_LENGTH 50

// Struct dai dien cho mot danh muc
typedef struct {
    char id[10];
    char name[MAX_NAME_LENGTH];
} Category;

// Bien toan cuc luu tru danh sach cac danh muc
Category category[MAX_CATEGORY];
int categoryCount = 0;

// Ham tien ich
void clearBuffer() {
    while (getchar() != '\n');
}
void displayCategory() {
    printf("\n%-10s %-20s\n", "ID", "Ten danh muc");
    printf("--------------------------------\n");
    for (int i = 0; i < categoryCount; i++) {
        printf("%-10s %-20s\n", category[i].id, category[i].name);
    }
}
int findCategory(char id[]) {
    for (int i = 0; i < categoryCount; i++) {
        if (strcmp(category[i].id, id) == 0) {
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
        if (strcmp(category[i].name, name) == 0) {
            return 0;
        }
    }
    return 1;
}

// Chuc nang chinh cua chuong trinh
void addCategory() {
    if (categoryCount == MAX_CATEGORY) {
        printf("Danh sach danh muc da day, khong the them moi\n");
        return;
    }
    Category newCategory;
    printf("Nhap ma danh muc: ");
    fgets(newCategory.id, sizeof(newCategory.id), stdin);
    newCategory.id[strcspn(newCategory.id, "\n")] = '\0'; // Remove newline character

    if (!isUniqueCategory(newCategory.id)) {
        printf("Ma danh muc da ton tai. Vui long nhap ma khac.\n");
        return;
    }

    printf("Nhap ten danh muc: ");
    fgets(newCategory.name, sizeof(newCategory.name), stdin);
    newCategory.name[strcspn(newCategory.name, "\n")] = '\0'; // Remove newline character

    if (!isUniqueCategoryName(newCategory.name)) {
        printf("Ten danh muc da ton tai. Vui long nhap ten khac.\n");
        return;
    }

    category[categoryCount++] = newCategory;
    printf("Danh muc da duoc them thanh cong.\n");
}

void updateCategory () {
    char id[10];
    printf("Nhap ma danh muc can cap nhat:");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';
    int index = findCategory(id);
    if (index == -1) {
        printf("Khong tim thay danh muc co ma la %s\n", id);
        return;
    }
    Category newCategory;
    printf("Nhap ten danh muc moi: ");
    fgets(newCategory.name, sizeof(newCategory.name), stdin);
    newCategory.name[strcspn(newCategory.name, "\n")] = '\0';
    if (!isUniqueCategoryName(newCategory.name)) {
        printf("Ten danh muc da ton tai. Vui long nhap ten khac.\n");
        return;
    }
    category[index] = newCategory;
    printf("Danh muc da duoc cap nhat thanh cong.\n");
}
// Ham xoa danh muc
void deleteCategory() {
    char id[10];
    printf("Nhap ma danh muc can xoa:");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';
    int index = findCategory(id);
    if (index == -1) {
        printf("Khong tim thay danh muc co ma la %s\n", id);
        return;
    }
    for (int i = index; i < categoryCount - 1; i++) {
        category[i] = category[i + 1];
    }
    categoryCount--;
    printf("Danh muc da duoc xoa thanh cong.\n");
}
// void tim kiem danh muc
void searchCategory() {
    char name[MAX_NAME_LENGTH];
    printf("Nhap ten danh muc can tim kiem: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    int found = 0;
    for (int i = 0; i < categoryCount; i++) {
        if (strstr(category[i].name, name) != NULL) {
            printf("%-10s %-20 \n", category[i].id, category[i].name);
            found = 1;
        }
    }
    if (!found) {
        printf("Khong tim thay danh muc nao.\n");
    }
}
// Ham luu danh muc vao file
void saveCategory() {
    FILE *file;
    file = fopen("category.txt", "w");
    if (file == NULL) {
        printf("Khong the mo file de ghi\n");
        return;
    }
    // Xu ly ghi danh muc vao file
    for (int i = 0; i < categoryCount; i++) {
        fprintf(file, "%s|%s\n", category[i].id, category[i].name);
    }
    fclose(file);
    printf("Da luu danh muc vao file thanh cong.\n");
}
// Ham load danh muc tu file
void loadCategory() {
    FILE *file;
    file = fopen("category.txt", "r");
    if (file == NULL) {
        printf("Khong the mo file de doc\n");
        return;
    }
    // Xu ly doc danh muc tu file
    categoryCount = 0;
    while (fscanf(file, "%d|%[^\n]%*c", category[categoryCount].id, category[categoryCount].name) != EOF) {
        categoryCount++;
    }
    fclose(file);
    printf("Da load danh muc tu file thanh cong.\n");
}
// Ham hien thi menu
void displayMenu() {
    printf("\n");
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

// Cau truc ngay thang nam
struct Date {
    int day;
    int month;
    int year;
};
// Cau truc ma va ten cua hang hoa
struct Category {
    char CategoryId[10];
    char CategoryName[10];
};
// Cau truc thong tin cua hang hoa
struct Product {
    char ProductId[10];
    char ProductName[10];
    char productName[10];
    int quantity;
    int price;
};
// Cau truc Order cua khach hang
struct Order {
    char orderId[10];
    char customerId[20];
    struct Date date;
    struct Product product;
};

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
    }
    while (choice != 8); 
    // Cau truc ngay thang nam
    struct Date date;
    printf("Moi ban nhap ngay: ");
    scanf("%d", &date.day);
    printf("Moi ban nhap thang: ");
    scanf("%d", &date.month);
    printf("Moi ban nhap nam: ");
    scanf("%d", &date.year);
    printf("Ngay thang nam ban vua nhap la: %d / %d / %d\n", date.day, date.month, date.year);
    // Cau truc ma va ten cua hang hoa
    struct Category category ;
    printf("Moi ban nhap ma cua hang hoa: ");
    fgets(category.CategoryI, sizeof(category.CategoryId), stdin);
    printf("Moi ban nhap ten cua hang hoa: ");
    fgets(category.CategoryName, sizeof(category.CategoryName), stdin);
    printf("Ma cua hang hoa ban vua nhap la: %s\n", category.CategoryId);
    printf("Ten cua hang hoa ban vua nhap la: %s\n", category.CategoryName);
    // Cau truc thong tin cua hang hoa
    struct Product product;
    printf("Moi ban nhap ma cua hang hoa: ");
    fgets(product.ProductId, sizeof(product.ProductId), stdin);
    printf("Moi ban nhap ten cua hang hoa: ");
    fgets(product.ProductName, sizeof(product.ProductName), stdin);
    printf("Moi ban nhap so luong cua hang hoa: ");
    scanf("%d", &product.quantity);
    printf("Moi ban nhap gia cua hang hoa: ");
    scanf("%d", &product.price);
    printf("Ma cua hang hoa ban vua nhap la: %s\n", product.ProductId);
    printf("Ten cua hang hoa ban vua nhap la: %s\n", product.ProductName);
    printf("So luong cua hang hoa ban vua nhap la: %d\n", product.quantity);
    printf("Gia cua hang hoa ban vua nhap la: %d\n", product.price);
    // Cau truc Order cua khach hang
    struct Order order;
    printf("Moi ban nhap ma cua don hang: ");
    fgets(order.orderId, sizeof(order.orderId), stdin);
    printf("Moi ban nhap ma cua khach hang: ");
    fgets(order.customerId, sizeof(order.customerId), stdin);
    printf("Moi ban nhap ngay thang nam cua don hang: ");
    scanf("%d %d %d", &order.date.day, &order.date.month, &order.date.year);
    printf("Moi ban nhap ma cua hang hoa: ");
    fgets(order.product.ProductId, sizeof(order.product.ProductId), stdin);
    printf("Moi ban nhap ten cua hang hoa: ");
    fgets(order.product.ProductName, sizeof(order.product.ProductName), stdin);
    printf("Moi ban nhap so luong cua hang hoa: ");
    scanf("%d", &order.product.quantity);
    printf("Moi ban nhap gia cua hang hoa: ");
    scanf("%d", &order.product.price);
    printf("Ma cua don hang ban vua nhap la: %s\n", order.orderId);
    printf("Ma cua khach hang ban vua nhap la: %s\n", order.customerId);
    printf("Ngay thang nam cua don hang ban vua nhap la: %d / %d / %d\n", order.date.day, order.date.month, order.date.year);
    printf("Ma cua hang hoa ban vua nhap la: %s\n", order.product.ProductId);
    printf("Ten cua hang hoa ban vua nhap la: %s\n", order.product.ProductName);
    printf("So luong cua hang hoa ban vua nhap la: %d\n", order.product.quantity);
    printf("Gia cua hang hoa ban vua nhap la: %d\n", order.product.price);
    return 0;
}
