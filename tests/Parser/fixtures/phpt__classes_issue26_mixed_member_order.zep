namespace Bugreport;

class ExampleAction {
	const MY_ACTION_NAME = "MY_ACTION_NAME";

	public function createMyAction(array payload) {
		return payload;
	}

	const MY_OTHER_ACTION_NAME = "MY_OTHER_ACTION_NAME";

	public function createMyOtherAction(array payload) {
		return payload;
	}
}