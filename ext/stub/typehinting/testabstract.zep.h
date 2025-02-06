
extern zend_class_entry *stub_typehinting_testabstract_ce;

ZEPHIR_INIT_CLASS(Stub_TypeHinting_TestAbstract);

PHP_METHOD(Stub_TypeHinting_TestAbstract, testFunc);
PHP_METHOD(Stub_TypeHinting_TestAbstract, returnOneOfScalar);
PHP_METHOD(Stub_TypeHinting_TestAbstract, returnInt);
PHP_METHOD(Stub_TypeHinting_TestAbstract, returnUint);
PHP_METHOD(Stub_TypeHinting_TestAbstract, returnLong);
PHP_METHOD(Stub_TypeHinting_TestAbstract, returnFloat);
PHP_METHOD(Stub_TypeHinting_TestAbstract, returnDouble);
PHP_METHOD(Stub_TypeHinting_TestAbstract, returnString);
PHP_METHOD(Stub_TypeHinting_TestAbstract, returnBoolean);
PHP_METHOD(Stub_TypeHinting_TestAbstract, returnChar);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typehinting_testabstract_testfunc, 0, 0, 0)
ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, text, IS_ARRAY, 0, "[]")
	ZEND_ARG_TYPE_INFO(0, text2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, flag, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, optional, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_typehinting_testabstract_returnoneofscalar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_typehinting_testabstract_returnint, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_typehinting_testabstract_returnuint, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_typehinting_testabstract_returnlong, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_typehinting_testabstract_returnfloat, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_typehinting_testabstract_returndouble, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_typehinting_testabstract_returnstring, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_typehinting_testabstract_returnboolean, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_typehinting_testabstract_returnchar, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_typehinting_testabstract_method_entry) {
	PHP_ME(Stub_TypeHinting_TestAbstract, testFunc, arginfo_stub_typehinting_testabstract_testfunc, ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
PHP_ME(Stub_TypeHinting_TestAbstract, returnOneOfScalar, arginfo_stub_typehinting_testabstract_returnoneofscalar, ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TypeHinting_TestAbstract, returnInt, arginfo_stub_typehinting_testabstract_returnint, ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TypeHinting_TestAbstract, returnUint, arginfo_stub_typehinting_testabstract_returnuint, ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TypeHinting_TestAbstract, returnLong, arginfo_stub_typehinting_testabstract_returnlong, ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TypeHinting_TestAbstract, returnFloat, arginfo_stub_typehinting_testabstract_returnfloat, ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TypeHinting_TestAbstract, returnDouble, arginfo_stub_typehinting_testabstract_returndouble, ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TypeHinting_TestAbstract, returnString, arginfo_stub_typehinting_testabstract_returnstring, ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TypeHinting_TestAbstract, returnBoolean, arginfo_stub_typehinting_testabstract_returnboolean, ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TypeHinting_TestAbstract, returnChar, arginfo_stub_typehinting_testabstract_returnchar, ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_FE_END
};
