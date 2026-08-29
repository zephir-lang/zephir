
extern zend_class_entry *stub_closures_ce;

ZEPHIR_INIT_CLASS(Stub_Closures);

PHP_METHOD(Stub_Closures, simple1);
PHP_METHOD(Stub_Closures, simple2);
PHP_METHOD(Stub_Closures, simple3);
PHP_METHOD(Stub_Closures, simple4);
PHP_METHOD(Stub_Closures, simple5);
PHP_METHOD(Stub_Closures, arrow1);
PHP_METHOD(Stub_Closures, arrow2);
PHP_METHOD(Stub_Closures, testUseCommand);
PHP_METHOD(Stub_Closures, issue1860);
PHP_METHOD(Stub_Closures, issue642);
PHP_METHOD(Stub_Closures, issue1036SetArgument);
PHP_METHOD(Stub_Closures, issue1036SetFunction);
PHP_METHOD(Stub_Closures, issue1036Call);
PHP_METHOD(Stub_Closures, issue2497Helper);
PHP_METHOD(Stub_Closures, issue2497ClosureThis);
PHP_METHOD(Stub_Closures, issue2497ClosureThisWithUse);
PHP_METHOD(Stub_Closures, issue2497PropertyAccess);
PHP_METHOD(Stub_Closures, issue2497SetName);
PHP_METHOD(Stub_Closures, issue1873);
PHP_METHOD(Stub_Closures, issue1873ArrayProperty);
PHP_METHOD(Stub_Closures, issue1873MultipleProperties);
PHP_METHOD(Stub_Closures, issue1873PropertyWriter);
PHP_METHOD(Stub_Closures, issue1873PropertyAndUse);
PHP_METHOD(Stub_Closures, issue2562StringUse);
PHP_METHOD(Stub_Closures, issue2321CallPrivateCallback);
PHP_METHOD(Stub_Closures, issue2321filterQuery);
PHP_METHOD(Stub_Closures, issue2321doUrlEncode);
PHP_METHOD(Stub_Closures, issue2321ProtectedCallback);
PHP_METHOD(Stub_Closures, issue2321ProtectedUpper);
PHP_METHOD(Stub_Closures, issue2321ArrayMapPrivate);
PHP_METHOD(Stub_Closures, issue2321Doubled);
PHP_METHOD(Stub_Closures, issue2638StringLocalUse);
PHP_METHOD(Stub_Closures, issue2638StringParamMutatedUse);
PHP_METHOD(Stub_Closures, issue2652Scalar);
PHP_METHOD(Stub_Closures, issue2652Str);
PHP_METHOD(Stub_Closures, issue2652Arr);
PHP_METHOD(Stub_Closures, issue2652Obj);
PHP_METHOD(Stub_Closures, issue2652WithThis);
PHP_METHOD(Stub_Closures, issue2652ReturnProperty);
PHP_METHOD(Stub_Closures, issue2652ReturnThis);
PHP_METHOD(Stub_Closures, issue2652Loop);
PHP_METHOD(Stub_Closures, issue2652Snapshot);
PHP_METHOD(Stub_Closures, issue2652BodyMutation);
PHP_METHOD(Stub_Closures, issue2652ByRefShared);
PHP_METHOD(Stub_Closures, issue2652ByRefWritesOut);
PHP_METHOD(Stub_Closures, issue2652ByRefReadsLateWrite);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_simple1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_simple2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_simple3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_simple4, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_simple5, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_arrow1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_arrow2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_testusecommand, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_issue1860, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, abc, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_issue642, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_issue1036setargument, 0, 0, 1)
	ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_issue1036setfunction, 0, 0, 1)
	ZEND_ARG_INFO(0, func)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_closures_issue1036call, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_closures_issue2497helper, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue2497closurethis, 0, 0, Closure, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue2497closurethiswithuse, 0, 1, Closure, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue2497propertyaccess, 0, 0, Closure, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_closures_issue2497setname, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue1873, 0, 0, Closure, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue1873arrayproperty, 0, 0, Closure, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue1873multipleproperties, 0, 0, Closure, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue1873propertywriter, 0, 0, Closure, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue1873propertyanduse, 0, 1, Closure, 0)
	ZEND_ARG_INFO(0, prefix)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue2562stringuse, 0, 1, Closure, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_closures_issue2321callprivatecallback, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_closures_issue2321filterquery, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_closures_issue2321dourlencode, 0, 1, IS_STRING, 0)
	ZEND_ARG_ARRAY_INFO(0, matches, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_closures_issue2321protectedcallback, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_closures_issue2321protectedupper, 0, 1, IS_STRING, 0)
	ZEND_ARG_ARRAY_INFO(0, matches, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_closures_issue2321arraymapprivate, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_ARRAY_INFO(0, values, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_closures_issue2321doubled, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, val, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue2638stringlocaluse, 0, 0, Closure, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue2638stringparammutateduse, 0, 1, Closure, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue2652scalar, 0, 1, Closure, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue2652str, 0, 1, Closure, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue2652arr, 0, 1, Closure, 0)
	ZEND_ARG_ARRAY_INFO(0, items, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue2652obj, 0, 1, Closure, 0)
	ZEND_ARG_INFO(0, box)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue2652withthis, 0, 1, Closure, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue2652returnproperty, 0, 1, Closure, 0)
	ZEND_ARG_INFO(0, suffix)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue2652returnthis, 0, 1, Closure, 0)
	ZEND_ARG_INFO(0, tag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_closures_issue2652loop, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue2652snapshot, 0, 0, Closure, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue2652bodymutation, 0, 1, Closure, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_closures_issue2652byrefshared, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_closures_issue2652byrefwritesout, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_closures_issue2652byrefreadslatewrite, 0, 0, Closure, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_closures_method_entry) {
PHP_ME(Stub_Closures, simple1, arginfo_stub_closures_simple1, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, simple2, arginfo_stub_closures_simple2, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, simple3, arginfo_stub_closures_simple3, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, simple4, arginfo_stub_closures_simple4, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, simple5, arginfo_stub_closures_simple5, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, arrow1, arginfo_stub_closures_arrow1, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, arrow2, arginfo_stub_closures_arrow2, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, testUseCommand, arginfo_stub_closures_testusecommand, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue1860, arginfo_stub_closures_issue1860, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, issue642, arginfo_stub_closures_issue642, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue1036SetArgument, arginfo_stub_closures_issue1036setargument, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue1036SetFunction, arginfo_stub_closures_issue1036setfunction, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Closures, issue1036Call, arginfo_stub_closures_issue1036call, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2497Helper, arginfo_stub_closures_issue2497helper, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2497ClosureThis, arginfo_stub_closures_issue2497closurethis, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2497ClosureThisWithUse, arginfo_stub_closures_issue2497closurethiswithuse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2497PropertyAccess, arginfo_stub_closures_issue2497propertyaccess, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2497SetName, arginfo_stub_closures_issue2497setname, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue1873, arginfo_stub_closures_issue1873, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue1873ArrayProperty, arginfo_stub_closures_issue1873arrayproperty, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue1873MultipleProperties, arginfo_stub_closures_issue1873multipleproperties, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue1873PropertyWriter, arginfo_stub_closures_issue1873propertywriter, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue1873PropertyAndUse, arginfo_stub_closures_issue1873propertyanduse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2562StringUse, arginfo_stub_closures_issue2562stringuse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2321CallPrivateCallback, arginfo_stub_closures_issue2321callprivatecallback, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2321filterQuery, arginfo_stub_closures_issue2321filterquery, ZEND_ACC_PRIVATE)
	PHP_ME(Stub_Closures, issue2321doUrlEncode, arginfo_stub_closures_issue2321dourlencode, ZEND_ACC_PRIVATE)
	PHP_ME(Stub_Closures, issue2321ProtectedCallback, arginfo_stub_closures_issue2321protectedcallback, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2321ProtectedUpper, arginfo_stub_closures_issue2321protectedupper, ZEND_ACC_PROTECTED)
	PHP_ME(Stub_Closures, issue2321ArrayMapPrivate, arginfo_stub_closures_issue2321arraymapprivate, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2321Doubled, arginfo_stub_closures_issue2321doubled, ZEND_ACC_PRIVATE)
	PHP_ME(Stub_Closures, issue2638StringLocalUse, arginfo_stub_closures_issue2638stringlocaluse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2638StringParamMutatedUse, arginfo_stub_closures_issue2638stringparammutateduse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2652Scalar, arginfo_stub_closures_issue2652scalar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2652Str, arginfo_stub_closures_issue2652str, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2652Arr, arginfo_stub_closures_issue2652arr, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2652Obj, arginfo_stub_closures_issue2652obj, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2652WithThis, arginfo_stub_closures_issue2652withthis, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2652ReturnProperty, arginfo_stub_closures_issue2652returnproperty, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2652ReturnThis, arginfo_stub_closures_issue2652returnthis, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2652Loop, arginfo_stub_closures_issue2652loop, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2652Snapshot, arginfo_stub_closures_issue2652snapshot, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2652BodyMutation, arginfo_stub_closures_issue2652bodymutation, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2652ByRefShared, arginfo_stub_closures_issue2652byrefshared, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2652ByRefWritesOut, arginfo_stub_closures_issue2652byrefwritesout, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Closures, issue2652ByRefReadsLateWrite, arginfo_stub_closures_issue2652byrefreadslatewrite, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
