#ifndef REQUEST_H
#define REQUEST_H

#include "utils.h"

/* ============================================================
 * Book Request Function Prototypes
 * ============================================================ */

/* Customer requests a book */
void requestBook(int customerId);

/* Customer views their own requests */
void viewMyRequests(int customerId);

/* Staff views all pending requests */
void viewAllRequests(void);

/* Staff approves a pending request */
void approveRequest(void);

/* Staff rejects a pending request */
void rejectRequest(void);

/* Find request by ID - returns index or -1 */
int findRequestById(int requestId);

/* Check if customer already has a pending request for this book */
int hasPendingRequest(int customerId, int bookId);

/* Display request with book and customer names */
void displayRequest(const struct BookRequest *req);

/* Get request status as string */
const char* getRequestStatusString(int status);

#endif /* REQUEST_H */