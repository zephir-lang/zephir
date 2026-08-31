
extern zend_class_entry *stub_issue2666_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2666);

PHP_METHOD(Stub_Issue2666, longParamToString);
PHP_METHOD(Stub_Issue2666, ulongParamToString);
PHP_METHOD(Stub_Issue2666, intParamToString);
PHP_METHOD(Stub_Issue2666, longRoundTrip);
PHP_METHOD(Stub_Issue2666, ulongRoundTrip);
PHP_METHOD(Stub_Issue2666, longThroughLocal);
PHP_METHOD(Stub_Issue2666, castToInt);
PHP_METHOD(Stub_Issue2666, castToLong);
PHP_METHOD(Stub_Issue2666, longAdd);
PHP_METHOD(Stub_Issue2666, longArrayKey);
PHP_METHOD(Stub_Issue2666, longArrayFetch);
PHP_METHOD(Stub_Issue2666, longArrayIsset);
PHP_METHOD(Stub_Issue2666, varLessThanLong);
PHP_METHOD(Stub_Issue2666, varGreaterThanLong);
PHP_METHOD(Stub_Issue2666, substrWithLongOffset);
PHP_METHOD(Stub_Issue2666, explodeWithLongLimit);
PHP_METHOD(Stub_Issue2666, divLongLong);
PHP_METHOD(Stub_Issue2666, divLongDouble);
PHP_METHOD(Stub_Issue2666, divDoubleLong);
PHP_METHOD(Stub_Issue2666, divDoubleDouble);
PHP_METHOD(Stub_Issue2666, divVarLong);
PHP_METHOD(Stub_Issue2666, divVarDouble);
PHP_METHOD(Stub_Issue2666, divLongVar);
PHP_METHOD(Stub_Issue2666, divDoubleVar);
PHP_METHOD(Stub_Issue2666, modLongLong);
PHP_METHOD(Stub_Issue2666, modLongDouble);
PHP_METHOD(Stub_Issue2666, modDoubleLong);
PHP_METHOD(Stub_Issue2666, modDoubleDouble);
PHP_METHOD(Stub_Issue2666, modVarLong);
PHP_METHOD(Stub_Issue2666, modVarDouble);
PHP_METHOD(Stub_Issue2666, modLongVar);
PHP_METHOD(Stub_Issue2666, modDoubleVar);
PHP_METHOD(Stub_Issue2666, divLiteralDoubleByDouble);
PHP_METHOD(Stub_Issue2666, modLiteralDoubleByDouble);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2666_longparamtostring, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2666_ulongparamtostring, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2666_intparamtostring, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2666_longroundtrip, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2666_ulongroundtrip, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2666_longthroughlocal, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, number, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2666_casttoint, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2666_casttolong, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2666_longadd, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2666_longarraykey, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_LONG, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_longarrayfetch, 0, 0, 2)
	ZEND_ARG_ARRAY_INFO(0, source, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2666_longarrayisset, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_ARRAY_INFO(0, source, 0)
	ZEND_ARG_TYPE_INFO(0, key, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2666_varlessthanlong, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_TYPE_INFO(0, limit, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2666_vargreaterthanlong, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_TYPE_INFO(0, limit, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2666_substrwithlongoffset, 0, 3, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, source, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, from, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2666_explodewithlonglimit, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, source, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, limit, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_divlonglong, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_divlongdouble, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_divdoublelong, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_divdoubledouble, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_divvarlong, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_divvardouble, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_divlongvar, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_divdoublevar, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_modlonglong, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_modlongdouble, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_moddoublelong, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_moddoubledouble, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_modvarlong, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_modvardouble, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_modlongvar, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_moddoublevar, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_divliteraldoublebydouble, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2666_modliteraldoublebydouble, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2666_method_entry) {
	PHP_ME(Stub_Issue2666, longParamToString, arginfo_stub_issue2666_longparamtostring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, ulongParamToString, arginfo_stub_issue2666_ulongparamtostring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, intParamToString, arginfo_stub_issue2666_intparamtostring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, longRoundTrip, arginfo_stub_issue2666_longroundtrip, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, ulongRoundTrip, arginfo_stub_issue2666_ulongroundtrip, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, longThroughLocal, arginfo_stub_issue2666_longthroughlocal, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, castToInt, arginfo_stub_issue2666_casttoint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, castToLong, arginfo_stub_issue2666_casttolong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, longAdd, arginfo_stub_issue2666_longadd, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, longArrayKey, arginfo_stub_issue2666_longarraykey, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, longArrayFetch, arginfo_stub_issue2666_longarrayfetch, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, longArrayIsset, arginfo_stub_issue2666_longarrayisset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, varLessThanLong, arginfo_stub_issue2666_varlessthanlong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, varGreaterThanLong, arginfo_stub_issue2666_vargreaterthanlong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, substrWithLongOffset, arginfo_stub_issue2666_substrwithlongoffset, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, explodeWithLongLimit, arginfo_stub_issue2666_explodewithlonglimit, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, divLongLong, arginfo_stub_issue2666_divlonglong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, divLongDouble, arginfo_stub_issue2666_divlongdouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, divDoubleLong, arginfo_stub_issue2666_divdoublelong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, divDoubleDouble, arginfo_stub_issue2666_divdoubledouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, divVarLong, arginfo_stub_issue2666_divvarlong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, divVarDouble, arginfo_stub_issue2666_divvardouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, divLongVar, arginfo_stub_issue2666_divlongvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, divDoubleVar, arginfo_stub_issue2666_divdoublevar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, modLongLong, arginfo_stub_issue2666_modlonglong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, modLongDouble, arginfo_stub_issue2666_modlongdouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, modDoubleLong, arginfo_stub_issue2666_moddoublelong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, modDoubleDouble, arginfo_stub_issue2666_moddoubledouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, modVarLong, arginfo_stub_issue2666_modvarlong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, modVarDouble, arginfo_stub_issue2666_modvardouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, modLongVar, arginfo_stub_issue2666_modlongvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, modDoubleVar, arginfo_stub_issue2666_moddoublevar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, divLiteralDoubleByDouble, arginfo_stub_issue2666_divliteraldoublebydouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2666, modLiteralDoubleByDouble, arginfo_stub_issue2666_modliteraldoublebydouble, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
