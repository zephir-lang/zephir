
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

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_testinstance1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_testinstance2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_testinstance3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_testinstance4, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_testinstance5, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_testinstance6, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_testinstance7, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_testinstance8, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_testinstance9, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_testinstance10, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_testinstance11, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_testinstance12, 0, 0, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_createinstancesinloop, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_createinstancesinloop, 0, 0, IS_ARRAY, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_method_entry) {
	PHP_ME(Stub_Oo, testInstance1, arginfo_stub_oo_testinstance1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance2, arginfo_stub_oo_testinstance2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance3, arginfo_stub_oo_testinstance3, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance4, arginfo_stub_oo_testinstance4, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance5, arginfo_stub_oo_testinstance5, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance6, arginfo_stub_oo_testinstance6, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance7, arginfo_stub_oo_testinstance7, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance8, arginfo_stub_oo_testinstance8, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance9, arginfo_stub_oo_testinstance9, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance10, arginfo_stub_oo_testinstance10, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance11, arginfo_stub_oo_testinstance11, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, testInstance12, arginfo_stub_oo_testinstance12, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo, createInstancesInLoop, arginfo_stub_oo_createinstancesinloop, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
