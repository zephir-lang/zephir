function test() {
	if true { }

	if true {
		let a = 1;
	} else {
		let a = 2;
	}

	if true {
		let a = 1;
	} elseif false {
		let a = 2;
	} elseif true {
		let a = 3;
	} else {
		let a = 4;
	}
}