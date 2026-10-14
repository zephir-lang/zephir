
extern zend_class_entry *stub_issue2391_staticaccessors_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2391_StaticAccessors);

PHP_METHOD(Stub_Issue2391_StaticAccessors, getPlain);
PHP_METHOD(Stub_Issue2391_StaticAccessors, setPlain);
PHP_METHOD(Stub_Issue2391_StaticAccessors, getCount);
PHP_METHOD(Stub_Issue2391_StaticAccessors, setCount);
PHP_METHOD(Stub_Issue2391_StaticAccessors, getFlag);
PHP_METHOD(Stub_Issue2391_StaticAccessors, __toString);
PHP_METHOD(Stub_Issue2391_StaticAccessors, plainValue);
PHP_METHOD(Stub_Issue2391_StaticAccessors, countValue);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_staticaccessors_getplain, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_staticaccessors_setplain, 0, 0, 1)
	ZEND_ARG_INFO(0, plain)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2391_staticaccessors_getcount, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_staticaccessors_setcount, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, count, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2391_staticaccessors_getflag, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2391_staticaccessors___tostring, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_staticaccessors_plainvalue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2391_staticaccessors_countvalue, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2391_staticaccessors_method_entry) {
PHP_ME(Stub_Issue2391_StaticAccessors, getPlain, arginfo_stub_issue2391_staticaccessors_getplain, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_StaticAccessors, setPlain, arginfo_stub_issue2391_staticaccessors_setplain, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_StaticAccessors, getCount, arginfo_stub_issue2391_staticaccessors_getcount, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_StaticAccessors, setCount, arginfo_stub_issue2391_staticaccessors_setcount, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_StaticAccessors, getFlag, arginfo_stub_issue2391_staticaccessors_getflag, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2391_StaticAccessors, __toString, arginfo_stub_issue2391_staticaccessors___tostring, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2391_StaticAccessors, plainValue, arginfo_stub_issue2391_staticaccessors_plainvalue, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
PHP_ME(Stub_Issue2391_StaticAccessors, countValue, arginfo_stub_issue2391_staticaccessors_countvalue, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
