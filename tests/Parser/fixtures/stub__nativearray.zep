namespace Stub;

class NativeArray
{
	protected prefixes = [];

	public function testArray1() -> array
	{
		var a;
		let a = [];
		return a;
	}

	public function testArray2() -> array
	{
		var a;
		let a = [1, 2, 3];
		return a;
	}

	public function testArray3() -> array
	{
		var a;
		let a = [1.1, 2.2, 3.3];
		return a;
	}

	public function testArray4() -> array
	{
		var a;
		let a = [false, true, false];
		return a;
	}

	public function testArray5() -> array
	{
		var a;
		let a = [null, null, null];
		return a;
	}

	public function testArray6() -> array
	{
		var a;
		let a = ["x", "y", "z"];
		return a;
	}

	public function testArray7() -> array
	{
		var d; int a, b, c;
		let a = 1, b = 2, c = 3;
		let d = [a, b, c];
		return d;
	}

	public function testArray8() -> array
	{
		var d; double a, b, c;
		let a = 1, b = 2, c = 3;
		let d = [a, b, c];
		return d;
	}

	public function testArray9() -> array
	{
		var d; boolean a, b, c;
		let a = true, b = false, c = true;
		let d = [a, b, c];
		return d;
	}

	public function testArray10() -> array
	{
		var d; string a, b, c;
		let a = "hello1", b = "hello2", c = "hello3";
		let d = [a, b, c];
		return d;
	}

	public function testArray11() -> array
	{
		var a, b, c;
		let a = [1, 2, 3];
		let b = [4, 5, 6];
		let c = [a, b];
		return c;
	}

	public function testArray12() -> array
	{
		var a, b, c;
		let a = [1, 2, 3];
		let b = [a];
		let c = [b];
		return c;
	}

	public function testArray13() -> array
	{
		var a;
		let a = [1: "hello1", 2: "hello2", 3: "hello3"];
		return a;
	}

	public function testArray14() -> array
	{
		var a;
		let a = ["hello1": 1, "hello2": 2, "hello3": 3];
		return a;
	}

	public function testArray15() -> array
	{
		var a;
		let a = ["hello1": true, "hello2": false, "hello3": true];
		return a;
	}

	public function testArray16() -> array
	{
		var a;
		let a = ["hello1": 1.0, "hello2": 2.0, "hello3": 3.0];
		return a;
	}

	public function testArray17() -> array
	{
		var a;
		let a = ["hello1": null, "hello2": null, "hello3": null];
		return a;
	}

	public function testArray18() -> array
	{
		var a;
		let a = ["hello1": "a", "hello2": "b", "hello3": "c"];
		return a;
	}

	public function testArray19() -> array
	{
		var a;
		let a = [0: true, 1: false, 2: true];
		return a;
	}

	public function testArray20() -> array
	{
		var a;
		let a = [0: 1.0, 1: 2.0, 2: 3.0];
		return a;
	}

	public function testArray21() -> array
	{
		var a;
		let a = [0: null, 1: null, 2: null];
		return a;
	}

	public function testArray22() -> array
	{
		var a;
		let a = [0: 4, 1: 5, 2: 6];
		return a;
	}

	public function testArray23() -> array
	{
		var a; int b;

		let b = 0,
			a = [0: b, 1: b, 2: b];
		return a;
	}

	public function testArray24() -> array
	{
		var a; double b;

		let b = 0.0,
			a = [0: b, 1: b, 2: b];
		return a;
	}

	public function testArray25() -> array
	{
		var a; boolean b;

		let b = false,
			a = [0: b, 1: b, 2: b];
		return a;
	}

	public function testArray26() -> array
	{
		var a; var b;

		let b = null,
			a = [0: b, 1: b, 2: b];
		return a;
	}

	public function testArray27() -> array
	{
		var a; string b;

		let b = "hello",
			a = [0: b, 1: b, 2: b];
		return a;
	}

	public function testArray28() -> array
	{
		var a; string b;

		let b = "hello",
			a = [0: b, 1: b, 2: b];
		return a;
	}

	public function testArray29() -> array
	{
		var a; long b;

		let b = 1,
			a = [b : "hello"];
		return a;
	}

