class Members
{
	#[Marker]
	const PLAIN = 1;

	/**
	 * Doc then attribute.
	 */
	#[Marker("c")]
	const DOCUMENTED = 2;

	const UNMARKED = 3;

	#[Marker]
	public plain = 0;

	#[Marker(1), Other]
	protected readonly int typed;

	#[Marker]
	/**
	 * Attribute then doc.
	 */
	private reversed = 0;

	public unmarked = 0;

	#[Marker]
	public function run()
	{
	}

	/**
	 * Doc then attribute.
	 */
	#[Marker(key: "v")]
	public static function build()
	{
	}

	public function unmarked()
	{
	}
}

interface Contract
{
	#[Marker]
	const LIMIT = 10;

	#[Marker]
	public function handle();

	public function plain();
}