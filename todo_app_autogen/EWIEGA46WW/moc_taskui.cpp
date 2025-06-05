/****************************************************************************
** Meta object code from reading C++ file 'taskui.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../taskui.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'taskui.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN6TaskUIE_t {};
} // unnamed namespace

template <> constexpr inline auto TaskUI::qt_create_metaobjectdata<qt_meta_tag_ZN6TaskUIE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TaskUI",
        "taskClicked",
        "",
        "onUpdateTaskToComplete",
        "taskId",
        "completed",
        "taskCreatedInIndex",
        "name",
        "desc",
        "dueDate",
        "priority",
        "catName",
        "catColor",
        "catIco",
        "id",
        "taskCreatedInCalendar"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'taskClicked'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'onUpdateTaskToComplete'
        QtMocHelpers::SignalData<void(const int, bool)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 4 }, { QMetaType::Bool, 5 },
        }}),
        // Signal 'taskCreatedInIndex'
        QtMocHelpers::SignalData<void(const QString, const QString, const QString, const int, const QString, QColor, QIcon, const int, const int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 8 }, { QMetaType::QString, 9 }, { QMetaType::Int, 10 },
            { QMetaType::QString, 11 }, { QMetaType::QColor, 12 }, { QMetaType::QIcon, 13 }, { QMetaType::Int, 14 },
            { QMetaType::Int, 5 },
        }}),
        // Signal 'taskCreatedInCalendar'
        QtMocHelpers::SignalData<void(const QString, const QString, const QString, const int, const QString, QColor, QIcon, const int, const int)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 8 }, { QMetaType::QString, 9 }, { QMetaType::Int, 10 },
            { QMetaType::QString, 11 }, { QMetaType::QColor, 12 }, { QMetaType::QIcon, 13 }, { QMetaType::Int, 14 },
            { QMetaType::Int, 5 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TaskUI, qt_meta_tag_ZN6TaskUIE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TaskUI::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6TaskUIE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6TaskUIE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6TaskUIE_t>.metaTypes,
    nullptr
} };

void TaskUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TaskUI *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->taskClicked(); break;
        case 1: _t->onUpdateTaskToComplete((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 2: _t->taskCreatedInIndex((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QColor>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<QIcon>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[8])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[9]))); break;
        case 3: _t->taskCreatedInCalendar((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<QColor>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<QIcon>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[8])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[9]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TaskUI::*)()>(_a, &TaskUI::taskClicked, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (TaskUI::*)(const int , bool )>(_a, &TaskUI::onUpdateTaskToComplete, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (TaskUI::*)(const QString , const QString , const QString , const int , const QString , QColor , QIcon , const int , const int )>(_a, &TaskUI::taskCreatedInIndex, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (TaskUI::*)(const QString , const QString , const QString , const int , const QString , QColor , QIcon , const int , const int )>(_a, &TaskUI::taskCreatedInCalendar, 3))
            return;
    }
}

const QMetaObject *TaskUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TaskUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6TaskUIE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TaskUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void TaskUI::taskClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TaskUI::onUpdateTaskToComplete(const int _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void TaskUI::taskCreatedInIndex(const QString _t1, const QString _t2, const QString _t3, const int _t4, const QString _t5, QColor _t6, QIcon _t7, const int _t8, const int _t9)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9);
}

// SIGNAL 3
void TaskUI::taskCreatedInCalendar(const QString _t1, const QString _t2, const QString _t3, const int _t4, const QString _t5, QColor _t6, QIcon _t7, const int _t8, const int _t9)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9);
}
QT_WARNING_POP
