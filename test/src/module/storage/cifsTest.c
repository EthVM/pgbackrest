/***********************************************************************************************************************************
Test CIFS Storage
***********************************************************************************************************************************/
#include "common/harnessConfig.h"

/***********************************************************************************************************************************
Test Run
***********************************************************************************************************************************/
void
testRun(void)
{
    FUNCTION_HARNESS_VOID();

    // *****************************************************************************************************************************
    if (testBegin("storageRepoGet() and StorageDriverCifs"))
    {
        // Load configuration
        StringList *argList = strLstNew();
        strLstAddZ(argList, "pgbackrest");
        strLstAddZ(argList, "--stanza=db");
        strLstAddZ(argList, "--repo1-type=cifs");
        strLstAdd(argList, strNewFmt("--repo1-path=%s", testPath()));
        strLstAddZ(argList, "archive-get");
        harnessCfgLoad(strLstSize(argList), strLstPtr(argList));

        const Storage *storage = NULL;
        TEST_ASSIGN(storage, storageRepoGet(strNew(STORAGE_TYPE_CIFS), true), "get cifs repo storage");
        TEST_RESULT_STR(strPtr(storage->type), "cifs", "check storage type");
        TEST_RESULT_BOOL(storageFeature(storage, storageFeaturePath), true, "    check path feature");
        TEST_RESULT_BOOL(storageFeature(storage, storageFeatureCompress), true, "    check compress feature");

        // Create a FileWrite object with path sync enabled and ensure that path sync is false in the write object
        // -------------------------------------------------------------------------------------------------------------------------
        StorageWrite *file = NULL;
        TEST_ASSIGN(file, storageNewWriteP(storage, strNew("somefile"), .noSyncPath = false), "new file write");

        TEST_RESULT_BOOL(storageWriteSyncPath(file), false, "path sync is disabled");

        // Test the path sync function -- pass a bogus path to ensure that this is a noop
        // -------------------------------------------------------------------------------------------------------------------------
        TEST_RESULT_VOID(storagePathSyncNP(storage, strNew(BOGUS_STR)), "path sync is a noop");
    }

    FUNCTION_HARNESS_RESULT_VOID();
}
