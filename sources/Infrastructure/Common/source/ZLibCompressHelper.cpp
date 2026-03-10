#include "ZLibCompressHelper.h"
#include "zlib.h"
#include "JackCAssert.h"
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;

size_t ZLibCompressHelper::CompressBound(size_t srcLen) const
{
    JACKC_ASSERT(srcLen > 0);
    return (size_t)compressBound((uLong)srcLen);
}

EnCompressErrorCode ZLibCompressHelper::Compress(
    unsigned char * pDestBuf,
    size_t& destLen,
    const unsigned char * pSrcBuf,
    size_t srcLen) const
{
    // 暂时不提供可调参数level
    //return compress2(pDestBuf, (uLongf*)&destLen, pSrcBuf, (uLong)srcLen, level);
    const int errorCode = compress(pDestBuf, (uLongf*)&destLen, pSrcBuf, (uLong)srcLen);

    if (errorCode == Z_OK)
    {
        return CEC_OK;
    }
    else if (errorCode == Z_BUF_ERROR)
    {
        return CEC_OUTPUT_BUF_OVERFLOW;
    }
    else
    {
        return CEC_UNKNOWN_ERROR;
    }
}

EnCompressErrorCode ZLibCompressHelper::UnCompress(
    unsigned char * pDestBuf,
    size_t& destLen,
    const unsigned char * pSrcBuf,
    size_t srcLen) const
{
    const int errorCode = uncompress(pDestBuf, (uLongf*)&destLen, pSrcBuf, (uLong)srcLen);

    if (errorCode == Z_OK)
    {
        return CEC_OK;
    }
    else if (errorCode == Z_BUF_ERROR)
    {
        return CEC_OUTPUT_BUF_OVERFLOW;
    }
    else
    {
        return CEC_UNKNOWN_ERROR;
    }
}

EnCompressHelperType ZLibCompressHelper::GetType() const
{
    return CHT_ZLIB;
}
