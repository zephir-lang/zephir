namespace Test;

class Foo
{
	use A;
	use B, Ns\Deep\C, CSV;

	public function m() -> int
	{
		return 1;
	}
}