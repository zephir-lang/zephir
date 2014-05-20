
namespace Test;

class InternalClasses
{
	public function testStaticCall()
	{
        return \Phalcon\Di::getDefault();
	}

	public function testStaticPropertyFetch()
	{
	    /**
	     * @todo Rewrite code in zephir for dinamic property check
	     */
        return \Phalcon\Mvc\Model\Query::TYPE_DELETE;
	}
}