namespace Stub;

/**
 * Assigning a non-constant boolean expression to a dynamic property used to
 * fail to compile with a cryptic "?" error.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/1790
 */
class Issue1790
{
	public flag;

	public function setStringProp(var value)
	{
		let this->{"flag"} = value > 5;
	}

	public function setVariableProp(var key, var value)
	{
		let this->{key} = value > 5;
	}
}
