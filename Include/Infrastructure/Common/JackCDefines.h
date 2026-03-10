#pragma once

// 用于表明一个类是不允许被拷贝的
#define CANNOT_COPY(Classname) \
    private: \
    Classname(const Classname&);

// 用于表明一个类是不允许被赋值的
#define CANNOT_ASSIGN(Classname) \
    private: \
    Classname& operator=(const Classname&);

// 用于表明一个类是不允许被拷贝或赋值的
#define CANNOT_COPY_OR_ASSIGN(Classname)  \
    private: \
    Classname(const Classname&); \
    Classname& operator=(const Classname&);

// 若指针不为空，则对其指定值
#define SAFE_INIT_POINTER(pointer, value) \
    if (pointer != nullptr) \
    { \
        *pointer = value; \
    }

// 指定父类名称为Super
#define DECLARE_SUPER(PARENT) private: typedef PARENT Super;

// 按索引 [0..size-1] 遍历
#define FOR_EACH_INDEX(INDEX, VECTOR) \
    for (int INDEX = 0; INDEX < (int)VECTOR.size(); ++INDEX)

// 按索引 [size-1..0] 遍历
#define FOR_EACH_INDEX_REVERSE(INDEX, VECTOR) \
    for (int INDEX = (int)VECTOR.size()-1; INDEX >= 0; --INDEX)

// 直接遍历容器元素
#ifdef  EMSCRIPTEN
#define FORCE_INLINE inline
#elif defined _MSC_VER
#define FORCE_INLINE __forceinline
#elif defined __linux__
#define FORCE_INLINE inline
#else
#error FORCE_INLINE not implemented
#endif

// 直接遍历容器元素
#if defined __cplusplus
#define FOR_EACH(ITEM, VECTOR) \
    for(auto& ITEM : VECTOR)
#else
#error FOR_EACH not implemented
#endif


// 直接遍历容器元素
#ifdef __linux__
void StaticAssertFalse(const char* file, int line, const char* comments);
#define STATIC_ASSERT_FALSE(COMMENTS) \
    StaticAssertFalse(__FILE__, __LINE__, COMMENTS)
#elif defined _MSC_VER
#define STATIC_ASSERT_FALSE(COMMENTS) \
    static_assert(false, COMMENTS);
#else
#error STATIC_ASSERT_FALSE not implemented
#endif

#ifdef _MSC_VER
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#else
#define DLL_EXPORT
#define DLL_IMPORT
#endif

#define  FINAL_INTERFACE //只提供内部实现的接口，不希望外部继承。

#ifdef _MSC_VER
#define WSTRING_OF(NAME) L#NAME
#elif defined __linux__
#define WSTRING_OF(NAME) L""#NAME
#else
#error WSTRING_OF not implemented
#endif

#ifdef __linux__
#define C_FUNC_EXPORT extern "C"
#else
#define C_FUNC_EXPORT extern "C" __declspec(dllexport)
#endif
