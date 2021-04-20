
extern zend_class_entry *stub_issue2165_issue_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2165_Issue);

PHP_METHOD(Stub_Issue2165_Issue, build);
PHP_METHOD(Stub_Issue2165_Issue, __construct);
PHP_METHOD(Stub_Issue2165_Issue, quick);
PHP_METHOD(Stub_Issue2165_Issue, ones);
PHP_METHOD(Stub_Issue2165_Issue, fill);
PHP_METHOD(Stub_Issue2165_Issue, reciprocal);
PHP_METHOD(Stub_Issue2165_Issue, divideMatrix);

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2165_issue_build, 0, 0, Stub\\Issue2165\\Issue, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, a, IS_ARRAY, 0, "[]")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2165_issue___construct, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, a, 0)
	ZEND_ARG_TYPE_INFO(0, validate, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2165_issue_quick, 0, 0, Stub\\Issue2165\\Issue, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, a, IS_ARRAY, 0, "[]")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2165_issue_ones, 0, 2, Stub\\Issue2165\\Issue, 0)
	ZEND_ARG_TYPE_INFO(0, m, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2165_issue_fill, 0, 3, Stub\\Issue2165\\Issue, 0)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_TYPE_INFO(0, m, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2165_issue_reciprocal, 0, 0, Stub\\Issue2165\\Issue, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_issue2165_issue_dividematrix, 0, 1, Stub\\Issue2165\\Issue, 0)
	ZEND_ARG_OBJ_INFO(0, b, Stub\\Issue2165\\Issue, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2165_issue_method_entry) {
	PHP_ME(Stub_Issue2165_Issue, build, arginfo_stub_issue2165_issue_build, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2165_Issue, __construct, arginfo_stub_issue2165_issue___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Issue2165_Issue, quick, arginfo_stub_issue2165_issue_quick, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2165_Issue, ones, arginfo_stub_issue2165_issue_ones, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2165_Issue, fill, arginfo_stub_issue2165_issue_fill, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2165_Issue, reciprocal, arginfo_stub_issue2165_issue_reciprocal, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2165_Issue, divideMatrix, arginfo_stub_issue2165_issue_dividematrix, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
