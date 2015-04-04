
namespace Test\Oo;

abstract class AbstractClass
{
	abstract public function testMethodDeclaration()
	{

	}

	abstract public function testMethodDeclarationWithReturnType() -> string
	{

	}

	abstract public function testMethodDeclarationWithParameter(var a)
	{

	}

	abstract public function testMethodDeclarationWithParameterAndReturnType(var a) -> string
	{

	}

	/**
	 *
	 */
	abstract public function testAbstractMethodDeclaration();

	/**
	 *
	 */
	abstract public function testAbstractMethodDeclarationWithReturnType() -> string;

	/**
	 *
	 */
	abstract public function testAbstractMethodDeclarationWithParameter(var a);

	/**
	 *
	 */
	abstract public function testAbstractMethodDeclarationWithParameterAndReturnType(var a) -> string;
}