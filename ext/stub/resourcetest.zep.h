
extern zend_class_entry *stub_resourcetest_ce;

ZEPHIR_INIT_CLASS(Stub_ResourceTest);

PHP_METHOD(Stub_ResourceTest, testLetStatementSTDIN);
PHP_METHOD(Stub_ResourceTest, testLetStatementSTDOUT);
PHP_METHOD(Stub_ResourceTest, testLetStatementSTDERR);
PHP_METHOD(Stub_ResourceTest, testTypeOffResource);
PHP_METHOD(Stub_ResourceTest, testIsResource);
PHP_METHOD(Stub_ResourceTest, testFunctionsForSTDIN);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_resourcetest_testletstatementstdin, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_resourcetest_testletstatementstdout, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_resourcetest_testletstatementstderr, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_resourcetest_testtypeoffresource, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_resourcetest_testisresource, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_resourcetest_testfunctionsforstdin, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_resourcetest_method_entry) {
PHP_ME(Stub_ResourceTest, testLetStatementSTDIN, arginfo_stub_resourcetest_testletstatementstdin, ZEND_ACC_PUBLIC)
PHP_ME(Stub_ResourceTest, testLetStatementSTDOUT, arginfo_stub_resourcetest_testletstatementstdout, ZEND_ACC_PUBLIC)
PHP_ME(Stub_ResourceTest, testLetStatementSTDERR, arginfo_stub_resourcetest_testletstatementstderr, ZEND_ACC_PUBLIC)
PHP_ME(Stub_ResourceTest, testTypeOffResource, arginfo_stub_resourcetest_testtypeoffresource, ZEND_ACC_PUBLIC)
PHP_ME(Stub_ResourceTest, testIsResource, arginfo_stub_resourcetest_testisresource, ZEND_ACC_PUBLIC)
PHP_ME(Stub_ResourceTest, testFunctionsForSTDIN, arginfo_stub_resourcetest_testfunctionsforstdin, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
