
extern zend_class_entry *stub_requires_ce;

ZEPHIR_INIT_CLASS(Stub_Requires);

PHP_METHOD(Stub_Requires, requireExternal1);
PHP_METHOD(Stub_Requires, requireExternal2);
PHP_METHOD(Stub_Requires, requireExternal3);
PHP_METHOD(Stub_Requires, setContent);
PHP_METHOD(Stub_Requires, renderTemplate);
PHP_METHOD(Stub_Requires, requireOnce);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_requires_requireexternal1, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_requires_requireexternal2, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_requires_requireexternal3, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_requires_setcontent, 0, 1, IS_VOID, 0)

	ZEND_ARG_INFO(0, content)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_requires_rendertemplate, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, templatePath, IS_STRING, 0)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_requires_requireonce, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_requires_method_entry) {
	PHP_ME(Stub_Requires, requireExternal1, arginfo_stub_requires_requireexternal1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Requires, requireExternal2, arginfo_stub_requires_requireexternal2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Requires, requireExternal3, arginfo_stub_requires_requireexternal3, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Requires, setContent, arginfo_stub_requires_setcontent, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Requires, renderTemplate, arginfo_stub_requires_rendertemplate, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Requires, requireOnce, arginfo_stub_requires_requireonce, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
