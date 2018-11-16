
extern zend_class_entry *test_oo_ce;

ZEPHIR_INIT_CLASS(Test_Oo);

PHP_METHOD(Test_Oo, testInstance1);
PHP_METHOD(Test_Oo, testInstance2);
PHP_METHOD(Test_Oo, testInstance3);
PHP_METHOD(Test_Oo, testInstance4);
PHP_METHOD(Test_Oo, testInstance5);
PHP_METHOD(Test_Oo, testInstance6);
PHP_METHOD(Test_Oo, testInstance7);
PHP_METHOD(Test_Oo, testInstance8);
PHP_METHOD(Test_Oo, testInstance9);
PHP_METHOD(Test_Oo, testInstance10);
PHP_METHOD(Test_Oo, testInstance11);
PHP_METHOD(Test_Oo, testInstance12);
PHP_METHOD(Test_Oo, createInstancesInLoop);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_oo_createinstancesinloop, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_oo_createinstancesinloop, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_oo_method_entry) {
	PHP_ME(Test_Oo, testInstance1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance11, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance12, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, createInstancesInLoop, arginfo_test_oo_createinstancesinloop, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
