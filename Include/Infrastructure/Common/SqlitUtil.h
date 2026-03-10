#pragma once
#include <xstring>
#include "JackCDevService.h"

struct sqlite3;
namespace JackC
{
    class JACKC_COMMON_EXPORT SqliteUtil final
    {
    public:
        SqliteUtil();
        ~SqliteUtil();

    public:
        bool Open(const std::string &dbPath);
        bool Close();
        bool Create(const std::string &dbPath);
        bool CreateTable(const std::string &sqlCreatetable);
        bool IsTableExist(const std::string &sTableName, int& nRow);
        bool Insert(const std::string &sqlVal);
        bool Delete(const std::string &sqlVal);
        bool Update(const std::string &sqlVal);
        bool LimitRowNum(const std::string &sTableName, const std::string& sKeyName, int nMaxRow);
        bool LimitRowWithFixKeyNum(const std::string &sTableName, const std::string &sKey, const std::string& sFixKey, int nMaxRow);

    private:
        sqlite3    *m_db {nullptr};
    };
}