namespace Stub;

/**
 * Holds an object implementing ArrayAccess in a property and writes to it
 * using array syntax.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2465
 */
class ArrayAccessProperty
{
	public data;

	public function __construct(<\ArrayAccess> data)
	{
		let this->data = data;
	}

	public function set(k, v)
	{
		// Must trigger offsetSet() on the property instead of converting
		// the ArrayAccess object into a plain array (issue #2465).
		let this->data[k] = v;
	}

	public function setNested(a, b, v)
	{
		// Chained write through an ArrayAccess property. Matches native PHP:
		// offsetGet() is fetched once, the indirect modification of the
		// by-value overloaded element has no effect, and a notice is raised.
		// The property must NOT be converted into an array (issue #2465).
		let this->data[a][b] = v;
	}
}
