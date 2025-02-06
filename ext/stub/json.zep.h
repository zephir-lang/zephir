
extern zend_class_entry *stub_json_ce;

ZEPHIR_INIT_CLASS(Stub_Json);

PHP_METHOD(Stub_Json, testEncodeObject);
PHP_METHOD(Stub_Json, testEncodeArray);
PHP_METHOD(Stub_Json, testEncodeOptions);
PHP_METHOD(Stub_Json, testDecodeObject);
PHP_METHOD(Stub_Json, testDecodeObject2);
PHP_METHOD(Stub_Json, testDecodeArray);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_json_testencodeobject, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_json_testencodearray, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_json_testencodeoptions, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_json_testdecodeobject, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_json_testdecodeobject2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_json_testdecodearray, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_json_method_entry) {
PHP_ME(Stub_Json, testEncodeObject, arginfo_stub_json_testencodeobject, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Json, testEncodeArray, arginfo_stub_json_testencodearray, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Json, testEncodeOptions, arginfo_stub_json_testencodeoptions, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Json, testDecodeObject, arginfo_stub_json_testdecodeobject, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Json, testDecodeObject2, arginfo_stub_json_testdecodeobject2, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Json, testDecodeArray, arginfo_stub_json_testdecodearray, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
