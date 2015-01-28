namespace Test;

class MethodArgs
{
	const GET = "get";

	const MY_DOUBLE = 1.32;

	const MY_BOOL = true;

	const MY_INT = 12345;

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

	/**
	 * For string parameter
	 * @link https://github.com/phalcon/zephir/issues/696
	 */
	public function methodOptionalStringValueWithDefaultStaticConstantAccess(string parameter = self::GET)
	{
		return parameter;
	}

	/**
	 * For double parameter
	 * @link https://github.com/phalcon/zephir/issues/696
	 */
	public function methodOptionalDoubleValueWithDefaultStaticConstantAccess(double parameter = self::MY_DOUBLE)
	{
		return parameter;
	}

	/**
	 * For bool parameter
	 * @link https://github.com/phalcon/zephir/issues/696
	 */
	public function methodOptionalBoolValueWithDefaultStaticConstantAccess(bool parameter = self::MY_BOOL)
	{
		return parameter;
	}

	/**
	 * For int parameter
	 * @link https://github.com/phalcon/zephir/issues/728
	 */
	public function methodOptionalIntValueWithDefaultStaticConstantAccess(int parameter = self::MY_INT)
	{
		return parameter;
	}
}
