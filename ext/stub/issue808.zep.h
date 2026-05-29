
extern zend_class_entry *stub_issue808_ce;

ZEPHIR_INIT_CLASS(Stub_Issue808);

PHP_METHOD(Stub_Issue808, testDynamicUnset);
PHP_METHOD(Stub_Issue808, testDynamicUnsetSelective);
PHP_METHOD(Stub_Issue808, testDynamicUnsetStringKey);
PHP_METHOD(Stub_Issue808, testDynamicUnsetNonExistentProperty);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue808_testdynamicunset, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, propertyName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue808_testdynamicunsetselective, 0, 0, 1)
	ZEND_ARG_INFO(0, toRemove)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue808_testdynamicunsetstringkey, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue808_testdynamicunsetnonexistentproperty, 0, 0, 1)
	ZEND_ARG_INFO(0, propertyName)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue808_method_entry) {
	PHP_ME(Stub_Issue808, testDynamicUnset, arginfo_stub_issue808_testdynamicunset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue808, testDynamicUnsetSelective, arginfo_stub_issue808_testdynamicunsetselective, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue808, testDynamicUnsetStringKey, arginfo_stub_issue808_testdynamicunsetstringkey, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue808, testDynamicUnsetNonExistentProperty, arginfo_stub_issue808_testdynamicunsetnonexistentproperty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
