#ifndef BOOK_H
#define BOOK_H

#include "utils.h"

/* ============================================================
 * Book Management Function Prototypes
 * ============================================================ */

/* Add a new book (staff function) */
void addBook(void);

/* View all active books in formatted table */
void viewAllBooks(void);

/* Search books by ID, title, or author */
void searchBook(void);

/* Update book details (staff function) */
void updateBook(void);

/* Soft delete a book (staff function) */
void deleteBook(void);

/* Find book index by ID - returns index or -1 */
int findBookById(int bookId);

/* Check if book ID is unique */
int isBookIdUnique(int bookId);

/* Display single book details */
void displayBook(const struct Book *book);

/* Print table header for book listings */
void displayBookTableHeader(void);

/* Check if book has available copies */
int hasAvailableCopies(int bookId);

#endif /* BOOK_H */