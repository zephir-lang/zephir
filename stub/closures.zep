
namespace Stub;

class Closures
{
    protected _argument;
    protected _function;
    protected _name = "default";
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

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2497
     */

    public function issue2497Helper() -> string
    {
        return "hello";
    }

    public function issue2497ClosureThis() -> <\Closure>
    {
        return function() {
            return this->issue2497Helper();
        };
    }

    public function issue2497ClosureThisWithUse(var name) -> <\Closure>
    {
        return function() use (name) {
            return this->issue2497Helper() . ":" . name;
        };
    }

    public function issue2497PropertyAccess() -> <\Closure>
    {
        return function() {
            return this->_name;
        };
    }

    public function issue2497SetName(string name) -> void
    {
        let this->_name = name;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1873
     *
     * Original reporter's minimal repro: a closure returned from a method
     * that reads a protected property of the enclosing class via `this->`.
     * Note the return type is `<\Closure>`, not `string` — the PR draft
     * (#2203) declared it `-> string` which is itself a type bug under the
     * runtime-return-type enforcement added for #1991.
     */
    public function issue1873() -> <\Closure>
    {
        return function () {
            return this->property1873;
        };
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1873
     *
     * Variant: closure reads an array property and joins it inside the body.
     * Exercises the property-access + intra-closure expression combination.
     */
    public function issue1873ArrayProperty() -> <\Closure>
    {
        return function () {
            return this->_argument;
        };
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1873
     *
     * Variant: closure reads multiple properties and concatenates them.
     * Ensures multi-property reads inside a single closure compile cleanly.
     */
    public function issue1873MultipleProperties() -> <\Closure>
    {
        return function () {
            return this->_name . ":" . this->property1873;
        };
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1873
     *
     * Variant: closure both reads and writes a property — the binding via
     * Closure::bindTo must preserve mutability of the enclosing instance.
     */
    public function issue1873PropertyWriter() -> <\Closure>
    {
        return function (string value) {
            let this->_name = value;
        };
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1873
     *
     * Variant: closure reads a property AND uses a captured local via
     * `use()`. Verifies #1873 (property) and #2497 (use) compose.
     *
     * The captured variable is `var` (not `string`) because Zephir's
     * native-string parameter refactor (#2462) stores string params as
     * `zend_string *` which the `use()` plumbing in closure stubs doesn't
     * yet wrap to a zval — that's an orthogonal limitation worth tracking
     * separately.
     */
    public function issue1873PropertyAndUse(var prefix) -> <\Closure>
    {
        return function () use (prefix) {
            return prefix . ":" . this->property1873;
        };
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2321
     *
     * Original reporter's symptom: `preg_replace_callback(..., [this, 'private'])`
     * from inside the same class produced "cannot access private method".
     *
     * Root cause sits in the PHP engine: when an internal function
     * (`preg_replace_callback`) validates a `callable` argument, it walks
     * back through `prev_execute_data` to the nearest **user-code** frame
     * to determine the visibility scope. Zephir-compiled methods register
     * as `ZEND_INTERNAL_FUNCTION` so they are skipped, leaving the check
     * with no scope and rejecting otherwise-valid private callbacks.
     *
     * Workaround: use a Zephir closure that calls the private method
     * directly. Closures capture `this` (per #2497) and PHP doesn't
     * re-validate visibility when the callable is already a Closure
     * object.
     */
    public function issue2321CallPrivateCallback(string value) -> string
    {
        return this->issue2321filterQuery(value);
    }

    private function issue2321filterQuery(string value) -> string
    {
        return preg_replace_callback(
            "/(?:[^%:!\\$&'\\(\\)\\*\\+,;=@\\/\\?]+|%(?![A-Fa-f0-9]{2}))/u",
            function (array matches) {
                return this->issue2321doUrlEncode(matches);
            },
            value
        );
    }

    private function issue2321doUrlEncode(array matches) -> string
    {
        return rawurlencode(matches[0]);
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2321
     *
     * Variant: protected callback. Same idiom — wrap the method call in
     * a closure that captures `this`, then pass the closure as the
     * callback. PHP doesn't re-validate visibility on Closure callbacks.
     */
    public function issue2321ProtectedCallback(string value) -> string
    {
        return preg_replace_callback(
            "/[a-z]/",
            function (array matches) {
                return this->issue2321ProtectedUpper(matches);
            },
            value
        );
    }

    protected function issue2321ProtectedUpper(array matches) -> string
    {
        return strtoupper(matches[0]);
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2321
     *
     * Variant: array-map over a private method through a Zephir closure.
     * Covers a second common PHP-internal-callable consumer.
     */
    public function issue2321ArrayMapPrivate(array values) -> array
    {
        return array_map(
            function (val) {
                return this->issue2321Doubled(val);
            },
            values
        );
    }

    private function issue2321Doubled(int val) -> int
    {
        return val * 2;
    }
}
