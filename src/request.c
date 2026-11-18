#include "request.h"
#include "utils.h"
#include "storage.h"
#include "book.h"
#include "customer.h"
#include "issue.h"

/* ============================================================
 * Customer Request a Book
 * ============================================================ */

void requestBook(int customerId) {
    printHeader("R E Q U E S T   A   B O O K");
    
    if (bookCount == 0) {
        printf("No books available in the library.\n");
        pauseScreen();
        return;
    }
    
    /* Show available books */
    printf("Available Books:\n");
    displayBookTableHeader();
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isActive && books[i].availableCopies > 0) {
            printf("%-6d %-30s %-20s %-15s %3d\n",
                   books[i].bookId,
                   books[i].title,
                   books[i].author,
                   books[i].category,
                   books[i].availableCopies);
        }
    }
    printSeparator();
    
    int bookId = readInt("Enter Book ID to request: ", 1, 999999);
    
    int bookIndex = findBookById(bookId);
    if (bookIndex < 0) {
        printf("\nBook not found.\n");
        pauseScreen();
        return;
    }
    
    if (!books[bookIndex].isActive) {
        printf("\nThis book is no longer available.\n");
        pauseScreen();
        return;
    }
    
    /* Check if customer already has a pending request for this book */
    if (hasPendingRequest(customerId, bookId)) {
        printf("\nYou have already requested this book. Please wait for staff approval.\n");
        pauseScreen();
        return;
    }
    
    /* Check if customer already has this book issued */
    for (int i = 0; i < issueCount; i++) {
        if (issues[i].customerId == customerId && 
            issues[i].bookId == bookId && 
            issues[i].status == STATUS_ISSUED) {
            printf("\nYou already have this book issued.\n");
            pauseScreen();
            return;
        }
    }
    
    if (requestCount >= MAX_RECORDS) {
        printf("\nRequest database is full. Cannot process new requests.\n");
        pauseScreen();
        return;
    }
    
    struct BookRequest *req = &requests[requestCount];
    req->requestId = nextRequestId++;
    req->bookId = bookId;
    req->customerId = customerId;
    req->requestDate = getCurrentDate();
    req->status = REQUEST_PENDING;
    
    requestCount++;
    saveRequests();
    
    printf("\n========================================\n");
    printf("Your request has been submitted successfully.\n");
    printf("Please visit the library to collect the book.\n");
    printf("Request ID: %d\n", req->requestId);
    printf("Book: %s\n", books[bookIndex].title);
    printf("========================================\n");
    pauseScreen();
}

/* ============================================================
 * View My Requests (Customer)
 * ============================================================ */

void viewMyRequests(int customerId) {
    printHeader("M Y   B O O K   R E Q U E S T S");
    
    int found = 0;
    
    for (int i = 0; i < requestCount; i++) {
        if (requests[i].customerId == customerId) {
            struct Book *book = &books[findBookById(requests[i].bookId)];
            if (book && book->isActive) {
                if (!found) {
                    printf("%-6s %-30s %-12s %s\n", "Req ID", "Book Title", "Req Date", "Status");
                    printSeparator();
                    found = 1;
                }
                
                printf("%-6d %-30s ", requests[i].requestId, book->title);
                printDate(requests[i].requestDate);
                printf("  %s\n", getRequestStatusString(requests[i].status));
            }
        }
    }
    
    if (!found) {
        printf("No requests found.\n");
    }
    
    printSeparator();
    pauseScreen();
}

/* ============================================================
 * View All Requests (Staff)
 * ============================================================ */

void viewAllRequests(void) {
    printHeader("A L L   B O O K   R E Q U E S T S");
    
    if (requestCount == 0) {
        printf("No book requests.\n");
        pauseScreen();
        return;
    }
    
    int filter = readInt("Filter by status:\n1. All\n2. Pending\n3. Approved\n4. Rejected\n5. Completed\nChoice: ", 1, 5);
    
    int found = 0;
    
    for (int i = 0; i < requestCount; i++) {
        int status = requests[i].status;
        if (filter == 1 || 
            (filter == 2 && status == REQUEST_PENDING) ||
            (filter == 3 && status == REQUEST_APPROVED) ||
            (filter == 4 && status == REQUEST_REJECTED) ||
            (filter == 5 && status == REQUEST_COMPLETED)) {
            
            struct Book *book = &books[findBookById(requests[i].bookId)];
            struct Customer *cust = &customers[requests[i].customerId];
            
            if (book && book->isActive && cust && cust->isActive) {
                if (!found) {
                    printf("%-6s %-30s %-20s %-12s %s\n", "Req ID", "Book", "Customer", "Req Date", "Status");
                    printSeparator();
                    found = 1;
                }
                
                printf("%-6d %-30s %-20s ", requests[i].requestId, book->title, cust->name);
                printDate(requests[i].requestDate);
                printf("  %s\n", getRequestStatusString(status));
            }
        }
    }
    
    if (!found) {
        printf("No requests matching the filter.\n");
    }
    
    printSeparator();
    pauseScreen();
}

/* ============================================================
 * Approve Request (Staff)
 * ============================================================ */

