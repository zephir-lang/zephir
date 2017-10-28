namespace Test\Properties;

class StaticPrivateProperties
{
	private static instance;

	public static function getInstance() -> <StaticPrivateProperties>
	{
		var instance;
		let instance = self::instance;

		if typeof instance !== "object" {
		   let instance = new self();
		   let self::instance = instance;
		}

		return self::instance;
	}
}
