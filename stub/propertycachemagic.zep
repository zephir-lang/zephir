
/**
 * Magic accessors over a backing store. Zephir rejects a *literal* access to
 * an undeclared property, so magic is reached only through the dynamic
 * `this->{name}` path (zephir_read_property_zval / _zval writes) — which the
 * property-access optimization deliberately does NOT touch. These methods
 * pin that magic path so we can prove it stays correct.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1884 (property access)
 */

namespace Stub;

class PropertyCacheMagic
{
	protected store = [];

	public function __get(string name)
	{
		return isset this->store[name] ? this->store[name] : null;
	}

	public function __set(string name, var value) -> void
	{
		let this->store[name] = value;
	}

	public function __isset(string name) -> bool
	{
		return isset this->store[name];
	}

	public function __unset(string name) -> void
	{
		unset this->store[name];
	}

	public function readVirtual(string name)
	{
		return this->{name};
	}

	public function writeVirtual(string name, var value) -> void
	{
		let this->{name} = value;
	}
}
