
extern zend_class_entry *stub_oo_ce;

ZEPHIR_INIT_CLASS(Stub_Oo);

PHP_METHOD(Stub_Oo, testInstance1);
PHP_METHOD(Stub_Oo, testInstance2);
PHP_METHOD(Stub_Oo, testInstance3);
PHP_METHOD(Stub_Oo, testInstance4);
PHP_METHOD(Stub_Oo, testInstance5);
PHP_METHOD(Stub_Oo, testInstance6);
PHP_METHOD(Stub_Oo, testInstance7);
PHP_METHOD(Stub_Oo, testInstance8);
PHP_METHOD(Stub_Oo, testInstance9);
PHP_METHOD(Stub_Oo, testInstance10);
PHP_METHOD(Stub_Oo, testInstance11);
PHP_METHOD(Stub_Oo, testInstance12);
PHP_METHOD(Stub_Oo, createInstancesInLoop);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_createinstancesinloop, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_createinstancesinloop, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_method_entry) {
	PHP_ME(Stub_Oo, testInstance1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance11, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance12, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, createInstancesInLoop, arginfo_stub_oo_createinstancesinloop, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
