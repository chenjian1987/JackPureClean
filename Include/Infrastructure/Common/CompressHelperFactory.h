#pragma once

#include "JackCDevService.h"

namespace JackC
{
    class ICompressHelper;
    enum EnCompressHelperType;
    class JACKC_COMMON_EXPORT CompressHelperFactory
    {
    public:
        static OwnerPtr<ICompressHelper> CreateCompressHelper(EnCompressHelperType cht);
    };
}
