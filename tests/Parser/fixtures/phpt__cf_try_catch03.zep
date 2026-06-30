function test() {
	try {
		if true { }
	} catch \CustomException | \ParserException {
	} catch \Exception { }
}