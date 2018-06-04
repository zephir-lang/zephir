namespace TypeHints;

class RetVal {
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

	public function retval_retval() -> <RetVal> {
		var _var;
		let _var = new RetVal;
		return _var;
	}
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
