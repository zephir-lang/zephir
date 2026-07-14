namespace Test;

trait Hello
{
	public function hello() -> string
	{
		return "hello";
	}
}

trait HelloWorld
{
	use Hello;

	public function helloWorld() -> string
	{
		return this->hello() . " world";
	}
}