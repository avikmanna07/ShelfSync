#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>

/* ============================================================
 * Constants & Limits
 * ============================================================ */
#define MAX_BOOKS        100
#define MAX_CUSTOMERS    100
#define MAX_RECORDS      200
#define MAX_STAFF        10
#define MAX_STR          50
#define CODE_LENGTH      7        /* 6 digits + null terminator */
#define LOGIN_ATTEMPTS   3
#define DUE_DAYS         14       /* Books due in 14 days */

/* Book status */
#define BOOK_ACTIVE      1
#define BOOK_DELETED     0

/* Customer status */
#define CUSTOMER_ACTIVE  1
#define CUSTOMER_INACTIVE 0

/* Issue record status */
#define STATUS_RETURNED  0
#define STATUS_ISSUED    1
#define STATUS_OVERDUE   2

/* Helper macros for issue status */
#define IS_ISSUED(s)     ((s) == STATUS_ISSUED || (s) == STATUS_OVERDUE)
#define IS_ACTIVE_ISSUE(s) IS_ISSUED(s)

/* Request status */
#define REQUEST_PENDING  0
#define REQUEST_APPROVED 1
#define REQUEST_REJECTED 2
#define REQUEST_COMPLETED 3

/* Main menu options */
#define MAIN_MENU_CUSTOMER  1
#define MAIN_MENU_STAFF     2
#define MAIN_MENU_EXIT      0

/* Staff menu options */
#define STAFF_ADD_BOOK           1
#define STAFF_VIEW_BOOKS         2
#define STAFF_SEARCH_BOOK        3
#define STAFF_UPDATE_BOOK        4
#define STAFF_DELETE_BOOK        5
#define STAFF_REGISTER_CUSTOMER  6
#define STAFF_VIEW_CUSTOMERS     7
#define STAFF_SEARCH_CUSTOMER    8
#define STAFF_VIEW_REQUESTS      9
#define STAFF_APPROVE_REQUEST    10
#define STAFF_RETURN_BOOK        11
#define STAFF_VIEW_OVERDUE       12
#define STAFF_STATISTICS         13
#define STAFF_CHANGE_PASSWORD    14
#define STAFF_SAVE_ALL           15
#define STAFF_REJECT_REQUEST     16
#define STAFF_LOGOUT             0

/* Customer menu options */
#define CUST_VIEW_ISSUED      1
#define CUST_VIEW_DUE         2
#define CUST_VIEW_RETURNED    3
#define CUST_SEARCH_BOOKS     4
#define CUST_REQUEST_BOOK     5
#define CUST_VIEW_REQUESTS    6
#define CUST_LOGOUT           0

/* ============================================================
 * Data Structures
 * ============================================================ */

struct Date {
    int day;
    int month;
    int year;
};

struct Book {
    int bookId;
    char title[MAX_STR];
    char author[MAX_STR];
    char category[MAX_STR];
    int totalCopies;
    int availableCopies;
    int isActive;
};

struct Customer {
    int customerId;
    char name[MAX_STR];
    char uniqueCode[CODE_LENGTH];
    int isActive;
};

struct IssueRecord {
    int issueId;
    int bookId;
    int customerId;
    struct Date issueDate;
    struct Date dueDate;
    struct Date returnDate;
    int status;
};

struct BookRequest {
    int requestId;
    int bookId;
    int customerId;
    struct Date requestDate;
    int status;
};

struct Staff {
    char username[MAX_STR];
    char password[MAX_STR];
    char fullName[MAX_STR];
    int isActive;
};

/* ============================================================
 * Global Data Declarations (defined in storage.c)
 * ============================================================ */
extern struct Book books[MAX_BOOKS];
extern int bookCount;

extern struct Customer customers[MAX_CUSTOMERS];
extern int customerCount;

extern struct IssueRecord issues[MAX_RECORDS];
extern int issueCount;

extern struct BookRequest requests[MAX_RECORDS];
extern int requestCount;

extern struct Staff staff[MAX_STAFF];
extern int staffCount;

extern int nextBookId;
extern int nextIssueId;
extern int nextRequestId;

/* ============================================================
 * Utility Function Prototypes
 * ============================================================ */

/* Input handling */
void clearInputBuffer(void);
void removeNewline(char *str);
int readInt(const char *prompt, int min, int max);
void readString(const char *prompt, char *buffer, int maxLen);
int isEmptyString(const char *str);

/* String utilities */
void toLowerCase(char *str);
int strCaseStr(const char *haystack, const char *needle);

/* Date utilities */
struct Date getCurrentDate(void);
struct Date addDays(struct Date date, int days);
int compareDates(struct Date d1, struct Date d2);
int isOverdue(struct Date dueDate);
void printDate(struct Date date);
int isValidDate(int day, int month, int year);
int daysInMonth(int month, int year);
int isLeapYear(int year);
int daysBetween(struct Date d1, struct Date d2);
int getOverdueDays(struct Date dueDate);

/* Display utilities */
void printHeader(const char *title);
void printSeparator(void);
void pauseScreen(void);
void printCentered(const char *text, int width);
void clearScreen(void);
void printBox(const char *title, const char *content[], int count);
void printMenu(const char *title, const char *options[], int count, int startNum);

/* Validation */
int isValidCode(const char *code);

/* Case-insensitive string comparison */
int strCaseCmp(const char *s1, const char *s2);

#endif /* UTILS_H */