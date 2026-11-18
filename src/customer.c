#include "customer.h"
#include "utils.h"
#include "storage.h"
#include "request.h"
#include "issue.h"
#include "book.h"
#include <stdlib.h>
#include <time.h>

/* ============================================================
 * Generate Unique 6-Digit Code
 * ============================================================ */

int generateUniqueCode(char *code) {
    int attempts = 0;
    const int maxAttempts = 1000;
    
    srand((unsigned int)time(NULL));
    
    while (attempts < maxAttempts) {
        int num = 100000 + rand() % 900000;  // 100000 to 999999
        snprintf(code, CODE_LENGTH, "%06d", num);
        
        if (isCodeUnique(code)) {
            return 1;
        }
        attempts++;
    }
    
    return 0;  // Failed to generate unique code
}

/* ============================================================
 * Check Code Uniqueness
 * ============================================================ */

int isCodeUnique(const char *code) {
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].isActive && strcmp(customers[i].uniqueCode, code) == 0) {
            return 0;
        }
    }
    return 1;
}

/* ============================================================
 * Register New Customer
 * ============================================================ */

int registerCustomer(void) {
    if (customerCount >= MAX_CUSTOMERS) {
        printf("\nCustomer database is full. Cannot register more customers.\n");
        pauseScreen();
        return 0;
    }
    
    printHeader("R E G I S T E R   N E W   C U S T O M E R");
    
    char name[MAX_STR];
    readString("Customer Full Name: ", name, MAX_STR);
    
    char code[CODE_LENGTH];
    if (!generateUniqueCode(code)) {
        printf("\nError: Could not generate unique library code. Please try again.\n");
        pauseScreen();
        return 0;
    }
    
    struct Customer *cust = &customers[customerCount];
    cust->customerId = customerCount + 1;
    strcpy(cust->name, name);
    strcpy(cust->uniqueCode, code);
    cust->isActive = CUSTOMER_ACTIVE;
    
    customerCount++;
    saveCustomers();
    
    printf("\n========================================\n");
    printf("Customer registered successfully!\n");
    printf("Name: %s\n", name);
    printf("Library Code: %s\n", code);
    printf("========================================\n");
    printf("Please give this code to the customer.\n");
    pauseScreen();
    
    return 1;
}

/* ============================================================
 * Find Customer by Code
 * ============================================================ */

int findCustomerByCode(const char *code) {
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].isActive && strcmp(customers[i].uniqueCode, code) == 0) {
            return i;
        }
    }
    return -1;
}

/* ============================================================
 * Find Customer by Name and Code
 * ============================================================ */

int findCustomerByNameAndCode(const char *name, const char *code) {
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].isActive && 
            strcmp(customers[i].uniqueCode, code) == 0 &&
            strCaseCmp(customers[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/* ============================================================
 * View All Customers
 * ============================================================ */

void viewAllCustomers(void) {
    printHeader("A L L   C U S T O M E R S");
    
    if (customerCount == 0) {
        printf("No customers registered.\n");
        pauseScreen();
        return;
    }
    
    printf("%-5s %-30s %-12s %s\n", "ID", "Name", "Library Code", "Status");
    printSeparator();
    
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].isActive) {
            printf("%-5d %-30s %-12s %s\n",
                   customers[i].customerId,
                   customers[i].name,
                   customers[i].uniqueCode,
                   "Active");
        }
    }
    
    printSeparator();
    printf("Total Customers: %d\n", customerCount);
    pauseScreen();
}

/* ============================================================
 * Search Customer
 * ============================================================ */

void searchCustomer(void) {
    printHeader("S E A R C H   C U S T O M E R");
    
    int searchBy = readInt("Search by:\n1. Library Code\n2. Name\nChoice: ", 1, 2);
    
    int index = -1;
    
    if (searchBy == 1) {
        char code[CODE_LENGTH];
        readString("Enter Library Code: ", code, CODE_LENGTH);
        index = findCustomerByCode(code);
    } else {
        char name[MAX_STR];
        readString("Enter Customer Name: ", name, MAX_STR);
        
        for (int i = 0; i < customerCount; i++) {
            if (customers[i].isActive && strCaseStr(customers[i].name, name)) {
                index = i;
                break;
            }
        }
    }
    
    if (index >= 0) {
        displayCustomerProfile(index);
    } else {
        printf("\nCustomer not found.\n");
        pauseScreen();
    }
}

