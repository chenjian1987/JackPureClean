#include "CompressHelper.h"
#include "CompressHelperFactory.h"
#include "ZLibCompressHelper.h"
#include "JackCNew.h"
#include "OwnerPtr.h"
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;

OwnerPtr<ICompressHelper> CompressHelperFactory::CreateCompressHelper(EnCompressHelperType cht)
{
    if (cht == CHT_ZLIB)
    {
        return NEW_AS_OWNER_PTR(ZLibCompressHelper);
    }
    DBG_WARN(L"Not implemented CompressHelper type", L"jack.chen", L"2025-07-19");
    return nullptr;
}
