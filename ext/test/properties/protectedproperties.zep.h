
extern zend_class_entry *test_properties_protectedproperties_ce;

ZEPHIR_INIT_CLASS(Test_Properties_ProtectedProperties);

PHP_METHOD(Test_Properties_ProtectedProperties, setSomeVar);
PHP_METHOD(Test_Properties_ProtectedProperties, getSomeVar);
PHP_METHOD(Test_Properties_ProtectedProperties, getSomeNull);
PHP_METHOD(Test_Properties_ProtectedProperties, setSomeNull);
PHP_METHOD(Test_Properties_ProtectedProperties, getSomeNullInitial);
PHP_METHOD(Test_Properties_ProtectedProperties, getSomeFalse);
PHP_METHOD(Test_Properties_ProtectedProperties, getSomeTrue);
PHP_METHOD(Test_Properties_ProtectedProperties, getSomeInteger);
PHP_METHOD(Test_Properties_ProtectedProperties, getSomeDouble);
PHP_METHOD(Test_Properties_ProtectedProperties, getSomeString);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_properties_protectedproperties_setsomevar, 0, 0, 1)
	ZEND_ARG_INFO(0, someVar)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_properties_protectedproperties_setsomenull, 0, 0, 1)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_properties_protectedproperties_method_entry) {
	PHP_ME(Test_Properties_ProtectedProperties, setSomeVar, arginfo_test_properties_protectedproperties_setsomevar, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_ProtectedProperties, getSomeVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_ProtectedProperties, getSomeNull, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_ProtectedProperties, setSomeNull, arginfo_test_properties_protectedproperties_setsomenull, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_ProtectedProperties, getSomeNullInitial, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_ProtectedProperties, getSomeFalse, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_ProtectedProperties, getSomeTrue, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_ProtectedProperties, getSomeInteger, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_ProtectedProperties, getSomeDouble, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Properties_ProtectedProperties, getSomeString, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
