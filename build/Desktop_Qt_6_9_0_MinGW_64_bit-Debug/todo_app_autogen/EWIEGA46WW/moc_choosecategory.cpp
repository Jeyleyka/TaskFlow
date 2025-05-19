/****************************************************************************
** Meta object code from reading C++ file 'choosecategory.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../choosecategory.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'choosecategory.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN14ChooseCategoryE_t {};
} // unnamed namespace

template <> constexpr inline auto ChooseCategory::qt_create_metaobjectdata<qt_meta_tag_ZN14ChooseCategoryE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ChooseCategory",
        "categorySelected",
        "",
        "name",
        "color",
        "icon",
        "AddCategorySignal",
        "addCategory",
        "addCategoryToUI",
        "onAddCategory"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'categorySelected'
        QtMocHelpers::SignalData<void(const QString &, const QColor &, const QIcon &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QColor, 4 }, { QMetaType::QIcon, 5 },
        }}),
        // Signal 'AddCategorySignal'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'addCategory'
        QtMocHelpers::SlotData<void(const QString &, const QColor &, const QIcon &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QColor, 4 }, { QMetaType::QIcon, 5 },
        }}),
        // Slot 'addCategoryToUI'
        QtMocHelpers::SlotData<void(const QString &, const QColor &, const QIcon &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QColor, 4 }, { QMetaType::QIcon, 5 },
        }}),
        // Slot 'onAddCategory'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ChooseCategory, qt_meta_tag_ZN14ChooseCategoryE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ChooseCategory::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ChooseCategoryE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ChooseCategoryE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14ChooseCategoryE_t>.metaTypes,
    nullptr
} };

void ChooseCategory::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ChooseCategory *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->categorySelected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QColor>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QIcon>>(_a[3]))); break;
        case 1: _t->AddCategorySignal(); break;
        case 2: _t->addCategory((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QColor>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QIcon>>(_a[3]))); break;
        case 3: _t->addCategoryToUI((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QColor>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QIcon>>(_a[3]))); break;
        case 4: _t->onAddCategory(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ChooseCategory::*)(const QString & , const QColor & , const QIcon & )>(_a, &ChooseCategory::categorySelected, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ChooseCategory::*)()>(_a, &ChooseCategory::AddCategorySignal, 1))
            return;
    }
}

const QMetaObject *ChooseCategory::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChooseCategory::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ChooseCategoryE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ChooseCategory::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ChooseCategory::categorySelected(const QString & _t1, const QColor & _t2, const QIcon & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3);
}

// SIGNAL 1
void ChooseCategory::AddCategorySignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
