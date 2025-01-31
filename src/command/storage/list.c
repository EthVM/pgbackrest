/***********************************************************************************************************************************
Storage List Command
***********************************************************************************************************************************/
#include "build.auto.h"

#include <unistd.h>

#include "common/debug.h"
#include "common/io/handleWrite.h"
#include "common/memContext.h"
#include "common/log.h"
#include "common/memContext.h"
#include "common/type/json.h"
#include "common/type/string.h"
#include "config/config.h"
#include "storage/helper.h"

/***********************************************************************************************************************************
Render storage list
***********************************************************************************************************************************/
typedef struct StorageListRenderCallbackData
{
    IoWrite *write;                                                 // Where to write output
    bool json;                                                      // Is this json output?
    bool first;                                                     // Is this the first item?
} StorageListRenderCallbackData;

void
storageListRenderCallback(void *data, const StorageInfo *info)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM_P(VOID, data);
        FUNCTION_TEST_PARAM(STORAGE_INFO, info);
    FUNCTION_TEST_END();

    ASSERT(data != NULL);
    ASSERT(info != NULL);

    StorageListRenderCallbackData *listData = (StorageListRenderCallbackData *)data;

    // Skip . path when output is text
    if (info->type == storageTypePath && strEq(info->name, DOT_STR) && !listData->json)
    {
        FUNCTION_TEST_RETURN_VOID();
        return;
    }

    // Add seperator character
    if (!listData->first)
    {
        if (listData->json)
            ioWrite(listData->write, COMMA_BUF);
        else
            ioWrite(listData->write, LF_BUF);
    }
    else
        listData->first = false;

    // Render in json
    if (listData->json)
    {
        ioWriteStr(listData->write, jsonFromStr(info->name));
        ioWrite(listData->write, BUFSTRDEF(":{\"type\":\""));

        switch (info->type)
        {
            case storageTypeFile:
            {
                ioWrite(listData->write, BUFSTRDEF("file\""));
                break;
            }

            case storageTypeLink:
            {
                ioWrite(listData->write, BUFSTRDEF("link\""));
                break;
            }

            case storageTypePath:
            {
                ioWrite(listData->write, BUFSTRDEF("path\""));
                break;
            }

            case storageTypeSpecial:
            {
                ioWrite(listData->write, BUFSTRDEF("special\""));
                break;
            }
        }

        if (info->type == storageTypeFile)
            ioWriteStr(listData->write, strNewFmt(",\"size\":%" PRIu64, info->size));

        if (info->type == storageTypeLink)
            ioWriteStr(listData->write, strNewFmt(",\"destination\":%s", strPtr(jsonFromStr(info->linkDestination))));

        ioWrite(listData->write, BRACER_BUF);
    }
    // Render in text
    else
    {
        ioWrite(listData->write, BUFSTR(info->name));
    }

    FUNCTION_TEST_RETURN_VOID();
}

static void
storageListRender(IoWrite *write)
{
    FUNCTION_LOG_BEGIN(logLevelDebug)
        FUNCTION_LOG_PARAM(IO_WRITE, write);

    FUNCTION_LOG_END();

    // Get sort order
    SortOrder sortOrder = sortOrderAsc;

    if (strEqZ(cfgOptionStr(cfgOptSort), "desc"))
        sortOrder = sortOrderDesc;
    else if (!strEqZ(cfgOptionStr(cfgOptSort), "asc"))
    {
        ASSERT(strEqZ(cfgOptionStr(cfgOptSort), "none"));
        sortOrder = sortOrderNone;
    }

    // Get path
    const String *path = NULL;

    if (strLstSize(cfgCommandParam()) == 1)
        path = strLstGet(cfgCommandParam(), 0);
    else if (strLstSize(cfgCommandParam()) > 1)
        THROW(ParamInvalidError, "only one path may be specified");

    // Get output
    bool json = strEqZ(cfgOptionStr(cfgOptOutput), "json") ? true : false;

    // Render the info list
    StorageListRenderCallbackData data =
    {
        .write = write,
        .json = json,
        .first = true,
    };

    ioWriteOpen(data.write);

    if (data.json)
        ioWrite(data.write, BRACEL_BUF);

    storageInfoListP(
        storageRepo(), path, storageListRenderCallback, &data, .sortOrder = sortOrder, .expression = cfgOptionStr(cfgOptFilter),
        .recurse = cfgOptionBool(cfgOptRecurse));

    if (data.json)
        ioWrite(data.write, BRACER_BUF);

    ioWriteClose(data.write);

    FUNCTION_LOG_RETURN_VOID();
}

/***********************************************************************************************************************************
Render storage list and output to stdout
***********************************************************************************************************************************/
void
cmdStorageList(void)
{
    FUNCTION_LOG_VOID(logLevelDebug);

    MEM_CONTEXT_TEMP_BEGIN()
    {
        storageListRender(ioHandleWriteNew(STRDEF("stdout"), STDOUT_FILENO));
        ioHandleWriteOneStr(STDOUT_FILENO, LF_STR);
    }
    MEM_CONTEXT_TEMP_END();

    FUNCTION_LOG_RETURN_VOID();
}
