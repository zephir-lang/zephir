
namespace Stub;

class Returns
{
	public function testReturnCast1() -> int
	{
		return (int) 5.0;
	}

	public function testReturnCast2() -> int
	{
		return (int) true;
	}

	public function testReturnCast3() -> int
	{
		return (int) false;
	}

	public function testReturnCast4() -> int
	{
		if true {
			return (int)1;
		} else {
			return (int)0;
		}
	}

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1003
     */
	public static function issue1003() -> int
    {
        var data, dataStr;

        let data = [
            "test": 1,
            "value": 2
        ];

        if isset data["test"] {
            return 1;
        } else {
            let dataStr = implode("-", data);

            return (int)sizeof(data);
        }
    }

	public static function returnWithParameter(string parameter)
	{
		return "Return back";
	}

	public static function returnWithoutParameter()
	{
		return "Return back";
	}
}
