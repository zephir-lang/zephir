
namespace Test;

class Exists
{
	public function testClassExists(var className, bool autoload=true)
	{
		return class_exists(className, autoload);
	}

	public function testInterfaceExists(var interfaceName, bool autoload=true)
	{
		return interface_exists(interfaceName, autoload);
	}

	public function testMethodExists(var obj, var methodName)
	{
		return method_exists(obj, methodName);
	}

	public function testFileExists(var fileName)
	{
		return file_exists(fileName);
	}
}
