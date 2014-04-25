
namespace Test\BuiltIn;

class CharMethods
{
	public function getHex()
	{
		return 'a'->toHex();
	}

	public function getHexForString(string str)
	{
		var o = "";
		char ch;

		for ch in str {
			let o .= ch->toHex();
		}
		return o;
	}
}