
extern zend_class_entry *stub_oo_oodestruct_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_OoDestruct);

PHP_METHOD(Stub_Oo_OoDestruct, getHeight);
PHP_METHOD(Stub_Oo_OoDestruct, getImage);
PHP_METHOD(Stub_Oo_OoDestruct, getMime);
PHP_METHOD(Stub_Oo_OoDestruct, getRealpath);
PHP_METHOD(Stub_Oo_OoDestruct, getType);
PHP_METHOD(Stub_Oo_OoDestruct, getWidth);
PHP_METHOD(Stub_Oo_OoDestruct, __construct);
PHP_METHOD(Stub_Oo_OoDestruct, __destruct);
PHP_METHOD(Stub_Oo_OoDestruct, check);
PHP_METHOD(Stub_Oo_OoDestruct, getVersion);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oodestruct_getheight, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oodestruct_getheight, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oodestruct_getmime, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oodestruct_getmime, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oodestruct_gettype, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oodestruct_gettype, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oodestruct_getwidth, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oodestruct_getwidth, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_oodestruct___construct, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, file, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, file)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 1)
#else
	ZEND_ARG_INFO(0, width)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 1)
#else
	ZEND_ARG_INFO(0, height)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oodestruct_check, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oodestruct_check, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oodestruct_getversion, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_oodestruct_getversion, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_oodestruct_method_entry) {
	PHP_ME(Stub_Oo_OoDestruct, getHeight, arginfo_stub_oo_oodestruct_getheight, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoDestruct, getImage, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoDestruct, getMime, arginfo_stub_oo_oodestruct_getmime, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoDestruct, getRealpath, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoDestruct, getType, arginfo_stub_oo_oodestruct_gettype, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoDestruct, getWidth, arginfo_stub_oo_oodestruct_getwidth, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoDestruct, __construct, arginfo_stub_oo_oodestruct___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Oo_OoDestruct, __destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	PHP_ME(Stub_Oo_OoDestruct, check, arginfo_stub_oo_oodestruct_check, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Oo_OoDestruct, getVersion, arginfo_stub_oo_oodestruct_getversion, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
