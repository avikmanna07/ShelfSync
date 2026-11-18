#ifndef AUTH_H
#define AUTH_H

#include "utils.h"

/* ============================================================
 * Authentication Function Prototypes
 * ============================================================ */

/* Staff login - returns 1 on success, 0 on failure */
int staffLogin(void);

/* Customer login - returns 1 on success, 0 on failure.
 * On success, sets customerIndex to the index in customers array. */
int customerLogin(int *customerIndex);

/* Change staff password */
void changeStaffPassword(void);

/* Initialize default staff account if none exists */
void initDefaultStaff(void);

#endif /* AUTH_H */