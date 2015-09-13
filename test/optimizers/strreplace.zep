
namespace Test\Optimizers;

class StrReplace
{
    /**
     * @link https://github.com/phalcon/zephir/issues/1055
     */
	static public function issue1055()
	{
		return str_replace(["aa"], ["aaa"], "bla bla aa aaa askks");
	}
}
