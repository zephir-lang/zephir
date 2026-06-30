namespace Stub\Properties;

class App
{
	/** @var \Stub\Properties\StaticPrivateProperties  */
	protected instance { get };

	public function __construct()
	{
		let this->instance = StaticPrivateProperties::getInstance();
	}
}
