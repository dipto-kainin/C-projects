#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ID_BUTTONS_START 1000
#define ID_EDIT 100
#define ID_BUTTON_CLEAR 101
#define ID_BUTTON_MEMORY_CLEAR 102
#define ID_BUTTON_BACKSPACE 103

HWND hwndEdit;
char szBuffer[256] = ""; // Buffer for storing input/output
double memory = 0; // Memory variable
BOOL decimalAdded = FALSE; // Flag to track decimal point addition

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void HandleButtonClick(int buttonId) {
    char buttonText[2] = "";
    sprintf(buttonText, "%d", buttonId - ID_BUTTONS_START);
    strcat(szBuffer, buttonText); // Concatenate the button number to the input buffer
    SetWindowText(hwndEdit, szBuffer); // Update the edit control text
}

void HandleOperator(char op) {
    // Check if the input buffer is empty or ends with an operator
    if (strlen(szBuffer) == 0 || szBuffer[strlen(szBuffer) - 1] == '+' ||
        szBuffer[strlen(szBuffer) - 1] == '-' || szBuffer[strlen(szBuffer) - 1] == '*' ||
        szBuffer[strlen(szBuffer) - 1] == '/') {
        return;
    }
    // Add the operator to the input buffer
    char opStr[2] = {op, '\0'};
    strcat(szBuffer, opStr);
    SetWindowText(hwndEdit, szBuffer); // Update the edit control text
    decimalAdded = FALSE; // Reset the decimal point flag
}

void HandleDecimal() {
    // Add decimal point to the input buffer if it doesn't already contain one
    if (!decimalAdded) {
        strcat(szBuffer, ".");
        SetWindowText(hwndEdit, szBuffer);
        decimalAdded = TRUE; // Set the flag to indicate decimal point addition
    }
}

void HandleParentheses() {
    // To be implemented: handle parentheses
    // This function is left empty as it depends on the specific requirements of handling parentheses in the calculator.
    // It may involve parsing the input expression and evaluating expressions within parentheses.
}

void HandleMemory(int buttonId) {
    switch (buttonId) {
        case ID_BUTTONS_START + 12: // Memory Recall button (MR)
            sprintf(szBuffer, "%.2f", memory);
            SetWindowText(hwndEdit, szBuffer);
            break;
        case ID_BUTTONS_START + 13: // Memory Add button (M+)
            memory += atof(szBuffer);
            break;
        case ID_BUTTONS_START + 14: // Memory Subtract button (M-)
            memory -= atof(szBuffer);
            break;
    }
}

void HandleScientific(int buttonId) {
    double currentValue = atof(szBuffer);
    switch (buttonId) {
        case ID_BUTTONS_START + 15: // Square root button (sqrt)
            currentValue = sqrt(currentValue);
            break;
        case ID_BUTTONS_START + 16: // Square button (x^2)
            currentValue *= currentValue;
            break;
    }
    sprintf(szBuffer, "%.2f", currentValue);
    SetWindowText(hwndEdit, szBuffer);
}

void CalculateResult() {
    double result = 0;
    if (strlen(szBuffer) > 0) {
        // Using sscanf to parse the expression and evaluate it
        if (sscanf(szBuffer, "%lf", &result) != 1) {
            MessageBox(NULL, "Invalid expression!", "Error", MB_OK | MB_ICONERROR);
            return;
        }
        memset(szBuffer, 0, sizeof(szBuffer)); // Clear the input buffer
        decimalAdded = FALSE; // Reset the decimal point flag
    }
    SetWindowText(hwndEdit, ""); // Clear the edit control text

    char szResult[256] = "";
    sprintf(szResult, "%.2f", result); // Format the result string
    SetWindowText(hwndEdit, szResult); // Display the result in the edit control
}

void HandleClear() {
    strcpy(szBuffer, "");
    SetWindowText(hwndEdit, szBuffer);
    decimalAdded = FALSE; // Reset the decimal point flag
}

void HandleMemoryClear() {
    memory = 0;
}

