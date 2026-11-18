#ifndef ISSUE_H
#define ISSUE_H

#include "utils.h"

/* ============================================================
 * Issue/Return Management Function Prototypes
 * ============================================================ */

/* Issue a book to a customer (called when staff approves request) */
void issueBook(int bookId, int customerId, int requestId);

/* Mark a book as returned (staff function) */
void returnBook(void);

/* View books currently issued to a customer */
void viewIssuedBooks(int customerId);

/* View customer's returned book history */
void viewReturnedBooks(int customerId);

/* View all overdue books across all customers (staff function) */
void viewOverdueBooks(void);

/* Update overdue status for all issued books */
void updateOverdueStatus(void);

/* Find active issue record for a book and customer */
int getActiveIssue(int bookId, int customerId);

/* Display issue record with book and customer names */
void displayIssueRecord(const struct IssueRecord *issue);

#endif /* ISSUE_H */