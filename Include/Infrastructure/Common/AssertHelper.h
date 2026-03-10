#pragma once

#include "JackCDevService.h"

#ifdef __cplusplus
#include "jackc_cassert.h"
#else
#include "jackc_assert.h"
#endif

#include <algorithm>
#include <functional>
#include <vector>



namespace JackC
{
    // Assert工具类
    class AssertHelper
    {
    private:
        AssertHelper() {}

        virtual ~AssertHelper() {}
    public:
        JACKC_COMMON_EXPORT static AssertHelper* GetInstance();

        JACKC_COMMON_EXPORT void RegisterAsPreShowListener(std::function<void()> listener);

        JACKC_COMMON_EXPORT void Assert(const std::wstring &text, const std::wstring &fileName, int line) const;

    private:
        std::vector<std::function<void()> > m_preShowListeners;
    };
}
