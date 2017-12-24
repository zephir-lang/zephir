
/**
 * Instance operations
 */

namespace Test;

class Instance
{
	public function __construct(
		<Arithmetic> a1,
		<ArrayObject> a2,
		<Assign> a3,
		<Bitwise> a4,
		<BranchPrediction> a5,
		<Cast> a6,
		<Cblock> a7,
		<Chars> a8,
		<Closures> a9,
		<Compare> a10,
		<Concat> a11
	)
	{

	}

	public static function testIssue1339()
	{
		var parameters;

		let parameters = [
			new Arithmetic(),
			new ArrayObject(),
			new Assign(),
			new Bitwise(),
			new BranchPrediction(),
			new Cast(),
			new Cblock(),
			new Chars(),
			new Closures(),
			new Compare(),
			new Concat()
		];

		return create_instance_params("Test\\Instance", parameters);
	}

	public static function testInstanceCreate(string className)
	{
		return create_instance(className);
	}
}
