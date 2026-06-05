
extern zend_class_entry *stub_arrayaccessproperty_ce;

ZEPHIR_INIT_CLASS(Stub_ArrayAccessProperty);

PHP_METHOD(Stub_ArrayAccessProperty, __construct);
PHP_METHOD(Stub_ArrayAccessProperty, set);
PHP_METHOD(Stub_ArrayAccessProperty, setNested);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccessproperty___construct, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, data, ArrayAccess, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccessproperty_set, 0, 0, 2)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_arrayaccessproperty_setnested, 0, 0, 3)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_arrayaccessproperty_method_entry) {
	PHP_ME(Stub_ArrayAccessProperty, __construct, arginfo_stub_arrayaccessproperty___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_ArrayAccessProperty, set, arginfo_stub_arrayaccessproperty_set, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ArrayAccessProperty, setNested, arginfo_stub_arrayaccessproperty_setnested, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
