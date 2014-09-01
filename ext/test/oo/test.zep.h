
extern zend_class_entry *test_oo_test_ce;

ZEPHIR_INIT_CLASS(Test_Oo_Test);

PHP_METHOD(Test_Oo_Test, setParam);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_test_setparam, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, param, "Param",0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_oo_test_method_entry) {
	PHP_ME(Test_Oo_Test, setParam, arginfo_test_oo_test_setparam, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
