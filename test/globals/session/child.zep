
namespace Test\Globals\Session;

class Child extends Base
{
	public function destroy()
	{
		return this->removeSessionData();
	}
}
