
extern zend_class_entry *stub_issue2831_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2831);

PHP_METHOD(Stub_Issue2831, read);
PHP_METHOD(Stub_Issue2831, readDynamic);
PHP_METHOD(Stub_Issue2831, readProtected);
PHP_METHOD(Stub_Issue2831, readPrivate);
PHP_METHOD(Stub_Issue2831, readTyped);
PHP_METHOD(Stub_Issue2831, readInCondition);
PHP_METHOD(Stub_Issue2831, isEmpty);
PHP_METHOD(Stub_Issue2831, isEmptyDynamic);
PHP_METHOD(Stub_Issue2831, issetOrEmpty);
PHP_METHOD(Stub_Issue2831, issetDim);
PHP_METHOD(Stub_Issue2831, issetDynamic);
PHP_METHOD(Stub_Issue2831, issetNested);
PHP_METHOD(Stub_Issue2831, isEmptyNested);
PHP_METHOD(Stub_Issue2831, issetThenMark);
PHP_METHOD(Stub_Issue2831, isEmptyThenMark);
PHP_METHOD(Stub_Issue2831, addAssign);
PHP_METHOD(Stub_Issue2831, addAssignDynamic);
PHP_METHOD(Stub_Issue2831, increment);
PHP_METHOD(Stub_Issue2831, readThenMark);
PHP_METHOD(Stub_Issue2831, readProtectedThenMark);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2831_read, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2831_readdynamic, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2831_readprotected, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2831_readprivate, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2831_readtyped, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2831_readincondition, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2831_isempty, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2831_isemptydynamic, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2831_issetorempty, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_TYPE_INFO(0, askIsset, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2831_issetdim, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2831_issetdynamic, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2831_issetnested, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2831_isemptynested, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2831_issetthenmark, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, target)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2831_isemptythenmark, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, target)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2831_addassign, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2831_addassigndynamic, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2831_increment, 0, 0, 1)
	ZEND_ARG_INFO(0, o)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2831_readthenmark, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, target)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2831_readprotectedthenmark, 0, 0, 2)
	ZEND_ARG_INFO(0, o)
	ZEND_ARG_INFO(0, target)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2831_method_entry) {
	PHP_ME(Stub_Issue2831, read, arginfo_stub_issue2831_read, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, readDynamic, arginfo_stub_issue2831_readdynamic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, readProtected, arginfo_stub_issue2831_readprotected, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, readPrivate, arginfo_stub_issue2831_readprivate, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, readTyped, arginfo_stub_issue2831_readtyped, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, readInCondition, arginfo_stub_issue2831_readincondition, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, isEmpty, arginfo_stub_issue2831_isempty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, isEmptyDynamic, arginfo_stub_issue2831_isemptydynamic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, issetOrEmpty, arginfo_stub_issue2831_issetorempty, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, issetDim, arginfo_stub_issue2831_issetdim, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, issetDynamic, arginfo_stub_issue2831_issetdynamic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, issetNested, arginfo_stub_issue2831_issetnested, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, isEmptyNested, arginfo_stub_issue2831_isemptynested, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, issetThenMark, arginfo_stub_issue2831_issetthenmark, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, isEmptyThenMark, arginfo_stub_issue2831_isemptythenmark, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, addAssign, arginfo_stub_issue2831_addassign, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, addAssignDynamic, arginfo_stub_issue2831_addassigndynamic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, increment, arginfo_stub_issue2831_increment, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, readThenMark, arginfo_stub_issue2831_readthenmark, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Issue2831, readProtectedThenMark, arginfo_stub_issue2831_readprotectedthenmark, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
