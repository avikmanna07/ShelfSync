#include "utils.h"
#include "storage.h"
#include "auth.h"
#include "customer.h"
#include "book.h"
#include "issue.h"
#include "request.h"

/* ============================================================
 * Forward Declarations
 * ============================================================ */
void showWelcomeScreen(void);
void showMainMenu(void);
void handleMainMenuChoice(int choice);
void customerMenu(int customerIndex);
void staffMenu(void);
void runMainLoop(void);

/* ============================================================
 * Main Entry Point
 * ============================================================ */

int main(void) {
    /* Load all data at startup */
    loadAllData();
    
    /* Initialize default staff if needed */
    initDefaultStaff();
    
    runMainLoop();
    
    /* Save all data on exit */
    saveAllData();
    
    printf("\nThank you for using ShelfSync! Goodbye!\n");
    return 0;
}

/* ============================================================
 * Welcome Screen
 * ============================================================ */

void showWelcomeScreen(void) {
    clearScreen();
    
    const char *welcomeLines[] = {
        "S H E L F S Y N C",
        "Smart Library Management System",
    };
    
    printBox("Welcome", welcomeLines, 2);
}

/* ============================================================
 * Main Menu
 * ============================================================ */

void showMainMenu(void) {
    const int width = 70;
    int innerWidth = width - 4;
    
    printf("\n");
    printf("  +");
    for (int i = 0; i < width - 2; i++) printf("-");
    printf("+\n");
    
    const char *title = "M A I N   M E N U";
    int titleLen = (int)strlen(title);
    int padding = (innerWidth - titleLen) / 2;
    printf("  |");
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s", title);
    for (int i = 0; i < innerWidth - padding - titleLen; i++) printf(" ");
    printf(" |\n");
    
    printf("  +");
    for (int i = 0; i < width - 2; i++) printf("-");
    printf("+\n");
    
    const char *options[] = {
        "Customer Login",
        "Staff Login",
        "Exit"
    };
    
    for (int i = 0; i < 3; i++) {
        char line[100];
        int num = (i == 2) ? 0 : i + 1;  // 1, 2, 0
        snprintf(line, sizeof(line), "  %d. %s", num, options[i]);
        int len = (int)strlen(line);
        printf("  | %s", line);
        for (int j = 0; j < innerWidth - len; j++) printf(" ");
        printf(" |\n");
    }
    
    printf("  +");
    for (int i = 0; i < width - 2; i++) printf("-");
    printf("+\n");
}

/* ============================================================
 * Handle Main Menu Choice
 * ============================================================ */

void handleMainMenuChoice(int choice) {
    switch (choice) {
        case MAIN_MENU_CUSTOMER: {
            int customerIndex;
            if (customerLogin(&customerIndex)) {
                customerMenu(customerIndex);
            }
            break;
        }
        case MAIN_MENU_STAFF: {
            if (staffLogin()) {
                staffMenu();
            }
            break;
        }
        case MAIN_MENU_EXIT:
            printf("\nExiting ShelfSync...\n");
            break;
        default:
            printf("\nInvalid choice. Please try again.\n");
            pauseScreen();
    }
}

/* ============================================================
 * Main Menu Loop with EOF handling
 * ============================================================ */

void runMainLoop(void) {
    int choice;
    
    do {
        showWelcomeScreen();
        showMainMenu();
        
        int result = readInt("Enter your choice: ", 0, 2);
        
        /* Handle EOF */
        if (result == INT_MIN) {
            printf("\nExiting ShelfSync...\n");
            break;
        }
        choice = result;
        
        handleMainMenuChoice(choice);
    } while (choice != MAIN_MENU_EXIT);
}

/* ============================================================
 * Customer Menu
 * ============================================================ */

