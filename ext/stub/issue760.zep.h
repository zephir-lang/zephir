
extern zend_class_entry *stub_issue760_ce;

ZEPHIR_INIT_CLASS(Stub_Issue760);

PHP_METHOD(Stub_Issue760, dynIntOffset);
PHP_METHOD(Stub_Issue760, dynVarOffset);
PHP_METHOD(Stub_Issue760, strIntOffset);
PHP_METHOD(Stub_Issue760, localStringOffset);
PHP_METHOD(Stub_Issue760, charDest);
PHP_METHOD(Stub_Issue760, dynCharDest);
PHP_METHOD(Stub_Issue760, strVarOffset);
PHP_METHOD(Stub_Issue760, literalOffset);
PHP_METHOD(Stub_Issue760, returnStrOffset);
PHP_METHOD(Stub_Issue760, returnDynOffset);
PHP_METHOD(Stub_Issue760, dynIsset);
PHP_METHOD(Stub_Issue760, strIsset);
PHP_METHOD(Stub_Issue760, dynEmpty);
PHP_METHOD(Stub_Issue760, strEmpty);
PHP_METHOD(Stub_Issue760, arrayEmpty);
PHP_METHOD(Stub_Issue760, dynUnset);
PHP_METHOD(Stub_Issue760, dynAppend);
PHP_METHOD(Stub_Issue760, dynWrite);
PHP_METHOD(Stub_Issue760, dynWriteInt);
PHP_METHOD(Stub_Issue760, strWrite);
PHP_METHOD(Stub_Issue760, localStringWrite);
PHP_METHOD(Stub_Issue760, dynTraverse);
PHP_METHOD(Stub_Issue760, dynTraverseKeyed);
PHP_METHOD(Stub_Issue760, dynTraverseReverse);
PHP_METHOD(Stub_Issue760, strTraverse);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue760_dynintoffset, 0, 0, 2)
	ZEND_ARG_INFO(0, s)
	ZEND_ARG_TYPE_INFO(0, i, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue760_dynvaroffset, 0, 0, 2)
	ZEND_ARG_INFO(0, s)
	ZEND_ARG_INFO(0, i)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue760_strintoffset, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, s, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, i, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue760_localstringoffset, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, i, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue760_chardest, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, s, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, i, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue760_dynchardest, 0, 2, IS_LONG, 0)
	ZEND_ARG_INFO(0, s)
	ZEND_ARG_TYPE_INFO(0, i, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue760_strvaroffset, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, s, IS_STRING, 0)
	ZEND_ARG_INFO(0, i)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue760_literaloffset, 0, 0, 1)
	ZEND_ARG_INFO(0, i)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue760_returnstroffset, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, s, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, i, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue760_returndynoffset, 0, 0, 2)
	ZEND_ARG_INFO(0, s)
	ZEND_ARG_TYPE_INFO(0, i, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue760_dynisset, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, s)
	ZEND_ARG_INFO(0, i)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue760_strisset, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, s, IS_STRING, 0)
	ZEND_ARG_INFO(0, i)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue760_dynempty, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, s)
	ZEND_ARG_INFO(0, i)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue760_strempty, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, s, IS_STRING, 0)
	ZEND_ARG_INFO(0, i)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue760_arrayempty, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_ARRAY_INFO(0, a, 0)
	ZEND_ARG_INFO(0, i)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue760_dynunset, 0, 0, 2)
	ZEND_ARG_INFO(0, s)
	ZEND_ARG_INFO(0, i)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue760_dynappend, 0, 0, 1)
	ZEND_ARG_INFO(0, s)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue760_dynwrite, 0, 0, 3)
	ZEND_ARG_INFO(0, s)
	ZEND_ARG_INFO(0, i)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue760_dynwriteint, 0, 0, 3)
	ZEND_ARG_INFO(0, s)
	ZEND_ARG_TYPE_INFO(0, i, IS_LONG, 0)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue760_strwrite, 0, 3, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, s, IS_STRING, 0)
	ZEND_ARG_INFO(0, i)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue760_localstringwrite, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, i)
	ZEND_ARG_INFO(0, v)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue760_dyntraverse, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, s)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue760_dyntraversekeyed, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, s)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue760_dyntraversereverse, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, s)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue760_strtraverse, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, s, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue760_method_entry) {
	PHP_ME(Stub_Issue760, dynIntOffset, arginfo_stub_issue760_dynintoffset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, dynVarOffset, arginfo_stub_issue760_dynvaroffset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, strIntOffset, arginfo_stub_issue760_strintoffset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, localStringOffset, arginfo_stub_issue760_localstringoffset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, charDest, arginfo_stub_issue760_chardest, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, dynCharDest, arginfo_stub_issue760_dynchardest, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, strVarOffset, arginfo_stub_issue760_strvaroffset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, literalOffset, arginfo_stub_issue760_literaloffset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, returnStrOffset, arginfo_stub_issue760_returnstroffset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, returnDynOffset, arginfo_stub_issue760_returndynoffset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, dynIsset, arginfo_stub_issue760_dynisset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, strIsset, arginfo_stub_issue760_strisset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, dynEmpty, arginfo_stub_issue760_dynempty, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, strEmpty, arginfo_stub_issue760_strempty, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, arrayEmpty, arginfo_stub_issue760_arrayempty, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, dynUnset, arginfo_stub_issue760_dynunset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, dynAppend, arginfo_stub_issue760_dynappend, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, dynWrite, arginfo_stub_issue760_dynwrite, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, dynWriteInt, arginfo_stub_issue760_dynwriteint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, strWrite, arginfo_stub_issue760_strwrite, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, localStringWrite, arginfo_stub_issue760_localstringwrite, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, dynTraverse, arginfo_stub_issue760_dyntraverse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, dynTraverseKeyed, arginfo_stub_issue760_dyntraversekeyed, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, dynTraverseReverse, arginfo_stub_issue760_dyntraversereverse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue760, strTraverse, arginfo_stub_issue760_strtraverse, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
