/****************************************************************************
** Meta object code from reading C++ file 'indexwnd.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../indexwnd.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'indexwnd.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN8IndexWndE_t {};
} // unnamed namespace

template <> constexpr inline auto IndexWnd::qt_create_metaobjectdata<qt_meta_tag_ZN8IndexWndE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "IndexWnd",
        "switchToCalendar",
        "",
        "switchToFocus",
        "switchToProfile",
        "updateTasks",
        "Task",
        "task",
        "updateProfileIcon",
        "pixmap",
        "onTaskCreated",
        "onTaskUpdated",
        "onTaskDeleted",
        "taskId"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'switchToCalendar'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'switchToFocus'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'switchToProfile'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'updateTasks'
        QtMocHelpers::SignalData<void(const Task &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Slot 'updateProfileIcon'
        QtMocHelpers::SlotData<void(const QPixmap &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QPixmap, 9 },
        }}),
        // Slot 'onTaskCreated'
        QtMocHelpers::SlotData<void(const Task &)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Slot 'onTaskUpdated'
        QtMocHelpers::SlotData<void(const Task &)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Slot 'onTaskDeleted'
        QtMocHelpers::SlotData<void(const int)>(12, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<IndexWnd, qt_meta_tag_ZN8IndexWndE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject IndexWnd::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8IndexWndE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8IndexWndE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8IndexWndE_t>.metaTypes,
    nullptr
} };

void IndexWnd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<IndexWnd *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->switchToCalendar(); break;
        case 1: _t->switchToFocus(); break;
        case 2: _t->switchToProfile(); break;
        case 3: _t->updateTasks((*reinterpret_cast< std::add_pointer_t<Task>>(_a[1]))); break;
        case 4: _t->updateProfileIcon((*reinterpret_cast< std::add_pointer_t<QPixmap>>(_a[1]))); break;
        case 5: _t->onTaskCreated((*reinterpret_cast< std::add_pointer_t<Task>>(_a[1]))); break;
        case 6: _t->onTaskUpdated((*reinterpret_cast< std::add_pointer_t<Task>>(_a[1]))); break;
        case 7: _t->onTaskDeleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (IndexWnd::*)()>(_a, &IndexWnd::switchToCalendar, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (IndexWnd::*)()>(_a, &IndexWnd::switchToFocus, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (IndexWnd::*)()>(_a, &IndexWnd::switchToProfile, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (IndexWnd::*)(const Task & )>(_a, &IndexWnd::updateTasks, 3))
            return;
    }
}

const QMetaObject *IndexWnd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IndexWnd::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8IndexWndE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int IndexWnd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void IndexWnd::switchToCalendar()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void IndexWnd::switchToFocus()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void IndexWnd::switchToProfile()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void IndexWnd::updateTasks(const Task & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP
