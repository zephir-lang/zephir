function test() {
	var a;

	switch 3 {
	case 1:
	case 2:
		let a = "foobar";
		break;
	case 3:
		let a = "baz";
		break;
	default:
		let a = "biz";
		break;
	}

	switch foobar {
	case "hello":
	case hello:
	case HELLO:
	default:
		break;
	}
}