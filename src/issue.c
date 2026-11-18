#include "issue.h"
#include "utils.h"
#include "storage.h"
#include "book.h"
#include "request.h"
#include "customer.h"

/* ============================================================
 * Issue Book to Customer
 * ============================================================ */

void issueBook(int bookId, int customerId, int requestId) {
    int bookIndex = findBookById(bookId);
    if (bookIndex < 0) {
        printf("Error: Book not found.\n");
        return;
    }
    
    if (!hasAvailableCopies(bookId)) {
        printf("Error: No copies available for this book.\n");
        return;
    }
    
    if (issueCount >= MAX_RECORDS) {
        printf("Error: Issue records database is full.\n");
        return;
    }
    
    struct Book *book = &books[bookIndex];
    struct IssueRecord *issue = &issues[issueCount];
    
    issue->issueId = nextIssueId++;
    issue->bookId = bookId;
    issue->customerId = customerId;
    issue->issueDate = getCurrentDate();
    issue->dueDate = addDays(issue->issueDate, DUE_DAYS);
    issue->returnDate.day = 0;
    issue->returnDate.month = 0;
    issue->returnDate.year = 0;
    issue->status = STATUS_ISSUED;
    
    book->availableCopies--;
    
    issueCount++;
    
    /* Update request status if provided */
    if (requestId > 0) {
        int reqIndex = findRequestById(requestId);
        if (reqIndex >= 0) {
            requests[reqIndex].status = REQUEST_APPROVED;
            saveRequests();
        }
    }
    
    saveIssues();
    saveBooks();
    
    printf("\nBook issued successfully!\n");
    printf("Issue ID: %d\n", issue->issueId);
    printf("Book: %s\n", book->title);
    printf("Issue Date: "); printDate(issue->issueDate);
    printf("\nDue Date: "); printDate(issue->dueDate);
    printf("\n");
}

/* ============================================================
 * Return Book
 * ============================================================ */

void returnBook(void) {
    printHeader("M A R K   B O O K   A S   R E T U R N E D");
    
    if (issueCount == 0) {
        printf("No issue records found.\n");
        pauseScreen();
        return;
    }
    
    char code[CODE_LENGTH];
    readString("Enter Customer Library Code: ", code, CODE_LENGTH);
    
    int custIndex = findCustomerByCode(code);
    if (custIndex < 0) {
        printf("\nCustomer not found.\n");
        pauseScreen();
        return;
    }
    
    /* Show customer's issued books */
    printf("\nBooks currently issued to %s:\n", customers[custIndex].name);
    printSeparator();
    
    int issuedIndices[MAX_RECORDS];
    int issuedCount = 0;
    
    for (int i = 0; i < issueCount; i++) {
        if (issues[i].customerId == custIndex && issues[i].status == STATUS_ISSUED) {
            struct Book *book = &books[findBookById(issues[i].bookId)];
            if (book && book->isActive) {
                issuedIndices[issuedCount] = i;
                printf("%d. Book ID: %d | %s\n", issuedCount + 1, book->bookId, book->title);
                printf("   Issue ID: %d | Due: ", issues[i].issueId);
                printDate(issues[i].dueDate);
                if (isOverdue(issues[i].dueDate)) printf(" [OVERDUE]");
                printf("\n");
                issuedCount++;
            }
        }
    }
    
    if (issuedCount == 0) {
        printf("No books currently issued to this customer.\n");
        pauseScreen();
        return;
    }
    
    int choice = readInt("\nSelect book to return (1-%d, 0 to cancel): ", 0, issuedCount);
    
    if (choice == 0) {
        printf("Return cancelled.\n");
        pauseScreen();
        return;
    }
    
    int issueIndex = issuedIndices[choice - 1];
    struct IssueRecord *issue = &issues[issueIndex];
    struct Book *book = &books[findBookById(issue->bookId)];
    
    printf("\nConfirm return of: %s\n", book ? book->title : "Unknown Book");
    int confirm = readInt("Confirm? (1=Yes, 0=No): ", 0, 1);
    
    if (confirm == 1) {
        issue->status = STATUS_RETURNED;
        issue->returnDate = getCurrentDate();
        if (book) book->availableCopies++;
        
        saveIssues();
        saveBooks();
        
        printf("\nBook returned successfully!\n");
        printf("Return Date: "); printDate(issue->returnDate);
        printf("\n");
    } else {
        printf("\nReturn cancelled.\n");
    }
    
    pauseScreen();
}

/* ============================================================
 * View Issued Books for Customer
 * ============================================================ */

