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
    
    fwrite(&bookCount, sizeof(int), 1, fp);
    fwrite(&nextBookId, sizeof(int), 1, fp);
    fwrite(books, sizeof(struct Book), bookCount, fp);
    
    fclose(fp);
}

void saveCustomers(void) {
    FILE *fp = fopen("data/customers.dat", "wb");
    if (fp == NULL) {
        printf("Error: Cannot open customers.dat for writing.\n");
        return;
    }
    
    fwrite(&customerCount, sizeof(int), 1, fp);
    fwrite(customers, sizeof(struct Customer), customerCount, fp);
    
    fclose(fp);
}

void saveIssues(void) {
    FILE *fp = fopen("data/issues.dat", "wb");
    if (fp == NULL) {
        printf("Error: Cannot open issues.dat for writing.\n");
        return;
    }
    
    fwrite(&issueCount, sizeof(int), 1, fp);
    fwrite(&nextIssueId, sizeof(int), 1, fp);
    fwrite(issues, sizeof(struct IssueRecord), issueCount, fp);
    
    fclose(fp);
}

void saveRequests(void) {
    FILE *fp = fopen("data/requests.dat", "wb");
    if (fp == NULL) {
        printf("Error: Cannot open requests.dat for writing.\n");
        return;
    }
    
    fwrite(&requestCount, sizeof(int), 1, fp);
    fwrite(&nextRequestId, sizeof(int), 1, fp);
    fwrite(requests, sizeof(struct BookRequest), requestCount, fp);
    
    fclose(fp);
}

void saveStaff(void) {
    FILE *fp = fopen("data/staff.dat", "wb");
    if (fp == NULL) {
        printf("Error: Cannot open staff.dat for writing.\n");
        return;
    }
    
    fwrite(&staffCount, sizeof(int), 1, fp);
    fwrite(staff, sizeof(struct Staff), staffCount, fp);
    
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
    
    fread(&bookCount, sizeof(int), 1, fp);
    fread(&nextBookId, sizeof(int), 1, fp);
    fread(books, sizeof(struct Book), bookCount, fp);
    
    fclose(fp);
}

void loadCustomers(void) {
    FILE *fp = fopen("data/customers.dat", "rb");
    if (fp == NULL) {
        customerCount = 0;
        return;
    }
    
    fread(&customerCount, sizeof(int), 1, fp);
    fread(customers, sizeof(struct Customer), customerCount, fp);
    
    fclose(fp);
}

void loadIssues(void) {
    FILE *fp = fopen("data/issues.dat", "rb");
    if (fp == NULL) {
        issueCount = 0;
        nextIssueId = 1;
        return;
    }
    
    fread(&issueCount, sizeof(int), 1, fp);
    fread(&nextIssueId, sizeof(int), 1, fp);
    fread(issues, sizeof(struct IssueRecord), issueCount, fp);
    
    fclose(fp);
}

void loadRequests(void) {
    FILE *fp = fopen("data/requests.dat", "rb");
    if (fp == NULL) {
        requestCount = 0;
        nextRequestId = 1;
        return;
    }
    
    fread(&requestCount, sizeof(int), 1, fp);
    fread(&nextRequestId, sizeof(int), 1, fp);
    fread(requests, sizeof(struct BookRequest), requestCount, fp);
    
    fclose(fp);
}

void loadStaff(void) {
    FILE *fp = fopen("data/staff.dat", "rb");
    if (fp == NULL) {
        staffCount = 0;
        return;
    }
    
    fread(&staffCount, sizeof(int), 1, fp);
    fread(staff, sizeof(struct Staff), staffCount, fp);
    
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