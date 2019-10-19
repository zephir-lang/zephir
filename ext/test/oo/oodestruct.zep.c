
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
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/file.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/exception.h"
#include "kernel/concat.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/string.h"


/**
 * Class with dynamic new
 */
ZEPHIR_INIT_CLASS(Test_Oo_OoDestruct) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, OoDestruct, test, oo_oodestruct, test_oo_oodestruct_method_entry, 0);

	zend_declare_property_bool(test_oo_oodestruct_ce, SL("checked"), 0, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);

	zend_declare_property_null(test_oo_oodestruct_ce, SL("file"), ZEND_ACC_PROTECTED);

	/**
	 * Image height
	 *
	 * @var int
	 */
	zend_declare_property_null(test_oo_oodestruct_ce, SL("height"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_oo_oodestruct_ce, SL("image"), ZEND_ACC_PROTECTED);

	/**
	 * Image mime type
	 *
	 * @var string
	 */
	zend_declare_property_null(test_oo_oodestruct_ce, SL("mime"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(test_oo_oodestruct_ce, SL("realpath"), ZEND_ACC_PROTECTED);

	/**
	 * Image type
	 *
	 * Driver dependent
	 *
	 * @var int
	 */
	zend_declare_property_null(test_oo_oodestruct_ce, SL("type"), ZEND_ACC_PROTECTED);

	/**
	 * Image width
	 *
	 * @var int
	 */
	zend_declare_property_null(test_oo_oodestruct_ce, SL("width"), ZEND_ACC_PROTECTED);

	return SUCCESS;

}

/**
 *
 *
 * Image height
 *
 */
PHP_METHOD(Test_Oo_OoDestruct, getHeight) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "height");

}

PHP_METHOD(Test_Oo_OoDestruct, getImage) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "image");

}

/**
 *
 *
 * Image mime type
 *
 */
PHP_METHOD(Test_Oo_OoDestruct, getMime) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "mime");

}

PHP_METHOD(Test_Oo_OoDestruct, getRealpath) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "realpath");

}

/**
 *
 *
 * Image type
 * 
 * 
 * 
 * Driver dependent
 *
 */
PHP_METHOD(Test_Oo_OoDestruct, getType) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "type");

}

/**
 *
 *
 * Image width
 *
 */
PHP_METHOD(Test_Oo_OoDestruct, getWidth) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "width");

}

