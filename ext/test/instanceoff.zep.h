
extern zend_class_entry *test_instanceoff_ce;

ZEPHIR_INIT_CLASS(Test_Instanceoff);

PHP_METHOD(Test_Instanceoff, testInstanceOf1);
PHP_METHOD(Test_Instanceoff, testInstanceOf2);
PHP_METHOD(Test_Instanceoff, testInstanceOf3);
PHP_METHOD(Test_Instanceoff, testInstanceOf4);
PHP_METHOD(Test_Instanceoff, testInstanceOf5);
PHP_METHOD(Test_Instanceoff, testInstanceOf6);
PHP_METHOD(Test_Instanceoff, testInstanceOf7);
PHP_METHOD(Test_Instanceoff, testInstanceOf8);
PHP_METHOD(Test_Instanceoff, testInstanceOf9);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof1, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof1, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof2, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof2, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof3, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof3, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof4, 0, 1, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof4, 0, 1, _IS_BOOL, NULL, 0)
#endif
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof5, 0, 1, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof5, 0, 1, _IS_BOOL, NULL, 0)
#endif
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof6, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof6, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof7, 0, 1, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof7, 0, 1, _IS_BOOL, NULL, 0)
#endif
	ZEND_ARG_INFO(0, test)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof8, 0, 1, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof8, 0, 1, _IS_BOOL, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, test, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, test)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof9, 0, 2, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_instanceoff_testinstanceof9, 0, 2, _IS_BOOL, NULL, 0)
#endif
	ZEND_ARG_INFO(0, a)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, test, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, test)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_instanceoff_method_entry) {
	PHP_ME(Test_Instanceoff, testInstanceOf1, arginfo_test_instanceoff_testinstanceof1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Instanceoff, testInstanceOf2, arginfo_test_instanceoff_testinstanceof2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Instanceoff, testInstanceOf3, arginfo_test_instanceoff_testinstanceof3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Instanceoff, testInstanceOf4, arginfo_test_instanceoff_testinstanceof4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Instanceoff, testInstanceOf5, arginfo_test_instanceoff_testinstanceof5, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Instanceoff, testInstanceOf6, arginfo_test_instanceoff_testinstanceof6, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Instanceoff, testInstanceOf7, arginfo_test_instanceoff_testinstanceof7, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Instanceoff, testInstanceOf8, arginfo_test_instanceoff_testinstanceof8, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Instanceoff, testInstanceOf9, arginfo_test_instanceoff_testinstanceof9, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
