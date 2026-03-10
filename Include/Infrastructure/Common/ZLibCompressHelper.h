#pragma once

#include "CompressHelper.h"

namespace JackC
{
    class JACKC_COMMON_EXPORT ZLibCompressHelper : public ICompressHelper
    {
        // 实现ICompressHelper接口
    public:
        virtual size_t CompressBound(size_t srcLen) const override;

        virtual EnCompressErrorCode Compress(
            unsigned char * pDestBuf,
            size_t& destLen,
            const unsigned char * pSrcBuf,
            size_t srcLen) const override;

        virtual EnCompressErrorCode UnCompress(
            unsigned char * pDestBuf,
            size_t& destLen,
            const unsigned char * pSrcBuf,
            size_t srcLen) const override;

        virtual EnCompressHelperType GetType() const override;
    };
}
