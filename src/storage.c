#include "storage.h"
#include "utils.h"
#include <direct.h>  // For _mkdir on Windows

/* ============================================================
 * Data Directory Initialization
 * ============================================================ */

void initDataDirectory(void) {
    _mkdir("data");
}

/* ============================================================
 * Save Functions
 * ============================================================ */

void saveBooks(void) {
    FILE *fp = fopen("data/books.dat", "wb");
    if (fp == NULL) {
        printf("Error: Cannot open books.dat for writing.\n");
        return;
    }
    
    if (fwrite(&bookCount, sizeof(int), 1, fp) != 1 ||
        fwrite(&nextBookId, sizeof(int), 1, fp) != 1 ||
        fwrite(books, sizeof(struct Book), bookCount, fp) != (size_t)bookCount) {
        printf("Error: Failed to write books.dat\n");
    }
    
    fclose(fp);
}

void saveCustomers(void) {
    FILE *fp = fopen("data/customers.dat", "wb");
    if (fp == NULL) {
        printf("Error: Cannot open customers.dat for writing.\n");
        return;
    }
    
    if (fwrite(&customerCount, sizeof(int), 1, fp) != 1 ||
        fwrite(customers, sizeof(struct Customer), customerCount, fp) != (size_t)customerCount) {
        printf("Error: Failed to write customers.dat\n");
    }
    
    fclose(fp);
}

void saveIssues(void) {
    FILE *fp = fopen("data/issues.dat", "wb");
    if (fp == NULL) {
        printf("Error: Cannot open issues.dat for writing.\n");
        return;
    }
    
    if (fwrite(&issueCount, sizeof(int), 1, fp) != 1 ||
        fwrite(&nextIssueId, sizeof(int), 1, fp) != 1 ||
        fwrite(issues, sizeof(struct IssueRecord), issueCount, fp) != (size_t)issueCount) {
        printf("Error: Failed to write issues.dat\n");
    }
    
    fclose(fp);
}

void saveRequests(void) {
    FILE *fp = fopen("data/requests.dat", "wb");
    if (fp == NULL) {
        printf("Error: Cannot open requests.dat for writing.\n");
        return;
    }
    
    if (fwrite(&requestCount, sizeof(int), 1, fp) != 1 ||
        fwrite(&nextRequestId, sizeof(int), 1, fp) != 1 ||
        fwrite(requests, sizeof(struct BookRequest), requestCount, fp) != (size_t)requestCount) {
        printf("Error: Failed to write requests.dat\n");
    }
    
    fclose(fp);
}

void saveStaff(void) {
    FILE *fp = fopen("data/staff.dat", "wb");
    if (fp == NULL) {
        printf("Error: Cannot open staff.dat for writing.\n");
        return;
    }
    
    if (fwrite(&staffCount, sizeof(int), 1, fp) != 1 ||
        fwrite(staff, sizeof(struct Staff), staffCount, fp) != (size_t)staffCount) {
        printf("Error: Failed to write staff.dat\n");
    }
    
    fclose(fp);
}

void saveAllData(void) {
    saveBooks();
    saveCustomers();
    saveIssues();
    saveRequests();
    saveStaff();
    printf("All data saved successfully.\n");
}

/* ============================================================
 * Load Functions
 * ============================================================ */

void loadBooks(void) {
    FILE *fp = fopen("data/books.dat", "rb");
    if (fp == NULL) {
        bookCount = 0;
        nextBookId = 1;
        return;
    }
    
    int count, nextId;
    if (fread(&count, sizeof(int), 1, fp) != 1 ||
        fread(&nextId, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        bookCount = 0;
        nextBookId = 1;
        return;
    }
    
    /* Bounds checking */
    if (count < 0 || count > MAX_BOOKS) {
        fclose(fp);
        bookCount = 0;
        nextBookId = 1;
        printf("Warning: Invalid book count in data file, resetting.\n");
        return;
    }
    
    bookCount = count;
    nextBookId = nextId;
    
    if (fread(books, sizeof(struct Book), bookCount, fp) != (size_t)bookCount) {
        fclose(fp);
        bookCount = 0;
        nextBookId = 1;
        return;
    }
    
    fclose(fp);
}

void loadCustomers(void) {
    FILE *fp = fopen("data/customers.dat", "rb");
    if (fp == NULL) {
        customerCount = 0;
        return;
    }
    
    int count;
    if (fread(&count, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        customerCount = 0;
        return;
    }
    
    /* Bounds checking */
    if (count < 0 || count > MAX_CUSTOMERS) {
        fclose(fp);
        customerCount = 0;
        printf("Warning: Invalid customer count in data file, resetting.\n");
        return;
    }
    
    customerCount = count;
    
    if (fread(customers, sizeof(struct Customer), customerCount, fp) != (size_t)customerCount) {
        fclose(fp);
        customerCount = 0;
        return;
    }
    
    fclose(fp);
}

void loadIssues(void) {
    FILE *fp = fopen("data/issues.dat", "rb");
    if (fp == NULL) {
        issueCount = 0;
        nextIssueId = 1;
        return;
    }
    
    int count, nextId;
    if (fread(&count, sizeof(int), 1, fp) != 1 ||
        fread(&nextId, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        issueCount = 0;
        nextIssueId = 1;
        return;
    }
    
    /* Bounds checking */
    if (count < 0 || count > MAX_RECORDS) {
        fclose(fp);
        issueCount = 0;
        nextIssueId = 1;
        printf("Warning: Invalid issue count in data file, resetting.\n");
        return;
    }
    
    issueCount = count;
    nextIssueId = nextId;
    
    if (fread(issues, sizeof(struct IssueRecord), issueCount, fp) != (size_t)issueCount) {
        fclose(fp);
        issueCount = 0;
        nextIssueId = 1;
        return;
    }
    
    fclose(fp);
}

void loadRequests(void) {
    FILE *fp = fopen("data/requests.dat", "rb");
    if (fp == NULL) {
        requestCount = 0;
        nextRequestId = 1;
        return;
    }
    
    int count, nextId;
    if (fread(&count, sizeof(int), 1, fp) != 1 ||
        fread(&nextId, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        requestCount = 0;
        nextRequestId = 1;
        return;
    }
    
    /* Bounds checking */
    if (count < 0 || count > MAX_RECORDS) {
        fclose(fp);
        requestCount = 0;
        nextRequestId = 1;
        printf("Warning: Invalid request count in data file, resetting.\n");
        return;
    }
    
    requestCount = count;
    nextRequestId = nextId;
    
    if (fread(requests, sizeof(struct BookRequest), requestCount, fp) != (size_t)requestCount) {
        fclose(fp);
        requestCount = 0;
        nextRequestId = 1;
        return;
    }
    
    fclose(fp);
}

void loadStaff(void) {
    FILE *fp = fopen("data/staff.dat", "rb");
    if (fp == NULL) {
        staffCount = 0;
        return;
    }
    
    int count;
    if (fread(&count, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        staffCount = 0;
        return;
    }
    
    /* Bounds checking */
    if (count < 0 || count > MAX_STAFF) {
        fclose(fp);
        staffCount = 0;
        printf("Warning: Invalid staff count in data file, resetting.\n");
        return;
    }
    
    staffCount = count;
    
    if (fread(staff, sizeof(struct Staff), staffCount, fp) != (size_t)staffCount) {
        fclose(fp);
        staffCount = 0;
        return;
    }
    
    fclose(fp);
}

void loadAllData(void) {
    initDataDirectory();
    loadBooks();
    loadCustomers();
    loadIssues();
    loadRequests();
    loadStaff();
    printf("Data loaded successfully.\n");
}