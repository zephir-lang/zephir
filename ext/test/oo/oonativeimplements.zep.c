
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
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/operators.h"


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

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


}

PHP_METHOD(Test_Oo_OoNativeImplements, current) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


}

PHP_METHOD(Test_Oo_OoNativeImplements, key) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


}

PHP_METHOD(Test_Oo_OoNativeImplements, next) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


}

PHP_METHOD(Test_Oo_OoNativeImplements, rewind) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


}

PHP_METHOD(Test_Oo_OoNativeImplements, valid) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


}

PHP_METHOD(Test_Oo_OoNativeImplements, getInnerIterator) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


}

PHP_METHOD(Test_Oo_OoNativeImplements, getChildren) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


}

PHP_METHOD(Test_Oo_OoNativeImplements, hasChildren) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


}

PHP_METHOD(Test_Oo_OoNativeImplements, seek) {

	zval *position, position_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&position_sub);

	zephir_fetch_params(1, 1, 0, &position);




}

PHP_METHOD(Test_Oo_OoNativeImplements, getIterator) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


}

PHP_METHOD(Test_Oo_OoNativeImplements, offsetExists) {

	zval *offset, offset_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&offset_sub);

	zephir_fetch_params(1, 1, 0, &offset);




}

PHP_METHOD(Test_Oo_OoNativeImplements, offsetGet) {

	zval *offset, offset_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&offset_sub);

	zephir_fetch_params(1, 1, 0, &offset);




}

PHP_METHOD(Test_Oo_OoNativeImplements, offsetSet) {

	zval *offset, offset_sub, *value, value_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&offset_sub);
	ZVAL_UNDEF(&value_sub);

	zephir_fetch_params(1, 2, 0, &offset, &value);




}

PHP_METHOD(Test_Oo_OoNativeImplements, offsetUnset) {

	zval *offset, offset_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&offset_sub);

	zephir_fetch_params(1, 1, 0, &offset);




}

PHP_METHOD(Test_Oo_OoNativeImplements, serialize) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


}

PHP_METHOD(Test_Oo_OoNativeImplements, unserialize) {

	zval *serialized_param = NULL;
	zval serialized;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&serialized);

	zephir_fetch_params(1, 1, 0, &serialized_param);

	zephir_get_strval(&serialized, serialized_param);



}

