
extern zend_class_entry *test_arrayaccessobj_ce;

ZEPHIR_INIT_CLASS(Test_ArrayAccessObj);

PHP_METHOD(Test_ArrayAccessObj, __construct);
PHP_METHOD(Test_ArrayAccessObj, offsetSet);
PHP_METHOD(Test_ArrayAccessObj, offsetExists);
PHP_METHOD(Test_ArrayAccessObj, offsetUnset);
PHP_METHOD(Test_ArrayAccessObj, offsetGet);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_arrayaccessobj_offsetset, 0, 0, 2)
	ZEND_ARG_INFO(0, offset)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_arrayaccessobj_offsetexists, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_arrayaccessobj_offsetunset, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_arrayaccessobj_offsetget, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_arrayaccessobj_method_entry) {
	PHP_ME(Test_ArrayAccessObj, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Test_ArrayAccessObj, offsetSet, arginfo_test_arrayaccessobj_offsetset, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ArrayAccessObj, offsetExists, arginfo_test_arrayaccessobj_offsetexists, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ArrayAccessObj, offsetUnset, arginfo_test_arrayaccessobj_offsetunset, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ArrayAccessObj, offsetGet, arginfo_test_arrayaccessobj_offsetget, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
