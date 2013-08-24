
/**
 * Control Flow
 */

namespace Test;

class Flow
{

	public function testIf1()
	{
		int a;
		let a = 1;
		if (a) {
			return 1;
		}
		return 0;
	}

	public function testIf2()
	{
		int a;
		let a = 0;
		if (a) {
			return 1;
		}
		return 0;
	}

	public function testIf3()
	{
		double a;
		let a = 1;
		if (a) {
			return 1;
		}
		return 0;
	}

	public function testIf4()
	{
		double a;
		let a = 0;
		if (a) {
			return 1;
		}
		return 0;
	}

	public function testIf5()
	{
		boolean a;
		let a = 1;
		if (a) {
			return 1;
		}
		return 0;
	}

	public function testIf6()
	{
		boolean a;
		let a = 0;
		if (a) {
			return 1;
		}
		return 0;
	}

	public function testIf7()
	{
		int a;
		let a = 1;
		if (a) {
			return 1;
		} else {
			return 0;
		}
	}

	public function testIf8()
	{
		int a;
		let a = 0;
		if (a) {
			return 1;
		} else {
			return 0;
		}
	}

	public function testIf9()
	{
		int a;
		double b;
		boolean c;
		let a = 1;
		if (a) {
			let b = 1;
			if (b) {
				let c = 1;
				if (c) {
					return 1;
				}
			}
		}
		return 0;
	}

	public function testIf10()
	{
		int a;
		double b;
		boolean c;
		let a = 1;
		if (a) {
			let b = 1;
			if (b) {
				let c = 1;
				if (c) {
					return 1;
				} else {
					return 0;
				}
			} else {
				return 0;
			}
		} else {
			return 0;
		}
	}

	public function testIf12()
	{
		int a;
		double b;
		boolean c;
		let a = 1;
		if a {
			let b = 1;
			if b {
				let c = 1;
				if c {
					return 1;
				} else {
					return 0;
				}
			} else {
				return 0;
			}
		} else {
			return 0;
		}
	}

	public function testIf13()
	{
		int a, b;
		let a = 1, b = 2;
		if a + b { return 1; }
		return 0;
	}

	public function testIf14()
	{
		var a, b;
		let a = 1, b = 2;
		if a + b { return 1; }
		return 0;
	}

	public function testIf15()
	{
		var a, b, c;
		let a = 1, b = 2, c = 3;
		if a + b + c { return 1; }
		return 0;
	}

	public function testLoop1()
	{
		var a;
		let a = true;
		loop {
			if (a) {
				break;
			}
		}
		return a;
	}

	public function testLoop2()
	{
		int a;
		let a = 1;
		loop {
			let a = a + 1;
			if (a === 5) {
				break;
			}
		}
		return a;
	}

	public function testLoop3()
	{
		int a;
		let a = 1;
		loop {
			let a = a + 1;
			if (a === 3) {
				let a = a + 1;
				continue;
			}
			if (a === 5) {
				break;
			}
		}
		return a;
	}

	public function testLoop4()
	{
		int a;
		let a = 1;
		loop {
			let a = a + 1;
			if a === 3 {
				let a = a + 1;
				continue;
			}
			if a === 5 {
				break;
			}
		}
		return a;
	}

	public function testWhile1()
	{
		int a;
		let a = 5;
		while (a) {
			let a = a - 1;
		}
		return a;
	}

	public function testWhile2()
	{
		int a;
		let a = 5;
		while a {
			let a = a - 1;
		}
		return a;
	}

	public function testWhile3()
	{
		int a; var b; double c;
		let a = 5;
		while (a) {
			let b = 5;
			while (b) {
				let b = b - 1;
			}
			let a = a - 1;
		}
		let c = a + b;
		return c;
	}

	public function testWhile4()
	{
		int a; var b; double c;
		let a = 5;
		while a {
			let b = 5;
			while b {
				let b = b - 1;
			}
			let a = a - 1;
		}
		let c = a + b;
		return c;
	}

	public function testWhile5()
	{
		int a;
		let a = 5;
		while (a) {
			let a--;
		}
		return a;
	}

	public function testWhile6()
	{
		int a;
		let a = 5;
		while a {
			let a--;
		}
		return a;
	}

	public function testWhile7()
	{
		double a;
		let a = 5;
		while (a) {
			let a--;
		}
		return a;
	}

	public function testWhile8()
	{
		double a;
		let a = 5;
		while a {
			let a--;
		}
		return a;
	}

	public function testWhile9()
	{
		int a; var b; double c;
		let a = 5;
		while (a) {
			let b = 5;
			while (b) {
				let b--;
			}
			let a--;
		}
		let c = a + b;
		return c;
	}

	public function testFor1()
	{
		var v, b; int c;
		let c = 0,
			b = [1, 2, 3, 4];
		for v in b {
			let c += v;
		}
		return c;
	}

	public function testFor2()
	{
		var v, k, b; double c;
		let c = 0,
			b = [1, 2, 3, 4];
		for k, v in b {
			let c += k;
		}
		return c;
	}

	public function testFor3()
	{
		var v, b, c;
		let c = [],
			b = [1, 2, 3, 4];
		for v in reverse b {
			let c[] = v;
		}
		return c;
	}

	public function testFor10()
	{
		var a; string b; int v;
		let a = [],
			b = "hello";
		for v in b {
			let a[] = v;
		}
		return a;
	}

	public function testFor11()
	{
		var a; string b; int k, v;
		let a = [],
			b = "hello";
		for k, v in b {
			let a[k] = v;
		}
		return a;
	}

}
