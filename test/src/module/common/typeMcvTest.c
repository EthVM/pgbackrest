/***********************************************************************************************************************************
Test Most Common Value
***********************************************************************************************************************************/

/***********************************************************************************************************************************
Test Run
***********************************************************************************************************************************/
void
testRun(void)
{
    FUNCTION_HARNESS_VOID();

    // *****************************************************************************************************************************
    if (testBegin("Mcv"))
    {
        // String MCV
        // -------------------------------------------------------------------------------------------------------------------------
        MostCommonValue *mcv = NULL;
        TEST_ASSIGN(mcv, mcvNew(), "new mcv");
        TEST_RESULT_PTR(mcvResult(mcv), NULL, "immediate result is null");

        TEST_RESULT_VOID(mcvUpdate(mcv, varNewStrZ("string1")), "update string1");
        TEST_RESULT_STR(strPtr(varStr(mcvResult(mcv))), "string1", "result is string1");

        TEST_RESULT_VOID(mcvUpdate(mcv, varNewStrZ("string2")), "update string2");
        TEST_RESULT_STR(strPtr(varStr(mcvResult(mcv))), "string1", "result is string1");

        TEST_RESULT_VOID(mcvUpdate(mcv, varNewUInt(555)), "update 555");
        TEST_RESULT_VOID(mcvUpdate(mcv, varNewStrZ("string2")), "update string2");
        TEST_RESULT_STR(strPtr(varStr(mcvResult(mcv))), "string2", "result is string2");

        TEST_RESULT_VOID(mcvFree(mcv), "free mcv");

        // UInt MCV
        // -------------------------------------------------------------------------------------------------------------------------
        TEST_ASSIGN(mcv, mcvNew(), "new mcv");
        TEST_RESULT_PTR(mcvResult(mcv), NULL, "immediate result is null");

        TEST_RESULT_VOID(mcvUpdate(mcv, NULL), "update null");
        TEST_RESULT_PTR(mcvResult(mcv), NULL, "result is null");

        TEST_RESULT_VOID(mcvUpdate(mcv, varNewUInt(555)), "update 555");
        TEST_RESULT_PTR(mcvResult(mcv), NULL, "result is null");

        TEST_RESULT_VOID(mcvUpdate(mcv, varNewUInt(555)), "update 555");
        TEST_RESULT_UINT(varUInt(mcvResult(mcv)), 555, "result is 555");
    }

    FUNCTION_HARNESS_RESULT_VOID();
}
