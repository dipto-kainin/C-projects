#include <stdio.h>

// Function prototypes
void clearInputBuffer();
void temperatureConverter();
void currencyConverter();
void massConverter();
void displayMainMenu();

int main() {
    displayMainMenu();
    return 0;
}

void clearInputBuffer() {
    while (getchar() != '\n'); // Clear input buffer
}

void displayMainMenu() {
    char category;

    printf("\n=====================================================\n");
    printf("            Welcome to the Unit Converter!\n");
    printf("=====================================================\n\n");

    printf("Here are the conversion categories:\n");
    printf("  T - Temperature\n");
    printf("  C - Currency\n");
    printf("  M - Mass\n");
    printf("Please enter the letter of the category you want to convert (or 'Q' to quit): ");
    scanf(" %c", &category); // Space before %c to consume any leading whitespace

    switch (category) {
        case 'T':
            temperatureConverter();
            break;
        case 'C':
            currencyConverter();
            break;
        case 'M':
            massConverter();
            break;
        case 'Q':
            printf("\nExiting program...\n");
            break;
        default:
            printf("\nInvalid category. Please enter 'T', 'C', 'M', or 'Q'.\n");
            displayMainMenu();
    }
}

void temperatureConverter() {
    int choice;
    printf("\n=====================================================\n");
    printf("                 Temperature Converter\n");
    printf("=====================================================\n");
    printf("1. Fahrenheit to Celsius\n");
    printf("2. Celsius to Fahrenheit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice == 1) {
        float fahrenheit;
        printf("\nEnter temperature in Fahrenheit: ");
        scanf("%f", &fahrenheit);
        float celsius = (fahrenheit - 32) * 5 / 9;
        printf("%.2f Fahrenheit is equal to %.2f Celsius.\n", fahrenheit, celsius);
    } else if (choice == 2) {
        float celsius;
        printf("\nEnter temperature in Celsius: ");
        scanf("%f", &celsius);
        float fahrenheit = (celsius * 9 / 5) + 32;
        printf("%.2f Celsius is equal to %.2f Fahrenheit.\n", celsius, fahrenheit);
    } else {
        printf("\nInvalid choice.\n");
    }

    // Prompt to return to main menu
    printf("\nPress any key to return to the main menu...");
    getchar(); // Consume newline character
    displayMainMenu();
}

void currencyConverter() {
    int choice;
    printf("\n=====================================================\n");
    printf("                  Currency Converter\n");
    printf("=====================================================\n");
    printf("1. USD to Euro\n");
    printf("2. USD to Japanese Yen\n");
    printf("3. USD to Chinese Yuan\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice >= 1 && choice <= 3) {
        float usdAmount;
        printf("\nEnter amount in USD: ");
        scanf("%f", &usdAmount);

        float conversionRate;
        const char *currency;
        switch (choice) {
            case 1:
                conversionRate = 0.87;
                currency = "Euro";
                break;
            case 2:
                conversionRate = 111.09;
                currency = "Japanese Yen";
                break;
            case 3:
                conversionRate = 6.82;
                currency = "Chinese Yuan";
                break;
        }

        float convertedAmount = usdAmount * conversionRate;
        printf("%.2f USD is equal to %.2f %s.\n", usdAmount, convertedAmount, currency);
    } else {
        printf("\nInvalid choice.\n");
    }

    // Prompt to return to main menu
    printf("\nPress any key to return to the main menu...");
    getchar(); // Consume newline character
    displayMainMenu();
}

void massConverter() {
    int choice;
    printf("\n=====================================================\n");
    printf("                    Mass Converter\n");
    printf("=====================================================\n");
    printf("1. Ounces to Pounds\n");
    printf("2. Grams to Pounds\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice == 1) {
        float ounces;
        printf("\nEnter mass in ounces: ");
        scanf("%f", &ounces);
        float pounds = ounces * 0.0625;
        printf("%.2f ounces is equal to %.2f pounds.\n", ounces, pounds);
    } else if (choice == 2) {
        float grams;
        printf("\nEnter mass in grams: ");
        scanf("%f", &grams);
        float pounds = grams * 0.00220462;
        printf("%.2f grams is equal to %.2f pounds.\n", grams, pounds);
    } else {
        printf("\nInvalid choice.\n");
    }

    // Prompt to return to main menu
    printf("\nPress any key to return to the main menu...");
    getchar(); // Consume newline character
    displayMainMenu();
}
