namespace TypeHints;

class RetVal {
	/**
	 * Frankly speaking, the user should never do this.
	 * However, the language specification does not prohibit it.
	 * Therefore, we must test and verify the expected result.
	 *
	 * @link https://github.com/zephir-lang/zephir/pull/1681
	 * @var \Some\Non\Existent\Class\At\This\Time
	 */
	protected my_var = "some-value" {get};

	/**
	 * @link https://github.com/zephir-lang/zephir/pull/1681
	 * @var string
	 */
	protected my_string = "some-value" {get};

	/**
	 * @link https://github.com/zephir-lang/zephir/pull/1685
	 * @var array
	 */
	protected my_array {get};

	/** @link https://github.com/zephir-lang/zephir/pull/1681 */
	public function retval_var_var() -> var {
		var _var;
		return _var;
	}

	/** @link https://github.com/zephir-lang/zephir/pull/1681 */
	public function retval_var_var_builit_1() -> var|string {
		var _var;
		return _var;
	}

	/** @link https://github.com/zephir-lang/zephir/pull/1681 */
	public function retval_var_var_builit_2() -> int|var {
		var _var;
		return _var;
	}

	public function retval_var() {
		var _var;
		return _var;
	}

	public function retval_string() -> string {
		string _var;
		return _var;
	}

	public function retval_boolean() -> boolean {
		boolean _var;
		return _var;
	}

	public function retval_int() -> int {
		int _var;
		return _var;
	}

	public function retval_long() -> long {
		long _var;
		return _var;
	}

	public function retval_double() -> double {
		double _var;
		return _var;
	}

	public function retval_array() -> array {
		return [];
	}

    /** @link https://github.com/zephir-lang/zephir/issues/1779 */
    public function retval_array_of_objects() -> <\stdClass[]> {
		return [];
	}

	public function retval_collection() -> <RetVal[]> {
		return [];
	}

	public function retval_retval() -> <RetVal> {
		var _var;
		let _var = new RetVal;
		return _var;
	}

	public function retval_nullable_int() -> int | null {
		return null;
	}

	public function retval_nullable_uint() -> uint | null {
		return null;
	}

	public function retval_nullable_long() -> long | null {
		return null;
	}

	public function retval_nullable_float() -> float | null {
		return null;
	}

	public function retval_nullable_double() -> double | null {
		return null;
	}

	public function retval_nullable_string() -> string | null {
		return null;
	}

	public function retval_nullable_boolean() -> boolean | null {
		return null;
	}

	public function retval_nullable_char() -> char | null {
		return null;
	}

	public function retval_nullable_array() -> array | null {
		return null;
	}

	public function retval_object_or_scalar() -> <\stdClass> | bool {
		return false;
	}

	public static function retval_static_object_or_scalar() -> <\stdClass> | bool {
		return new stdClass();
	}
}

/** @link https://github.com/zephir-lang/zephir/pull/1681 */
function retval_var_var_builit_1() -> var|string {
	var _var;
	return _var;
}

/** @link https://github.com/zephir-lang/zephir/pull/1681 */
function retval_var_var_builit_2() -> int|var {
	var _var;
	return _var;
}

function retval_var() {
	var _var;
	return _var;
}

function retval_string() -> string {
	string _var;
	return _var;
}

function retval_boolean() -> boolean {
	boolean _var;
	return _var;
}

function retval_int() -> int {
	int _var;
	return _var;
}

function retval_long() -> long {
	long _var;
	return _var;
}

function retval_double() -> double {
	double _var;
	return _var;
}

function retval_retval() -> <RetVal> {
	var _var;
	let _var = new RetVal;
	return _var;
}

function retval_nullable_int() -> int | null {
    return null;
}

function retval_nullable_uint() -> uint | null {
    return null;
}

function retval_nullable_long() -> long | null {
    return null;
}

function retval_nullable_float() -> float | null {
    return null;
}

function retval_nullable_double() -> double | null {
    return null;
}

function retval_nullable_string() -> string | null {
    return null;
}

function retval_nullable_boolean() -> boolean | null {
    return null;
}

function retval_nullable_char() -> char | null {
    return null;
}

function retval_object_or_scalar() -> <\stdClass> | bool {
    return false;
}

/** @link https://github.com/zephir-lang/zephir/issues/1779 */
function retval_array_of_objects() -> <\stdClass[]> {
    return [];
}
