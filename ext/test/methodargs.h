
extern zend_class_entry *test_methodargs_ce;

ZEPHIR_INIT_CLASS(Test_MethodArgs);

PHP_METHOD(Test_MethodArgs, setCallable);
PHP_METHOD(Test_MethodArgs, setObject);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_methodargs_setcallable, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_methodargs_setobject, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_methodargs_method_entry) {
	PHP_ME(Test_MethodArgs, setCallable, arginfo_test_methodargs_setcallable, ZEND_ACC_PUBLIC)
	PHP_ME(Test_MethodArgs, setObject, arginfo_test_methodargs_setobject, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
