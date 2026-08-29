
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
     * String-typed captures are covered separately by #2562
     * (see issue2562StringUse).
     */
    public function issue1873PropertyAndUse(var prefix) -> <\Closure>
    {
        return function () use (prefix) {
            return prefix . ":" . this->property1873;
        };
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2562
     *
     * Capturing a scalar `string`-typed variable in a closure via `use()`.
     * String params are stored as `zend_string *` (#2462), so the capture
     * must box them with ZVAL_STR, not ZVAL_STRING (which expects char *).
     */
    public function issue2562StringUse(string name) -> <\Closure>
    {
        return function () use (name) {
            return this->issue2497Helper() . ":" . name;
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

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2638
     *
     * Capturing a `string` **local**. Locals never use the native
     * `zend_string *` strategy, so the capture is already a zval and must
     * not be boxed again — boxing it emitted `ZVAL_STRING` on a zval, which
     * does not compile.
     */
    public function issue2638StringLocalUse() -> <\Closure>
    {
        string name = "";

        let name = "abc";

        return function () use (name) {
            return name;
        };
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2638
     *
     * Variant: a `string` **parameter** that is reassigned in the body. Two
     * mutations disqualify it from the native-string strategy, so it is held
     * as a zval exactly like a local — the same capture path, reachable
     * without changing any compiler option.
     */
    public function issue2638StringParamMutatedUse(string name) -> <\Closure>
    {
        let name = name . "!";

        return function () use (name) {
            return name;
        };
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     *
     * Captures used to live in `public static` properties on the synthetic
     * per-source-site closure class, so every closure created from one source
     * line shared a single slot and the newest creation overwrote the older
     * ones. Each of the methods below is called twice with different arguments
     * and both closures are kept alive, which is exactly the shape that fails.
     */
    public function issue2652Scalar(int n) -> <\Closure>
    {
        return function () use (n) {
            return n;
        };
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     *
     * A native `zend_string *` parameter: the capture goes through the
     * companion `<name>_zv` zval rather than the boxing switch.
     */
    public function issue2652Str(string name) -> <\Closure>
    {
        return function () use (name) {
            return name;
        };
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     */
    public function issue2652Arr(array items) -> <\Closure>
    {
        return function () use (items) {
            return items;
        };
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     *
     * An object capture is by handle in PHP: mutating the object after the
     * closure was created is visible inside it.
     */
    public function issue2652Obj(var box) -> <\Closure>
    {
        return function () use (box) {
            return box->value;
        };
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     *
     * Capture plus `this`: the closure's bound `$this` is the only
     * per-instance slot the engine gives an internal-function closure, so it
     * now holds the capture carrier and the enclosing object rides along on
     * it.
     */
    public function issue2652WithThis(var name) -> <\Closure>
    {
        return function () use (name) {
            return this->issue2497Helper() . ":" . name;
        };
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     *
     * `return this->prop;` is emitted as `RETURN_MEMBER(getThis(), ...)`,
     * which does not follow the rebound `this_ptr`. Without the fix this
     * reads the property off the capture carrier instead of the enclosing
     * object.
     */
    public function issue2652ReturnProperty(var suffix) -> <\Closure>
    {
        return function () use (suffix) {
            if suffix === "" {
                return this->_name;
            }

            return this->_name . suffix;
        };
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     *
     * Same story for `return this;`, emitted as `RETURN_THIS()`.
     */
    public function issue2652ReturnThis(var tag) -> <\Closure>
    {
        return function () use (tag) {
            if tag === "self" {
                return this;
            }

            return tag;
        };
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     *
     * Closures built in a loop each capture their own iteration value.
     */
    public function issue2652Loop() -> array
    {
        var result, i;

        let result = [];

        for i in [0, 1, 2] {
            let result[] = function () use (i) {
                return i;
            };
        }

        return result;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     *
     * A by-value capture is a snapshot taken when the closure is created;
     * later writes to the enclosing variable are not visible inside it.
     */
    public function issue2652Snapshot() -> <\Closure>
    {
        var value, callback;

        let value = 5;
        let callback = function () use (value) {
            return value;
        };
        let value = 99;

        return callback;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     *
     * Writing to a by-value capture inside the body is local to the call: PHP
     * re-seeds the bound variable on every invocation, so two calls of the
     * same closure both return the same value.
     */
    public function issue2652BodyMutation(int n) -> <\Closure>
    {
        return function () use (n) {
            let n = n + 1;

            return n;
        };
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     *
     * `use (&x)` shares one storage slot: writes inside the closure are
     * visible to every other holder of the same capture, and persist across
     * calls.
     */
    public function issue2652ByRefShared() -> array
    {
        var counter, bump, read;

        let counter = 0;

        let bump = function () use (&counter) {
            let counter = counter + 1;

            return counter;
        };

        let read = function () use (&counter) {
            return counter;
        };

        return [bump, read];
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     *
     * A by-reference write inside the closure is visible to the enclosing
     * scope.
     */
    public function issue2652ByRefWritesOut() -> int
    {
        var value, bump;

        let value = 0;

        let bump = function () use (&value) {
            let value = value + 10;
        };

        call_user_func(bump);
        call_user_func(bump);

        return value;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2652
     *
     * And a write in the enclosing scope after the closure was created is
     * visible inside it, which is what separates `use (&x)` from `use (x)`.
     */
    public function issue2652ByRefReadsLateWrite() -> <\Closure>
    {
        var value, reader;

        let value = 1;

        let reader = function () use (&value) {
            return value;
        };

        let value = 42;

        return reader;
    }
}
