
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
		if a {
			return 1;
		}
		return 0;
	}

	public function testIf2()
	{
		int a;
		let a = 0;
		if a {
			return 1;
		}
		return 0;
	}

	public function testIf3()
	{
		double a;
		let a = 1;
		if a {
			return 1;
		}
		return 0;
	}

	public function testIf4()
	{
		double a;
		let a = 0;
		if a {
			return 1;
		}
		return 0;
	}

	public function testIf5()
	{
		boolean a;
		let a = 1;
		if a {
			return 1;
		}
		return 0;
	}

	public function testIf6()
	{
		boolean a;
		let a = 0;
		if a {
			return 1;
		}
		return 0;
	}

	public function testIf7()
	{
		int a;
		let a = 1;
		if a {
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
		if a {
			let b = 1;
			if b {
				let c = 1;
				if c {
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
			if a {
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
			if a === 5 {
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
		while a {
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
		while a {
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
		while a {
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
		while a {
			let b = 5;
			while b {
				let b--;
			}
			let a--;
		}
		let c = a + b;
		return c;
	}

	public function testWhile10(var a)
	{
		var b; double c;
		while a {
			let b = 5;
			while b {
				let b--;
			}
			let a--;
		}
		let c = a + b;
		return c;
	}

	public function testWhile11(var a, var d)
	{
		var b; double c;
		while a {
			let b = d;
			while b {
				let b--;
			}
			let a--;
		}
		let c = a + b;
		return c;
	}

	public function testDoWhile1()
	{
		do {

		} while(1);
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

	public function testFor4()
	{
		var a, b;
		let b = 0;
		for a in range(1, 10) {
			let b += a;
		}
		return b;
	}

	public function testFor5()
	{
		int a, b;
		let b = 0;
		for a in range(1, 10) {
			let b += a;
		}
		return b;
	}

	public function testFor6()
	{
		var a, b;
		let b = 0;
		for a in reverse range(1, 10) {
			let b += a;
		}
		return b;
	}

	public function testFor7()
	{
		int a, b;
		let b = 0;
		for a in reverse range(1, 10) {
			let b += a;
		}
		return b;
	}

	public function testFor8()
	{
		int a, b, c, d;
		let b = 0, c = 1, d = 10;
		for a in range(c, d) {
			let b += a;
		}
		return b;
	}

	public function testFor9()
	{
		int a, b, c, d;
		let b = 0, c = 1, d = 10;
		for a in reverse range(c, d) {
			let b += a;
		}
		return b;
	}

	public function testFor10()
	{
		var a, b, c, d;
		let b = 0, c = 1, d = 10;
		for a in range(c, d) {
			let b += a;
		}
		return b;
	}

	public function testFor11()
	{
		char a; string b;
		let b = "";
		for a in range('a', 'z') {
			let b .= a;
		}
		return b;
	}

	public function testFor12()
	{
		char a; string b;
		let b = "";
		for a in reverse range('a', 'z') {
			let b .= a;
		}
		return b;
	}

	public function testFor13()
	{
		char a; string b;
		let b = "";
		for a in range('0', '9') {
			let b .= a;
		}
		return b;
	}

	public function testFor14()
	{
		char a; string b;
		let b = "";
		for a in reverse range('0', '9') {
			let b .= a;
		}
		return b;
	}

	public function testFor15(int c, int d)
	{
		int a; int b;
		let b = 0;
		for a in reverse range(c, d) {
			let b += a;
		}
		return b;
	}

	public function testFor16()
	{
		var a, b, c;
		let c = [];
		for a, b in range(1, 10) {
			let c[a] = b;
		}
		return c;
	}

	public function testFor17()
	{
		var a, b, c;
		let c = [];
		for a, b in reverse range(1, 10) {
			let c[a] = b;
		}
		return c;
	}

	public function testFor18()
	{
		int a, b; var c;
		let c = [];
		for a, b in range(1, 10) {
			let c[a] = b;
		}
		return c;
	}

	public function testFor19()
	{
		var a, b;
		let b = 0;
		for a in range(1, 10, 2) {
			let b += a;
		}
		return b;
	}

	public function testFor20()
	{
		var a, b;
		let b = 0;
		for a in range(1, 10, 2) {
			let b += a;
		}
		return b;
	}

	public function testFor21()
	{
		var a, b;
		let b = 0;
		for a in range(10, 1, -2) {
			let b += a;
		}
		return b;
	}

	public function testFor22()
	{
		var a, b;
		let b = 0;
		for a in reverse range(10, 1, 2) {
			let b += a;
		}
		return b;
	}

	public function testFor23()
	{
		var a; string b;
		let b = "";
		for a in reverse range('a', 'z', 2) {
			let b .= (char) a;
		}
		return b;
	}

	public function testFor30()
	{
		var a; string b; int v;
		let a = [], b = "hello";
		for v in b {
			let a[] = v;
		}
		return a;
	}

	public function testFor31()
	{
		var a; string b; int k, v;
		let a = [], b = "hello";
		for k, v in b {
			let a[k] = v;
		}
		return a;
	}

	public function testSwitch1()
	{
		var a;
		let a = 10;
		switch a { }
		return a;
	}

	public function testSwitch2()
	{
		var a;
		let a = 10;
		switch a {
			case 10:
				return true;
		}
		return false;
	}

	public function testSwitch3()
	{
		var a;
		let a = 10;
		switch a {
			case 5:
				return true;
		}
		return false;
	}

	public function testSwitch4()
	{
		var a;
		let a = 10;
		switch a {
			case 5:
				return 1;
			default:
				return 2;
		}
		return 3;
	}

	public function testSwitch5()
	{
		var a;
		let a = 10;
		switch a {
			case 10:
				return 1;
			default:
				return 2;
		}
		return 3;
	}

	public function testSwitch6()
	{
		var a;
		let a = 10;
		switch a {
			case 10:
			case 4:
				return 1;
		}
		return 2;
	}

	public function testSwitch7()
	{
		var a;
		let a = 4;
		switch a {
			case 10:
			case 4:
				return 1;
		}
		return 2;
	}

	public function testSwitch8()
	{
		var a;
		let a = 5;
		switch a {
			case 10:
			case 4:
				return 1;
		}
		return 2;
	}

	public function testSwitch9()
	{
		var a;
		let a = 5;
		switch a {
			case 3:
			case 4:
				return 0;
			case 10:
			case 4:
				return 1;
		}
		return 2;
	}

	public function testSwitch10()
	{
		var a;
		let a = 5;
		switch a {
			case 3:
			case 4:
				return 0;
			case 10:
			case 4:
				return 1;
		}
		return 2;
	}

}
