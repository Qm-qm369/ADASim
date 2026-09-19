/****************************************************************************
** Meta object code from reading C++ file 'View2D.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gui/View2D.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'View2D.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_View2D_t {
    QByteArrayData data[25];
    char stringdata0[300];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_View2D_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_View2D_t qt_meta_stringdata_View2D = {
    {
QT_MOC_LITERAL(0, 0, 6), // "View2D"
QT_MOC_LITERAL(1, 7, 17), // "userObstacleAdded"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 6), // "worldX"
QT_MOC_LITERAL(4, 33, 6), // "worldY"
QT_MOC_LITERAL(5, 40, 19), // "updatePredictedPath"
QT_MOC_LITERAL(6, 60, 16), // "QVector<QPointF>"
QT_MOC_LITERAL(7, 77, 4), // "path"
QT_MOC_LITERAL(8, 82, 15), // "updateObstacles"
QT_MOC_LITERAL(9, 98, 9), // "obstacles"
QT_MOC_LITERAL(10, 108, 21), // "updateVehiclePosition"
QT_MOC_LITERAL(11, 130, 1), // "x"
QT_MOC_LITERAL(12, 132, 1), // "y"
QT_MOC_LITERAL(13, 134, 3), // "yaw"
QT_MOC_LITERAL(14, 138, 14), // "drawTrajectory"
QT_MOC_LITERAL(15, 153, 9), // "QPainter&"
QT_MOC_LITERAL(16, 163, 7), // "painter"
QT_MOC_LITERAL(17, 171, 23), // "updatePlannedTrajectory"
QT_MOC_LITERAL(18, 195, 10), // "trajectory"
QT_MOC_LITERAL(19, 206, 19), // "updateTrackingDebug"
QT_MOC_LITERAL(20, 226, 11), // "targetPoint"
QT_MOC_LITERAL(21, 238, 12), // "lateralError"
QT_MOC_LITERAL(22, 251, 13), // "steeringAngle"
QT_MOC_LITERAL(23, 265, 18), // "clearTrackingDebug"
QT_MOC_LITERAL(24, 284, 15) // "showReplayFrame"

    },
    "View2D\0userObstacleAdded\0\0worldX\0"
    "worldY\0updatePredictedPath\0QVector<QPointF>\0"
    "path\0updateObstacles\0obstacles\0"
    "updateVehiclePosition\0x\0y\0yaw\0"
    "drawTrajectory\0QPainter&\0painter\0"
    "updatePlannedTrajectory\0trajectory\0"
    "updateTrackingDebug\0targetPoint\0"
    "lateralError\0steeringAngle\0"
    "clearTrackingDebug\0showReplayFrame"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_View2D[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   64,    2, 0x0a /* Public */,
       8,    1,   67,    2, 0x0a /* Public */,
      10,    3,   70,    2, 0x0a /* Public */,
      14,    1,   77,    2, 0x0a /* Public */,
      17,    1,   80,    2, 0x0a /* Public */,
      19,    3,   83,    2, 0x0a /* Public */,
      23,    0,   90,    2, 0x0a /* Public */,
      24,    3,   91,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 6,    9,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double,   11,   12,   13,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void, 0x80000000 | 6,   18,
    QMetaType::Void, QMetaType::QPointF, QMetaType::Double, QMetaType::Double,   20,   21,   22,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double,   11,   12,   13,

       0        // eod
};

void View2D::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<View2D *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->userObstacleAdded((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->updatePredictedPath((*reinterpret_cast< const QVector<QPointF>(*)>(_a[1]))); break;
        case 2: _t->updateObstacles((*reinterpret_cast< const QVector<QPointF>(*)>(_a[1]))); break;
        case 3: _t->updateVehiclePosition((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 4: _t->drawTrajectory((*reinterpret_cast< QPainter(*)>(_a[1]))); break;
        case 5: _t->updatePlannedTrajectory((*reinterpret_cast< const QVector<QPointF>(*)>(_a[1]))); break;
        case 6: _t->updateTrackingDebug((*reinterpret_cast< const QPointF(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 7: _t->clearTrackingDebug(); break;
        case 8: _t->showReplayFrame((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
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
        case 5:
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
            using _t = void (View2D::*)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&View2D::userObstacleAdded)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject View2D::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_View2D.data,
    qt_meta_data_View2D,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *View2D::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *View2D::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_View2D.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int View2D::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void View2D::userObstacleAdded(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
