
namespace Stub\Properties;

/**
 * @link https://github.com/phalcon/zephir/issues/520
 */
class PropertyArray
{
	/**
	 * This is a public property with an initial empty-array value
	 */
	public someEmptyArray = [];

	/**
	 * This is a public property with an initial array value
	 */
	public someArray = [1, 2, 3, 4];

	/**
	 * This is a protected property with an initial empty-array value
	 */
	protected otherArray = [];

	public function __construct()
	{
		let this->someArray = [1];
	}

	public function appendSome(var value)
	{
		let this->someArray[] = (string) value;
	}

	public function setOtherArray(arr){
		let this->otherArray = arr;
	}

	public function getOtherArray(){
		return this->otherArray;
	}

	public function testIssues1831(){
		var info;
		var headers;

		let headers = [];
		while( true ){
			let info = array_shift(this->otherArray);

			if( stripos(info, "header") !== false ){
				let headers[] = info;
				continue;
			} else {
				array_unshift(this->otherArray, info);
				break;
			}
		}

		return headers;
	}
}
