#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For sleep()
#include <windows.h>

// Define constants for text colors
#define COLOR_RED     12
#define COLOR_GREEN   10
#define COLOR_YELLOW  14
#define COLOR_RESET   15

// Define a struct for the cafe menu items
struct MenuItem {
    char name[50];
    double price;
};

// Define a struct for the cafe order
struct Order {
    struct MenuItem item;
    int quantity;
};

// Define a function to clear the screen
void clearScreen() {
    system("cls");
}

// Define a function to set text color
void setTextColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Define a function to print the menu
void printMenu(struct MenuItem menu[], int size) {
    clearScreen();
    setTextColor(COLOR_YELLOW);
    printf("==============================\n");
    printf("            MENU\n");
    printf("==============================\n");
    for (int i = 0; i < size; i++) {
        printf("%d. %-20s $%.2f\n", i + 1, menu[i].name, menu[i].price);
    }
    printf("==============================\n");
    printf("0. Return to main menu\n");
    setTextColor(COLOR_RESET);
}

// Define a function to take an order
void takeOrder(struct MenuItem menu[], int size, struct Order orders[], int *orderCount) {
    int choice, quantity;
    printf("\n");
    printf("Enter the item number to order (0 to return): ");
    scanf("%d", &choice);
    if (choice >= 1 && choice <= size) {
        printf("Enter the quantity to order: ");
        scanf("%d", &quantity);
        if (quantity > 0) {
            orders[*orderCount].item = menu[choice - 1];
            orders[*orderCount].quantity = quantity;
            (*orderCount)++;
            setTextColor(COLOR_GREEN);
            printf("Order placed successfully!\n");
            setTextColor(COLOR_RESET);
            sleep(1); // Pause for 1 second to give feedback
        } else {
            setTextColor(COLOR_RED);
            printf("Quantity must be greater than zero.\n");
            setTextColor(COLOR_RESET);
            sleep(1); // Pause for 1 second to give feedback
        }
    } else if (choice == 0) {
        // Return to main menu
    } else {
        setTextColor(COLOR_RED);
        printf("Invalid choice.\n");
        setTextColor(COLOR_RESET);
        sleep(1); // Pause for 1 second to give feedback
    }
}

// Define a function to print the order
void printOrder(struct Order order) {
    printf("%-20s x %d - $%.2f\n", order.item.name, order.quantity, order.item.price * order.quantity);
}

// Define a function to calculate the total price of the order
double calculateTotal(struct Order orders[], int orderCount) {
    double total = 0.0;
    for (int i = 0; i < orderCount; i++) {
        total += orders[i].item.price * orders[i].quantity;
    }
    return total;
}

// Main function
int main() {
    struct MenuItem menu[3] = {{"Coffee", 2.50}, {"Sandwich", 5.00}, {"Cake", 3.00}};
    struct Order orders[10];
    int orderCount = 0;
    int choice;
    clearScreen();
    setTextColor(COLOR_YELLOW);
    printf("==============================\n");
    printf("   Welcome to the Cafe!\n");
    printf("==============================\n");
    setTextColor(COLOR_RESET);
    do {
        printf("\n");
        setTextColor(COLOR_YELLOW);
        printf("============ MENU ============\n");
        setTextColor(COLOR_RESET);
        printf("1. View menu\n");
        printf("2. Place order\n");
        printf("3. View order\n");
        printf("4. Calculate total\n");
        printf("5. Exit\n");
        printf("==============================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printMenu(menu, 3);
                break;
            case 2:
                takeOrder(menu, 3, orders, &orderCount);
                break;
            case 3:
                clearScreen();
                setTextColor(COLOR_YELLOW);
                printf("========== CURRENT ORDER ==========\n");
                setTextColor(COLOR_RESET);
                if (orderCount == 0) {
                    setTextColor(COLOR_YELLOW);
                    printf("No orders placed yet.\n");
                    setTextColor(COLOR_RESET);
                } else {
                    for (int i = 0; i < orderCount; i++) {
                        printOrder(orders[i]);
                    }
                }
                break;
            case 4:
                clearScreen();
                if (orderCount == 0) {
                    setTextColor(COLOR_YELLOW);
                    printf("No orders placed yet.\n");
                    setTextColor(COLOR_RESET);
                } else {
                    printf("Total: $%.2f\n", calculateTotal(orders, orderCount));
                }
                break;
            case 5:
                printf("Exiting.\n");
                break;
            default:
                setTextColor(COLOR_RED);
                printf("Invalid choice.\n");
                setTextColor(COLOR_RESET);
                break;
        }
        sleep(2); // Pause for 2 seconds before displaying the menu again
    } while (choice != 5);
    return 0;
}
