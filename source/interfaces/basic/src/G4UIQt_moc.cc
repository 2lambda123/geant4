/****************************************************************************
** G4UIQt meta object code from reading C++ file 'G4UIQt.hh'
**
** Created: Tue Nov 13 18:13:09 2007
**      by: The Qt MOC ($Id: G4UIQt_moc.cc,v 1.6 2007-11-14 10:38:49 lgarnier Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#ifdef G4UI_BUILD_QT_SESSION

#undef QT_NO_COMPAT
#include "../include/G4UIQt.hh"
#include <qmetaobject.h>
#include <qapplication.h>

#if QT_VERSION < 0x040000

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *G4UIQt::className() const
{
    return "G4UIQt";
}

QMetaObject *G4UIQt::metaObj = 0;
static QMetaObjectCleanUp cleanUp_G4UIQt( "G4UIQt", &G4UIQt::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString G4UIQt::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "G4UIQt", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString G4UIQt::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "G4UIQt", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* G4UIQt::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"ClearButtonCallback", 0, 0 };
    static const QUMethod slot_1 = {"CommandEnteredCallback", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"ButtonCallback", 1, param_slot_2 };
    static const QUMethod slot_3 = {"HelpTreeClicCallback", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_ptr, "QTreeWidgetItem", QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"HelpTreeDoubleClicCallback", 2, param_slot_4 };
    static const QUMethod slot_5 = {"ShowHelpCallback", 0, 0 };
    static const QUMethod slot_6 = {"CommandHistoryCallback", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "ClearButtonCallback()", &slot_0, QMetaData::Private },
	{ "CommandEnteredCallback()", &slot_1, QMetaData::Private },
	{ "ButtonCallback(const QString&)", &slot_2, QMetaData::Private },
	{ "HelpTreeClicCallback()", &slot_3, QMetaData::Private },
	{ "HelpTreeDoubleClicCallback(QTreeWidgetItem*,int)", &slot_4, QMetaData::Private },
	{ "ShowHelpCallback()", &slot_5, QMetaData::Private },
	{ "CommandHistoryCallback()", &slot_6, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"myClicked", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "myClicked(const QString&)", &signal_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"G4UIQt", parentObject,
	slot_tbl, 7,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_G4UIQt.setMetaObject( metaObj );
    return metaObj;
}

void* G4UIQt::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "G4UIQt" ) )
	return this;
    if ( !qstrcmp( clname, "G4VBasicShell" ) )
	return (G4VBasicShell*)this;
    if ( !qstrcmp( clname, "G4VInteractiveSession" ) )
	return (G4VInteractiveSession*)this;
    return QObject::qt_cast( clname );
}

// SIGNAL myClicked
void G4UIQt::myClicked( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool G4UIQt::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: ClearButtonCallback(); break;
    case 1: CommandEnteredCallback(); break;
    case 2: ButtonCallback((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: HelpTreeClicCallback(); break;
#if QT_VERSION < 0x040000
    case 4: HelpTreeDoubleClicCallback((QListViewItem*)static_QUType_ptr.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
#else
    case 4: HelpTreeDoubleClicCallback((QTreeWidgetItem*)static_QUType_ptr.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
#endif
    case 5: ShowHelpCallback(); break;
    case 6: CommandHistoryCallback(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool G4UIQt::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: myClicked((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool G4UIQt::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool G4UIQt::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES

#endif
#endif


/****************************************************************************
** Meta object code from reading C++ file 'G4UIQt.hh'
**
** Created: Mon Oct 1 10:59:35 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if QT_VERSION >= 0x040000

#ifdef G4UI_BUILD_QT_SESSION

#include "../include/G4UIQt.hh"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'G4UIQt.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_G4UIQt[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
       7,   26,   31,   31, 0x05,

 // slots: signature, parameters, type, tag, flags
      32,   31,   31,   31, 0x08,
      54,   31,   31,   31, 0x08,
      79,   31,   31,   31, 0x08,
     103,   31,   31,   31, 0x08,
     126,  175,   31,   31, 0x08,
     177,   31,   31,   31, 0x08,
     196,   31,   31,   31, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_G4UIQt[] = {
    "G4UIQt\0myClicked(QString)\0text\0\0"
    "ClearButtonCallback()\0CommandEnteredCallback()\0"
    "ButtonCallback(QString)\0HelpTreeClicCallback()\0"
    "HelpTreeDoubleClicCallback(QTreeWidgetItem*,int)\0"
    ",\0ShowHelpCallback()\0CommandHistoryCallback()\0"
};

const QMetaObject G4UIQt::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_G4UIQt,
      qt_meta_data_G4UIQt, 0 }
};

const QMetaObject *G4UIQt::metaObject() const
{
    return &staticMetaObject;
}

void *G4UIQt::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_G4UIQt))
	return static_cast<void*>(const_cast<G4UIQt*>(this));
    if (!strcmp(_clname, "G4VBasicShell"))
	return static_cast<G4VBasicShell*>(const_cast<G4UIQt*>(this));
    if (!strcmp(_clname, "G4VInteractiveSession"))
	return static_cast<G4VInteractiveSession*>(const_cast<G4UIQt*>(this));
    return QObject::qt_metacast(_clname);
}

int G4UIQt::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: myClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: ClearButtonCallback(); break;
        case 2: CommandEnteredCallback(); break;
        case 3: ButtonCallback((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: HelpTreeClicCallback(); break;
        case 5: HelpTreeDoubleClicCallback((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: ShowHelpCallback(); break;
        case 7: CommandHistoryCallback(); break;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void G4UIQt::myClicked(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

#endif

#endif
