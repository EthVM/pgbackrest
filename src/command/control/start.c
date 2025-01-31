/***********************************************************************************************************************************
Start Command
***********************************************************************************************************************************/
#include "build.auto.h"

#include "command/control/common.h"
#include "common/debug.h"
#include "config/config.h"
#include "storage/helper.h"
#include "storage/storage.h"

void
cmdStart(void)
{
    FUNCTION_LOG_VOID(logLevelDebug);

    MEM_CONTEXT_TEMP_BEGIN()
    {
        // Remove the stop file so processes can run
        String *stopFile = lockStopFileName(cfgOptionStr(cfgOptStanza));

        // If the stop file exists, then remove it
        if (storageExistsNP(storageLocal(), stopFile))
        {
            // If the file cannot be removed, storageRemove() will throw an error if the error is not ENOENT
            storageRemoveNP(storageLocalWrite(), stopFile);
        }
        else
        {
            LOG_WARN("stop file does not exist%s",
                (cfgOptionTest(cfgOptStanza) ? strPtr(strNewFmt(" for stanza %s", strPtr(cfgOptionStr(cfgOptStanza)))) : ""));
        }
    }
    MEM_CONTEXT_TEMP_END();

    FUNCTION_LOG_RETURN_VOID();
}
