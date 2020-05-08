
extern zend_class_entry *stub_optimizers_substr_ce;

ZEPHIR_INIT_CLASS(Stub_Optimizers_Substr);

PHP_METHOD(Stub_Optimizers_Substr, testTwoArguments);
PHP_METHOD(Stub_Optimizers_Substr, testThreeArguments);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_substr_testtwoarguments, 0, 0, 2)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, start)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_optimizers_substr_testthreearguments, 0, 0, 3)
	ZEND_ARG_INFO(0, str)
	ZEND_ARG_INFO(0, start)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_optimizers_substr_method_entry) {
	PHP_ME(Stub_Optimizers_Substr, testTwoArguments, arginfo_stub_optimizers_substr_testtwoarguments, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Optimizers_Substr, testThreeArguments, arginfo_stub_optimizers_substr_testthreearguments, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
