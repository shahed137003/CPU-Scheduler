/****************************************************************************
** Meta object code from reading C++ file 'GanttChart.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../GanttChart.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GanttChart.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.6. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSGanttChartENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSGanttChartENDCLASS = QtMocHelpers::stringData(
    "GanttChart",
    "requestUpdateGantt",
    "",
    "std::queue<char>&",
    "processes",
    "std::queue<std::vector<float>>&",
    "timeSlots",
    "live",
    "handleUpdateGantt"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSGanttChartENDCLASS_t {
    uint offsetsAndSizes[18];
    char stringdata0[11];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[18];
    char stringdata4[10];
    char stringdata5[32];
    char stringdata6[10];
    char stringdata7[5];
    char stringdata8[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSGanttChartENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSGanttChartENDCLASS_t qt_meta_stringdata_CLASSGanttChartENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "GanttChart"
        QT_MOC_LITERAL(11, 18),  // "requestUpdateGantt"
        QT_MOC_LITERAL(30, 0),  // ""
        QT_MOC_LITERAL(31, 17),  // "std::queue<char>&"
        QT_MOC_LITERAL(49, 9),  // "processes"
        QT_MOC_LITERAL(59, 31),  // "std::queue<std::vector<float>>&"
        QT_MOC_LITERAL(91, 9),  // "timeSlots"
        QT_MOC_LITERAL(101, 4),  // "live"
        QT_MOC_LITERAL(106, 17)   // "handleUpdateGantt"
    },
    "GanttChart",
    "requestUpdateGantt",
    "",
    "std::queue<char>&",
    "processes",
    "std::queue<std::vector<float>>&",
    "timeSlots",
    "live",
    "handleUpdateGantt"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSGanttChartENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    3,   26,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    3,   33,    2, 0x08,    5 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5, QMetaType::Bool,    4,    6,    7,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5, QMetaType::Bool,    4,    6,    7,

       0        // eod
};

Q_CONSTINIT const QMetaObject GanttChart::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSGanttChartENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSGanttChartENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSGanttChartENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<GanttChart, std::true_type>,
        // method 'requestUpdateGantt'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::queue<char> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::queue<std::vector<float>> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'handleUpdateGantt'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::queue<char> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::queue<std::vector<float>> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void GanttChart::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GanttChart *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->requestUpdateGantt((*reinterpret_cast< std::add_pointer_t<std::queue<char>&>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::queue<std::vector<float>>&>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        case 1: _t->handleUpdateGantt((*reinterpret_cast< std::add_pointer_t<std::queue<char>&>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::queue<std::vector<float>>&>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GanttChart::*)(std::queue<char> & , std::queue<std::vector<float>> & , bool );
            if (_t _q_method = &GanttChart::requestUpdateGantt; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *GanttChart::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GanttChart::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSGanttChartENDCLASS.stringdata0))
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
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void GanttChart::requestUpdateGantt(std::queue<char> & _t1, std::queue<std::vector<float>> & _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
