/****************************************************************************
** Meta object code from reading C++ file 'Socket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/communication/Socket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Socket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SocketServer_t {
    QByteArrayData data[12];
    char stringdata0[155];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SocketServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SocketServer_t qt_meta_stringdata_SocketServer = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SocketServer"
QT_MOC_LITERAL(1, 13, 12), // "dataReceived"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 4), // "data"
QT_MOC_LITERAL(4, 32, 15), // "clientConnected"
QT_MOC_LITERAL(5, 48, 18), // "clientDisconnected"
QT_MOC_LITERAL(6, 67, 12), // "networkError"
QT_MOC_LITERAL(7, 80, 12), // "errorMessage"
QT_MOC_LITERAL(8, 93, 12), // "sendToClient"
QT_MOC_LITERAL(9, 106, 15), // "onNewConnection"
QT_MOC_LITERAL(10, 122, 11), // "onReadyRead"
QT_MOC_LITERAL(11, 134, 20) // "onClientDisconnected"

    },
    "SocketServer\0dataReceived\0\0data\0"
    "clientConnected\0clientDisconnected\0"
    "networkError\0errorMessage\0sendToClient\0"
    "onNewConnection\0onReadyRead\0"
    "onClientDisconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SocketServer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    0,   57,    2, 0x06 /* Public */,
       5,    0,   58,    2, 0x06 /* Public */,
       6,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   62,    2, 0x0a /* Public */,
       9,    0,   65,    2, 0x08 /* Private */,
      10,    0,   66,    2, 0x08 /* Private */,
      11,    0,   67,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SocketServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SocketServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dataReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->clientConnected(); break;
        case 2: _t->clientDisconnected(); break;
        case 3: _t->networkError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->sendToClient((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 5: _t->onNewConnection(); break;
        case 6: _t->onReadyRead(); break;
        case 7: _t->onClientDisconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SocketServer::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketServer::dataReceived)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SocketServer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketServer::clientConnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SocketServer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketServer::clientDisconnected)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SocketServer::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SocketServer::networkError)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SocketServer::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_SocketServer.data,
    qt_meta_data_SocketServer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SocketServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SocketServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SocketServer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SocketServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void SocketServer::dataReceived(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SocketServer::clientConnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SocketServer::clientDisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SocketServer::networkError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
