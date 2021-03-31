
extern zend_class_entry *stub_reflection_ce;

ZEPHIR_INIT_CLASS(Stub_Reflection);

PHP_METHOD(Stub_Reflection, getReflectionClass);
PHP_METHOD(Stub_Reflection, getReflectionFunction);
PHP_METHOD(Stub_Reflection, setReflectionParameter);
PHP_METHOD(Stub_Reflection, getReflectionParameter);

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_reflection_getreflectionclass, 0, 0, ReflectionClass, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_reflection_getreflectionfunction, 0, 0, ReflectionFunction, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_reflection_setreflectionparameter, 0, 1, ReflectionParameter, 0)
	ZEND_ARG_OBJ_INFO(0, parameter, ReflectionParameter, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_reflection_getreflectionparameter, 0, 0, ReflectionParameter, 1)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_reflection_method_entry) {
	PHP_ME(Stub_Reflection, getReflectionClass, arginfo_stub_reflection_getreflectionclass, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Reflection, getReflectionFunction, arginfo_stub_reflection_getreflectionfunction, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Reflection, setReflectionParameter, arginfo_stub_reflection_setreflectionparameter, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Reflection, getReflectionParameter, arginfo_stub_reflection_getreflectionparameter, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
