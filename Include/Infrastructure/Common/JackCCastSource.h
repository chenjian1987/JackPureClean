#pragma once

#include "DebugMessage.h"
#include "UniqueString.h"

#define IMPLEMENT_CAST_START(ClassFullName) \
    const char* ClassFullName::ClassType_() \
    { \
        static const char* s_classType = JackC::UniqueString::Find(#ClassFullName)->GetContent().c_str(); \
        return s_classType; \
    } \
    const char* ClassFullName::ClassType() \
    { \
        static const char* s_classType = ClassType_(); \
        return s_classType; \
    } \
    void* ClassFullName::QueryObject(const char* classType) const \
    { \
        if(!classType) \
            return nullptr; \
        if(ClassType() == classType) \
            return const_cast<ClassFullName*>(this); 

#define IMPLEMENT_CAST_BASE(BaseClassFullName) \
        if(void* pRes = BaseClassFullName::QueryObject(classType)) \
            return pRes;

#define IMPLEMENT_CAST_END() \
        return nullptr; \
    }