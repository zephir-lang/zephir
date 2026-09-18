namespace Stub;

/**
 * PHP hands `__call()` and `__callStatic()` the method name exactly as it was
 * written at the call site. Codegen lower-cased the name to look the method up
 * in the class definition and emitted that lowered string as the C literal, so
 * a Zephir caller handed userland a name PHP would never have produced.
 *
 * The receivers are untyped on purpose: a magic method can only be reached
 * through a class the compiler cannot resolve, since a call to a method a known
 * class does not declare is a compile error.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2715
 */
class Issue2715
{
	public function callMixedCase(var receiver)
	{
		return receiver->NoPeCaSe();
	}

	public function callDynamicStringName(var receiver)
	{
		return receiver->{"NoPeCaSe"}();
	}

	public function callDynamicVariableName(var receiver, var name)
	{
		return receiver->{name}();
	}

	public function callStaticMixedCase(var className)
	{
		return {className}::NoPeCaSe();
	}

	public function callStaticDynamicName(var className)
	{
		string name = "NoPeCaSe";

		return {className}::{name}();
	}

	/**
	 * The shape `Phalcon\Di::__call()` has: the service name is derived from
	 * the spelling, so a lowered name looks up a service nobody registered.
	 */
	public function callServiceGetter(var receiver)
	{
		return receiver->getMyService();
	}

	/**
	 * Dispatch stays case-insensitive, as PHP's is.
	 */
	public function callDeclaredMethod(var receiver)
	{
		return receiver->DECLAREDmethod();
	}

	public static function staticControl() -> string
	{
		return "self";
	}

	public function callSelfMixedCase() -> string
	{
		return self::STATICCONTROL();
	}
}
