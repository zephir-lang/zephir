
extern zend_class_entry *test_requires_ce;

ZEPHIR_INIT_CLASS(Test_Requires);

PHP_METHOD(Test_Requires, requireExternal1);
PHP_METHOD(Test_Requires, requireExternal2);
PHP_METHOD(Test_Requires, requireExternal3);
PHP_METHOD(Test_Requires, setContent);
PHP_METHOD(Test_Requires, renderTemplate);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_requires_requireexternal1, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_requires_requireexternal2, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_requires_requireexternal3, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_requires_setcontent, 0, 0, 1)
	ZEND_ARG_INFO(0, content)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_requires_rendertemplate, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, templatePath, IS_STRING, 0)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_requires_method_entry) {
	PHP_ME(Test_Requires, requireExternal1, arginfo_test_requires_requireexternal1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Requires, requireExternal2, arginfo_test_requires_requireexternal2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Requires, requireExternal3, arginfo_test_requires_requireexternal3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Requires, setContent, arginfo_test_requires_setcontent, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Requires, renderTemplate, arginfo_test_requires_rendertemplate, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
