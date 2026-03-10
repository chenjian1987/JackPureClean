#pragma once

#include "JackCDevService.h"

namespace JackC
{
    class IAppWindow;
    class IProgressDialogDefinition;

    class JACKC_COMMON_EXPORT IProgressDialogFactory
    {
    public:

        static IProgressDialogFactory* Get();

        virtual IProgressDialogDefinition* CreateProgressDialog(const std::wstring &labelText, int minimum, int maximum) = 0;

    protected:
        static IProgressDialogFactory* ms_pInstance;
    };
}