void HandleBackspace() {
    if (strlen(szBuffer) > 0) {
        if (szBuffer[strlen(szBuffer) - 1] == '.') {
            decimalAdded = FALSE; // Reset the decimal point flag if backspacing from a decimal point
        }
        szBuffer[strlen(szBuffer) - 1] = '\0';
        SetWindowText(hwndEdit, szBuffer);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register Window Class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, GetModuleHandle(NULL), LoadIcon(NULL, IDI_APPLICATION),
                       LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_WINDOW + 1), NULL, "Calculator", NULL };
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create Window
    HWND hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "Calculator", "Calculator", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 320, 480, NULL, NULL, hInstance, NULL);
    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Initialize and display the window
    hwndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_RIGHT | WS_BORDER, 10, 10, 290, 50, hwnd, (HMENU)ID_EDIT, GetModuleHandle(NULL), NULL);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Create Number buttons
    for (int i = 0; i < 10; i++) {
        char buttonText[2] = "";
        sprintf(buttonText, "%d", i);
        CreateWindow("BUTTON", buttonText, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10 + (i % 3) * 80, 80 + (i / 3) * 60, 70, 50, hwnd, (HMENU)(ID_BUTTONS_START + i), GetModuleHandle(NULL), NULL);
    }

    // Create Arithmetic operation buttons
    char arithmeticButtons[] = {'+', '-', '*', '/'};
    for (int i = 0; i < 4; i++) {
        CreateWindow("BUTTON", &arithmeticButtons[i], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 250, 80 + i * 60, 50, 50, hwnd, (HMENU)(ID_BUTTONS_START + 10 + i), GetModuleHandle(NULL), NULL);
    }

    // Create Decimal, Parentheses, Memory, and Scientific buttons
    CreateWindow("BUTTON", ".", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 90, 380, 70, 50, hwnd, (HMENU)(ID_BUTTONS_START + 10), GetModuleHandle(NULL), NULL);
    CreateWindow("BUTTON", "MR", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 380, 70, 50, hwnd, (HMENU)(ID_BUTTONS_START + 12), GetModuleHandle(NULL), NULL);
    CreateWindow("BUTTON", "M+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 170, 380, 70, 50, hwnd, (HMENU)(ID_BUTTONS_START + 13), GetModuleHandle(NULL), NULL);
    CreateWindow("BUTTON", "M-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 250, 380, 70, 50, hwnd, (HMENU)(ID_BUTTONS_START + 14), GetModuleHandle(NULL), NULL);
    CreateWindow("BUTTON", "sqrt", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 440, 70, 50, hwnd, (HMENU)(ID_BUTTONS_START + 15), GetModuleHandle(NULL), NULL);
    CreateWindow("BUTTON", "x^2", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 90, 440, 70, 50, hwnd, (HMENU)(ID_BUTTONS_START + 16), GetModuleHandle(NULL), NULL);

    // Create Clear, Memory Clear, and Backspace buttons
    CreateWindow("BUTTON", "C", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 170, 440, 70, 50, hwnd, (HMENU)ID_BUTTON_CLEAR, GetModuleHandle(NULL), NULL);
    CreateWindow("BUTTON", "MC", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 250, 440, 70, 50, hwnd, (HMENU)ID_BUTTON_MEMORY_CLEAR, GetModuleHandle(NULL), NULL);
    CreateWindow("BUTTON", "<-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 500, 150, 50, hwnd, (HMENU)ID_BUTTON_BACKSPACE, GetModuleHandle(NULL), NULL);

    // Create Equals button
    CreateWindow("BUTTON", "=", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 170, 500, 150, 50, hwnd, (HMENU)(ID_BUTTONS_START + 17), GetModuleHandle(NULL), NULL);

    // Message Loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            switch (wmId) {
                case ID_BUTTONS_START + 10: // Decimal button
                    HandleDecimal();
                    break;
                case ID_BUTTONS_START + 11: // Parentheses button
                    HandleParentheses();
                    break;
                case ID_BUTTONS_START + 12: // Memory Recall button (MR)
                case ID_BUTTONS_START + 13: // Memory Add button (M+)
                case ID_BUTTONS_START + 14: // Memory Subtract button (M-)
                    HandleMemory(wmId);
                    break;
                case ID_BUTTONS_START + 15: // Square root button (sqrt)
                case ID_BUTTONS_START + 16: // Square button (x^2)
                    HandleScientific(wmId);
                    break;
                case ID_BUTTONS_START + 17: // Equals button
                    CalculateResult();
                    break;
                case ID_BUTTON_CLEAR: // Clear button
                    HandleClear();
                    break;
                case ID_BUTTON_MEMORY_CLEAR: // Memory Clear button
                    HandleMemoryClear();
                    break;
                case ID_BUTTON_BACKSPACE: // Backspace button
                    HandleBackspace();
                    break;
                default:
                    if (wmId >= ID_BUTTONS_START && wmId < ID_BUTTONS_START + 10) {
                        HandleButtonClick(wmId);
                    } else if (wmId >= ID_BUTTONS_START + 10 && wmId < ID_BUTTONS_START + 14) {
                        // Operator buttons (+, -, *, /)
                        char operators[] = {'+', '-', '*', '/'};
                        HandleOperator(operators[wmId - ID_BUTTONS_START - 10]);
                    }
                    break;
            }
            break;
        }
        case WM_CHAR: // Handle keyboard input
            switch (wParam) {
                case VK_RETURN: // Equals button (Enter key)
                    CalculateResult();
                    break;
                case VK_ESCAPE: // Clear button (Esc key)
                    HandleClear();
                    break;
                case VK_BACK: // Backspace button
                    HandleBackspace();
                    break;
                case '+':
                case '-':
                case '*':
                case '/':
                    HandleOperator((char)wParam);
                    break;
                default:
                    if (wParam >= '0' && wParam <= '9') { // Numeric input
                        char buttonText[2] = { (char)wParam, '\0' };
                        strcat(szBuffer, buttonText);
                        SetWindowText(hwndEdit, szBuffer);
                    } else if (wParam == '.') { // Decimal point input
                        HandleDecimal();
                    }
                    break;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
