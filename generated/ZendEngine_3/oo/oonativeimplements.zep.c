
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "ext/spl/spl_iterators.h"
#include "kernel/operators.h"
#include "kernel/memory.h"


/**
 * Class with constructor
 */
ZEPHIR_INIT_CLASS(Test_Oo_OoNativeImplements) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, OoNativeImplements, test, oo_oonativeimplements, test_oo_oonativeimplements_method_entry, 0);

	zend_class_implements(test_oo_oonativeimplements_ce TSRMLS_CC, 1, spl_ce_Countable);
	zend_class_implements(test_oo_oonativeimplements_ce TSRMLS_CC, 1, zend_ce_iterator);
	zend_class_implements(test_oo_oonativeimplements_ce TSRMLS_CC, 1, spl_ce_OuterIterator);
	zend_class_implements(test_oo_oonativeimplements_ce TSRMLS_CC, 1, spl_ce_RecursiveIterator);
	zend_class_implements(test_oo_oonativeimplements_ce TSRMLS_CC, 1, spl_ce_SeekableIterator);
	zend_class_implements(test_oo_oonativeimplements_ce TSRMLS_CC, 1, zend_ce_aggregate);
	return SUCCESS;

}

PHP_METHOD(Test_Oo_OoNativeImplements, count) {

	ZEPHIR_INIT_THIS();



}

PHP_METHOD(Test_Oo_OoNativeImplements, current) {

	ZEPHIR_INIT_THIS();



}

PHP_METHOD(Test_Oo_OoNativeImplements, key) {

	ZEPHIR_INIT_THIS();



}

PHP_METHOD(Test_Oo_OoNativeImplements, next) {

	ZEPHIR_INIT_THIS();



}

PHP_METHOD(Test_Oo_OoNativeImplements, rewind) {

	ZEPHIR_INIT_THIS();



}

PHP_METHOD(Test_Oo_OoNativeImplements, valid) {

	ZEPHIR_INIT_THIS();



}

PHP_METHOD(Test_Oo_OoNativeImplements, getInnerIterator) {

	ZEPHIR_INIT_THIS();



}

PHP_METHOD(Test_Oo_OoNativeImplements, getChildren) {

	ZEPHIR_INIT_THIS();



}

PHP_METHOD(Test_Oo_OoNativeImplements, hasChildren) {

	ZEPHIR_INIT_THIS();



}

PHP_METHOD(Test_Oo_OoNativeImplements, seek) {

	zval *position_param = NULL;
	int position;
	ZEPHIR_INIT_THIS();


	zephir_fetch_params(0, 1, 0, &position_param);

	position = zephir_get_intval(position_param);



}

PHP_METHOD(Test_Oo_OoNativeImplements, getIterator) {

	ZEPHIR_INIT_THIS();



}

PHP_METHOD(Test_Oo_OoNativeImplements, offsetExists) {

	zval *offset, offset_sub;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&offset_sub);

	zephir_fetch_params(0, 1, 0, &offset);




}

PHP_METHOD(Test_Oo_OoNativeImplements, offsetGet) {

	zval *offset, offset_sub;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&offset_sub);

	zephir_fetch_params(0, 1, 0, &offset);




}

PHP_METHOD(Test_Oo_OoNativeImplements, offsetSet) {

	zval *offset, offset_sub, *value, value_sub;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&offset_sub);
	ZVAL_UNDEF(&value_sub);

	zephir_fetch_params(0, 2, 0, &offset, &value);




}

PHP_METHOD(Test_Oo_OoNativeImplements, offsetUnset) {

	zval *offset, offset_sub;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&offset_sub);

	zephir_fetch_params(0, 1, 0, &offset);




}

PHP_METHOD(Test_Oo_OoNativeImplements, serialize) {

	ZEPHIR_INIT_THIS();



}

PHP_METHOD(Test_Oo_OoNativeImplements, unserialize) {

	zval *serialized_param = NULL;
	zval serialized;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&serialized);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &serialized_param);

	zephir_get_strval(&serialized, serialized_param);



}

