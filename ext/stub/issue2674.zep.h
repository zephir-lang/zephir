
extern zend_class_entry *stub_issue2674_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2674);

PHP_METHOD(Stub_Issue2674, explodeStrLimit);
PHP_METHOD(Stub_Issue2674, explodeLimit);
PHP_METHOD(Stub_Issue2674, explodeNoLimit);
PHP_METHOD(Stub_Issue2674, explodeConstLimit);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2674_explodestrlimit, 0, 0, 2)
	ZEND_ARG_INFO(0, source)
	ZEND_ARG_INFO(0, limit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2674_explodelimit, 0, 0, 3)
	ZEND_ARG_INFO(0, delimiter)
	ZEND_ARG_INFO(0, source)
	ZEND_ARG_INFO(0, limit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2674_explodenolimit, 0, 0, 2)
	ZEND_ARG_INFO(0, delimiter)
	ZEND_ARG_INFO(0, source)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2674_explodeconstlimit, 0, 0, 1)
	ZEND_ARG_INFO(0, source)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2674_method_entry) {
	PHP_ME(Stub_Issue2674, explodeStrLimit, arginfo_stub_issue2674_explodestrlimit, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2674, explodeLimit, arginfo_stub_issue2674_explodelimit, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2674, explodeNoLimit, arginfo_stub_issue2674_explodenolimit, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2674, explodeConstLimit, arginfo_stub_issue2674_explodeconstlimit, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
