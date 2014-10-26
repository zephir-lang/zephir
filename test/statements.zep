namespace Test;

class Statements
{
	public tmp1 = "test";

	public tmp2 = "test string";

	public function testElseIf(int num)
	{
		if num > 0 {
			return "more";
		} elseif num == 0 {
			return "equal";
		} elseif num == -1 {
			return "-1";
		} else {
			return "less";
		}
	}

	public function testPropertyAcccessAvoidTmpReuse()
	{
		var result1, result2, result3, result4;

		let result1 = strpos(this->tmp2, this->tmp1);

		let result2 = strpos(this->tmp2, this->tmp1);

		let result3 = strpos(this->tmp2, this->tmp1);

		let result4 = strpos(this->tmp2, this->tmp1);
	}
}
