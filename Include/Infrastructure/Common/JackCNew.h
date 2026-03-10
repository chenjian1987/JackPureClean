#pragma once

#include "OwnerPtr.h"
#include "DebugMessage.h"

#define NEW_AS_OWNER_PTR(TYPE, ...) JackC::OwnerPtr<TYPE>(new TYPE(__VA_ARGS__))
#define NEW_AS_SHARED_PTR(TYPE, ...) std::shared_ptr<TYPE>(new TYPE(__VA_ARGS__))

// 只能用在不能使用OwnerPtr的地方
#define NEW_AS_UNIQUE_PTR(TYPE, ...) std::unique_ptr<TYPE>(new TYPE(__VA_ARGS__))

// QT only
#define NEW_AS_QT_CHILD(TYPE, ...) \
    ([=]()->TYPE*{ \
        static_assert(std::is_base_of<QObject, TYPE>::value, "only QT object is supported."); \
        TYPE* pQtChild = new TYPE(__VA_ARGS__); \
        DBG_WARN_AND_RETURN_NULLPTR_UNLESS(pQtChild->parent(), L"parent cannot be null", L"jack.chen", L"2025-07-19"); \
        return pQtChild; \
    }())

// QT only  parent可以为nullptr
#define NEW_AS_QT(TYPE, ...) \
    ([=]()->TYPE*{ \
        TYPE* pQtChild = new TYPE(__VA_ARGS__); \
        return pQtChild; \
    }())

// New Occ object
#define NEW_AS_OCC(TYPE, ...) \
    ([=]()->TYPE*{ \
        TYPE* pOCCObject = new TYPE(__VA_ARGS__); \
        return pOCCObject; \
    }())


// 非顶层QLayout对象创建的时候没有parent，后面需要调用QLayout::addLayout()或者QWidget::setLayout()添加parent
#define NEW_AS_QT_LAYOUT(TYPE, ...) \
    ([=]()->TYPE*{ \
        static_assert(std::is_base_of<QLayout, TYPE>::value, "only QLayout object is supported."); \
        TYPE* pQLayout = new TYPE(__VA_ARGS__); \
        return pQLayout; \
    }())

#define DELETE_QTREEWIDGET_CHILDREN(pTreeWidgetItem) \
    if(pTreeWidgetItem != nullptr) \
    { \
        QList<QTreeWidgetItem*> children = pTreeWidgetItem->takeChildren(); \
        int childCount = children.count(); \
        for (int childIndex = 0; childIndex < children.count(); childIndex++) \
        { \
            delete children[childIndex]; \
        } \
        children.clear(); \
    }
