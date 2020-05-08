namespace TypeHints;

class Args {
	public function args(_var, string _string, boolean _bool, int _int, long _long, double _double, <Args> _args) {
		echo _var;
		echo _string;
		echo _bool;
		echo _long;
		echo _double;
		echo _args;
	}
}

function args(_var, string _string, boolean _bool, int _int, long _long, double _double, <Args> _args) {
	echo _var;
	echo _string;
	echo _bool;
	echo _long;
	echo _double;
	echo _args;
}
