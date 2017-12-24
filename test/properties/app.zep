namespace Test\Properties;

class App
{
	/** @var \Test\Properties\StaticPrivateProperties  */
	protected instance { get };

	public function __construct()
	{
		let this->instance = StaticPrivateProperties::getInstance();
	}
}
