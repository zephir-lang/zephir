namespace Test\Oo\Scopes;

class PrivateScopeTester extends HasPrivateMethod implements ScopeTesterInterface
{
	public function run() -> string
	{
		return this->callPrivateMethod();
	}
}
