/****************************************************************************
** Meta object code from reading C++ file 'profilewnd.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../profilewnd.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'profilewnd.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10ProfileWndE_t {};
} // unnamed namespace

template <> constexpr inline auto ProfileWnd::qt_create_metaobjectdata<qt_meta_tag_ZN10ProfileWndE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ProfileWnd",
        "switchToIndex",
        "",
        "switchToCalendar",
        "switchToFocus",
        "onChangeImg",
        "pixmap",
        "onLogOut",
        "initTasksStatus",
        "updateTasksData"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'switchToIndex'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'switchToCalendar'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'switchToFocus'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'onChangeImg'
        QtMocHelpers::SignalData<void(QPixmap)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QPixmap, 6 },
        }}),
        // Signal 'onLogOut'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'initTasksStatus'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateTasksData'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ProfileWnd, qt_meta_tag_ZN10ProfileWndE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ProfileWnd::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ProfileWndE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ProfileWndE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10ProfileWndE_t>.metaTypes,
    nullptr
} };

void ProfileWnd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ProfileWnd *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->switchToIndex(); break;
        case 1: _t->switchToCalendar(); break;
        case 2: _t->switchToFocus(); break;
        case 3: _t->onChangeImg((*reinterpret_cast< std::add_pointer_t<QPixmap>>(_a[1]))); break;
        case 4: _t->onLogOut(); break;
        case 5: _t->initTasksStatus(); break;
        case 6: _t->updateTasksData(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ProfileWnd::*)()>(_a, &ProfileWnd::switchToIndex, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProfileWnd::*)()>(_a, &ProfileWnd::switchToCalendar, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProfileWnd::*)()>(_a, &ProfileWnd::switchToFocus, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProfileWnd::*)(QPixmap )>(_a, &ProfileWnd::onChangeImg, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ProfileWnd::*)()>(_a, &ProfileWnd::onLogOut, 4))
            return;
    }
}

const QMetaObject *ProfileWnd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProfileWnd::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ProfileWndE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ProfileWnd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void ProfileWnd::switchToIndex()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ProfileWnd::switchToCalendar()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ProfileWnd::switchToFocus()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ProfileWnd::onChangeImg(QPixmap _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void ProfileWnd::onLogOut()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
