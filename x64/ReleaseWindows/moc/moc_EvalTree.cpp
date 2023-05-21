/****************************************************************************
** Meta object code from reading C++ file 'EvalTree.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../UI/EvalTree.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EvalTree.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_EvalTree_t {
    uint offsetsAndSizes[30];
    char stringdata0[9];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[22];
    char stringdata4[6];
    char stringdata5[10];
    char stringdata6[6];
    char stringdata7[15];
    char stringdata8[13];
    char stringdata9[5];
    char stringdata10[13];
    char stringdata11[17];
    char stringdata12[5];
    char stringdata13[14];
    char stringdata14[13];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_EvalTree_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_EvalTree_t qt_meta_stringdata_EvalTree = {
    {
        QT_MOC_LITERAL(0, 8),  // "EvalTree"
        QT_MOC_LITERAL(9, 10),  // "BoardTiles"
        QT_MOC_LITERAL(20, 0),  // ""
        QT_MOC_LITERAL(21, 21),  // "std::vector<UI::Tile>"
        QT_MOC_LITERAL(43, 5),  // "tiles"
        QT_MOC_LITERAL(49, 9),  // "EvalScore"
        QT_MOC_LITERAL(59, 5),  // "score"
        QT_MOC_LITERAL(65, 14),  // "UpdateEvalTree"
        QT_MOC_LITERAL(80, 12),  // "UI::TileTree"
        QT_MOC_LITERAL(93, 4),  // "tree"
        QT_MOC_LITERAL(98, 12),  // "TreeExpanded"
        QT_MOC_LITERAL(111, 16),  // "QTreeWidgetItem*"
        QT_MOC_LITERAL(128, 4),  // "item"
        QT_MOC_LITERAL(133, 13),  // "TreeCollapsed"
        QT_MOC_LITERAL(147, 12)   // "NodeSelected"
    },
    "EvalTree",
    "BoardTiles",
    "",
    "std::vector<UI::Tile>",
    "tiles",
    "EvalScore",
    "score",
    "UpdateEvalTree",
    "UI::TileTree",
    "tree",
    "TreeExpanded",
    "QTreeWidgetItem*",
    "item",
    "TreeCollapsed",
    "NodeSelected"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_EvalTree[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x06,    1 /* Public */,
       5,    1,   53,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    1,   56,    2, 0x0a,    5 /* Public */,
      10,    1,   59,    2, 0x08,    7 /* Private */,
      13,    1,   62,    2, 0x08,    9 /* Private */,
      14,    1,   65,    2, 0x08,   11 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Float,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 11,   12,

       0        // eod
};

Q_CONSTINIT const QMetaObject EvalTree::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_EvalTree.offsetsAndSizes,
    qt_meta_data_EvalTree,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_EvalTree_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<EvalTree, std::true_type>,
        // method 'BoardTiles'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::vector<UI::Tile>, std::false_type>,
        // method 'EvalScore'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'UpdateEvalTree'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<UI::TileTree, std::false_type>,
        // method 'TreeExpanded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTreeWidgetItem *, std::false_type>,
        // method 'TreeCollapsed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTreeWidgetItem *, std::false_type>,
        // method 'NodeSelected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTreeWidgetItem *, std::false_type>
    >,
    nullptr
} };

void EvalTree::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<EvalTree *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->BoardTiles((*reinterpret_cast< std::add_pointer_t<std::vector<UI::Tile>>>(_a[1]))); break;
        case 1: _t->EvalScore((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 2: _t->UpdateEvalTree((*reinterpret_cast< std::add_pointer_t<UI::TileTree>>(_a[1]))); break;
        case 3: _t->TreeExpanded((*reinterpret_cast< std::add_pointer_t<QTreeWidgetItem*>>(_a[1]))); break;
        case 4: _t->TreeCollapsed((*reinterpret_cast< std::add_pointer_t<QTreeWidgetItem*>>(_a[1]))); break;
        case 5: _t->NodeSelected((*reinterpret_cast< std::add_pointer_t<QTreeWidgetItem*>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (EvalTree::*)(std::vector<UI::Tile> );
            if (_t _q_method = &EvalTree::BoardTiles; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (EvalTree::*)(float );
            if (_t _q_method = &EvalTree::EvalScore; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *EvalTree::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EvalTree::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EvalTree.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int EvalTree::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void EvalTree::BoardTiles(std::vector<UI::Tile> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EvalTree::EvalScore(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
