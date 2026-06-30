namespace Stub\Oo;

abstract class AbstractStatic
{
	public static function getCalledClass() -> string
	{
		return get_called_class();
	}
}
