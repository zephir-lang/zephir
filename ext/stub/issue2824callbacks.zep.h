
extern zend_class_entry *stub_issue2824callbacks_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2824Callbacks);

PHP_METHOD(Stub_Issue2824Callbacks, secret);
void zep_Stub_Issue2824Callbacks_viaInternal(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used);
void zep_Stub_Issue2824Callbacks_viaInternalFunc(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used);
PHP_METHOD(Stub_Issue2824Callbacks, fromInternal);
PHP_METHOD(Stub_Issue2824Callbacks, fromMethod);
PHP_METHOD(Stub_Issue2824Callbacks, fromMethodArray);
PHP_METHOD(Stub_Issue2824Callbacks, fromMethodNamed);
PHP_METHOD(Stub_Issue2824Callbacks, fromGenerator);
PHP_METHOD(Stub_Issue2824Callbacks, notCallable);
PHP_METHOD(Stub_Issue2824Callbacks, notCallableWithArgs);
PHP_METHOD(Stub_Issue2824Callbacks, notCallableArray);
PHP_METHOD(Stub_Issue2824Callbacks, notArray);
void zep_Stub_Issue2824Callbacks_zephir_gen_step_fromGenerator(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext );

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824callbacks_secret, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0, b, "''")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824callbacks_viainternal, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824callbacks_viainternalfunc, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824callbacks_frominternal, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824callbacks_frommethod, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824callbacks_frommethodarray, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2824callbacks_frommethodnamed, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2824callbacks_fromgenerator, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2824callbacks_notcallable, 0, 0, 1)
	ZEND_ARG_INFO(0, f)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2824callbacks_notcallablewithargs, 0, 0, 1)
	ZEND_ARG_INFO(0, f)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2824callbacks_notcallablearray, 0, 0, 1)
	ZEND_ARG_INFO(0, f)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2824callbacks_notarray, 0, 0, 2)
	ZEND_ARG_INFO(0, f)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2824callbacks_zephir_gen_step_fromgenerator, 0, 0, 1)
	ZEND_ARG_INFO(0, zephir_gen)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2824callbacks_method_entry) {
	PHP_ME(Stub_Issue2824Callbacks, secret, arginfo_stub_issue2824callbacks_secret, ZEND_ACC_PRIVATE)
	PHP_ME(Stub_Issue2824Callbacks, fromInternal, arginfo_stub_issue2824callbacks_frominternal, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824Callbacks, fromMethod, arginfo_stub_issue2824callbacks_frommethod, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824Callbacks, fromMethodArray, arginfo_stub_issue2824callbacks_frommethodarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824Callbacks, fromMethodNamed, arginfo_stub_issue2824callbacks_frommethodnamed, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2824Callbacks, fromGenerator, arginfo_stub_issue2824callbacks_fromgenerator, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824Callbacks, notCallable, arginfo_stub_issue2824callbacks_notcallable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824Callbacks, notCallableWithArgs, arginfo_stub_issue2824callbacks_notcallablewithargs, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824Callbacks, notCallableArray, arginfo_stub_issue2824callbacks_notcallablearray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2824Callbacks, notArray, arginfo_stub_issue2824callbacks_notarray, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
