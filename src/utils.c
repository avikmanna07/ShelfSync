#include "utils.h"

/* ============================================================
 * Global Data Definitions
 * ============================================================ */
struct Book books[MAX_BOOKS];
int bookCount = 0;

struct Customer customers[MAX_CUSTOMERS];
int customerCount = 0;

struct IssueRecord issues[MAX_RECORDS];
int issueCount = 0;

struct BookRequest requests[MAX_RECORDS];
int requestCount = 0;

struct Staff staff[MAX_STAFF];
int staffCount = 0;

int nextBookId = 1;
int nextIssueId = 1;
int nextRequestId = 1;

/* ============================================================
 * Input Handling
 * ============================================================ */

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void removeNewline(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int readInt(const char *prompt, int min, int max) {
    int value;
    char input[100];
    
    while (1) {
        if (prompt) printf("%s", prompt);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            /* EOF reached - return a default/min value to exit gracefully */
            printf("\n");
            return min;
        }
        
        removeNewline(input);
        
        if (isEmptyString(input)) {
            printf("Input cannot be empty. Please try again.\n");
            continue;
        }
        
        char *endptr;
        long val = strtol(input, &endptr, 10);
        
        if (endptr == input || *endptr != '\0') {
            printf("Invalid number. Please enter a valid integer.\n");
            continue;
        }
        
        if (val < min || val > max) {
            printf("Value must be between %d and %d. Please try again.\n", min, max);
            continue;
        }
        
        value = (int)val;
        break;
    }
    
    return value;
}

void readString(const char *prompt, char *buffer, int maxLen) {
    if (prompt) printf("%s", prompt);
    
    while (1) {
        if (fgets(buffer, maxLen, stdin) == NULL) {
            /* EOF reached - set empty string and return */
            buffer[0] = '\0';
            printf("\n");
            return;
        }
        
        removeNewline(buffer);
        
        if (isEmptyString(buffer)) {
            printf("Input cannot be empty. Please try again: ");
            continue;
        }
        
        break;
    }
}

int isEmptyString(const char *str) {
    if (str == NULL) return 1;
    while (*str) {
        if (!isspace((unsigned char)*str)) return 0;
        str++;
    }
    return 1;
}

/* ============================================================
 * String Utilities
 * ============================================================ */

void toLowerCase(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int strCaseStr(const char *haystack, const char *needle) {
    if (haystack == NULL || needle == NULL) return 0;
    if (*needle == '\0') return 1;
    
    size_t haystack_len = strlen(haystack);
    size_t needle_len = strlen(needle);
    
    if (needle_len > haystack_len) return 0;
    
    for (size_t i = 0; i <= haystack_len - needle_len; i++) {
        int match = 1;
        for (size_t j = 0; j < needle_len; j++) {
            if (tolower((unsigned char)haystack[i + j]) != tolower((unsigned char)needle[j])) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    
    return 0;
}

/* ============================================================
 * Date Utilities
 * ============================================================ */

struct Date getCurrentDate(void) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    struct Date date;
    date.day = tm->tm_mday;
    date.month = tm->tm_mon + 1;
    date.year = tm->tm_year + 1900;
    return date;
}

int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(int month, int year) {
    static const int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) return 29;
    return days[month];
}

int isValidDate(int day, int month, int year) {
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > daysInMonth(month, year)) return 0;
    return 1;
}

struct Date addDays(struct Date date, int days) {
    struct Date result = date;
    result.day += days;
    
    while (result.day > daysInMonth(result.month, result.year)) {
        result.day -= daysInMonth(result.month, result.year);
        result.month++;
        if (result.month > 12) {
            result.month = 1;
            result.year++;
        }
    }
    
    return result;
}

int compareDates(struct Date d1, struct Date d2) {
    if (d1.year != d2.year) return (d1.year < d2.year) ? -1 : 1;
    if (d1.month != d2.month) return (d1.month < d2.month) ? -1 : 1;
    if (d1.day != d2.day) return (d1.day < d2.day) ? -1 : 1;
    return 0;
}

int isOverdue(struct Date dueDate) {
    struct Date today = getCurrentDate();
    return compareDates(today, dueDate) > 0;
}

void printDate(struct Date date) {
    printf("%02d/%02d/%04d", date.day, date.month, date.year);
}

