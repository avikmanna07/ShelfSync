#include "auth.h"
#include "utils.h"
#include "storage.h"
#include "customer.h"

/* ============================================================
 * Staff Login
 * ============================================================ */

int staffLogin(void) {
    char username[MAX_STR];
    char password[MAX_STR];
    int attempts = 0;
    
    printHeader("S T A F F   L O G I N");
    
    while (attempts < LOGIN_ATTEMPTS) {
        printf("Attempt %d of %d\n", attempts + 1, LOGIN_ATTEMPTS);
        readString("Username: ", username, MAX_STR);
        readString("Password: ", password, MAX_STR);
        
        for (int i = 0; i < staffCount; i++) {
            if (staff[i].isActive &&
                strcmp(staff[i].username, username) == 0 &&
                strcmp(staff[i].password, password) == 0) {
                
                printf("\nLogin successful! Welcome, %s\n", staff[i].fullName);
                pauseScreen();
                return 1;
            }
        }
        
        attempts++;
        if (attempts < LOGIN_ATTEMPTS) {
            printf("\nInvalid credentials. %d attempts remaining.\n\n", LOGIN_ATTEMPTS - attempts);
        }
    }
    
    printf("\nMaximum login attempts exceeded. Returning to main menu.\n");
    pauseScreen();
    return 0;
}

/* ============================================================
 * Customer Login
 * ============================================================ */

int customerLogin(int *customerIndex) {
    char name[MAX_STR];
    char code[CODE_LENGTH];
    
    printHeader("C U S T O M E R   L O G I N");
    
    readString("Full Name: ", name, MAX_STR);
    readString("6-Digit Library Code: ", code, CODE_LENGTH);
    
    if (!isValidCode(code)) {
        printf("\nInvalid code format. Must be 6 digits.\n");
        pauseScreen();
        return 0;
    }
    
    int index = findCustomerByNameAndCode(name, code);
    if (index >= 0) {
        *customerIndex = index;
        printf("\nLogin successful! Welcome, %s\n", customers[index].name);
        pauseScreen();
        return 1;
    }
    
    printf("\nInvalid name or library code. Please try again.\n");
    pauseScreen();
    return 0;
}

/* ============================================================
 * Change Staff Password
 * ============================================================ */

void changeStaffPassword(void) {
    char currentPass[MAX_STR];
    char newPass[MAX_STR];
    char confirmPass[MAX_STR];
    
    printHeader("C H A N G E   P A S S W O R D");
    
    readString("Current Password: ", currentPass, MAX_STR);
    
    /* Verify current password against first active staff (admin) */
    int found = 0;
    for (int i = 0; i < staffCount; i++) {
        if (staff[i].isActive && strcmp(staff[i].password, currentPass) == 0) {
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("\nIncorrect current password.\n");
        pauseScreen();
        return;
    }
    
    readString("New Password: ", newPass, MAX_STR);
    readString("Confirm New Password: ", confirmPass, MAX_STR);
    
    if (strcmp(newPass, confirmPass) != 0) {
        printf("\nPasswords do not match.\n");
        pauseScreen();
        return;
    }
    
    if (strlen(newPass) < 4) {
        printf("\nPassword must be at least 4 characters.\n");
        pauseScreen();
        return;
    }
    
    /* Update password for all active staff (in this simple system) */
    for (int i = 0; i < staffCount; i++) {
        if (staff[i].isActive) {
            strcpy(staff[i].password, newPass);
        }
    }
    
    saveStaff();
    printf("\nPassword changed successfully.\n");
    pauseScreen();
}

/* ============================================================
 * Initialize Default Staff
 * ============================================================ */

void initDefaultStaff(void) {
    if (staffCount == 0) {
        strcpy(staff[0].username, "admin");
        strcpy(staff[0].password, "admin123");
        strcpy(staff[0].fullName, "System Administrator");
        staff[0].isActive = 1;
        staffCount = 1;
        saveStaff();
        printf("Default staff account created: admin / admin123\n");
    }
}