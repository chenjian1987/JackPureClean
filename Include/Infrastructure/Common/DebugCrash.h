#pragma once
#include "CrashHelper.h"

#ifdef _WINDOWS
    #define DBG_CRASH(EXPRESSION, CRASHTIPS) \
    if(!(EXPRESSION)) \
    { \
        JackC::CrashHelper::GetInstance()->Crash(_CRT_WIDE(#EXPRESSION), _CRT_WIDE(__FILE__), __LINE__,  _CRT_WIDE(__FUNCTION__), CRASHTIPS); \
    }
#else
    #define DBG_CRASH(EXPRESSION, CRASHTIPS) \
    if(!(EXPRESSION)) \
    { \
        abort();\
    }
#endif