/* ============================================================
 * Display Utilities
 * ============================================================ */

void printHeader(const char *title) {
    int width = 60;
    int titleLen = strlen(title);
    int padding = (width - titleLen - 2) / 2;
    
    printf("\n");
    for (int i = 0; i < width; i++) printf("=");
    printf("\n");
    
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s", title);
    printf("\n");
    
    for (int i = 0; i < width; i++) printf("=");
    printf("\n\n");
}

void printSeparator(void) {
    for (int i = 0; i < 60; i++) printf("-");
    printf("\n");
}

void pauseScreen(void) {
    printf("\nPress Enter to continue...");
    clearInputBuffer();
}

void printCentered(const char *text, int width) {
    int len = strlen(text);
    int padding = (width - len) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s", text);
}

/* ============================================================
 * Validation
 * ============================================================ */

int isValidCode(const char *code) {
    if (code == NULL || strlen(code) != 6) return 0;
    for (int i = 0; i < 6; i++) {
        if (!isdigit((unsigned char)code[i])) return 0;
    }
    return 1;
}

/* Case-insensitive string comparison */
int strCaseCmp(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) return -1;
    
    while (*s1 && *s2) {
        char c1 = tolower((unsigned char)*s1);
        char c2 = tolower((unsigned char)*s2);
        if (c1 != c2) return c1 - c2;
        s1++;
        s2++;
    }
    
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

/* ============================================================
 * Enhanced UI Functions
 * ============================================================ */

void clearScreen(void) {
    /* ANSI escape code for clearing screen - works on modern Windows 10+, Linux, macOS */
    printf("\033[2J\033[H");
}

void printBox(const char *title, const char *content[], int count) {
    int width = 70;
    int titleLen = title ? (int)strlen(title) : 0;
    int innerWidth = width - 4;
    
    /* Top border */
    printf("\n");
    printf("  +");
    for (int i = 0; i < width - 2; i++) printf("-");
    printf("+\n");
    
    /* Title line */
    if (title && titleLen > 0) {
        int padding = (innerWidth - titleLen) / 2;
        printf("  |");
        for (int i = 0; i < padding; i++) printf(" ");
        printf("%s", title);
        for (int i = 0; i < innerWidth - padding - titleLen; i++) printf(" ");
        printf(" |\n");
        
        /* Separator under title */
        printf("  +");
        for (int i = 0; i < width - 2; i++) printf("-");
        printf("+\n");
    }
    
    /* Content lines */
    for (int i = 0; i < count; i++) {
        if (content[i]) {
            int len = (int)strlen(content[i]);
            printf("  | %s", content[i]);
            for (int j = 0; j < innerWidth - len - 1; j++) printf(" ");
            printf(" |\n");
        } else {
            printf("  |");
            for (int j = 0; j < innerWidth - 1; j++) printf(" ");
            printf(" |\n");
        }
    }
    
    /* Bottom border */
    printf("  +");
    for (int i = 0; i < width - 2; i++) printf("-");
    printf("+\n\n");
}

void printMenu(const char *title, const char *options[], int count, int startNum) {
    const int width = 70;
    int innerWidth = width - 4;
    
    /* Top border with title */
    printf("\n");
    printf("  +");
    for (int i = 0; i < width - 2; i++) printf("-");
    printf("+\n");
    
    if (title) {
        int titleLen = (int)strlen(title);
        int padding = (innerWidth - titleLen) / 2;
        printf("  |");
        for (int i = 0; i < padding; i++) printf(" ");
        printf("%s", title);
        for (int i = 0; i < innerWidth - padding - titleLen; i++) printf(" ");
        printf(" |\n");
        
        /* Separator */
        printf("  +");
        for (int i = 0; i < width - 2; i++) printf("-");
        printf("+\n");
    }
    
    /* Options */
    for (int i = 0; i < count; i++) {
        char line[100];
        snprintf(line, sizeof(line), "  %d. %s", startNum + i, options[i]);
        int len = (int)strlen(line);
        printf("  | %s", line);
        for (int j = 0; j < innerWidth - len; j++) printf(" ");
        printf(" |\n");
    }
    
    /* Bottom border */
    printf("  +");
    for (int i = 0; i < width - 2; i++) printf("-");
    printf("+\n");
}