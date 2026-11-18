#ifndef STORAGE_H
#define STORAGE_H

#include "utils.h"

/* ============================================================
 * Storage Function Prototypes
 * ============================================================ */

/* Initialize data directory */
void initDataDirectory(void);

/* Load all data at startup */
void loadAllData(void);

/* Save all data (used on exit) */
void saveAllData(void);

/* Individual save functions */
void saveBooks(void);
void saveCustomers(void);
void saveIssues(void);
void saveRequests(void);
void saveStaff(void);

/* Individual load functions */
void loadBooks(void);
void loadCustomers(void);
void loadIssues(void);
void loadRequests(void);
void loadStaff(void);

#endif /* STORAGE_H */