#include "book.h"
#include "utils.h"
#include "storage.h"

/* ============================================================
 * Add New Book
 * ============================================================ */

void addBook(void) {
    if (bookCount >= MAX_BOOKS) {
        printf("\nBook database is full. Cannot add more books.\n");
        pauseScreen();
        return;
    }
    
    printHeader("A D D   N E W   B O O K");
    
    int bookId = readInt("Book ID: ", 1, 999999);
    
    if (!isBookIdUnique(bookId)) {
        printf("\nError: Book ID %d already exists.\n", bookId);
        pauseScreen();
        return;
    }
    
    char title[MAX_STR];
    readString("Title: ", title, MAX_STR);
    
    char author[MAX_STR];
    readString("Author: ", author, MAX_STR);
    
    char category[MAX_STR];
    readString("Category: ", category, MAX_STR);
    
    int totalCopies = readInt("Total Copies: ", 1, 1000);
    
    struct Book *book = &books[bookCount];
    book->bookId = bookId;
    strcpy(book->title, title);
    strcpy(book->author, author);
    strcpy(book->category, category);
    book->totalCopies = totalCopies;
    book->availableCopies = totalCopies;
    book->isActive = BOOK_ACTIVE;
    
    bookCount++;
    saveBooks();
    
    printf("\nBook added successfully!\n");
    pauseScreen();
}

/* ============================================================
 * View All Books
 * ============================================================ */

void viewAllBooks(void) {
    printHeader("A L L   B O O K S   ( S T O C K )");
    
    if (bookCount == 0) {
        printf("No books in the library.\n");
        pauseScreen();
        return;
    }
    
    displayBookTableHeader();
    
    int activeCount = 0;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isActive) {
            printf("%-6d %-30s %-20s %-15s %3d / %3d  %s\n",
                   books[i].bookId,
                   books[i].title,
                   books[i].author,
                   books[i].category,
                   books[i].availableCopies,
                   books[i].totalCopies,
                   books[i].availableCopies > 0 ? "Available" : "Unavailable");
            activeCount++;
        }
    }
    
    printSeparator();
    printf("Total Active Books: %d\n", activeCount);
    pauseScreen();
}

/* ============================================================
 * Search Books
 * ============================================================ */

void searchBook(void) {
    printHeader("S E A R C H   B O O K S");
    
    if (bookCount == 0) {
        printf("No books in the library.\n");
        pauseScreen();
        return;
    }
    
    int searchBy = readInt("Search by:\n1. Book ID\n2. Title (partial, case-insensitive)\n3. Author (partial, case-insensitive)\n4. Category (partial, case-insensitive)\nChoice: ", 1, 4);
    
    int foundCount = 0;
    
    if (searchBy == 1) {
        int bookId = readInt("Enter Book ID: ", 1, 999999);
        int index = findBookById(bookId);
        if (index >= 0) {
            displayBook(&books[index]);
            foundCount = 1;
        }
    } else if (searchBy == 2) {
        char title[MAX_STR];
        readString("Enter Title (partial): ", title, MAX_STR);
        
        displayBookTableHeader();
        for (int i = 0; i < bookCount; i++) {
            if (books[i].isActive && strCaseStr(books[i].title, title)) {
                printf("%-6d %-30s %-20s %-15s %3d / %3d  %s\n",
                       books[i].bookId,
                       books[i].title,
                       books[i].author,
                       books[i].category,
                       books[i].availableCopies,
                       books[i].totalCopies,
                       books[i].availableCopies > 0 ? "Available" : "Unavailable");
                foundCount++;
            }
        }
        printSeparator();
    } else if (searchBy == 3) {
        char author[MAX_STR];
        readString("Enter Author (partial): ", author, MAX_STR);
        
        displayBookTableHeader();
        for (int i = 0; i < bookCount; i++) {
            if (books[i].isActive && strCaseStr(books[i].author, author)) {
                printf("%-6d %-30s %-20s %-15s %3d / %3d  %s\n",
                       books[i].bookId,
                       books[i].title,
                       books[i].author,
                       books[i].category,
                       books[i].availableCopies,
                       books[i].totalCopies,
                       books[i].availableCopies > 0 ? "Available" : "Unavailable");
                foundCount++;
            }
        }
        printSeparator();
    } else if (searchBy == 4) {
        char category[MAX_STR];
        readString("Enter Category (partial): ", category, MAX_STR);
        
        displayBookTableHeader();
        for (int i = 0; i < bookCount; i++) {
            if (books[i].isActive && strCaseStr(books[i].category, category)) {
                printf("%-6d %-30s %-20s %-15s %3d / %3d  %s\n",
                       books[i].bookId,
                       books[i].title,
                       books[i].author,
                       books[i].category,
                       books[i].availableCopies,
                       books[i].totalCopies,
                       books[i].availableCopies > 0 ? "Available" : "Unavailable");
                foundCount++;
            }
        }
        printSeparator();
    }
    
    if (foundCount == 0) {
        printf("No books found matching your search.\n");
    } else {
        printf("Found %d book(s).\n", foundCount);
    }
    
    pauseScreen();
}

