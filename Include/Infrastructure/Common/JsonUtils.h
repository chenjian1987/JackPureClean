#pragma once

#include "JackCDevService.h"

namespace Json
{
    class Value;
}

namespace JackC
{
    class JACKC_COMMON_EXPORT JsonUtils
    {
    public:
        static std::string SerializeToString(const Json::Value& jsonValue);
        static Json::Value UnserializeFromString(const std::string& dataString);
        static std::wstring ToWString(const Json::Value& jsonValue);
        static Json::Value ToJson(const std::wstring& dataString);
        static Json::Value UnserializeFromFile(const std::wstring& filePath); 

        static std::string SerializeToRawString(const Json::Value& jsonValue);
        static Json::Value UnserializeFromRawString(const std::string& dataString);

        static bool JsonDataSaveAsFile(Json::Value& jsonValue, std::string fileName);

        static Json::Value JsonDataFileReader(const std::string& fileName);
    };
}
