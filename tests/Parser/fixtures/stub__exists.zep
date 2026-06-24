
namespace Stub;

class Exists
{
	public function testClassExists(var className, bool autoload=true) -> bool
	{
		return class_exists(className, autoload);
	}

	public function testInterfaceExists(var interfaceName, bool autoload=true) -> bool
	{
		return interface_exists(interfaceName, autoload);
	}

	public function testMethodExists(var obj, var methodName) -> bool
	{
		return method_exists(obj, methodName);
	}

	public function testFileExists(var fileName) -> bool
	{
		return file_exists(fileName);
	}
}
