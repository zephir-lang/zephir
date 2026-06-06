
namespace Stub\Globals\Session;

class Child extends Base
{
	public function destroy()
	{
		return this->removeSessionData();
	}
}