void viewIssuedBooks(int customerId) {
    printHeader("M Y   I S S U E D   B O O K S");
    
    int found = 0;
    updateOverdueStatus();
    
    for (int i = 0; i < issueCount; i++) {
        if (issues[i].customerId == customerId && 
            (issues[i].status == STATUS_ISSUED || issues[i].status == STATUS_OVERDUE)) {
            
            struct Book *book = &books[findBookById(issues[i].bookId)];
            if (book && book->isActive) {
                if (!found) {
                    printf("%-6s %-30s %-12s %-12s %s\n", "ID", "Title", "Issue Date", "Due Date", "Status");
                    printSeparator();
                    found = 1;
                }
                
                printf("%-6d %-30s ", book->bookId, book->title);
                printDate(issues[i].issueDate);
                printf("  ");
                printDate(issues[i].dueDate);
                printf("  ");
                
                if (issues[i].status == STATUS_OVERDUE) {
                    printf("OVERDUE");
                } else {
                    printf("Issued");
                }
                printf("\n");
            }
        }
    }
    
    if (!found) {
        printf("No books currently issued.\n");
    }
    
    printSeparator();
    pauseScreen();
}

/* ============================================================
 * View Returned Books for Customer
 * ============================================================ */

void viewReturnedBooks(int customerId) {
    printHeader("M Y   R E T U R N E D   B O O K S");
    
    int found = 0;
    
    for (int i = 0; i < issueCount; i++) {
        if (issues[i].customerId == customerId && issues[i].status == STATUS_RETURNED) {
            struct Book *book = &books[findBookById(issues[i].bookId)];
            if (book && book->isActive) {
                if (!found) {
                    printf("%-6s %-30s %-12s %s\n", "ID", "Title", "Issue Date", "Return Date");
                    printSeparator();
                    found = 1;
                }
                
                printf("%-6d %-30s ", book->bookId, book->title);
                printDate(issues[i].issueDate);
                printf("  ");
                printDate(issues[i].returnDate);
                printf("\n");
            }
        }
    }
    
    if (!found) {
        printf("No returned books in history.\n");
    }
    
    printSeparator();
    pauseScreen();
}

/* ============================================================
 * View Overdue Books (Staff)
 * ============================================================ */

void viewOverdueBooks(void) {
    printHeader("O V E R D U E   B O O K S");
    
    updateOverdueStatus();
    
    int found = 0;
    
    for (int i = 0; i < issueCount; i++) {
        if (issues[i].status == STATUS_OVERDUE) {
            struct Book *book = &books[findBookById(issues[i].bookId)];
            struct Customer *cust = &customers[issues[i].customerId];
            
            if (book && book->isActive && cust && cust->isActive) {
                if (!found) {
                    printf("%-6s %-30s %-20s %-12s %s\n", "Book ID", "Title", "Customer", "Due Date", "Code");
                    printSeparator();
                    found = 1;
                }
                
                printf("%-6d %-30s %-20s ", book->bookId, book->title, cust->name);
                printDate(issues[i].dueDate);
                printf("  %s\n", cust->uniqueCode);
            }
        }
    }
    
    if (!found) {
        printf("No overdue books.\n");
    }
    
    printSeparator();
    pauseScreen();
}

/* ============================================================
 * Update Overdue Status
 * ============================================================ */

void updateOverdueStatus(void) {
    for (int i = 0; i < issueCount; i++) {
        if (issues[i].status == STATUS_ISSUED && isOverdue(issues[i].dueDate)) {
            issues[i].status = STATUS_OVERDUE;
        }
    }
}

/* ============================================================
 * Helper Functions
 * ============================================================ */

int getActiveIssue(int bookId, int customerId) {
    for (int i = 0; i < issueCount; i++) {
        if (issues[i].bookId == bookId && 
            issues[i].customerId == customerId && 
            issues[i].status == STATUS_ISSUED) {
            return i;
        }
    }
    return -1;
}

void displayIssueRecord(const struct IssueRecord *issue) {
    if (issue == NULL) return;
    
    struct Book *book = &books[findBookById(issue->bookId)];
    struct Customer *cust = &customers[issue->customerId];
    
    printf("Issue ID: %d\n", issue->issueId);
    printf("Book: %s (ID: %d)\n", book ? book->title : "Unknown", issue->bookId);
    printf("Customer: %s (Code: %s)\n", cust ? cust->name : "Unknown", cust ? cust->uniqueCode : "N/A");
    printf("Issue Date: "); printDate(issue->issueDate); printf("\n");
    printf("Due Date: "); printDate(issue->dueDate); printf("\n");
    printf("Status: ");
    switch (issue->status) {
        case STATUS_ISSUED: printf("Issued"); break;
        case STATUS_RETURNED: printf("Returned"); break;
        case STATUS_OVERDUE: printf("Overdue"); break;
        default: printf("Unknown");
    }
    printf("\n");
    if (issue->status == STATUS_RETURNED) {
        printf("Return Date: "); printDate(issue->returnDate); printf("\n");
    }
}