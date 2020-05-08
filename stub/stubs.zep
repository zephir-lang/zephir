/**
 * Tests for IDE Stubs Generation
 */
namespace Stub;

/**
 * Sample Class Description
 */
class Stubs
{
	const INTEGER_CONST = 1;
	const DOUBLE_CONST = 10.24;
	const STRING_CONST = "Zephir";

	public propString = "Zephir public";

	/** @var integer - declared in Zephir DocBlock */
	public propIntDeclared = 100;
	protected propStringProtected = "Zephir protected";
	private propStringPrivate = "Zephir private";

	/**
	 * Sample Method Description with Return type -> string
	 */
	public function testDockBlockAndReturnType() -> string
	{
		return this->propString;
	}

	/**
	 * Sample Method Description with Return type -> string
	 *
	 * @return string - declared in Zephir DocBlock
	 */
	public function testDocBlockAndReturnTypeDeclared() -> string
	{
		return this->propStringProtected;
	}

	/**
	 * Method with input params, mixed & int
	 * testMixedInputParamsDocBlock(intOrString, int number = 1)
	 * int number - not declared into DocBlock
	 *
	 * @param int|string $intOrString - declared in Zephir DocBlock
	 * @return int|null - declared in Zephir DocBlock
	 */
	public function testMixedInputParamsDocBlock(intOrString, int number = 1) -> int | null
	{
		return number;
	}

	/**
	 * Method with input params, mixed & int
	 * testMixedInputParamsDocBlockDeclared(intOrString, int number = 1)
	 * all params declared into DocBlock
	 *
	 * @param int|string $intOrString - declared in Zephir DocBlock
	 * @param int $number - declared in Zephir DocBlock
	 * @return int|null - declared in Zephir DocBlock
	 */
	public function testMixedInputParamsDocBlockDeclared(intOrString, int number = 1) -> int | null
	{
		return number;
	}
}
