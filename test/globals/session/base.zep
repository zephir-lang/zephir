
namespace Test\Globals\Session;

class Base
{
	public function set(string index, var value)
	{
		let _SESSION[index] = value;

		return _SESSION;
	}

	public function remove(string index)
	{
		unset _SESSION[index];
	}

	public function __set(string index, var value)
	{
		return this->set(index, value);
	}

	public function __unset(string index)
	{
		this->remove(index);
	}

	protected function removeSessionData()
	{
		var key;

		for key, _ in _SESSION {
			unset _SESSION[key];
		}

		return _SESSION;
	}
}
