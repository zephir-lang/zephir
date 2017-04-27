namespace Test;

class Invoke
{
	private a;

	public function __construct()
	{
		let this->a = "aaa";
	}

	public function __invoke() -> string
	{
		return gettype(this->a);
	}

	public static function test() -> string
	{
		var func;
		let func = new Invoke();
		return {func}();
	}
}