/* ============================================================
 * Update Book Details
 * ============================================================ */

void updateBook(void) {
    printHeader("U P D A T E   B O O K   D E T A I L S");
    
    if (bookCount == 0) {
        printf("No books in the library.\n");
        pauseScreen();
        return;
    }
    
    int bookId = readInt("Enter Book ID to update: ", 1, 999999);
    int index = findBookById(bookId);
    
    if (index < 0) {
        printf("\nBook not found.\n");
        pauseScreen();
        return;
    }
    
    struct Book *book = &books[index];
    
    printf("\nCurrent Details:\n");
    displayBook(book);
    printSeparator();
    printf("Enter new values (press Enter to keep current):\n\n");
    
    char input[MAX_STR];
    
    printf("Title [%s]: ", book->title);
    fgets(input, MAX_STR, stdin);
    removeNewline(input);
    if (!isEmptyString(input)) strcpy(book->title, input);
    
    printf("Author [%s]: ", book->author);
    fgets(input, MAX_STR, stdin);
    removeNewline(input);
    if (!isEmptyString(input)) strcpy(book->author, input);
    
    printf("Category [%s]: ", book->category);
    fgets(input, MAX_STR, stdin);
    removeNewline(input);
    if (!isEmptyString(input)) strcpy(book->category, input);
    
    printf("Total Copies [%d]: ", book->totalCopies);
    fgets(input, MAX_STR, stdin);
    removeNewline(input);
    if (!isEmptyString(input)) {
        char *endptr;
        long val = strtol(input, &endptr, 10);
        if (endptr != input && *endptr == '\0' && val >= 1 && val <= 1000) {
            int newTotal = (int)val;
            if (newTotal != book->totalCopies) {
                int diff = newTotal - book->totalCopies;
                book->totalCopies = newTotal;
                book->availableCopies += diff;
                if (book->availableCopies < 0) book->availableCopies = 0;
            }
        } else {
            printf("  Invalid input, keeping current value.\n");
        }
    }
    
    saveBooks();
    printf("\nBook updated successfully!\n");
    pauseScreen();
}

/* ============================================================
 * Delete Book (Soft Delete)
 * ============================================================ */

void deleteBook(void) {
    printHeader("D E L E T E   B O O K   ( S O F T   D E L E T E )");
    
    if (bookCount == 0) {
        printf("No books in the library.\n");
        pauseScreen();
        return;
    }
    
    int bookId = readInt("Enter Book ID to delete: ", 1, 999999);
    int index = findBookById(bookId);
    
    if (index < 0) {
        printf("\nBook not found.\n");
        pauseScreen();
        return;
    }
    
    struct Book *book = &books[index];
    
    printf("\nBook to delete:\n");
    displayBook(book);
    
    /* Check if book has active issues (issued or overdue) */
    int hasActiveIssues = 0;
    for (int i = 0; i < issueCount; i++) {
        if (issues[i].bookId == bookId && IS_ISSUED(issues[i].status)) {
            hasActiveIssues = 1;
            break;
        }
    }
    
    if (hasActiveIssues) {
        printf("\nWarning: This book has active issues. It cannot be deleted until all copies are returned.\n");
        pauseScreen();
        return;
    }
    
    int confirm = readInt("\nConfirm deletion? (1=Yes, 0=No): ", 0, 1);
    
    if (confirm == 1) {
        book->isActive = BOOK_DELETED;
        book->availableCopies = 0;
        saveBooks();
        printf("\nBook deleted (soft delete) successfully.\n");
    } else {
        printf("\nDeletion cancelled.\n");
    }
    
    pauseScreen();
}

/* ============================================================
 * Helper Functions
 * ============================================================ */

int findBookById(int bookId) {
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isActive && books[i].bookId == bookId) {
            return i;
        }
    }
    return -1;
}

int isBookIdUnique(int bookId) {
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isActive && books[i].bookId == bookId) {
            return 0;
        }
    }
    return 1;
}

void displayBook(const struct Book *book) {
    if (book == NULL) return;
    
    printf("Book ID: %d\n", book->bookId);
    printf("Title: %s\n", book->title);
    printf("Author: %s\n", book->author);
    printf("Category: %s\n", book->category);
    printf("Total Copies: %d\n", book->totalCopies);
    printf("Available Copies: %d\n", book->availableCopies);
    printf("Status: %s\n", book->isActive ? "Active" : "Deleted");
}

void displayBookTableHeader(void) {
    printf("%-6s %-30s %-20s %-15s %3s / %3s  %s\n", "ID", "Title", "Author", "Category", "Avl", "Tot", "Status");
    printSeparator();
}

int hasAvailableCopies(int bookId) {
    int index = findBookById(bookId);
    if (index >= 0) {
        return books[index].availableCopies > 0;
    }
    return 0;
}