void customerMenu(int customerIndex) {
    int choice;
    
    const char *options[] = {
        "View My Issued Books",
        "View Pending Returns and Due Dates",
        "View My Returned Books",
        "Search Available Books",
        "Request a Book",
        "View My Requests",
        "Logout"
    };
    
    do {
        clearScreen();
        
        char welcomeTitle[80];
        snprintf(welcomeTitle, sizeof(welcomeTitle), "Welcome, %s", customers[customerIndex].name);
        
        const int width = 70;
        int innerWidth = width - 4;
        
        printf("\n");
        printf("  +");
        for (int i = 0; i < width - 2; i++) printf("-");
        printf("+\n");
        
        int titleLen = (int)strlen(welcomeTitle);
        int padding = (innerWidth - titleLen) / 2;
        printf("  |");
        for (int i = 0; i < padding; i++) printf(" ");
        printf("%s", welcomeTitle);
        for (int i = 0; i < innerWidth - padding - titleLen; i++) printf(" ");
        printf(" |\n");
        
        printf("  +");
        for (int i = 0; i < width - 2; i++) printf("-");
        printf("+\n");
        
        for (int i = 0; i < 7; i++) {
            char line[100];
            int num = (i == 6) ? 0 : i + 1;  // 1-6, 0 for Logout
            snprintf(line, sizeof(line), "  %d. %s", num, options[i]);
            int len = (int)strlen(line);
            printf("  | %s", line);
            for (int j = 0; j < innerWidth - len; j++) printf(" ");
            printf(" |\n");
        }
        
        printf("  +");
        for (int i = 0; i < width - 2; i++) printf("-");
        printf("+\n");
        
        int result = readInt("Enter your choice: ", 0, 6);
        
        /* Handle EOF */
        if (result == INT_MIN) {
            printf("\nExiting...\n");
            break;
        }
        choice = result;
        
        switch (choice) {
            case CUST_VIEW_ISSUED:
                viewIssuedBooks(customerIndex);
                break;
            case CUST_VIEW_DUE:
                viewIssuedBooks(customerIndex);  // Same as issued books but shows due dates
                break;
            case CUST_VIEW_RETURNED:
                viewReturnedBooks(customerIndex);
                break;
            case CUST_SEARCH_BOOKS:
                searchBook();
                break;
            case CUST_REQUEST_BOOK:
                requestBook(customerIndex);
                break;
            case CUST_VIEW_REQUESTS:
                viewMyRequests(customerIndex);
                break;
            case CUST_LOGOUT:
                printf("\nLogging out...\n");
                pauseScreen();
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
                pauseScreen();
        }
    } while (choice != CUST_LOGOUT);
}

/* Forward declaration for statistics function */
void showLibraryStatistics(void);

/* ============================================================
 * Staff Menu
 * ============================================================ */

