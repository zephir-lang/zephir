namespace Stub;

class Issue1404
{
	/* PHP Version Tests Using various data types */
	public function testIsPhpVersionUsingInteger70000() -> bool
	{
		return is_php_version(7);
	}

	public function testIsPhpVersionUsingDouble70100() -> bool
	{
		return is_php_version(7.1);
	}

	public function testIsPhpVersionUsingLong70000() -> bool
	{
		return is_php_version(2147483647);
	}

	public function testIsPhpVersionUsingString70000() -> bool
	{
		return is_php_version("7");
	}

	public function testIsPhpVersionUsingZero() -> bool
	{
		return is_php_version(0);
	}

	/* PHP Releases version Tests */
	public function testIsPhpVersionUsing50000() -> bool
	{
		return is_php_version("5");
	}

	public function testIsPhpVersionUsing50500() -> bool
	{
		return is_php_version("5.5");
	}

	public function testIsPhpVersionUsing50600() -> bool
	{
		return is_php_version("5.6");
	}

	public function testIsPhpVersionUsing70101() -> bool
	{
		return is_php_version("7.1.1");
	}

	public function testIsPhpVersionUsing70102() -> bool
	{
		return is_php_version("7.1.2");
	}

	public function testIsPhpVersionUsing70103() -> bool
	{
		return is_php_version("7.1.3");
	}

	public function testIsPhpVersionUsing70104() -> bool
	{
		return is_php_version("7.1.4");
	}

	public function testIsPhpVersionUsing70105() -> bool
	{
		return is_php_version("7.1.5");
	}

	public function testIsPhpVersionUsing70106() -> bool
	{
		return is_php_version("7.1.6");
	}

	public function testIsPhpVersionUsing70107() -> bool
	{
		return is_php_version("7.1.7");
	}

	public function testIsPhpVersionUsing70108() -> bool
	{
		return is_php_version("7.1.8");
	}

	public function testIsPhpVersionUsing70109() -> bool
	{
		return is_php_version("7.1.9");
	}

	public function testIsPhpVersionUsing70110() -> bool
	{
		return is_php_version("7.1.10");
	}

	public function testIsPhpVersionUsing70111() -> bool
	{
		return is_php_version("7.1.11");
	}

	public function testIsPhpVersionUsing70112() -> bool
	{
		return is_php_version("7.1.12");
	}

	public function testIsPhpVersionUsing70113() -> bool
	{
		return is_php_version("7.1.13");
	}

	public function testIsPhpVersionUsing70114() -> bool
	{
		return is_php_version("7.1.14");
	}

	public function testIsPhpVersionUsing70115() -> bool
	{
		return is_php_version("7.1.15");
	}

	public function testIsPhpVersionUsing70116() -> bool
	{
		return is_php_version("7.1.16");
	}

	public function testIsPhpVersionUsing70117() -> bool
	{
		return is_php_version("7.1.17");
	}

	/* PHP MINOR Version Tests */
	public function testIsPhpVersionUsing70100() -> bool
	{
		return is_php_version("7.1");
	}

	public function testIsPhpVersionUsing70200() -> bool
	{
		return is_php_version("7.2");
	}

	public function testIsPhpVersionUsing70300() -> bool
	{
		return is_php_version("7.3");
	}
}
