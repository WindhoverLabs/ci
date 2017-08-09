
#ifndef CI_TEST_UTILS_H
#define CI_TEST_UTILS_H

/*
 * Includes
 */

#include "ci_app.h"

extern CI_AppData_t  CI_AppData;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Function Definitions
 */

void CI_Test_Setup(void);
void CI_Test_TearDown(void);


#ifdef __cplusplus
}
#endif

#endif /* CI_TEST_UTILS_H */

