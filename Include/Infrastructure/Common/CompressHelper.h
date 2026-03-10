#pragma once

#include "JackCDevService.h"

namespace JackC
{
    enum EnCompressHelperType
    {
        CHT_LZMA = 0,   /*Compress/UnCompress algorithm used by 7Z for .7z format*/
        CHT_ZLIB = 1    /*Compress/UnCompress algorithm used by ZLib for .zip format*/
    };

    enum EnCompressErrorCode
    {
        CEC_OK                  = 0, /*No error*/
        CEC_OUTPUT_BUF_OVERFLOW = 1, /*Output buffer overflow*/
        CEC_UNKNOWN_ERROR            /*Unknown error*/
    };

    class JACKC_COMMON_EXPORT ICompressHelper
    {
    public:
        virtual ~ICompressHelper() {}

        /*!
        @brief      根据源数据的长度，获取压缩所需缓存的长度。大部分情况下，该函数返回最小的满足条件的大小，但也有可能缓存还是不够大。
                    最终需要根据Compress()的返回码判断，CEC_OUTPUT_BUF_OVERFLOW表明需要增大缓存。增加策略由调用者决定
        @param[in]  待压缩数据的字节数。要求输入值大于0
        @return     size_t  压缩需要的缓存大小
        */
        virtual size_t CompressBound(size_t srcLen) const = 0;

        /*!
        *@brief     压缩srcBuf中的数据。目前使用压缩器默认设置，暂不提供可调参数
        *param[out] destBuf     放置压缩后数据的缓存。缓存由调用者分配，释放，一般不会比srcLen大
        *param[out] destLen     缓存的长度，输入值由CompressBound()提供，返回压缩后实际数据的长度
        *param[in]  srcBuf      需要被压缩数据的缓存。缓存由调用者提供
        *param[in]  srcLen      需要被压缩的数据的长度
        *return     EnCompressErrorCode CEC_OK表示压缩成功
        */
        virtual EnCompressErrorCode Compress(
            unsigned char * pDestBuf,
            size_t& destLen,
            const unsigned char * pSrcBuf,
            size_t srcLen
        ) const = 0;

        /*!
        *@brief     解压srcBuf中的数据。目前使用压缩器默认设置，不提供可调参数
        *param[out] destBuf     放置解压后数据的缓存。缓存由调用者分配，释放，需要保证足够大，一般由调用者通过其它途径得知将要解压后的大小
        *param[out] destLen     返回解压后数据的长度
        *param[in]  srcBuf      需要被解压数据的缓存。缓存由调用者提供
        *param[in]  srcLen      需要被解压的数据的长度
        *return     EnCompressErrorCode CEC_OK表示压缩成功
        */
        virtual EnCompressErrorCode UnCompress(
            unsigned char * pDestBuf,
            size_t& destLen,
            const unsigned char * pSrcBuf,
            size_t srcLen
        ) const = 0;

        /*!
        @brief      返回此CompressHelper的类型
        @return     EnCompressHelperType
        */
        virtual EnCompressHelperType GetType() const = 0;
    };
}
