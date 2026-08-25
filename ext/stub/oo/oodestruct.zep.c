
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
#include "kernel/string.h"


/**
 * Class with dynamic new
 */
ZEPHIR_INIT_CLASS(Stub_Oo_OoDestruct)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Oo, OoDestruct, stub, oo_oodestruct, stub_oo_oodestruct_method_entry, 0);

	zend_declare_property_bool(stub_oo_oodestruct_ce, SL("checked"), 0, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_null(stub_oo_oodestruct_ce, SL("file"), ZEND_ACC_PROTECTED);
	/**
	 * Image height
	 *
	 * @var int
	 */
	zend_declare_property_null(stub_oo_oodestruct_ce, SL("height"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_oo_oodestruct_ce, SL("image"), ZEND_ACC_PROTECTED);
	/**
	 * Image mime type
	 *
	 * @var string
	 */
	zend_declare_property_null(stub_oo_oodestruct_ce, SL("mime"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_oo_oodestruct_ce, SL("realpath"), ZEND_ACC_PROTECTED);
	/**
	 * Image type
	 *
	 * Driver dependent
	 *
	 * @var int
	 */
	zend_declare_property_null(stub_oo_oodestruct_ce, SL("type"), ZEND_ACC_PROTECTED);
	/**
	 * Image width
	 *
	 * @var int
	 */
	zend_declare_property_null(stub_oo_oodestruct_ce, SL("width"), ZEND_ACC_PROTECTED);
	return SUCCESS;
}

/**
 * Image height
 */
PHP_METHOD(Stub_Oo_OoDestruct, getHeight)
{

	RETURN_MEMBER_TYPED(getThis(), "height", IS_LONG);
}

PHP_METHOD(Stub_Oo_OoDestruct, getImage)
{

	RETURN_MEMBER(getThis(), "image");
}

/**
 * Image mime type
 */
PHP_METHOD(Stub_Oo_OoDestruct, getMime)
{

	RETURN_MEMBER_TYPED(getThis(), "mime", IS_STRING);
}

PHP_METHOD(Stub_Oo_OoDestruct, getRealpath)
{

	RETURN_MEMBER(getThis(), "realpath");
}

/**
 * Image type
 *
 *
 * Driver dependent
 *
 */
PHP_METHOD(Stub_Oo_OoDestruct, getType)
{

	RETURN_MEMBER_TYPED(getThis(), "type", IS_LONG);
}

/**
 * Image width
 */
PHP_METHOD(Stub_Oo_OoDestruct, getWidth)
{

	RETURN_MEMBER_TYPED(getThis(), "width", IS_LONG);
}

PHP_METHOD(Stub_Oo_OoDestruct, __construct)
{
	zend_bool _30$$13;
	zend_class_entry *_26$$11 = NULL, *_22$$12 = NULL, *_32$$14 = NULL;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long width, height, ZEPHIR_LAST_CALL_STATUS;
	zval file_zv, *width_param = NULL, *height_param = NULL, __$true, imageinfo, _0, _1, _2$$4, _3$$4, _4$$4, _9$$4, _28$$4, _29$$4, _5$$5, _6$$5, _7$$5, _8$$5, _10$$6, _11$$6, _12$$7, _13$$7, _14$$8, _15$$8, _16$$9, _17$$9, _18$$10, _19$$10, _20$$11, _25$$11, _27$$11, _21$$12, _23$$12, _24$$12, _35$$13, _36$$13, _37$$13, _38$$13, _39$$13, _40$$13, _31$$14, _33$$14, _34$$14;
	zend_string *file = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&file_zv);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&imageinfo);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2$$4);
	ZVAL_UNDEF(&_3$$4);
	ZVAL_UNDEF(&_4$$4);
	ZVAL_UNDEF(&_9$$4);
	ZVAL_UNDEF(&_28$$4);
	ZVAL_UNDEF(&_29$$4);
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
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	static zend_string *_zephir_prop_2 = NULL;
	static zend_string *_zephir_prop_3 = NULL;
	static zend_string *_zephir_prop_4 = NULL;
	static zend_string *_zephir_prop_5 = NULL;
	static zend_string *_zephir_prop_6 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("file", 4, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("realpath", 8, 1);
	}
	if (UNEXPECTED(!_zephir_prop_2)) {
		_zephir_prop_2 = zend_string_init("width", 5, 1);
	}
	if (UNEXPECTED(!_zephir_prop_3)) {
		_zephir_prop_3 = zend_string_init("height", 6, 1);
	}
	if (UNEXPECTED(!_zephir_prop_4)) {
		_zephir_prop_4 = zend_string_init("type", 4, 1);
	}
	if (UNEXPECTED(!_zephir_prop_5)) {
		_zephir_prop_5 = zend_string_init("mime", 4, 1);
	}
	if (UNEXPECTED(!_zephir_prop_6)) {
		_zephir_prop_6 = zend_string_init("image", 5, 1);
	}

	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_STR(file)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG_OR_NULL(width, is_null_true)
		Z_PARAM_LONG_OR_NULL(height, is_null_true)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	if (ZEND_NUM_ARGS() > 1) {
		width_param = ZEND_CALL_ARG(execute_data, 2);
	}
	if (ZEND_NUM_ARGS() > 2) {
		height_param = ZEND_CALL_ARG(execute_data, 3);
	}
	zephir_memory_observe(&file_zv);
	ZVAL_STR_COPY(&file_zv, file);
	if (!width_param) {
		width = 0;
	} else {
		}
	if (!height_param) {
		height = 0;
	} else {
		}
	zephir_read_static_property_ce(&_0, stub_oo_oodestruct_ce, SL("checked"), PH_NOISY_CC | PH_READONLY);
	if (!(zephir_is_true(&_0))) {
		ZEPHIR_CALL_SELF(NULL, "check", NULL, 0);
		zephir_check_call_status();
	}
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 69, &file_zv);
	zephir_read_property_cached(&_1, this_ptr, _zephir_prop_0, 69, PH_NOISY_CC | PH_READONLY);
	if ((zephir_file_exists(&_1) == SUCCESS)) {
		zephir_read_property_cached(&_2$$4, this_ptr, _zephir_prop_0, 69, PH_NOISY_CC | PH_READONLY);
		ZEPHIR_CALL_FUNCTION(&_3$$4, "realpath", NULL, 82, &_2$$4);
		zephir_check_call_status();
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_1, 70, &_3$$4);
		zephir_read_property_cached(&_4$$4, this_ptr, _zephir_prop_0, 69, PH_NOISY_CC | PH_READONLY);
		ZEPHIR_CALL_FUNCTION(&imageinfo, "getimagesize", NULL, 83, &_4$$4);
		zephir_check_call_status();
		if (zephir_is_true(&imageinfo)) {
			zephir_array_fetch_long(&_5$$5, &imageinfo, 0, PH_NOISY | PH_READONLY, "stub/oo/oodestruct.zep", 62);
			zephir_update_property_zval_cached(this_ptr, _zephir_prop_2, 71, &_5$$5);
			zephir_array_fetch_long(&_6$$5, &imageinfo, 1, PH_NOISY | PH_READONLY, "stub/oo/oodestruct.zep", 63);
			zephir_update_property_zval_cached(this_ptr, _zephir_prop_3, 72, &_6$$5);
			zephir_array_fetch_long(&_7$$5, &imageinfo, 2, PH_NOISY | PH_READONLY, "stub/oo/oodestruct.zep", 64);
			zephir_update_property_zval_cached(this_ptr, _zephir_prop_4, 73, &_7$$5);
			zephir_array_fetch_string(&_8$$5, &imageinfo, SL("mime"), PH_NOISY | PH_READONLY, "stub/oo/oodestruct.zep", 65);
			zephir_update_property_zval_cached(this_ptr, _zephir_prop_5, 74, &_8$$5);
		}
		zephir_read_property_cached(&_9$$4, this_ptr, _zephir_prop_4, 73, PH_NOISY_CC | PH_READONLY);
		if (ZEPHIR_IS_LONG(&_9$$4, 1)) { goto zephir_switch_0_clause_0; }
		if (ZEPHIR_IS_LONG(&_9$$4, 2)) { goto zephir_switch_0_clause_1; }
		if (ZEPHIR_IS_LONG(&_9$$4, 3)) { goto zephir_switch_0_clause_2; }
		if (ZEPHIR_IS_LONG(&_9$$4, 15)) { goto zephir_switch_0_clause_3; }
		if (ZEPHIR_IS_LONG(&_9$$4, 16)) { goto zephir_switch_0_clause_4; }
		goto zephir_switch_0_clause_5;
		zephir_switch_0_clause_0: ;
			zephir_read_property_cached(&_10$$6, this_ptr, _zephir_prop_0, 69, PH_NOISY_CC | PH_READONLY);
			ZEPHIR_CALL_FUNCTION(&_11$$6, "imagecreatefromgif", NULL, 0, &_10$$6);
			zephir_check_call_status();
			zephir_update_property_zval_cached(this_ptr, _zephir_prop_6, 75, &_11$$6);
			goto zephir_switch_0_end;
		zephir_switch_0_clause_1: ;
			zephir_read_property_cached(&_12$$7, this_ptr, _zephir_prop_0, 69, PH_NOISY_CC | PH_READONLY);
			ZEPHIR_CALL_FUNCTION(&_13$$7, "imagecreatefromjpeg", NULL, 0, &_12$$7);
			zephir_check_call_status();
			zephir_update_property_zval_cached(this_ptr, _zephir_prop_6, 75, &_13$$7);
			goto zephir_switch_0_end;
		zephir_switch_0_clause_2: ;
			zephir_read_property_cached(&_14$$8, this_ptr, _zephir_prop_0, 69, PH_NOISY_CC | PH_READONLY);
			ZEPHIR_CALL_FUNCTION(&_15$$8, "imagecreatefrompng", NULL, 0, &_14$$8);
			zephir_check_call_status();
			zephir_update_property_zval_cached(this_ptr, _zephir_prop_6, 75, &_15$$8);
			goto zephir_switch_0_end;
		zephir_switch_0_clause_3: ;
			zephir_read_property_cached(&_16$$9, this_ptr, _zephir_prop_0, 69, PH_NOISY_CC | PH_READONLY);
			ZEPHIR_CALL_FUNCTION(&_17$$9, "imagecreatefromwbmp", NULL, 0, &_16$$9);
			zephir_check_call_status();
			zephir_update_property_zval_cached(this_ptr, _zephir_prop_6, 75, &_17$$9);
			goto zephir_switch_0_end;
		zephir_switch_0_clause_4: ;
			zephir_read_property_cached(&_18$$10, this_ptr, _zephir_prop_0, 69, PH_NOISY_CC | PH_READONLY);
			ZEPHIR_CALL_FUNCTION(&_19$$10, "imagecreatefromxbm", NULL, 0, &_18$$10);
			zephir_check_call_status();
			zephir_update_property_zval_cached(this_ptr, _zephir_prop_6, 75, &_19$$10);
			goto zephir_switch_0_end;
		zephir_switch_0_clause_5: ;
			zephir_read_property_cached(&_20$$11, this_ptr, _zephir_prop_5, 74, PH_NOISY_CC | PH_READONLY);
			if (zephir_is_true(&_20$$11)) {
				ZEPHIR_INIT_VAR(&_21$$12);
				if (!_22$$12) {
				_22$$12 = zephir_fetch_class_str_ex(SL("Stub\\Oo\\Exception"), ZEND_FETCH_CLASS_AUTO);
				}
				object_init_ex(&_21$$12, _22$$12);
				if (zephir_has_constructor(&_21$$12)) {
					zephir_read_property_cached(&_23$$12, this_ptr, _zephir_prop_5, 74, PH_NOISY_CC | PH_READONLY);
					ZEPHIR_INIT_VAR(&_24$$12);
					ZEPHIR_CONCAT_SVS(&_24$$12, "Installed GD does not support ", &_23$$12, " images");
					ZEPHIR_CALL_METHOD(NULL, &_21$$12, "__construct", NULL, 0, &_24$$12);
					zephir_check_call_status();
				}

				zephir_throw_exception_debug(&_21$$12, "stub/oo/oodestruct.zep", 93);
				ZEPHIR_MM_RESTORE();
				return;
			}
			ZEPHIR_INIT_VAR(&_25$$11);
			if (!_26$$11) {
			_26$$11 = zephir_fetch_class_str_ex(SL("Stub\\Oo\\Exception"), ZEND_FETCH_CLASS_AUTO);
			}
			object_init_ex(&_25$$11, _26$$11);
			if (zephir_has_constructor(&_25$$11)) {
				ZEPHIR_INIT_VAR(&_27$$11);
				ZVAL_STRING(&_27$$11, "Installed GD does not support such images");
				ZEPHIR_CALL_METHOD(NULL, &_25$$11, "__construct", NULL, 0, &_27$$11);
				zephir_check_call_status();
			}

			zephir_throw_exception_debug(&_25$$11, "stub/oo/oodestruct.zep", 98);
			ZEPHIR_MM_RESTORE();
			return;
		zephir_switch_0_end: ;

		zephir_read_property_cached(&_28$$4, this_ptr, _zephir_prop_6, 75, PH_NOISY_CC | PH_READONLY);
		ZVAL_BOOL(&_29$$4, 1);
		ZEPHIR_CALL_FUNCTION(NULL, "imagesavealpha", NULL, 0, &_28$$4, &_29$$4);
		zephir_check_call_status();
	} else {
		_30$$13 = !width;
		if (!(_30$$13)) {
			_30$$13 = !height;
		}
		if (UNEXPECTED(_30$$13)) {
			ZEPHIR_INIT_VAR(&_31$$14);
			if (!_32$$14) {
			_32$$14 = zephir_fetch_class_str_ex(SL("Stub\\Oo\\Exception"), ZEND_FETCH_CLASS_AUTO);
			}
			object_init_ex(&_31$$14, _32$$14);
			if (zephir_has_constructor(&_31$$14)) {
				zephir_read_property_cached(&_33$$14, this_ptr, _zephir_prop_0, 69, PH_NOISY_CC | PH_READONLY);
				ZEPHIR_INIT_VAR(&_34$$14);
				ZEPHIR_CONCAT_SV(&_34$$14, "Failed to create image from file ", &_33$$14);
				ZEPHIR_CALL_METHOD(NULL, &_31$$14, "__construct", NULL, 0, &_34$$14);
				zephir_check_call_status();
			}

			zephir_throw_exception_debug(&_31$$14, "stub/oo/oodestruct.zep", 107);
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZVAL_LONG(&_35$$13, width);
		ZVAL_LONG(&_36$$13, height);
		ZEPHIR_CALL_FUNCTION(&_37$$13, "imagecreatetruecolor", NULL, 0, &_35$$13, &_36$$13);
		zephir_check_call_status();
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_6, 75, &_37$$13);
		zephir_read_property_cached(&_35$$13, this_ptr, _zephir_prop_6, 75, PH_NOISY_CC | PH_READONLY);
		ZEPHIR_CALL_FUNCTION(NULL, "imagealphablending", NULL, 84, &_35$$13, &__$true);
		zephir_check_call_status();
		zephir_read_property_cached(&_36$$13, this_ptr, _zephir_prop_6, 75, PH_NOISY_CC | PH_READONLY);
		ZVAL_BOOL(&_38$$13, 1);
		ZEPHIR_CALL_FUNCTION(NULL, "imagesavealpha", NULL, 0, &_36$$13, &_38$$13);
		zephir_check_call_status();
		zephir_read_property_cached(&_38$$13, this_ptr, _zephir_prop_0, 69, PH_NOISY_CC | PH_READONLY);
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_1, 70, &_38$$13);
		ZVAL_UNDEF(&_39$$13);
		ZVAL_LONG(&_39$$13, width);
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_2, 71, &_39$$13);
		ZVAL_UNDEF(&_39$$13);
		ZVAL_LONG(&_39$$13, height);
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_3, 72, &_39$$13);
		ZVAL_UNDEF(&_39$$13);
		ZVAL_LONG(&_39$$13, 3);
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_4, 73, &_39$$13);
		ZEPHIR_INIT_VAR(&_40$$13);
		ZEPHIR_INIT_NVAR(&_40$$13);
		ZVAL_STRING(&_40$$13, "image/png");
		zephir_update_property_zval_cached(this_ptr, _zephir_prop_5, 74, &_40$$13);
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Oo_OoDestruct, __destruct)
{
	zval image, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&image);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("image", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 75, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_CPY_WRT(&image, &_0);
	if (Z_TYPE_P(&image) == IS_RESOURCE) {
		ZEPHIR_CALL_FUNCTION(NULL, "imagedestroy", NULL, 0, &image);
		zephir_check_call_status();
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Oo_OoDestruct, check)
{
	zend_class_entry *_2$$4 = NULL, *_8$$5 = NULL;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$true, version, _0, _4, _5, _6, _10, _1$$4, _3$$4, _7$$5, _9$$5;
	zend_long ZEPHIR_LAST_CALL_STATUS;

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
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_read_static_property_ce(&_0, stub_oo_oodestruct_ce, SL("checked"), PH_NOISY_CC | PH_READONLY);
	if (zephir_is_true(&_0)) {
		RETURN_MM_BOOL(1);
	}
	if (UNEXPECTED(!((zephir_function_exists_ex(ZEND_STRL("gd_info")) == SUCCESS)))) {
		ZEPHIR_INIT_VAR(&_1$$4);
		if (!_2$$4) {
		_2$$4 = zephir_fetch_class_str_ex(SL("Stub\\Oo\\Exception"), ZEND_FETCH_CLASS_AUTO);
		}
		object_init_ex(&_1$$4, _2$$4);
		if (zephir_has_constructor(&_1$$4)) {
			ZEPHIR_INIT_VAR(&_3$$4);
			ZVAL_STRING(&_3$$4, "GD is either not installed or not enabled, check your configuration");
			ZEPHIR_CALL_METHOD(NULL, &_1$$4, "__construct", NULL, 0, &_3$$4);
			zephir_check_call_status();
		}

		zephir_throw_exception_debug(&_1$$4, "stub/oo/oodestruct.zep", 146);
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CALL_SELF(&version, "getversion", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_STRING(&_4, "2.0.1");
	ZEPHIR_INIT_VAR(&_5);
	ZVAL_STRING(&_5, ">=");
	ZEPHIR_CALL_FUNCTION(&_6, "version_compare", NULL, 74, &version, &_4, &_5);
	zephir_check_call_status();
	if (UNEXPECTED(!zephir_is_true(&_6))) {
		ZEPHIR_INIT_VAR(&_7$$5);
		if (!_8$$5) {
		_8$$5 = zephir_fetch_class_str_ex(SL("Stub\\Oo\\Exception"), ZEND_FETCH_CLASS_AUTO);
		}
		object_init_ex(&_7$$5, _8$$5);
		if (zephir_has_constructor(&_7$$5)) {
			ZEPHIR_INIT_VAR(&_9$$5);
			ZEPHIR_CONCAT_SV(&_9$$5, "Requires GD version '2.0.1' or greater, you have ", &version);
			ZEPHIR_CALL_METHOD(NULL, &_7$$5, "__construct", NULL, 0, &_9$$5);
			zephir_check_call_status();
		}

		zephir_throw_exception_debug(&_7$$5, "stub/oo/oodestruct.zep", 154);
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_static_property_ce(stub_oo_oodestruct_ce, ZEND_STRL("checked"), &__$true);
	zephir_read_static_property_ce(&_10, stub_oo_oodestruct_ce, SL("checked"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_10);
}

PHP_METHOD(Stub_Oo_OoDestruct, getVersion)
{
	zend_class_entry *_1$$3 = NULL;
	zval version, info, matches, _3, _4, _0$$3, _2$$3, _5$$5, _6$$5, _7$$5, _8$$5;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

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
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	if (UNEXPECTED(!((zephir_function_exists_ex(ZEND_STRL("gd_info")) == SUCCESS)))) {
		ZEPHIR_INIT_VAR(&_0$$3);
		if (!_1$$3) {
		_1$$3 = zephir_fetch_class_str_ex(SL("Stub\\Oo\\Exception"), ZEND_FETCH_CLASS_AUTO);
		}
		object_init_ex(&_0$$3, _1$$3);
		if (zephir_has_constructor(&_0$$3)) {
			ZEPHIR_INIT_VAR(&_2$$3);
			ZVAL_STRING(&_2$$3, "GD is either not installed or not enabled, check your configuration");
			ZEPHIR_CALL_METHOD(NULL, &_0$$3, "__construct", NULL, 0, &_2$$3);
			zephir_check_call_status();
		}

		zephir_throw_exception_debug(&_0$$3, "stub/oo/oodestruct.zep", 171);
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&version);
	ZVAL_NULL(&version);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_STRING(&_3, "GD_VERSION");
	ZEPHIR_CALL_FUNCTION(&_4, "defined", NULL, 85, &_3);
	zephir_check_call_status();
	if (zephir_is_true(&_4)) {
		ZEPHIR_INIT_NVAR(&version);
		ZEPHIR_GET_CONSTANT(&version, "GD_VERSION");
	} else {
		ZEPHIR_CALL_FUNCTION(&info, "gd_info", NULL, 86);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(&matches);
		ZVAL_NULL(&matches);
		zephir_array_fetch_string(&_5$$5, &info, SL("GD Version"), PH_NOISY | PH_READONLY, "stub/oo/oodestruct.zep", 182);
		ZEPHIR_INIT_VAR(&_6$$5);
		ZVAL_STRING(&_6$$5, "/\\d+\\.\\d+(?:\\.\\d+)?/");
		ZEPHIR_INIT_VAR(&_7$$5);
		ZEPHIR_INIT_VAR(&_8$$5);
		ZVAL_STRING(&_8$$5, "/\\d+\\.\\d+(?:\\.\\d+)?/");
		zephir_preg_match(&_7$$5, &_8$$5, &_5$$5, &matches, 0, 0 , 0 );
		if (zephir_is_true(&_7$$5)) {
			ZEPHIR_OBS_NVAR(&version);
			zephir_array_fetch_long(&version, &matches, 0, PH_NOISY, "stub/oo/oodestruct.zep", 183);
		}
	}
	RETURN_CCTOR(&version);
}

