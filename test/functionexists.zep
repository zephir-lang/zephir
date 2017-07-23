
namespace Test;

class FunctionExists
{
	public function testWithPassedName(string func) -> bool
	{
		return function_exists(func);
	}

	public function testBuiltInFunctions() -> array
	{
		var func;
		array result = [];

		array functions = [
			"substr",
			"cubstr",
			"ucfirst",
			"bcfirst",
			"stripos",
			"ktripos",
			"trim",
			"prim"
		];

		for func in functions {
			let result[func] = function_exists(func);
		}

		return result;
	}

	public function testWithString() -> bool
	{
		return function_exists("substr");
	}
}
