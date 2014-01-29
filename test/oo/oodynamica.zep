
/**
 * Class with dynamic new
 */

namespace Test\Oo;

class OoDynamicA
{
	public static function getNew()
	{
		var className, fullClassName;
		let className = get_called_class();
		let fullClassName = "\\" . className;
		return new {fullClassName}();
	}
}