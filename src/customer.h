#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "utils.h"

/* ============================================================
 * Customer Management Function Prototypes
 * ============================================================ */

/* Generate a unique 6-digit library code */
int generateUniqueCode(char *code);

/* Register a new customer (staff function) */
int registerCustomer(void);

/* Find customer by unique code - returns index or -1 */
int findCustomerByCode(const char *code);

/* Find customer by name and code - returns index or -1 */
int findCustomerByNameAndCode(const char *name, const char *code);

/* Check if a library code is unique */
int isCodeUnique(const char *code);

/* Display all customers (staff function) */
void viewAllCustomers(void);

/* Search for a customer and display full profile (staff function) */
void searchCustomer(void);

/* Display customer profile with issued books, due dates, overdue, requests, history */
void displayCustomerProfile(int custIndex);

#endif /* CUSTOMER_H */