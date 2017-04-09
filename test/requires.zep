
namespace Test;

use Test\Requires\External3;

class Requires
{
	protected content;

	public function requireExternal1(var path)
	{
		return require path;
	}

	public function requireExternal2(var path)
	{
		require path;
		return true;
	}

	public function requireExternal3(var path)
	{
		var external3;

		create_symbol_table();
		let external3 = new External3();
		external3->req(path, this);
		return this->content;
	}

	public function setContent(var content)
	{
		let this->content = content;
	}
}
