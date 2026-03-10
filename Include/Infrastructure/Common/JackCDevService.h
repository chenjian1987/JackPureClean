#pragma once

#include "JackCDefines.h"

#ifdef JACKC_COMMON_HOME
#define JACKC_COMMON_EXPORT DLL_EXPORT
#else
#define JACKC_COMMON_EXPORT DLL_IMPORT
#endif

#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <functional>
#include <unordered_set>
#include <iosfwd>
#include "OneTimeInitializer.h"
#include "OwnerPtr.h"
#include "JackCAssert.h"