	public function testArray30() -> array
	{
		var a; string b;

		let b = "hello",
			a = [b : "hello"];
		return a;
	}

	public function testArrayAccess1() -> long
	{
		var a, b;

		let a = [1, 2, 3], b = a[0];

		return b;
	}

	public function testArrayAccess2() -> long
	{
		var a, b;

		let a = ["a": 1, "b": 2, "c": 3],
			b = a["a"];

		return b;
	}

	public function testArrayAccess3() -> long
	{
		var a, b;
		long c;

		let a = [1, 2, 3],
			c = 0,
			b = a[c];

		return b;
	}

	public function testArrayAccess4() -> long
	{
		var a, b;
		string c;

		let a = ["a": 1, "b": 2, "c": 3],
			c = "a",
			b = a[c];

		return b;
	}

	public function testArrayAccess5() -> long
	{
		var a, b, c;

		let a = ["a": 1, "b": 2, "c": 3],
			c = "a",
			b = a[c];

		return b;
	}

	public function testArrayAccess6() -> long
	{
		var a, b;

		let a = ["a": 1, "b": 2, "c": 3],
			b = a["a"] + a["b"];

		return b;
	}

	public function testArrayMultipleAccess1() -> string
	{
		var a, b;

		let a = [0: [0: "a", 1: "b"]],
			b = a[0][0];

		return b;
	}

	public function testArrayMultipleAccess2() -> string
	{
		var a, b;

		let a = [["a", "b"]],
			b = a[0][1];

		return b;
	}

	public function testArrayMultipleAccess3() -> string
	{
		var a, b;

		let a = [[["a", "b"]]],
			b = a[0][0][1];

		return b;
	}

	public function testArrayMultipleAccess4() -> string
	{
		var a, b;

		let a = [[["a", "b"]]],
			b = a[0][0][1];

		return b;
	}

	public function testArrayMultipleAccess5() -> int
	{
		var a, b;

		let a = ["a": ["b": ["c": 0, "d": 1]]],
			b = a["a"]["b"]["c"];

		return b;
	}

	public function testArrayUpdate1() -> array
	{
		var a;
		let a = [1, 2, 3], a[0] = 4;
		return a;
	}

	public function testArrayUpdate2() -> array
	{
		var a; int b;
		let a = [1, 2, 3],
			b = 0,
			a[b] = 4;
		return a;
	}

	public function testArrayUpdate3() -> array
	{
		var a; int b;
		let a = [1, 2, 3],
			b = 0,
			a[b] = 4,
			b = 1,
			a[b] = 5;
		return a;
	}

	public function testArrayUpdate4() -> array
	{
		var a;
		let a = ["a": 1, "b": 2, "c": 3], a["a"] = 4;
		return a;
	}

	public function testArrayUpdate5() -> array
	{
		var a;
		let a = [1, 2, 3],
			a[0] = [1, 2, 3],
			a[1] = [1, 2, 3],
			a[2] = [1, 2, 3];
		return a;
	}

	public function testArrayAppend1() -> array
	{
		var a;
		let a = [], a[] = "hello", a[] = null, a[] = false, a[] = 1.10, a[] = 5, a[] = [1, 2, 3];
		return a;
	}

	public function testArrayAppend2() -> array
	{
		var a; string b = "hello"; var c = null; boolean d = false; double e = 1.10; int f = 5; var g = [1, 2, 3];
		let a = [], a[] = b, a[] = c, a[] = d, a[] = e, a[] = f, a[] = g;
		return a;
	}

	public function testMultipleArrayUpdate1() -> array
	{
		var a;

		let a = [], a["x"]["y"] = null;

		return a;
	}

	public function testMultipleArrayUpdate2() -> array
	{
		var a;

		let a = [], a["x"]["y"]["z"] = null;

		return a;
	}

	public function testMultipleArrayUpdate3() -> array
	{
		var a;

		let a = [], a[0][1] = null;

		return a;
	}

	public function testMultipleArrayUpdate4() -> array
	{
		var a;

		let a = [], a[0][1][2] = null;

		return a;
	}