void staffMenu(void) {
    int choice;
    
    const char *options[] = {
        "Add Book",
        "View All Books / Stock",
        "Search Book",
        "Update Book Details",
        "Delete Book",
        "Register New Customer",
        "View All Customers",
        "Customer Search",
        "View Book Requests",
        "Approve / Issue Requested Book",
        "Reject Book Request",
        "Mark Book as Returned",
        "View Overdue Books",
        "Library Statistics",
        "Change Staff Password",
        "Save All Records",
        "Logout"
    };
    
    do {
        clearScreen();
        
        const int width = 70;
        int innerWidth = width - 4;
        
        printf("\n");
        printf("  +");
        for (int i = 0; i < width - 2; i++) printf("-");
        printf("+\n");
        
        const char *title = "S T A F F   M E N U";
        int titleLen = (int)strlen(title);
        int padding = (innerWidth - titleLen) / 2;
        printf("  |");
        for (int i = 0; i < padding; i++) printf(" ");
        printf("%s", title);
        for (int i = 0; i < innerWidth - padding - titleLen; i++) printf(" ");
        printf(" |\n");
        
        printf("  +");
        for (int i = 0; i < width - 2; i++) printf("-");
        printf("+\n");
        
        for (int i = 0; i < 17; i++) {
            char line[100];
            int num = (i == 16) ? 0 : i + 1;  // 1-16, 0 for Logout
            snprintf(line, sizeof(line), "  %d. %s", num, options[i]);
            int len = (int)strlen(line);
            printf("  | %s", line);
            for (int j = 0; j < innerWidth - len; j++) printf(" ");
            printf(" |\n");
        }
        
        printf("  +");
        for (int i = 0; i < width - 2; i++) printf("-");
        printf("+\n");
        
        int result = readInt("Enter your choice: ", 0, 16);
        
        /* Handle EOF */
        if (result == INT_MIN) {
            printf("\nExiting...\n");
            break;
        }
        choice = result;
        
        switch (choice) {
            case STAFF_ADD_BOOK:
                addBook();
                break;
            case STAFF_VIEW_BOOKS:
                viewAllBooks();
                break;
            case STAFF_SEARCH_BOOK:
                searchBook();
                break;
            case STAFF_UPDATE_BOOK:
                updateBook();
                break;
            case STAFF_DELETE_BOOK:
                deleteBook();
                break;
            case STAFF_REGISTER_CUSTOMER:
                registerCustomer();
                break;
            case STAFF_VIEW_CUSTOMERS:
                viewAllCustomers();
                break;
            case STAFF_SEARCH_CUSTOMER:
                searchCustomer();
                break;
            case STAFF_VIEW_REQUESTS:
                viewAllRequests();
                break;
            case STAFF_APPROVE_REQUEST:
                approveRequest();
                break;
            case STAFF_REJECT_REQUEST:
                rejectRequest();
                break;
            case STAFF_RETURN_BOOK:
                returnBook();
                break;
            case STAFF_VIEW_OVERDUE:
                viewOverdueBooks();
                break;
            case STAFF_STATISTICS:
                showLibraryStatistics();
                break;
            case STAFF_CHANGE_PASSWORD:
                changeStaffPassword();
                break;
            case STAFF_SAVE_ALL:
                saveAllData();
                pauseScreen();
                break;
            case STAFF_LOGOUT:
                printf("\nLogging out...\n");
                pauseScreen();
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
                pauseScreen();
        }
    } while (choice != STAFF_LOGOUT);
}

/* ============================================================
 * Library Statistics (separate function)
 * ============================================================ */

void showLibraryStatistics(void) {
    printHeader("L I B R A R Y   S T A T I S T I C S");
    
    int totalBooks = 0, totalCopies = 0, availableCopies = 0, issuedBooks = 0;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isActive) {
            totalBooks++;
            totalCopies += books[i].totalCopies;
            availableCopies += books[i].availableCopies;
        }
    }
    
    for (int i = 0; i < issueCount; i++) {
        if (IS_ISSUED(issues[i].status)) {
            issuedBooks++;
        }
    }
    
    int pendingRequests = 0;
    for (int i = 0; i < requestCount; i++) {
        if (requests[i].status == REQUEST_PENDING) pendingRequests++;
    }
    
    int rejectedRequests = 0;
    for (int i = 0; i < requestCount; i++) {
        if (requests[i].status == REQUEST_REJECTED) rejectedRequests++;
    }
    
    int completedRequests = 0;
    for (int i = 0; i < requestCount; i++) {
        if (requests[i].status == REQUEST_COMPLETED) completedRequests++;
    }
    
    int activeCustomers = 0;
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].isActive) activeCustomers++;
    }
    
    int overdueBooks = 0;
    for (int i = 0; i < issueCount; i++) {
        if (issues[i].status == STATUS_OVERDUE) overdueBooks++;
    }
    
    printf("Total Books (titles):       %d\n", totalBooks);
    printf("Total Copies:               %d\n", totalCopies);
    printf("Available Copies:           %d\n", availableCopies);
    printf("Currently Issued:           %d\n", issuedBooks);
    printf("Pending Requests:           %d\n", pendingRequests);
    printf("Rejected Requests:          %d\n", rejectedRequests);
    printf("Completed Requests:         %d\n", completedRequests);
    printf("Registered Customers:       %d\n", activeCustomers);
    printf("Overdue Books:              %d\n", overdueBooks);
    printSeparator();
    pauseScreen();
}