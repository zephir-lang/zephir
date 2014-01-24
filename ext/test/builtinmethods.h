
extern zend_class_entry *test_builtinmethods_ce;

ZEPHIR_INIT_CLASS(Test_BuiltInMethods);

PHP_METHOD(Test_BuiltInMethods, stringMethodLength1);
PHP_METHOD(Test_BuiltInMethods, stringMethodLength2);
PHP_METHOD(Test_BuiltInMethods, stringMethodLength3);
PHP_METHOD(Test_BuiltInMethods, stringMethodLength4);
PHP_METHOD(Test_BuiltInMethods, stringMethodLength5);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_builtinmethods_stringmethodlength4, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_builtinmethods_stringmethodlength5, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_builtinmethods_method_entry) {
	PHP_ME(Test_BuiltInMethods, stringMethodLength1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltInMethods, stringMethodLength2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltInMethods, stringMethodLength3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltInMethods, stringMethodLength4, arginfo_test_builtinmethods_stringmethodlength4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_BuiltInMethods, stringMethodLength5, arginfo_test_builtinmethods_stringmethodlength5, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
