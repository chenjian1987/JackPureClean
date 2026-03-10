#include "StlContainerUtils.h"
#include "ElementId.h"

using namespace JackC;

namespace JackC
{
    //判断vector中是否存在重复的ElementId
    bool StlContainerUtils::HasRepeatItem(const std::vector<ElementId>& container)
    {
        std::unordered_set<ElementId, ElementIdHash> containerSet;
        FOR_EACH(item, container)
        {
            bool insertSucceed = containerSet.insert(item).second;
            if (!insertSucceed)
            {
                return true;
            }
        }
        return false;
    }

    //去除vector中的重复的ElementId
    void StlContainerUtils::RemoveRepeatItem(std::vector<ElementId>& container)
    {
        std::sort(container.begin(), container.end());
        container.erase(std::unique(container.begin(), container.end()), container.end());
    }
}