	public function testMultipleArrayUpdate5() -> array
	{
		var a;

		let a = [], a["a"][1]["c"] = null;

		return a;
	}

	public function testMultipleArrayUpdate6() -> array
	{
		var a;

		let a = [], a[0]["b"][2] = null;

		return a;
	}

	public function testMultipleArrayUpdate7() -> array
	{
		var a;

		let a = [],
			a["a"][1]["c"] = true,
			a[0]["b"][2] = false;

		return a;
	}

	public function testMultipleArrayUpdate8() -> array
	{
		var a;

		let a = [],
			a["a"][0] = null,
			a["a"][1] = true,
			a["a"][2] = false;

		return a;
	}

	public function testMultipleArrayUpdate9() -> array
	{
		var a;

		let a = [],
			a["a"][0] = null,
			a["a"][1] = true,
			a["a"][1] = false;

		return a;
	}

	public function testMultipleArrayUpdate10() -> array
	{
		var a;

		let a = [],
			a["a"][0] = null,
			a["a"][1] = true,
			a["a"][1]["c"] = false;

		return a;
	}

	public function testMultipleArrayUpdate11() -> array
	{
		var a, b = "y", c = "x";

		let a = [], a[b][c] = null;

		return a;
	}

	public function testMultipleArrayUpdate12() -> array
	{
		var a, b = "y", c = "x", d = "z";

		let a = [], a[b][c][d] = null;

		return a;
	}

	public function testMultipleArrayUpdate13() -> array
	{
		var a; array b, c, d;

		let d = ["x": null, "y": null, "z": null],
			b = ["a": 1, "b": 2, "c": d],
			c = b,
			a = [0: b, 1: ["a": 1, "b": 2, "c": 3]];

		let a[0]["a"]["y"] = false;

		return a;
	}

	public function testMultipleArrayAppend1() -> array
	{
		var a = []; var b = "y";
		let a[0]["hello"][b][] = null;
		return a;
	}

	public function testMultipleArrayAppend2() -> array
	{
		var a = []; int b = 100; string x = "hello";
		let a[0]["hello"][b][x][] = null;
		return a;
	}

	public function testMultipleArrayAppend3() -> array
	{
		var a = []; var b = "y";
		let a[0]["hello"][b] = null;
		let a[0]["hello"][b][] = 100;
		let a[0]["hello"][b][] = false;
		let a[0]["hello"][b][] = 1.2;
		let a[0]["hello"][b][] = [1, 2, 3];
		return a;
	}

	public function testArrayKeys(var param) -> array
	{
		return array_keys(param);
	}

	public function testImplodeArray(var param) -> string
	{
		return implode("|", array_keys(param));
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/110
	 */
	public function issue110()-> string
	{
		var byteUnits;
		let byteUnits = ["B": 0, "K": 10, "M": 20, "G": 30, "T": 40, "KB": 10, "MB": 20, "GB": 30, "TB": 40];
		return implode("|", array_keys(byteUnits));
	}

	public function issue264(array tokens) -> bool
	{
	    return !isset(tokens[1]);
	}

	public function issue743a(array current743a) -> array
	{
		let current743a[42]["str"] = "ok";
		return current743a;
	}

	public function issue743b(array current) -> array
	{
		let current["str"][42] = "ok";
		return current;
	}

	public function issue743c(array current) -> array
	{
		var key;
		let key = "hey";
		let current["str"][$key] = "ok";
		return current;
	}

	/**
	 * @link https://github.com/zephir-lang/zephir/issues/709
	 */
	public function issue709() -> bool
	{
		var c, arr;
		var works = true;

		for c in range(0, 50) {
			let arr = [1, 2];
			let arr = arr[array_rand(arr)];
			if arr < 0 || arr > 2 {
				let works = false;
			}
		}
		return works;
	}

	public function Issue1140(string prefix, string baseDir)
	{
		if !isset this->prefixes[prefix] {
		    let this->prefixes[prefix] = [];
		}
		array_push(this->prefixes[prefix], baseDir);
		return this->prefixes;
	}

	public function Issue1159()
	{
		var myvar;
		let myvar = 1;

		let myvar = [myvar];
		return myvar;
	}
}
