
extern zend_class_entry *test_oo_dynamicprop_ce;

ZEPHIR_INIT_CLASS(Test_Oo_DynamicProp);

PHP_METHOD(Test_Oo_DynamicProp, setPropertyInt);
PHP_METHOD(Test_Oo_DynamicProp, setPropertyBoolTrue);
PHP_METHOD(Test_Oo_DynamicProp, setPropertyBoolFalse);
PHP_METHOD(Test_Oo_DynamicProp, setPropertyString);
PHP_METHOD(Test_Oo_DynamicProp, setPropertyChar);
PHP_METHOD(Test_Oo_DynamicProp, setPropertyUChar);
PHP_METHOD(Test_Oo_DynamicProp, setPropertyNull);
PHP_METHOD(Test_Oo_DynamicProp, setPropertyVariableInt);
PHP_METHOD(Test_Oo_DynamicProp, setPropertyVariableString);
PHP_METHOD(Test_Oo_DynamicProp, setPropertyVariableBoolTrue);
PHP_METHOD(Test_Oo_DynamicProp, setPropertyVariableBoolFalse);
PHP_METHOD(Test_Oo_DynamicProp, setExistingStringProperty);
PHP_METHOD(Test_Oo_DynamicProp, setExistingStringPropertyString);
PHP_METHOD(Test_Oo_DynamicProp, setNonExistingStringProperty);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_dynamicprop_setpropertyint, 0, 0, 1)
	ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_dynamicprop_setpropertybooltrue, 0, 0, 1)
	ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_dynamicprop_setpropertyboolfalse, 0, 0, 1)
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

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_dynamicprop_setpropertynull, 0, 0, 1)
	ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_dynamicprop_setpropertyvariableint, 0, 0, 1)
	ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_dynamicprop_setpropertyvariablestring, 0, 0, 1)
	ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_dynamicprop_setpropertyvariablebooltrue, 0, 0, 1)
	ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_dynamicprop_setpropertyvariableboolfalse, 0, 0, 1)
	ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_dynamicprop_setexistingstringpropertystring, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_oo_dynamicprop_method_entry) {
	PHP_ME(Test_Oo_DynamicProp, setPropertyInt, arginfo_test_oo_dynamicprop_setpropertyint, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setPropertyBoolTrue, arginfo_test_oo_dynamicprop_setpropertybooltrue, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setPropertyBoolFalse, arginfo_test_oo_dynamicprop_setpropertyboolfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setPropertyString, arginfo_test_oo_dynamicprop_setpropertystring, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setPropertyChar, arginfo_test_oo_dynamicprop_setpropertychar, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setPropertyUChar, arginfo_test_oo_dynamicprop_setpropertyuchar, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setPropertyNull, arginfo_test_oo_dynamicprop_setpropertynull, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setPropertyVariableInt, arginfo_test_oo_dynamicprop_setpropertyvariableint, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setPropertyVariableString, arginfo_test_oo_dynamicprop_setpropertyvariablestring, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setPropertyVariableBoolTrue, arginfo_test_oo_dynamicprop_setpropertyvariablebooltrue, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setPropertyVariableBoolFalse, arginfo_test_oo_dynamicprop_setpropertyvariableboolfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setExistingStringProperty, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setExistingStringPropertyString, arginfo_test_oo_dynamicprop_setexistingstringpropertystring, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DynamicProp, setNonExistingStringProperty, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
