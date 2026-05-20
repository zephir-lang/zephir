
extern zend_class_entry *stub_issue2167_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2167);

PHP_METHOD(Stub_Issue2167, __construct);
PHP_METHOD(Stub_Issue2167, reduceWithArrayCallable);
PHP_METHOD(Stub_Issue2167, reduceWithClosureWrapper);
PHP_METHOD(Stub_Issue2167, implodeRow);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2167___construct, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, data, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2167_reducewitharraycallable, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2167_reducewithclosurewrapper, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2167_imploderow, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, carry, IS_STRING, 0)
	ZEND_ARG_ARRAY_INFO(0, row, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2167_method_entry) {
	PHP_ME(Stub_Issue2167, __construct, arginfo_stub_issue2167___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Issue2167, reduceWithArrayCallable, arginfo_stub_issue2167_reducewitharraycallable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2167, reduceWithClosureWrapper, arginfo_stub_issue2167_reducewithclosurewrapper, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2167, implodeRow, arginfo_stub_issue2167_imploderow, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC)
	PHP_FE_END
};