PHP_METHOD(Test_Oo_OoDestruct, __construct) {

	zend_bool _30$$13;
	zend_class_entry *_26$$11 = NULL, *_22$$12 = NULL, *_32$$14 = NULL;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_29 = NULL;
	zend_long width, height, ZEPHIR_LAST_CALL_STATUS;
	zval *file_param = NULL, *width_param = NULL, *height_param = NULL, __$true, imageinfo, _0, _1, _2$$4, _3$$4, _4$$4, _9$$4, _28$$4, _5$$5, _6$$5, _7$$5, _8$$5, _10$$6, _11$$6, _12$$7, _13$$7, _14$$8, _15$$8, _16$$9, _17$$9, _18$$10, _19$$10, _20$$11, _25$$11, _27$$11, _21$$12, _23$$12, _24$$12, _35$$13, _36$$13, _37$$13, _38$$13, _39$$13, _40$$13, _31$$14, _33$$14, _34$$14;
	zval file;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&file);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&imageinfo);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2$$4);
	ZVAL_UNDEF(&_3$$4);
	ZVAL_UNDEF(&_4$$4);
	ZVAL_UNDEF(&_9$$4);
	ZVAL_UNDEF(&_28$$4);
	ZVAL_UNDEF(&_5$$5);
	ZVAL_UNDEF(&_6$$5);
	ZVAL_UNDEF(&_7$$5);
	ZVAL_UNDEF(&_8$$5);
	ZVAL_UNDEF(&_10$$6);
	ZVAL_UNDEF(&_11$$6);
	ZVAL_UNDEF(&_12$$7);
	ZVAL_UNDEF(&_13$$7);
	ZVAL_UNDEF(&_14$$8);
	ZVAL_UNDEF(&_15$$8);
	ZVAL_UNDEF(&_16$$9);
	ZVAL_UNDEF(&_17$$9);
	ZVAL_UNDEF(&_18$$10);
	ZVAL_UNDEF(&_19$$10);
	ZVAL_UNDEF(&_20$$11);
	ZVAL_UNDEF(&_25$$11);
	ZVAL_UNDEF(&_27$$11);
	ZVAL_UNDEF(&_21$$12);
	ZVAL_UNDEF(&_23$$12);
	ZVAL_UNDEF(&_24$$12);
	ZVAL_UNDEF(&_35$$13);
	ZVAL_UNDEF(&_36$$13);
	ZVAL_UNDEF(&_37$$13);
	ZVAL_UNDEF(&_38$$13);
	ZVAL_UNDEF(&_39$$13);
	ZVAL_UNDEF(&_40$$13);
	ZVAL_UNDEF(&_31$$14);
	ZVAL_UNDEF(&_33$$14);
	ZVAL_UNDEF(&_34$$14);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 2, &file_param, &width_param, &height_param);

	if (UNEXPECTED(Z_TYPE_P(file_param) != IS_STRING && Z_TYPE_P(file_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'file' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(file_param) == IS_STRING)) {
		zephir_get_strval(&file, file_param);
	} else {
		ZEPHIR_INIT_VAR(&file);
		ZVAL_EMPTY_STRING(&file);
	}
	if (!width_param) {
		width = 0;
	} else {
		width = zephir_get_intval(width_param);
	}
	if (!height_param) {
		height = 0;
	} else {
		height = zephir_get_intval(height_param);
	}


	zephir_read_static_property_ce(&_0, test_oo_oodestruct_ce, SL("checked"), PH_NOISY_CC | PH_READONLY);
	if (!(zephir_is_true(&_0))) {
		ZEPHIR_CALL_SELF(NULL, "check", NULL, 0);
		zephir_check_call_status();
	}
	zephir_update_property_zval(this_ptr, SL("file"), &file);
	zephir_read_property(&_1, this_ptr, SL("file"), PH_NOISY_CC | PH_READONLY);
	if ((zephir_file_exists(&_1) == SUCCESS)) {
		zephir_read_property(&_2$$4, this_ptr, SL("file"), PH_NOISY_CC | PH_READONLY);
		ZEPHIR_CALL_FUNCTION(&_3$$4, "realpath", NULL, 58, &_2$$4);
		zephir_check_call_status();
		zephir_update_property_zval(this_ptr, SL("realpath"), &_3$$4);
		zephir_read_property(&_4$$4, this_ptr, SL("file"), PH_NOISY_CC | PH_READONLY);
		ZEPHIR_CALL_FUNCTION(&imageinfo, "getimagesize", NULL, 59, &_4$$4);
		zephir_check_call_status();
		if (zephir_is_true(&imageinfo)) {
			zephir_array_fetch_long(&_5$$5, &imageinfo, 0, PH_NOISY | PH_READONLY, "test/oo/oodestruct.zep", 62);
			zephir_update_property_zval(this_ptr, SL("width"), &_5$$5);
			zephir_array_fetch_long(&_6$$5, &imageinfo, 1, PH_NOISY | PH_READONLY, "test/oo/oodestruct.zep", 63);
			zephir_update_property_zval(this_ptr, SL("height"), &_6$$5);
			zephir_array_fetch_long(&_7$$5, &imageinfo, 2, PH_NOISY | PH_READONLY, "test/oo/oodestruct.zep", 64);
			zephir_update_property_zval(this_ptr, SL("type"), &_7$$5);
			zephir_array_fetch_string(&_8$$5, &imageinfo, SL("mime"), PH_NOISY | PH_READONLY, "test/oo/oodestruct.zep", 65);
			zephir_update_property_zval(this_ptr, SL("mime"), &_8$$5);
		}
		zephir_read_property(&_9$$4, this_ptr, SL("type"), PH_NOISY_CC | PH_READONLY);
		do {
			if (ZEPHIR_IS_LONG(&_9$$4, 1)) {
				zephir_read_property(&_10$$6, this_ptr, SL("file"), PH_NOISY_CC | PH_READONLY);
				ZEPHIR_CALL_FUNCTION(&_11$$6, "imagecreatefromgif", NULL, 60, &_10$$6);
				zephir_check_call_status();
				zephir_update_property_zval(this_ptr, SL("image"), &_11$$6);
				break;
			}
			if (ZEPHIR_IS_LONG(&_9$$4, 2)) {
				zephir_read_property(&_12$$7, this_ptr, SL("file"), PH_NOISY_CC | PH_READONLY);
				ZEPHIR_CALL_FUNCTION(&_13$$7, "imagecreatefromjpeg", NULL, 61, &_12$$7);
				zephir_check_call_status();
				zephir_update_property_zval(this_ptr, SL("image"), &_13$$7);
				break;
			}
			if (ZEPHIR_IS_LONG(&_9$$4, 3)) {
				zephir_read_property(&_14$$8, this_ptr, SL("file"), PH_NOISY_CC | PH_READONLY);
				ZEPHIR_CALL_FUNCTION(&_15$$8, "imagecreatefrompng", NULL, 62, &_14$$8);
				zephir_check_call_status();
				zephir_update_property_zval(this_ptr, SL("image"), &_15$$8);
				break;
			}
			if (ZEPHIR_IS_LONG(&_9$$4, 15)) {
				zephir_read_property(&_16$$9, this_ptr, SL("file"), PH_NOISY_CC | PH_READONLY);
				ZEPHIR_CALL_FUNCTION(&_17$$9, "imagecreatefromwbmp", NULL, 63, &_16$$9);
				zephir_check_call_status();
				zephir_update_property_zval(this_ptr, SL("image"), &_17$$9);
				break;
			}
			if (ZEPHIR_IS_LONG(&_9$$4, 16)) {
				zephir_read_property(&_18$$10, this_ptr, SL("file"), PH_NOISY_CC | PH_READONLY);
				ZEPHIR_CALL_FUNCTION(&_19$$10, "imagecreatefromxbm", NULL, 64, &_18$$10);
				zephir_check_call_status();
				zephir_update_property_zval(this_ptr, SL("image"), &_19$$10);
				break;
			}
			zephir_read_property(&_20$$11, this_ptr, SL("mime"), PH_NOISY_CC | PH_READONLY);
			if (zephir_is_true(&_20$$11)) {
				ZEPHIR_INIT_VAR(&_21$$12);
				if (!_22$$12) {
				_22$$12 = zephir_fetch_class_str_ex(SL("Test\\Oo\\Exception"), ZEND_FETCH_CLASS_AUTO);
				}
				object_init_ex(&_21$$12, _22$$12);
				if (zephir_has_constructor(&_21$$12)) {
					zephir_read_property(&_23$$12, this_ptr, SL("mime"), PH_NOISY_CC | PH_READONLY);
					ZEPHIR_INIT_VAR(&_24$$12);
					ZEPHIR_CONCAT_SVS(&_24$$12, "Installed GD does not support ", &_23$$12, " images");
					ZEPHIR_CALL_METHOD(NULL, &_21$$12, "__construct", NULL, 0, &_24$$12);
					zephir_check_call_status();
				}
				zephir_throw_exception_debug(&_21$$12, "test/oo/oodestruct.zep", 93);
				ZEPHIR_MM_RESTORE();
				return;
			}
			ZEPHIR_INIT_VAR(&_25$$11);
			if (!_26$$11) {
			_26$$11 = zephir_fetch_class_str_ex(SL("Test\\Oo\\Exception"), ZEND_FETCH_CLASS_AUTO);
			}
			object_init_ex(&_25$$11, _26$$11);
			if (zephir_has_constructor(&_25$$11)) {
				ZEPHIR_INIT_VAR(&_27$$11);
				ZVAL_STRING(&_27$$11, "Installed GD does not support such images");
				ZEPHIR_CALL_METHOD(NULL, &_25$$11, "__construct", NULL, 0, &_27$$11);
				zephir_check_call_status();
			}
			zephir_throw_exception_debug(&_25$$11, "test/oo/oodestruct.zep", 98);
			ZEPHIR_MM_RESTORE();
			return;
		} while(0);

		zephir_read_property(&_28$$4, this_ptr, SL("image"), PH_NOISY_CC | PH_READONLY);
		ZEPHIR_CALL_FUNCTION(NULL, "imagesavealpha", &_29, 65, &_28$$4, &__$true);
		zephir_check_call_status();
	} else {
		_30$$13 = !width;
		if (!(_30$$13)) {
			_30$$13 = !height;
		}
		if (UNEXPECTED(_30$$13)) {
			ZEPHIR_INIT_VAR(&_31$$14);
			if (!_32$$14) {
			_32$$14 = zephir_fetch_class_str_ex(SL("Test\\Oo\\Exception"), ZEND_FETCH_CLASS_AUTO);
			}
			object_init_ex(&_31$$14, _32$$14);
			if (zephir_has_constructor(&_31$$14)) {
				zephir_read_property(&_33$$14, this_ptr, SL("file"), PH_NOISY_CC | PH_READONLY);
				ZEPHIR_INIT_VAR(&_34$$14);
				ZEPHIR_CONCAT_SV(&_34$$14, "Failed to create image from file ", &_33$$14);
				ZEPHIR_CALL_METHOD(NULL, &_31$$14, "__construct", NULL, 0, &_34$$14);
				zephir_check_call_status();
			}
			zephir_throw_exception_debug(&_31$$14, "test/oo/oodestruct.zep", 107);
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZVAL_LONG(&_35$$13, width);
		ZVAL_LONG(&_36$$13, height);
		ZEPHIR_CALL_FUNCTION(&_37$$13, "imagecreatetruecolor", NULL, 66, &_35$$13, &_36$$13);
		zephir_check_call_status();
		zephir_update_property_zval(this_ptr, SL("image"), &_37$$13);
		zephir_read_property(&_35$$13, this_ptr, SL("image"), PH_NOISY_CC | PH_READONLY);
		ZEPHIR_CALL_FUNCTION(NULL, "imagealphablending", NULL, 67, &_35$$13, &__$true);
		zephir_check_call_status();
		zephir_read_property(&_36$$13, this_ptr, SL("image"), PH_NOISY_CC | PH_READONLY);
		ZEPHIR_CALL_FUNCTION(NULL, "imagesavealpha", &_29, 65, &_36$$13, &__$true);
		zephir_check_call_status();
		zephir_read_property(&_38$$13, this_ptr, SL("file"), PH_NOISY_CC | PH_READONLY);
		zephir_update_property_zval(this_ptr, SL("realpath"), &_38$$13);
		ZEPHIR_INIT_ZVAL_NREF(_39$$13);
		ZVAL_LONG(&_39$$13, width);
		zephir_update_property_zval(this_ptr, SL("width"), &_39$$13);
		ZEPHIR_INIT_ZVAL_NREF(_39$$13);
		ZVAL_LONG(&_39$$13, height);
		zephir_update_property_zval(this_ptr, SL("height"), &_39$$13);
		ZEPHIR_INIT_ZVAL_NREF(_39$$13);
		ZVAL_LONG(&_39$$13, 3);
		zephir_update_property_zval(this_ptr, SL("type"), &_39$$13);
		ZEPHIR_INIT_VAR(&_40$$13);
		ZEPHIR_INIT_NVAR(&_40$$13);
		ZVAL_STRING(&_40$$13, "image/png");
		zephir_update_property_zval(this_ptr, SL("mime"), &_40$$13);
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_OoDestruct, __destruct) {

	zval image, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&image);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	zephir_read_property(&_0, this_ptr, SL("image"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_CPY_WRT(&image, &_0);
	if (Z_TYPE_P(&image) == IS_RESOURCE) {
		ZEPHIR_CALL_FUNCTION(NULL, "imagedestroy", NULL, 68, &image);
		zephir_check_call_status();
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_OoDestruct, check) {

	zend_class_entry *_2$$4 = NULL, *_8$$5 = NULL;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$true, version, _0, _4, _5, _6, _10, _1$$4, _3$$4, _7$$5, _9$$5;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&version);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&_10);
	ZVAL_UNDEF(&_1$$4);
	ZVAL_UNDEF(&_3$$4);
	ZVAL_UNDEF(&_7$$5);
	ZVAL_UNDEF(&_9$$5);

	ZEPHIR_MM_GROW();

	zephir_read_static_property_ce(&_0, test_oo_oodestruct_ce, SL("checked"), PH_NOISY_CC | PH_READONLY);
	if (zephir_is_true(&_0)) {
		RETURN_MM_BOOL(1);
	}
	if (UNEXPECTED(!((zephir_function_exists_ex(SL("gd_info")) == SUCCESS)))) {
		ZEPHIR_INIT_VAR(&_1$$4);
		if (!_2$$4) {
		_2$$4 = zephir_fetch_class_str_ex(SL("Test\\Oo\\Exception"), ZEND_FETCH_CLASS_AUTO);
		}
		object_init_ex(&_1$$4, _2$$4);
		if (zephir_has_constructor(&_1$$4)) {
			ZEPHIR_INIT_VAR(&_3$$4);
			ZVAL_STRING(&_3$$4, "GD is either not installed or not enabled, check your configuration");
			ZEPHIR_CALL_METHOD(NULL, &_1$$4, "__construct", NULL, 0, &_3$$4);
			zephir_check_call_status();
		}
		zephir_throw_exception_debug(&_1$$4, "test/oo/oodestruct.zep", 146);
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CALL_SELF(&version, "getversion", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_STRING(&_4, "2.0.1");
	ZEPHIR_INIT_VAR(&_5);
	ZVAL_STRING(&_5, ">=");
	ZEPHIR_CALL_FUNCTION(&_6, "version_compare", NULL, 69, &version, &_4, &_5);
	zephir_check_call_status();
	if (UNEXPECTED(!zephir_is_true(&_6))) {
		ZEPHIR_INIT_VAR(&_7$$5);
		if (!_8$$5) {
		_8$$5 = zephir_fetch_class_str_ex(SL("Test\\Oo\\Exception"), ZEND_FETCH_CLASS_AUTO);
		}
		object_init_ex(&_7$$5, _8$$5);
		if (zephir_has_constructor(&_7$$5)) {
			ZEPHIR_INIT_VAR(&_9$$5);
			ZEPHIR_CONCAT_SV(&_9$$5, "Requires GD version '2.0.1' or greater, you have ", &version);
			ZEPHIR_CALL_METHOD(NULL, &_7$$5, "__construct", NULL, 0, &_9$$5);
			zephir_check_call_status();
		}
		zephir_throw_exception_debug(&_7$$5, "test/oo/oodestruct.zep", 154);
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_static_property_ce(test_oo_oodestruct_ce, ZEND_STRL("checked"), &__$true);
	zephir_read_static_property_ce(&_10, test_oo_oodestruct_ce, SL("checked"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_10);

}

PHP_METHOD(Test_Oo_OoDestruct, getVersion) {

	zend_class_entry *_1$$3 = NULL;
	zval version, info, matches, _3, _4, _0$$3, _2$$3, _5$$5, _6$$5, _7$$5, _8$$5;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&version);
	ZVAL_UNDEF(&info);
	ZVAL_UNDEF(&matches);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_5$$5);
	ZVAL_UNDEF(&_6$$5);
	ZVAL_UNDEF(&_7$$5);
	ZVAL_UNDEF(&_8$$5);

	ZEPHIR_MM_GROW();

	if (UNEXPECTED(!((zephir_function_exists_ex(SL("gd_info")) == SUCCESS)))) {
		ZEPHIR_INIT_VAR(&_0$$3);
		if (!_1$$3) {
		_1$$3 = zephir_fetch_class_str_ex(SL("Test\\Oo\\Exception"), ZEND_FETCH_CLASS_AUTO);
		}
		object_init_ex(&_0$$3, _1$$3);
		if (zephir_has_constructor(&_0$$3)) {
			ZEPHIR_INIT_VAR(&_2$$3);
			ZVAL_STRING(&_2$$3, "GD is either not installed or not enabled, check your configuration");
			ZEPHIR_CALL_METHOD(NULL, &_0$$3, "__construct", NULL, 0, &_2$$3);
			zephir_check_call_status();
		}
		zephir_throw_exception_debug(&_0$$3, "test/oo/oodestruct.zep", 171);
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&version);
	ZVAL_NULL(&version);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_STRING(&_3, "GD_VERSION");
	ZEPHIR_CALL_FUNCTION(&_4, "defined", NULL, 70, &_3);
	zephir_check_call_status();
	if (zephir_is_true(&_4)) {
		ZEPHIR_INIT_NVAR(&version);
		ZEPHIR_GET_CONSTANT(&version, "GD_VERSION");
	} else {
		ZEPHIR_CALL_FUNCTION(&info, "gd_info", NULL, 71);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(&matches);
		ZVAL_NULL(&matches);
		zephir_array_fetch_string(&_5$$5, &info, SL("GD Version"), PH_NOISY | PH_READONLY, "test/oo/oodestruct.zep", 182);
		ZEPHIR_INIT_VAR(&_6$$5);
		ZVAL_STRING(&_6$$5, "/\\d+\\.\\d+(?:\\.\\d+)?/");
		ZEPHIR_INIT_VAR(&_7$$5);
		ZEPHIR_INIT_VAR(&_8$$5);
		ZVAL_STRING(&_8$$5, "/\\d+\\.\\d+(?:\\.\\d+)?/");
		zephir_preg_match(&_7$$5, &_8$$5, &_5$$5, &matches, 0, 0 , 0 );
		if (zephir_is_true(&_7$$5)) {
			ZEPHIR_OBS_NVAR(&version);
			zephir_array_fetch_long(&version, &matches, 0, PH_NOISY, "test/oo/oodestruct.zep", 183);
		}
	}
	RETURN_CCTOR(&version);

}

