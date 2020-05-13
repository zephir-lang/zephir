
namespace Stub;

use Stub\Requires\External3;

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

		create_symbol_table();

		let external3 = new External3();
		external3->req(path, this);
		return this->content;
	}

	public function setContent(var content) -> void
	{
		let this->content = content;
	}

	public function renderTemplate(string! templatePath, var params) -> var
	{
		create_symbol_table();

		var key, value;

		if typeof params == "array" {
			for key, value in params {
				let {key} = value;
			}
		}

		return require templatePath;
	}
}
