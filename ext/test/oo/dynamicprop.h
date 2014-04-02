
extern zend_class_entry *test_oo_dynamicprop_ce;

ZEPHIR_INIT_CLASS(Test_Oo_DynamicProp);

PHP_METHOD(Test_Oo_DynamicProp, setProperty);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_dynamicprop_setproperty, 0, 0, 1)
	ZEND_ARG_INFO(0, property)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_oo_dynamicprop_method_entry) {
	PHP_ME(Test_Oo_DynamicProp, setProperty, arginfo_test_oo_dynamicprop_setproperty, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
