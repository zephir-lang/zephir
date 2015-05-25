
extern zend_class_entry *test_oo_ooconstructparams_ce;

ZEPHIR_INIT_CLASS(Test_Oo_OoConstructParams);

PHP_METHOD(Test_Oo_OoConstructParams, __construct);
PHP_METHOD(Test_Oo_OoConstructParams, getA);
PHP_METHOD(Test_Oo_OoConstructParams, getB);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_ooconstructparams___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_oo_ooconstructparams_method_entry) {
	PHP_ME(Test_Oo_OoConstructParams, __construct, arginfo_test_oo_ooconstructparams___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Test_Oo_OoConstructParams, getA, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_OoConstructParams, getB, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
