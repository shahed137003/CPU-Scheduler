/****************************************************************************
** Meta object code from reading C++ file 'GanttChart.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../GanttChart.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GanttChart.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10GanttChartE_t {};
} // unnamed namespace

template <> constexpr inline auto GanttChart::qt_create_metaobjectdata<qt_meta_tag_ZN10GanttChartE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GanttChart",
        "requestUpdateGantt",
        "",
        "std::queue<char>",
        "processes",
        "std::queue<std::vector<float>>",
        "timeSlots",
        "live",
        "handleUpdateGantt"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'requestUpdateGantt'
        QtMocHelpers::SignalData<void(const std::queue<char> &, const std::queue<std::vector<float>> &, bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { 0x80000000 | 5, 6 }, { QMetaType::Bool, 7 },
        }}),
        // Slot 'handleUpdateGantt'
        QtMocHelpers::SlotData<void(const std::queue<char> &, const std::queue<std::vector<float>> &, bool)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { 0x80000000 | 5, 6 }, { QMetaType::Bool, 7 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GanttChart, qt_meta_tag_ZN10GanttChartE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GanttChart::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10GanttChartE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10GanttChartE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10GanttChartE_t>.metaTypes,
    nullptr
} };

void GanttChart::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GanttChart *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->requestUpdateGantt((*reinterpret_cast< std::add_pointer_t<std::queue<char>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::queue<std::vector<float>>>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        case 1: _t->handleUpdateGantt((*reinterpret_cast< std::add_pointer_t<std::queue<char>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::queue<std::vector<float>>>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (GanttChart::*)(const std::queue<char> & , const std::queue<std::vector<float>> & , bool )>(_a, &GanttChart::requestUpdateGantt, 0))
            return;
    }
}

const QMetaObject *GanttChart::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GanttChart::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10GanttChartE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int GanttChart::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void GanttChart::requestUpdateGantt(const std::queue<char> & _t1, const std::queue<std::vector<float>> & _t2, bool _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3);
}
QT_WARNING_POP
