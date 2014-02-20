
extern zend_class_entry *test_resource_ce;

ZEPHIR_INIT_CLASS(Test_Resource);

PHP_METHOD(Test_Resource, testLetStatementSTDIN);
PHP_METHOD(Test_Resource, testLetStatementSTDOUT);
PHP_METHOD(Test_Resource, testLetStatementSTDERR);
PHP_METHOD(Test_Resource, testTypeOffResource);
PHP_METHOD(Test_Resource, testIsResource);

ZEPHIR_INIT_FUNCS(test_resource_method_entry) {
	PHP_ME(Test_Resource, testLetStatementSTDIN, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Resource, testLetStatementSTDOUT, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Resource, testLetStatementSTDERR, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Resource, testTypeOffResource, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Resource, testIsResource, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
