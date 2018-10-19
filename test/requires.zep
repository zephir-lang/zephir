
namespace Test;

use Test\Requires\External3;

class Requires
{
	protected content;

	public function requireExternal1(var path) -> var
	{
		return require path;
	}

	public function requireExternal2(var path)
	{
		require path;
		return true;
	}

	public function requireExternal3(var path) -> var
	{
		var external3;

		if PHP_MAJOR_VERSION == 5 {
			create_symbol_table();
		}

		let external3 = new External3();
		external3->req(path, this);
		return this->content;
	}

	public function setContent(var content) -> void
	{
		let this->content = content;
	}
}
