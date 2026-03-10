#pragma once
#include <cstdint>

namespace JackC
{
    //日志文件中每一行日志的类型
    /// ！！注意！！ 若要添加新项，请同步添加对应字符串至s_logMgrLogTypeNames
    enum class LogTypeOfLogManager
    {
        INVALID = 0,            
        DEBUG_CRASH = 1,        
        DEBUG_ERROR = 2,        
        DEBUG_WARN = 3,         
        DEBUG_INFO = 4,         
        DIAGNOSE = 5,           
        FYI = 6,                
        JOURNAL = 7,            
        JOURNAL_WARN = 8,       
        JOURNAL_ERROR = 9,      
        CONSISTENCY = 10,       
        NUM_OF_TYPES = 11,      
    };
}
