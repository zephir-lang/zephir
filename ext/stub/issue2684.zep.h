
extern zend_class_entry *stub_issue2684_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2684);

PHP_METHOD(Stub_Issue2684, explodeNoLimit);
PHP_METHOD(Stub_Issue2684, explodeStr);
PHP_METHOD(Stub_Issue2684, explodeLimit);
PHP_METHOD(Stub_Issue2684, explodeStrLimit);
PHP_METHOD(Stub_Issue2684, explodeConstLimit);
PHP_METHOD(Stub_Issue2684, stopsAfterThrow);
PHP_METHOD(Stub_Issue2684, catchesInTry);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2684_explodenolimit, 0, 0, 2)
	ZEND_ARG_INFO(0, delimiter)
	ZEND_ARG_INFO(0, source)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2684_explodestr, 0, 0, 1)
	ZEND_ARG_INFO(0, source)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2684_explodelimit, 0, 0, 3)
	ZEND_ARG_INFO(0, delimiter)
	ZEND_ARG_INFO(0, source)
	ZEND_ARG_INFO(0, limit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2684_explodestrlimit, 0, 0, 2)
	ZEND_ARG_INFO(0, source)
	ZEND_ARG_INFO(0, limit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2684_explodeconstlimit, 0, 0, 1)
	ZEND_ARG_INFO(0, source)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2684_stopsafterthrow, 0, 0, 2)
	ZEND_ARG_INFO(0, delimiter)
	ZEND_ARG_INFO(0, source)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2684_catchesintry, 0, 0, 2)
	ZEND_ARG_INFO(0, delimiter)
	ZEND_ARG_INFO(0, source)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2684_method_entry) {
	PHP_ME(Stub_Issue2684, explodeNoLimit, arginfo_stub_issue2684_explodenolimit, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2684, explodeStr, arginfo_stub_issue2684_explodestr, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2684, explodeLimit, arginfo_stub_issue2684_explodelimit, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2684, explodeStrLimit, arginfo_stub_issue2684_explodestrlimit, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2684, explodeConstLimit, arginfo_stub_issue2684_explodeconstlimit, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2684, stopsAfterThrow, arginfo_stub_issue2684_stopsafterthrow, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2684, catchesInTry, arginfo_stub_issue2684_catchesintry, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
