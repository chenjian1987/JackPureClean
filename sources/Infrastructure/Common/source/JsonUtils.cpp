#include "JsonUtils.h"
#include "StringUtils.h"
#include "DebugMessage.h"
#include "base64.h"
#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"
#include <iostream>
#include <fstream>
#include "FileSystem.h"

using namespace JackC;

std::string JsonUtils::SerializeToString(const Json::Value& jsonValue)
{
    Json::FastWriter writer;
    std::string ret = writer.write(jsonValue);
    return base64_encode((const unsigned char*)ret.data(), (unsigned int)ret.size());
}

Json::Value JsonUtils::UnserializeFromString(const std::string& dataStr)
{
    std::string jsonStr = base64_decode(dataStr);
    Json::Value ret;
    Json::Reader reader;
    DBG_WARN_AND_RETURN_UNLESS(reader.parse(jsonStr, ret, false), Json::Value(), L"json parse error", L"jack.chen", L"2025-07-19");
    return ret;
}

std::string JsonUtils::SerializeToRawString(const Json::Value& jsonValue)
{
    Json::FastWriter writer;
    std::string ret = writer.write(jsonValue);
    return ret;
}

Json::Value JsonUtils::UnserializeFromRawString(const std::string& dataStr)
{
    Json::Value ret;
    Json::Reader reader;
    DBG_WARN_AND_RETURN_UNLESS(
        reader.parse(dataStr, ret, false), 
        Json::Value(), 
        L"json parse error", 
        L"jack.chen", 
        L"2025-07-19");
    return ret;
}

Json::Value JsonUtils::UnserializeFromFile(const std::wstring& filePath)
{
    Json::Reader reader;
    Json::Value root;
    std::ifstream ifile;
    if (FileSystem::OpenStream(ifile, filePath))
    {
        DBG_WARN_AND_RETURN_UNLESS(
            reader.parse(ifile, root),
            Json::Value(), 
            L"json parse error",
            L"jack.chen", 
            L"2025-07-19");
        return root;
    }
    return Json::Value();
}

std::wstring JsonUtils::ToWString(const Json::Value& jsonValue)
{
    if (jsonValue.isString())
    {
        return StringUtils::ToWString(jsonValue.asString());
    }

    return L"";
}

Json::Value JsonUtils::ToJson(const std::wstring& wstr)
{
    std::string tempStr = StringUtils::ToString(wstr);
    return Json::Value(tempStr);
}


bool JsonUtils::JsonDataSaveAsFile(Json::Value& root, std::string fileName)
{
    Json::FastWriter writer;
    std::ofstream    os;
    os.open(fileName, std::ios::out | std::ios::trunc);
    DBG_WARN_AND_RETURN_FALSE_UNLESS(
        os.is_open(), L"Error: can not find or create the file!", L"jack.chen", L"2025-11-07");
    os << writer.write(root);
    os.close();
    return true;
}

Json::Value JsonUtils::JsonDataFileReader(const std::string& fileName)
{
    Json::Reader  reader;
    Json::Value   root;
    std::ifstream ifs(fileName);

    DBG_WARN_AND_RETURN_FALSE_UNLESS(
        ifs.is_open(), L"Error: can not find or open the file!", L"jack.chen", L"2025-11-07");
    DBG_WARN_AND_RETURN_FALSE_UNLESS(
        reader.parse(ifs, root, false), L"Error: can not parse the file!", L"jack.chen", L"2025-11-07");
    ifs.close();
    return root;
}
