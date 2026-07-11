
/**
 * Subclass of PropertyCache. Inherits getPub()/setPub() unchanged so the
 * parent's cached access site runs with this_ptr of a different class entry
 * (the engine must re-validate ce, not blindly reuse the parent's cache).
 *
 * @see https://github.com/zephir-lang/zephir/issues/1884 (property access)
 */

namespace Stub;

class PropertyCacheChild extends PropertyCache
{
	public childProp = 100;

	public function getChildProp()
	{
		return this->childProp;
	}

	/**
	 * Reads an inherited property through a method declared on the child.
	 */
	public function getInheritedPub()
	{
		return this->pub;
	}
}
