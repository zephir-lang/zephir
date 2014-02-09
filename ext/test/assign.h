
extern zend_class_entry *test_assign_ce;

ZEPHIR_INIT_CLASS(Test_Assign);

PHP_METHOD(Test_Assign, getTestVar);
PHP_METHOD(Test_Assign, testAssign1);
PHP_METHOD(Test_Assign, testAssign2);
PHP_METHOD(Test_Assign, testAssign3);
PHP_METHOD(Test_Assign, testAssign4);
PHP_METHOD(Test_Assign, testAssign5);
PHP_METHOD(Test_Assign, testAssign6);
PHP_METHOD(Test_Assign, testAssign7);
PHP_METHOD(Test_Assign, testAssign8);
PHP_METHOD(Test_Assign, testAssign9);
PHP_METHOD(Test_Assign, testAssign10);
PHP_METHOD(Test_Assign, testAssign11);
PHP_METHOD(Test_Assign, testAssign12);
PHP_METHOD(Test_Assign, testAssign13);
PHP_METHOD(Test_Assign, testAssign14);
PHP_METHOD(Test_Assign, testAssign15);
PHP_METHOD(Test_Assign, testAssign16);
PHP_METHOD(Test_Assign, testAssign17);
PHP_METHOD(Test_Assign, testAssign18);
PHP_METHOD(Test_Assign, testAssign19);
PHP_METHOD(Test_Assign, testAssign20);
PHP_METHOD(Test_Assign, testAssign21);
PHP_METHOD(Test_Assign, testAssign22);
PHP_METHOD(Test_Assign, testAssign23);
PHP_METHOD(Test_Assign, testAssign24);
PHP_METHOD(Test_Assign, testAssign25);
PHP_METHOD(Test_Assign, testAssign26);
PHP_METHOD(Test_Assign, testAssign27);
PHP_METHOD(Test_Assign, testAssign28);
PHP_METHOD(Test_Assign, testAssign29);
PHP_METHOD(Test_Assign, testAssign30);
PHP_METHOD(Test_Assign, testAssign31);
PHP_METHOD(Test_Assign, testAssign32);
PHP_METHOD(Test_Assign, testAssign33);
PHP_METHOD(Test_Assign, testAssign34);
PHP_METHOD(Test_Assign, testAssign35);
PHP_METHOD(Test_Assign, testAssign36);
PHP_METHOD(Test_Assign, testPropertyIncr1);
PHP_METHOD(Test_Assign, testPropertyAddAssign1);
PHP_METHOD(Test_Assign, testPropertyAddAssign2);
PHP_METHOD(Test_Assign, testPropertyAssignValuePlus1);
PHP_METHOD(Test_Assign, testPropertyDecr);
PHP_METHOD(Test_Assign, testPropertySubAssign1);
PHP_METHOD(Test_Assign, testPropertySubAssign2);
PHP_METHOD(Test_Assign, testPropertyMulAssign1);
PHP_METHOD(Test_Assign, testPropertyMulAssign2);
PHP_METHOD(Test_Assign, testPropertyAssignStringConcat);
PHP_METHOD(Test_Assign, testArrayVarAssign1);
PHP_METHOD(Test_Assign, testArrayVarAssign2);
PHP_METHOD(Test_Assign, testGlobalVarAssign);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_testarrayvarassign1, 0, 0, 2)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_testarrayvarassign2, 0, 0, 2)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_assign_testglobalvarassign, 0, 0, 2)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_assign_method_entry) {
	PHP_ME(Test_Assign, getTestVar, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign11, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign12, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign13, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign14, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign15, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign16, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign17, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign18, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign19, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign20, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign21, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign22, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign23, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign24, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign25, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign26, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign27, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign28, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign29, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign30, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign31, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign32, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign33, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign34, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign35, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testAssign36, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyIncr1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyAddAssign1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyAddAssign2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyAssignValuePlus1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyDecr, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertySubAssign1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertySubAssign2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyMulAssign1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyMulAssign2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testPropertyAssignStringConcat, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testArrayVarAssign1, arginfo_test_assign_testarrayvarassign1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testArrayVarAssign2, arginfo_test_assign_testarrayvarassign2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Assign, testGlobalVarAssign, arginfo_test_assign_testglobalvarassign, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