/* ============================================================
 * Display Customer Profile
 * ============================================================ */

void displayCustomerProfile(int custIndex) {
    struct Customer *cust = &customers[custIndex];
    
    printHeader("C U S T O M E R   P R O F I L E");
    
    printf("Name: %s\n", cust->name);
    printf("Library Code: %s\n", cust->uniqueCode);
    printf("Customer ID: %d\n", cust->customerId);
    printf("Status: %s\n", cust->isActive ? "Active" : "Inactive");
    printSeparator();
    
    /* Books Currently Issued */
    printf("\n--- BOOKS CURRENTLY ISSUED ---\n");
    int issuedCount = 0;
    for (int i = 0; i < issueCount; i++) {
        if (issues[i].customerId == custIndex && issues[i].status == STATUS_ISSUED) {
            struct Book *book = &books[findBookById(issues[i].bookId)];
            if (book && book->isActive) {
                printf("  Book ID: %d | Title: %s\n", book->bookId, book->title);
                printf("  Issue Date: "); printDate(issues[i].issueDate);
                printf(" | Due Date: "); printDate(issues[i].dueDate);
                if (isOverdue(issues[i].dueDate)) {
                    printf(" [OVERDUE]");
                }
                printf("\n");
                issuedCount++;
            }
        }
    }
    if (issuedCount == 0) printf("  No books currently issued.\n");
    
    /* Overdue Books */
    printf("\n--- OVERDUE BOOKS ---\n");
    int overdueCount = 0;
    for (int i = 0; i < issueCount; i++) {
        if (issues[i].customerId == custIndex && 
            issues[i].status == STATUS_ISSUED && 
            isOverdue(issues[i].dueDate)) {
            struct Book *book = &books[findBookById(issues[i].bookId)];
            if (book && book->isActive) {
                printf("  Book ID: %d | Title: %s\n", book->bookId, book->title);
                printf("  Due Date: "); printDate(issues[i].dueDate);
                printf(" (Overdue by %d days)\n", 0); // Could calculate days overdue
                overdueCount++;
            }
        }
    }
    if (overdueCount == 0) printf("  No overdue books.\n");
    
    /* Pending Requests */
    printf("\n--- PENDING REQUESTS ---\n");
    int pendingCount = 0;
    for (int i = 0; i < requestCount; i++) {
        if (requests[i].customerId == custIndex && requests[i].status == REQUEST_PENDING) {
            struct Book *book = &books[findBookById(requests[i].bookId)];
            if (book && book->isActive) {
                printf("  Request ID: %d | Book: %s\n", requests[i].requestId, book->title);
                printf("  Request Date: "); printDate(requests[i].requestDate);
                printf("\n");
                pendingCount++;
            }
        }
    }
    if (pendingCount == 0) printf("  No pending requests.\n");
    
    /* Returned Book History */
    printf("\n--- RETURNED BOOK HISTORY ---\n");
    int returnedCount = 0;
    for (int i = 0; i < issueCount; i++) {
        if (issues[i].customerId == custIndex && issues[i].status == STATUS_RETURNED) {
            struct Book *book = &books[findBookById(issues[i].bookId)];
            if (book && book->isActive) {
                printf("  Book ID: %d | Title: %s\n", book->bookId, book->title);
                printf("  Issue Date: "); printDate(issues[i].issueDate);
                printf(" | Return Date: "); printDate(issues[i].returnDate);
                printf("\n");
                returnedCount++;
            }
        }
    }
    if (returnedCount == 0) printf("  No returned books in history.\n");
    
    pauseScreen();
}