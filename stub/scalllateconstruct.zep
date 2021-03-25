/**
 * Static Function call via constructor and late binding
 */

namespace Stub;

class ScallLateConstruct
{
    protected protectedVar;

    public function __construct()
    {
        this->testPublicInit();
    }
    
	static public function testStaticInit() -> string
	{
		return "hello public";
	}
    
    public function testPublicInit()
    {
        let this->protectedVar = self::testStaticInit();
    }
    
    public function varValue() -> string
    {
        return this->protectedVar;
    }
}
