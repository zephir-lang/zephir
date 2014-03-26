
extern zend_class_entry *test_oo_dynamicprop_ce;

ZEPHIR_INIT_CLASS(Test_Oo_DynamicProp);

PHP_METHOD(Test_Oo_DynamicProp, setPropertyInt);
PHP_METHOD(Test_Oo_DynamicProp, setPropertyBool);
PHP_METHOD(Test_Oo_DynamicProp, setPropertyString);
PHP_METHOD(Test_Oo_DynamicProp, setPropertyChar);
PHP_METHOD(Test_Oo_DynamicProp, setPropertyUChar);
PHP_METHOD(Test_Oo_DynamicProp, getProperty);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_dynamicprop_setpropertyint, 0, 0, 1)
	ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_dynamicprop_setpropertybool, 0, 0, 1)
	ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_dynamicprop_setpropertystring, 0, 0, 1)
	ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_dynamicprop_setpropertychar, 0, 0, 1)
	ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_dynamicprop_setpropertyuchar, 0, 0, 1)
	ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_dynamicprop_getproperty, 0, 0, 1)
	ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_oo_dynamicprop_method_entry) {
	PHP_ME(Test_Oo_DynamicProp, setPropertyInt, arginfo_test_oo_dynamicprop_setpropertyint, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setPropertyBool, arginfo_test_oo_dynamicprop_setpropertybool, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setPropertyString, arginfo_test_oo_dynamicprop_setpropertystring, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setPropertyChar, arginfo_test_oo_dynamicprop_setpropertychar, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setPropertyUChar, arginfo_test_oo_dynamicprop_setpropertyuchar, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, getProperty, arginfo_test_oo_dynamicprop_getproperty, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
