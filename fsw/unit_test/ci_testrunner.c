
#include "uttest.h"

#include "ci_app_test.h"
#include "ci_cmds_test.h"
#include "ci_config_tbl_test.h"
#include "ci_cds_tbl_test.h"

int main(void)
{   
    CI_App_Test_AddTestCases();
    CI_Cmds_Test_AddTestCases();
    CI_Config_Tbl_Test_AddTestCases();
    CI_CDS_Tbl_Test_AddTestCases();

    return(UtTest_Run());

}

