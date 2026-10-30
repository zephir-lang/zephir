namespace Fixture;

class InstanceofStatic
{
	public function check(var a, var b)
	{
		if a instanceof static {
			return !a instanceof static;
		}

		let b = a instanceof static && b instanceof self;

		return a instanceof static::class || a instanceof static::name();
	}
}
