#pragma once

#include <stdint.h>
#include <unordered_set>

// 表示允许多个计算器
#define MULTIPLE_CALCULATOR_EXTENDEDID INT64_MIN

//关联更新数据的唯一标识F
struct RegenDataId
{
public:
    int64_t ObjectId;   //参与关联更新的对象的ElementId
    int64_t DataId;     //参与关联更新的数据的Id
    int64_t ExtendedId; //参与更新的扩展数据，暂时未设置

    RegenDataId() : ObjectId(-1), DataId(-1), ExtendedId(-1) {}

    RegenDataId(int64_t nObjectID, int64_t nDataID, int64_t nExtendedID=-1)
        : ObjectId(nObjectID), DataId(nDataID), ExtendedId(nExtendedID)
    {

    }

    bool operator==(const RegenDataId& dataId) const {return ObjectId == dataId.ObjectId && DataId == dataId.DataId && ExtendedId == dataId.ExtendedId;} 
    bool operator!=(const RegenDataId& dataId) const {return !(*this == dataId);}

    bool operator<(const RegenDataId& other) const 
    {
        if (ObjectId != other.ObjectId)
        {
            return ObjectId < other.ObjectId;
        }
            
        if (DataId != other.DataId)
        {
            return DataId < other.DataId;
        }
            
        return ExtendedId < other.ExtendedId;
    }
};

struct RegenDataIdHash 
{
    std::size_t operator()(const RegenDataId& key) const
    {
        return std::hash<int64_t>()((key.ExtendedId<<48) + (key.DataId<<32) + (key.ObjectId));
    }
};

