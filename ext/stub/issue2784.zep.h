
extern zend_class_entry *stub_issue2784_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2784);

PHP_METHOD(Stub_Issue2784, readProt);
PHP_METHOD(Stub_Issue2784, readPriv);
PHP_METHOD(Stub_Issue2784, readUndefined);
PHP_METHOD(Stub_Issue2784, writeProt);
PHP_METHOD(Stub_Issue2784, unsetProt);
PHP_METHOD(Stub_Issue2784, unsetPriv);
PHP_METHOD(Stub_Issue2784, unsetNamed);
PHP_METHOD(Stub_Issue2784, unsetProtThenMark);
PHP_METHOD(Stub_Issue2784, unsetOffset);
PHP_METHOD(Stub_Issue2784, writeProtOffset);
PHP_METHOD(Stub_Issue2784, pushIntoProt);
PHP_METHOD(Stub_Issue2784, readMagicProtected);
PHP_METHOD(Stub_Issue2784, readMagicPrivate);
PHP_METHOD(Stub_Issue2784, readMagicUndeclared);
PHP_METHOD(Stub_Issue2784, issetPrivateOffset);
PHP_METHOD(Stub_Issue2784, emptyPropertyOfProtected);
PHP_METHOD(Stub_Issue2784, fetchPrivateOffset);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2784_readprot, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2784_readpriv, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2784_readundefined, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2784_writeprot, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2784_unsetprot, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2784_unsetpriv, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2784_unsetnamed, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2784_unsetprotthenmark, 0, 2, IS_VOID, 0)

	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, target)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2784_unsetoffset, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2784_writeprotoffset, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2784_pushintoprot, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2784_readmagicprotected, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, o, Stub\\Issue2784Magic, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2784_readmagicprivate, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, o, Stub\\Issue2784Magic, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2784_readmagicundeclared, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, o, Stub\\Issue2784Magic, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2784_issetprivateoffset, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, o, Stub\\Issue2784Base, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2784_emptypropertyofprotected, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, o, Stub\\Issue2784Base, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2784_fetchprivateoffset, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, o, Stub\\Issue2784Base, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2784_method_entry) {
	PHP_ME(Stub_Issue2784, readProt, arginfo_stub_issue2784_readprot, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2784, readPriv, arginfo_stub_issue2784_readpriv, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2784, readUndefined, arginfo_stub_issue2784_readundefined, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2784, writeProt, arginfo_stub_issue2784_writeprot, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2784, unsetProt, arginfo_stub_issue2784_unsetprot, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2784, unsetPriv, arginfo_stub_issue2784_unsetpriv, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2784, unsetNamed, arginfo_stub_issue2784_unsetnamed, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2784, unsetProtThenMark, arginfo_stub_issue2784_unsetprotthenmark, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2784, unsetOffset, arginfo_stub_issue2784_unsetoffset, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2784, writeProtOffset, arginfo_stub_issue2784_writeprotoffset, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2784, pushIntoProt, arginfo_stub_issue2784_pushintoprot, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2784, readMagicProtected, arginfo_stub_issue2784_readmagicprotected, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2784, readMagicPrivate, arginfo_stub_issue2784_readmagicprivate, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2784, readMagicUndeclared, arginfo_stub_issue2784_readmagicundeclared, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2784, issetPrivateOffset, arginfo_stub_issue2784_issetprivateoffset, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2784, emptyPropertyOfProtected, arginfo_stub_issue2784_emptypropertyofprotected, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2784, fetchPrivateOffset, arginfo_stub_issue2784_fetchprivateoffset, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
