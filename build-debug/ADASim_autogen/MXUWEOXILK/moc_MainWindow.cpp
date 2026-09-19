/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[29];
    char stringdata0[412];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 20), // "simulationFrameReady"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 1), // "x"
QT_MOC_LITERAL(4, 35, 1), // "y"
QT_MOC_LITERAL(5, 37, 3), // "yaw"
QT_MOC_LITERAL(6, 41, 16), // "QVector<QPointF>"
QT_MOC_LITERAL(7, 58, 6), // "points"
QT_MOC_LITERAL(8, 65, 24), // "onLateralControlReceived"
QT_MOC_LITERAL(9, 90, 6), // "offset"
QT_MOC_LITERAL(10, 97, 17), // "onStartSimulation"
QT_MOC_LITERAL(11, 115, 17), // "onPauseSimulation"
QT_MOC_LITERAL(12, 133, 16), // "onStopSimulation"
QT_MOC_LITERAL(13, 150, 14), // "onStatusUpdate"
QT_MOC_LITERAL(14, 165, 6), // "status"
QT_MOC_LITERAL(15, 172, 16), // "startLateralPlan"
QT_MOC_LITERAL(16, 189, 12), // "targetOffset"
QT_MOC_LITERAL(17, 202, 22), // "calculatePlannedOffset"
QT_MOC_LITERAL(18, 225, 19), // "calculatePlannedYaw"
QT_MOC_LITERAL(19, 245, 14), // "normalizeAngle"
QT_MOC_LITERAL(20, 260, 5), // "angle"
QT_MOC_LITERAL(21, 266, 24), // "rebuildPlannedTrajectory"
QT_MOC_LITERAL(22, 291, 16), // "onSimulationTick"
QT_MOC_LITERAL(23, 308, 21), // "onReplayFrameSelected"
QT_MOC_LITERAL(24, 330, 5), // "index"
QT_MOC_LITERAL(25, 336, 30), // "onFrontObstacleDistanceUpdated"
QT_MOC_LITERAL(26, 367, 8), // "distance"
QT_MOC_LITERAL(27, 376, 22), // "onTerminationRequested"
QT_MOC_LITERAL(28, 399, 12) // "signalNumber"

    },
    "MainWindow\0simulationFrameReady\0\0x\0y\0"
    "yaw\0QVector<QPointF>\0points\0"
    "onLateralControlReceived\0offset\0"
    "onStartSimulation\0onPauseSimulation\0"
    "onStopSimulation\0onStatusUpdate\0status\0"
    "startLateralPlan\0targetOffset\0"
    "calculatePlannedOffset\0calculatePlannedYaw\0"
    "normalizeAngle\0angle\0rebuildPlannedTrajectory\0"
    "onSimulationTick\0onReplayFrameSelected\0"
    "index\0onFrontObstacleDistanceUpdated\0"
    "distance\0onTerminationRequested\0"
    "signalNumber"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   98,    2, 0x08 /* Private */,
      10,    0,  101,    2, 0x08 /* Private */,
      11,    0,  102,    2, 0x08 /* Private */,
      12,    0,  103,    2, 0x08 /* Private */,
      13,    1,  104,    2, 0x08 /* Private */,
      15,    1,  107,    2, 0x08 /* Private */,
      17,    1,  110,    2, 0x08 /* Private */,
      18,    1,  113,    2, 0x08 /* Private */,
      19,    1,  116,    2, 0x08 /* Private */,
      21,    0,  119,    2, 0x08 /* Private */,
      22,    1,  120,    2, 0x08 /* Private */,
      23,    1,  123,    2, 0x08 /* Private */,
      25,    1,  126,    2, 0x08 /* Private */,
      27,    1,  129,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, 0x80000000 | 6,    3,    4,    5,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::Double,   16,
    QMetaType::Double, QMetaType::Double,    3,
    QMetaType::Double, QMetaType::Double,    3,
    QMetaType::Double, QMetaType::Double,   20,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Int,   24,
    QMetaType::Void, QMetaType::Double,   26,
    QMetaType::Void, QMetaType::Int,   28,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->simulationFrameReady((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< const QVector<QPointF>(*)>(_a[4]))); break;
        case 1: _t->onLateralControlReceived((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->onStartSimulation(); break;
        case 3: _t->onPauseSimulation(); break;
        case 4: _t->onStopSimulation(); break;
        case 5: _t->onStatusUpdate((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->startLateralPlan((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: { double _r = _t->calculatePlannedOffset((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = std::move(_r); }  break;
        case 8: { double _r = _t->calculatePlannedYaw((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = std::move(_r); }  break;
        case 9: { double _r = _t->normalizeAngle((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = std::move(_r); }  break;
        case 10: _t->rebuildPlannedTrajectory(); break;
        case 11: _t->onSimulationTick((*reinterpret_cast< const QVector<QPointF>(*)>(_a[1]))); break;
        case 12: _t->onReplayFrameSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->onFrontObstacleDistanceUpdated((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 14: _t->onTerminationRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 3:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QPointF> >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QPointF> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(double , double , double , const QVector<QPointF> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::simulationFrameReady)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::simulationFrameReady(double _t1, double _t2, double _t3, const QVector<QPointF> & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