void approveRequest(void) {
    printHeader("A P P R O V E   B O O K   R E Q U E S T");
    
    int pendingCount = 0;
    for (int i = 0; i < requestCount; i++) {
        if (requests[i].status == REQUEST_PENDING) pendingCount++;
    }
    
    if (pendingCount == 0) {
        printf("No pending requests.\n");
        pauseScreen();
        return;
    }
    
    /* Show pending requests */
    printf("Pending Requests:\n");
    printf("%-6s %-30s %-20s %-12s\n", "Req ID", "Book", "Customer", "Req Date");
    printSeparator();
    
    for (int i = 0; i < requestCount; i++) {
        if (requests[i].status == REQUEST_PENDING) {
            struct Book *book = &books[findBookById(requests[i].bookId)];
            struct Customer *cust = &customers[requests[i].customerId];
            
            if (book && book->isActive && cust && cust->isActive) {
                printf("%-6d %-30s %-20s ", requests[i].requestId, book->title, cust->name);
                printDate(requests[i].requestDate);
                printf("\n");
            }
        }
    }
    printSeparator();
    
    int requestId = readInt("Enter Request ID to approve: ", 1, 999999);
    int reqIndex = findRequestById(requestId);
    
    if (reqIndex < 0) {
        printf("\nRequest not found.\n");
        pauseScreen();
        return;
    }
    
    if (requests[reqIndex].status != REQUEST_PENDING) {
        printf("\nThis request is not pending.\n");
        pauseScreen();
        return;
    }
    
    int bookId = requests[reqIndex].bookId;
    int customerId = requests[reqIndex].customerId;
    
    if (!hasAvailableCopies(bookId)) {
        printf("\nNo copies available for this book. Cannot approve request.\n");
        pauseScreen();
        return;
    }
    
    /* Issue the book */
    issueBook(bookId, customerId, requestId);
    
    printf("\nRequest approved and book issued successfully!\n");
    pauseScreen();
}

/* ============================================================
 * Reject Request (Staff)
 * ============================================================ */

void rejectRequest(void) {
    printHeader("R E J E C T   B O O K   R E Q U E S T");
    
    int pendingCount = 0;
    for (int i = 0; i < requestCount; i++) {
        if (requests[i].status == REQUEST_PENDING) pendingCount++;
    }
    
    if (pendingCount == 0) {
        printf("No pending requests to reject.\n");
        pauseScreen();
        return;
    }
    
    /* Show pending requests */
    printf("Pending Requests:\n");
    printf("%-6s %-30s %-20s %-12s\n", "Req ID", "Book", "Customer", "Req Date");
    printSeparator();
    
    for (int i = 0; i < requestCount; i++) {
        if (requests[i].status == REQUEST_PENDING) {
            struct Book *book = &books[findBookById(requests[i].bookId)];
            struct Customer *cust = &customers[requests[i].customerId];
            
            if (book && book->isActive && cust && cust->isActive) {
                printf("%-6d %-30s %-20s ", requests[i].requestId, book->title, cust->name);
                printDate(requests[i].requestDate);
                printf("\n");
            }
        }
    }
    printSeparator();
    
    int requestId = readInt("Enter Request ID to reject: ", 1, 999999);
    int reqIndex = findRequestById(requestId);
    
    if (reqIndex < 0) {
        printf("\nRequest not found.\n");
        pauseScreen();
        return;
    }
    
    if (requests[reqIndex].status != REQUEST_PENDING) {
        printf("\nThis request is not pending.\n");
        pauseScreen();
        return;
    }
    
    requests[reqIndex].status = REQUEST_REJECTED;
    saveRequests();
    
    printf("\nRequest rejected successfully.\n");
    pauseScreen();
}

/* ============================================================
 * Helper Functions
 * ============================================================ */

int findRequestById(int requestId) {
    for (int i = 0; i < requestCount; i++) {
        if (requests[i].requestId == requestId) {
            return i;
        }
    }
    return -1;
}

int hasPendingRequest(int customerId, int bookId) {
    for (int i = 0; i < requestCount; i++) {
        if (requests[i].customerId == customerId && 
            requests[i].bookId == bookId && 
            requests[i].status == REQUEST_PENDING) {
            return 1;
        }
    }
    return 0;
}

void displayRequest(const struct BookRequest *req) {
    if (req == NULL) return;
    
    struct Book *book = &books[findBookById(req->bookId)];
    struct Customer *cust = &customers[req->customerId];
    
    printf("Request ID: %d\n", req->requestId);
    printf("Book: %s (ID: %d)\n", book ? book->title : "Unknown", req->bookId);
    printf("Customer: %s (Code: %s)\n", cust ? cust->name : "Unknown", cust ? cust->uniqueCode : "N/A");
    printf("Request Date: "); printDate(req->requestDate); printf("\n");
    printf("Status: %s\n", getRequestStatusString(req->status));
}

const char* getRequestStatusString(int status) {
    switch (status) {
        case REQUEST_PENDING: return "PENDING";
        case REQUEST_APPROVED: return "APPROVED/ISSUED";
        case REQUEST_REJECTED: return "REJECTED";
        case REQUEST_COMPLETED: return "COMPLETED";
        default: return "UNKNOWN";
    }
}