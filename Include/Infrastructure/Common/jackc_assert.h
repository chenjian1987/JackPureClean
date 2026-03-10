#pragma once

#include "JackCDefines.h"
#include <string>


#ifdef _WINDOWS
#include <crtdefs.h>

#undef  jackc_assert_impl

#ifdef  __cplusplus

extern "C" {
#endif

    _CRTIMP void __cdecl _wassert(_In_z_ const wchar_t* _Message, _In_z_ const wchar_t* _File, _In_ unsigned _Line);

#ifdef  __cplusplus
}
#endif

#define jackc_assert_impl(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )

#else 

#include <assert.h>
#define jackc_assert_impl(_Expression)  assert(_Expression)

#endif

