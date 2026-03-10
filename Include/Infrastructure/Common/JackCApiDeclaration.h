// 本文件定义了用于声明过期、废弃的接口类和接口函数的宏

#pragma once

#if defined(_MSC_VER)
#define JACKC_DEPRECATED(msg) __declspec(deprecated(msg))
#else
#define JACKC_DEPRECATED(msg) __attribute__((deprecated(msg)))
#endif

// 废弃的类，date 之后会被删除
#define JACKC_DEPRECATED_CLASS(date) JACKC_DEPRECATED("请不要使用这个类，它将在" #date "之后会被删除.")

// 废弃的接口，date 之后会被删除
#define JACKC_DEPRECATED_API(date) JACKC_DEPRECATED("请不要使用这个函数，它将在" #date "之后会被删除.")

// 过期的接口，date 之后会被删除，请使用 ... 中给出的新接口替换
#define JACKC_OBSOLETE_API(date, ...) JACKC_DEPRECATED("这个函数将在" #date "之后过期. 请使用 " #__VA_ARGS__ ".")
