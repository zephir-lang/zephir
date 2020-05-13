namespace Stub\Properties;

class StaticPrivateProperties
{
	private static instance;

	public static function getInstance() -> <StaticPrivateProperties>
	{
		var localInstance;
		let localInstance = self::instance;

		if unlikely typeof localInstance !== "object" {
			let localInstance = new self();
			let self::instance = localInstance;
		}

		return self::instance;
	}
}
