
/**
 * Class with dynamic new
 */

namespace Stub\Oo;

class OoDynamicA
{
	public static function getNew()
	{
		var className, fullClassName;
		let className = get_called_class();
		let fullClassName = "\\" . className;
		return new {fullClassName}();
	}

	public static function call2() {
		return \Stub\Oo\OoDynamicA::getNew();
	}

	public static function call1() {
		return \Stub\Oo\OoDynamicA::call2();
	}

	public function execute() {
		return "A";
	}

	public function execute2() {
		return "AA";
	}
}
