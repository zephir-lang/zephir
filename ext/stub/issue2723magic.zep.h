
extern zend_class_entry *stub_issue2723magic_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2723Magic);

PHP_METHOD(Stub_Issue2723Magic, __call);
PHP_METHOD(Stub_Issue2723Magic, __callStatic);
PHP_METHOD(Stub_Issue2723Magic, selfDynamic);
PHP_METHOD(Stub_Issue2723Magic, selfDynamicStatic);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2723magic___call, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_ARRAY_INFO(0, arguments, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2723magic___callstatic, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_ARRAY_INFO(0, arguments, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723magic_selfdynamic, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2723magic_selfdynamicstatic, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2723magic_method_entry) {
	PHP_ME(Stub_Issue2723Magic, __call, arginfo_stub_issue2723magic___call, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723Magic, __callStatic, arginfo_stub_issue2723magic___callstatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2723Magic, selfDynamic, arginfo_stub_issue2723magic_selfdynamic, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2723Magic, selfDynamicStatic, arginfo_stub_issue2723magic_selfdynamicstatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
