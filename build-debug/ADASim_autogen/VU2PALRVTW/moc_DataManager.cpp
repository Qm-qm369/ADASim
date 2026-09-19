/****************************************************************************
** Meta object code from reading C++ file 'DataManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/backend/DataManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DataManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DataManager_t {
    QByteArrayData data[27];
    char stringdata0[401];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataManager_t qt_meta_stringdata_DataManager = {
    {
QT_MOC_LITERAL(0, 0, 11), // "DataManager"
QT_MOC_LITERAL(1, 12, 13), // "pathPredicted"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 16), // "QVector<QPointF>"
QT_MOC_LITERAL(4, 44, 19), // "predictedTrajectory"
QT_MOC_LITERAL(5, 64, 17), // "obstaclesDetected"
QT_MOC_LITERAL(6, 82, 17), // "obstaclePositions"
QT_MOC_LITERAL(7, 100, 21), // "mergedPointCloudReady"
QT_MOC_LITERAL(8, 122, 6), // "points"
QT_MOC_LITERAL(9, 129, 28), // "frontObstacleDistanceUpdated"
QT_MOC_LITERAL(10, 158, 8), // "distance"
QT_MOC_LITERAL(11, 167, 19), // "vehicleStateUpdated"
QT_MOC_LITERAL(12, 187, 1), // "x"
QT_MOC_LITERAL(13, 189, 1), // "y"
QT_MOC_LITERAL(14, 191, 3), // "yaw"
QT_MOC_LITERAL(15, 195, 16), // "plannerDataReady"
QT_MOC_LITERAL(16, 212, 4), // "data"
QT_MOC_LITERAL(17, 217, 22), // "lateralControlReceived"
QT_MOC_LITERAL(18, 240, 6), // "offset"
QT_MOC_LITERAL(19, 247, 19), // "plannerMessageError"
QT_MOC_LITERAL(20, 267, 7), // "message"
QT_MOC_LITERAL(21, 275, 20), // "onPointCloudReceived"
QT_MOC_LITERAL(22, 296, 18), // "updateVehicleState"
QT_MOC_LITERAL(23, 315, 19), // "onUserObstacleAdded"
QT_MOC_LITERAL(24, 335, 25), // "onVehiclePositionReceived"
QT_MOC_LITERAL(25, 361, 21), // "onPlannerDataReceived"
QT_MOC_LITERAL(26, 383, 17) // "onSimulationFrame"

    },
    "DataManager\0pathPredicted\0\0QVector<QPointF>\0"
    "predictedTrajectory\0obstaclesDetected\0"
    "obstaclePositions\0mergedPointCloudReady\0"
    "points\0frontObstacleDistanceUpdated\0"
    "distance\0vehicleStateUpdated\0x\0y\0yaw\0"
    "plannerDataReady\0data\0lateralControlReceived\0"
    "offset\0plannerMessageError\0message\0"
    "onPointCloudReceived\0updateVehicleState\0"
    "onUserObstacleAdded\0onVehiclePositionReceived\0"
    "onPlannerDataReceived\0onSimulationFrame"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       5,    1,   87,    2, 0x06 /* Public */,
       7,    1,   90,    2, 0x06 /* Public */,
       9,    1,   93,    2, 0x06 /* Public */,
      11,    3,   96,    2, 0x06 /* Public */,
      15,    1,  103,    2, 0x06 /* Public */,
      17,    1,  106,    2, 0x06 /* Public */,
      19,    1,  109,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      21,    1,  112,    2, 0x0a /* Public */,
      22,    3,  115,    2, 0x0a /* Public */,
      23,    2,  122,    2, 0x0a /* Public */,
      24,    3,  127,    2, 0x0a /* Public */,
      25,    1,  134,    2, 0x0a /* Public */,
      26,    4,  137,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    6,
    QMetaType::Void, 0x80000000 | 3,    8,
    QMetaType::Void, QMetaType::Double,   10,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double,   12,   13,   14,
    QMetaType::Void, QMetaType::QByteArray,   16,
    QMetaType::Void, QMetaType::Double,   18,
    QMetaType::Void, QMetaType::QString,   20,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    8,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double,   12,   13,   14,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   12,   13,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double,   12,   13,   14,
    QMetaType::Void, QMetaType::QByteArray,   16,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, 0x80000000 | 3,   12,   13,   14,    8,

       0        // eod
};

void DataManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DataManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->pathPredicted((*reinterpret_cast< const QVector<QPointF>(*)>(_a[1]))); break;
        case 1: _t->obstaclesDetected((*reinterpret_cast< const QVector<QPointF>(*)>(_a[1]))); break;
        case 2: _t->mergedPointCloudReady((*reinterpret_cast< const QVector<QPointF>(*)>(_a[1]))); break;
        case 3: _t->frontObstacleDistanceUpdated((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->vehicleStateUpdated((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 5: _t->plannerDataReady((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 6: _t->lateralControlReceived((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->plannerMessageError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->onPointCloudReceived((*reinterpret_cast< const QVector<QPointF>(*)>(_a[1]))); break;
        case 9: _t->updateVehicleState((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 10: _t->onUserObstacleAdded((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 11: _t->onVehiclePositionReceived((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 12: _t->onPlannerDataReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 13: _t->onSimulationFrame((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< const QVector<QPointF>(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QPointF> >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QPointF> >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QPointF> >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QPointF> >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 3:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QPointF> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DataManager::*)(const QVector<QPointF> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataManager::pathPredicted)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DataManager::*)(const QVector<QPointF> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataManager::obstaclesDetected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DataManager::*)(const QVector<QPointF> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataManager::mergedPointCloudReady)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DataManager::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataManager::frontObstacleDistanceUpdated)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (DataManager::*)(double , double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataManager::vehicleStateUpdated)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (DataManager::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataManager::plannerDataReady)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (DataManager::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataManager::lateralControlReceived)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (DataManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataManager::plannerMessageError)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DataManager::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_DataManager.data,
    qt_meta_data_DataManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DataManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DataManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DataManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void DataManager::pathPredicted(const QVector<QPointF> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DataManager::obstaclesDetected(const QVector<QPointF> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DataManager::mergedPointCloudReady(const QVector<QPointF> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DataManager::frontObstacleDistanceUpdated(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DataManager::vehicleStateUpdated(double _t1, double _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DataManager::plannerDataReady(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DataManager::lateralControlReceived(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void DataManager::plannerMessageError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
