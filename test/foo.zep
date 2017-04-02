namespace Test;

class Foo extends \Test\Bar
{
	public function run()
	{
		return this->runBar();
	}
}
