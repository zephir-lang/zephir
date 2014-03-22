/**
 *
 */

namespace Test\Oo;

class DynamicProp {

	public function setProperty(string property) -> void {
		let this->{property} = 10;
	}
	
}
