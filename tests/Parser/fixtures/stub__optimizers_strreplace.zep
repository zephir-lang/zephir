
namespace Stub\Optimizers;

class StrReplace
{
	/**
	 * @link https://github.com/zephir-lang/zephir/issues/1055
	 */
	static public function issue1055()
	{
		return str_replace(["aa"], ["aaa"], "bla bla aa aaa askks");
	}

	static public function issue1087()
	{
		var filtered, sanitize;
		let filtered = "test_test";
		let sanitize = ["/", "'", "\"", "?", "!", "%", "$", "&"];
		let filtered = str_replace(sanitize, "", filtered);
		return filtered;
	}

	static public function issue732A()
	{
		array  search  = ["A", "B", "C", "D", "E"];
		array  replace = ["B", "C", "D", "E", "F"];
		string subject = "A";

		return str_replace(search, replace, subject);
	}

	static public function issue732B()
	{
		array  patterns     = ["/quick/", "/brown/", "/fox/"];
		array  replacements = ["bear", "black", "slow"];
		string subject      = "The quick brown fox jumped over the lazy dog.";

		return preg_replace(patterns, replacements, subject);
	}
}
