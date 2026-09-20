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
    QByteArrayData data[24];
    char stringdata0[362];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 20), // "startEngineRequested"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 19), // "stopEngineRequested"
QT_MOC_LITERAL(4, 53, 20), // "replayFrameRequested"
QT_MOC_LITERAL(5, 74, 5), // "index"
QT_MOC_LITERAL(6, 80, 22), // "controllerGainsChanged"
QT_MOC_LITERAL(7, 103, 11), // "headingGain"
QT_MOC_LITERAL(8, 115, 11), // "lateralGain"
QT_MOC_LITERAL(9, 127, 17), // "onStartSimulation"
QT_MOC_LITERAL(10, 145, 17), // "onPauseSimulation"
QT_MOC_LITERAL(11, 163, 16), // "onStopSimulation"
QT_MOC_LITERAL(12, 180, 14), // "onStatusUpdate"
QT_MOC_LITERAL(13, 195, 6), // "status"
QT_MOC_LITERAL(14, 202, 21), // "onReplayFrameSelected"
QT_MOC_LITERAL(15, 224, 24), // "onSimulationFrameUpdated"
QT_MOC_LITERAL(16, 249, 15), // "SimulationFrame"
QT_MOC_LITERAL(17, 265, 5), // "frame"
QT_MOC_LITERAL(18, 271, 12), // "currentIndex"
QT_MOC_LITERAL(19, 284, 13), // "targetSettled"
QT_MOC_LITERAL(20, 298, 18), // "onReplayFrameReady"
QT_MOC_LITERAL(21, 317, 8), // "maxIndex"
QT_MOC_LITERAL(22, 326, 22), // "onTerminationRequested"
QT_MOC_LITERAL(23, 349, 12) // "signalNumber"

    },
    "MainWindow\0startEngineRequested\0\0"
    "stopEngineRequested\0replayFrameRequested\0"
    "index\0controllerGainsChanged\0headingGain\0"
    "lateralGain\0onStartSimulation\0"
    "onPauseSimulation\0onStopSimulation\0"
    "onStatusUpdate\0status\0onReplayFrameSelected\0"
    "onSimulationFrameUpdated\0SimulationFrame\0"
    "frame\0currentIndex\0targetSettled\0"
    "onReplayFrameReady\0maxIndex\0"
    "onTerminationRequested\0signalNumber"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    0,   75,    2, 0x06 /* Public */,
       4,    1,   76,    2, 0x06 /* Public */,
       6,    2,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   84,    2, 0x08 /* Private */,
      10,    0,   85,    2, 0x08 /* Private */,
      11,    0,   86,    2, 0x08 /* Private */,
      12,    1,   87,    2, 0x08 /* Private */,
      14,    1,   90,    2, 0x08 /* Private */,
      15,    3,   93,    2, 0x08 /* Private */,
      20,    3,  100,    2, 0x08 /* Private */,
      22,    1,  107,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    7,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, 0x80000000 | 16, QMetaType::Int, QMetaType::Bool,   17,   18,   19,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, 0x80000000 | 16,    5,   21,   17,
    QMetaType::Void, QMetaType::Int,   23,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startEngineRequested(); break;
        case 1: _t->stopEngineRequested(); break;
        case 2: _t->replayFrameRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->controllerGainsChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 4: _t->onStartSimulation(); break;
        case 5: _t->onPauseSimulation(); break;
        case 6: _t->onStopSimulation(); break;
        case 7: _t->onStatusUpdate((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->onReplayFrameSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->onSimulationFrameUpdated((*reinterpret_cast< const SimulationFrame(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 10: _t->onReplayFrameReady((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const SimulationFrame(*)>(_a[3]))); break;
        case 11: _t->onTerminationRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::startEngineRequested)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::stopEngineRequested)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::replayFrameRequested)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::controllerGainsChanged)) {
                *result = 3;
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
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::startEngineRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MainWindow::stopEngineRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MainWindow::replayFrameRequested(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::controllerGainsChanged(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
