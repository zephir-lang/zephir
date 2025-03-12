
namespace Stub;

class Closures
{
    protected _argument;
    protected _function;
    protected property1873 = "call from closure";

	public function simple1()
	{
		return function(){ };
	}

	public function simple2()
	{
		return function() {

		};
	}

	public function simple3()
	{
		return function(param1) {

		};
	}

	public function simple4()
	{
		return function(var param1) {

		};
	}

	public function simple5()
	{
		return function(var param1) {
			/*return function() {
				return 1;
			};*/
			return null;
		};
	}

	public function arrow1()
	{
		return x => x + 100;
	}

	public function arrow2()
	{
		return x => (x + 100) + (x * 150);
	}

	public function testUseCommand()
	{
		var abc = 1;
		return function() use (abc) {
			return abc + 1;
		};
	}

	public function issue1860(array! abc)
	{
		return function () use (abc) {
			return abc["a"];
		};
	}

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/642
     */
	public function issue642()
	{
        var obj, closure;

        let obj = new \stdClass();
        let closure = function(config) { return config; };

        return call_user_func_array(closure, [obj]);
	}

	public function issue1036SetArgument(var argument)
	{
	    let this->_argument = argument;
	}

	public function issue1036SetFunction(callable func)
	{
	    let this->_function = func;
	}

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1036
     */
	public function issue1036Call()
	{
	    return call_user_func(this->_function, this->_argument);
	}

	public function issue1873() -> string
	{
	    return function () {
            return this->property1873;
        };
	}
}
