namespace Test\Mcall;

class Caller
{
	public static function start(callable f) -> var
	{
		return {f}();
	}

	public static function perform() -> string
	{
		return __METHOD__;
	}
}
