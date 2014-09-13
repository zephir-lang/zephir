namespace Test;

class MethodArgs
{
	const GET = "get";

	public a;

	public function setCallable(callable a)
	{
		let this->a = a;
	}

	public function setObject(object a)
	{
		let this->a = a;
	}

	public function setCallableStrict(callable! a)
	{
		let this->a = a;
	}

	public function setObjectStrict(object! a)
	{
		let this->a = a;
	}

	public function setResourceStrict(resource! a)
	{
		let this->a = a;
	}

	/**
	 * @link https://github.com/phalcon/zephir/issues/336
	 */
	public function methodOptionalValueWithDefaultStaticConstantAccess(var method = self::GET)
	{
		return method;
	}
	
	public static function testParamTypeString(string str) -> string
	{
		let str = trim(str);
		return str;
	}
}
