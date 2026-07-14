namespace Test;

trait Greeter
{
	protected greeting = "hello";

	const STEP = 2;

	public function greet(string name) -> string
	{
		return this->greeting . " " . name;
	}

	public static function shout() -> string
	{
		return "HI";
	}

	abstract public function label() -> string;
}