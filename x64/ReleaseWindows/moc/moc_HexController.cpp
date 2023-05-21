/****************************************************************************
** Meta object code from reading C++ file 'HexController.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../UI/HexController.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HexController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_HexController_t {
    uint offsetsAndSizes[36];
    char stringdata0[14];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[22];
    char stringdata4[6];
    char stringdata5[11];
    char stringdata6[25];
    char stringdata7[6];
    char stringdata8[9];
    char stringdata9[13];
    char stringdata10[3];
    char stringdata11[12];
    char stringdata12[5];
    char stringdata13[17];
    char stringdata14[5];
    char stringdata15[5];
    char stringdata16[5];
    char stringdata17[5];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_HexController_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_HexController_t qt_meta_stringdata_HexController = {
    {
        QT_MOC_LITERAL(0, 13),  // "HexController"
        QT_MOC_LITERAL(14, 10),  // "BoardState"
        QT_MOC_LITERAL(25, 0),  // ""
        QT_MOC_LITERAL(26, 21),  // "std::vector<UI::Tile>"
        QT_MOC_LITERAL(48, 5),  // "tiles"
        QT_MOC_LITERAL(54, 10),  // "BoardMoves"
        QT_MOC_LITERAL(65, 24),  // "std::vector<std::string>"
        QT_MOC_LITERAL(90, 5),  // "moves"
        QT_MOC_LITERAL(96, 8),  // "EvalTree"
        QT_MOC_LITERAL(105, 12),  // "UI::TileTree"
        QT_MOC_LITERAL(118, 2),  // "tt"
        QT_MOC_LITERAL(121, 11),  // "TileClicked"
        QT_MOC_LITERAL(133, 4),  // "tile"
        QT_MOC_LITERAL(138, 16),  // "NewGameRequested"
        QT_MOC_LITERAL(155, 4),  // "size"
        QT_MOC_LITERAL(160, 4),  // "time"
        QT_MOC_LITERAL(165, 4),  // "P1AI"
        QT_MOC_LITERAL(170, 4)   // "P2AI"
    },
    "HexController",
    "BoardState",
    "",
    "std::vector<UI::Tile>",
    "tiles",
    "BoardMoves",
    "std::vector<std::string>",
    "moves",
    "EvalTree",
    "UI::TileTree",
    "tt",
    "TileClicked",
    "tile",
    "NewGameRequested",
    "size",
    "time",
    "P1AI",
    "P2AI"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_HexController[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   44,    2, 0x06,    1 /* Public */,
       5,    1,   47,    2, 0x06,    3 /* Public */,
       8,    1,   50,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      11,    1,   53,    2, 0x0a,    7 /* Public */,
      13,    4,   56,    2, 0x0a,    9 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::Bool, QMetaType::Bool,   14,   15,   16,   17,

       0        // eod
};

Q_CONSTINIT const QMetaObject HexController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_HexController.offsetsAndSizes,
    qt_meta_data_HexController,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_HexController_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<HexController, std::true_type>,
        // method 'BoardState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::vector<UI::Tile>, std::false_type>,
        // method 'BoardMoves'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::vector<std::string>, std::false_type>,
        // method 'EvalTree'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<UI::TileTree, std::false_type>,
        // method 'TileClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'NewGameRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void HexController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HexController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->BoardState((*reinterpret_cast< std::add_pointer_t<std::vector<UI::Tile>>>(_a[1]))); break;
        case 1: _t->BoardMoves((*reinterpret_cast< std::add_pointer_t<std::vector<std::string>>>(_a[1]))); break;
        case 2: _t->EvalTree((*reinterpret_cast< std::add_pointer_t<UI::TileTree>>(_a[1]))); break;
        case 3: _t->TileClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->NewGameRequested((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HexController::*)(std::vector<UI::Tile> );
            if (_t _q_method = &HexController::BoardState; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (HexController::*)(std::vector<std::string> );
            if (_t _q_method = &HexController::BoardMoves; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (HexController::*)(UI::TileTree );
            if (_t _q_method = &HexController::EvalTree; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *HexController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HexController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HexController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int HexController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void HexController::BoardState(std::vector<UI::Tile> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HexController::BoardMoves(std::vector<std::string> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HexController::EvalTree(UI::TileTree _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
