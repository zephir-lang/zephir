
extern zend_class_entry *stub_issue2675_ce;

ZEPHIR_INIT_CLASS(Stub_Issue2675);

PHP_METHOD(Stub_Issue2675, divLongLong);
PHP_METHOD(Stub_Issue2675, divLongVar);
PHP_METHOD(Stub_Issue2675, divVarLong);
PHP_METHOD(Stub_Issue2675, divVarVar);
PHP_METHOD(Stub_Issue2675, divLongDouble);
PHP_METHOD(Stub_Issue2675, divDoubleLong);
PHP_METHOD(Stub_Issue2675, divDoubleDouble);
PHP_METHOD(Stub_Issue2675, divVarDouble);
PHP_METHOD(Stub_Issue2675, divDoubleVar);
PHP_METHOD(Stub_Issue2675, divVarLiteralDouble);
PHP_METHOD(Stub_Issue2675, divLiteralExact);
PHP_METHOD(Stub_Issue2675, divLiteralInexact);
PHP_METHOD(Stub_Issue2675, divLongBool);
PHP_METHOD(Stub_Issue2675, divBoolLong);
PHP_METHOD(Stub_Issue2675, divVarBool);
PHP_METHOD(Stub_Issue2675, divBoolVar);
PHP_METHOD(Stub_Issue2675, divDoubleBool);
PHP_METHOD(Stub_Issue2675, divBoolDouble);
PHP_METHOD(Stub_Issue2675, divBoolBool);
PHP_METHOD(Stub_Issue2675, divLongByTrue);
PHP_METHOD(Stub_Issue2675, divTrueByLong);
PHP_METHOD(Stub_Issue2675, divInferredLocal);
PHP_METHOD(Stub_Issue2675, divReturnDouble);
PHP_METHOD(Stub_Issue2675, divTypedDouble);
PHP_METHOD(Stub_Issue2675, divTypedLong);
PHP_METHOD(Stub_Issue2675, divChained);
PHP_METHOD(Stub_Issue2675, divAssignVarLong);
PHP_METHOD(Stub_Issue2675, divAssignVarVar);
PHP_METHOD(Stub_Issue2675, divAssignInferredLocal);
PHP_METHOD(Stub_Issue2675, divAssignProperty);
PHP_METHOD(Stub_Issue2675, divAssignPropertyVar);
PHP_METHOD(Stub_Issue2675, divAssignPropertyLiteral);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divlonglong, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divlongvar, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divvarlong, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divvarvar, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divlongdouble, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divdoublelong, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divdoubledouble, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divvardouble, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divdoublevar, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divvarliteraldouble, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divliteralexact, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divliteralinexact, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divlongbool, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divboollong, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divvarbool, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_TYPE_INFO(0, b, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divboolvar, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divdoublebool, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, b, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divbooldouble, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divboolbool, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, b, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divlongbytrue, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divtruebylong, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divinferredlocal, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_issue2675_divreturndouble, 0, 2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divtypeddouble, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divtypedlong, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divchained, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divassignvarlong, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divassignvarvar, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divassigninferredlocal, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divassignproperty, 0, 0, 2)
	ZEND_ARG_INFO(0, obj)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divassignpropertyvar, 0, 0, 2)
	ZEND_ARG_INFO(0, obj)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_issue2675_divassignpropertyliteral, 0, 0, 1)
	ZEND_ARG_INFO(0, obj)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_issue2675_method_entry) {
	PHP_ME(Stub_Issue2675, divLongLong, arginfo_stub_issue2675_divlonglong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divLongVar, arginfo_stub_issue2675_divlongvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divVarLong, arginfo_stub_issue2675_divvarlong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divVarVar, arginfo_stub_issue2675_divvarvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divLongDouble, arginfo_stub_issue2675_divlongdouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divDoubleLong, arginfo_stub_issue2675_divdoublelong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divDoubleDouble, arginfo_stub_issue2675_divdoubledouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divVarDouble, arginfo_stub_issue2675_divvardouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divDoubleVar, arginfo_stub_issue2675_divdoublevar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divVarLiteralDouble, arginfo_stub_issue2675_divvarliteraldouble, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2675, divLiteralExact, arginfo_stub_issue2675_divliteralexact, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Issue2675, divLiteralInexact, arginfo_stub_issue2675_divliteralinexact, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divLongBool, arginfo_stub_issue2675_divlongbool, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divBoolLong, arginfo_stub_issue2675_divboollong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divVarBool, arginfo_stub_issue2675_divvarbool, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divBoolVar, arginfo_stub_issue2675_divboolvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divDoubleBool, arginfo_stub_issue2675_divdoublebool, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divBoolDouble, arginfo_stub_issue2675_divbooldouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divBoolBool, arginfo_stub_issue2675_divboolbool, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divLongByTrue, arginfo_stub_issue2675_divlongbytrue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divTrueByLong, arginfo_stub_issue2675_divtruebylong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divInferredLocal, arginfo_stub_issue2675_divinferredlocal, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divReturnDouble, arginfo_stub_issue2675_divreturndouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divTypedDouble, arginfo_stub_issue2675_divtypeddouble, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divTypedLong, arginfo_stub_issue2675_divtypedlong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divChained, arginfo_stub_issue2675_divchained, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divAssignVarLong, arginfo_stub_issue2675_divassignvarlong, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divAssignVarVar, arginfo_stub_issue2675_divassignvarvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divAssignInferredLocal, arginfo_stub_issue2675_divassigninferredlocal, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divAssignProperty, arginfo_stub_issue2675_divassignproperty, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divAssignPropertyVar, arginfo_stub_issue2675_divassignpropertyvar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Issue2675, divAssignPropertyLiteral, arginfo_stub_issue2675_divassignpropertyliteral, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
