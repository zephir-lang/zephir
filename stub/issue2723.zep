namespace Stub;

use Stub\Issue2723Base as BASE2723;

/**
 * Static calls whose method name, or class, is only known at run time.
 *
 * The method name of `{className}::{name}()` was emitted as the bare C name and
 * read by value, so a parameter did not compile. A dynamic name on a known
 * class, and every `{"name"}` form, compiled to nothing. The class part
 * dispatched an object or a non-string on stdClass and turned a missing class
 * into an uncatchable fatal error.
 *
 * Methods only: a property would shift every later property cache slot.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2723
 */
class Issue2723 extends Issue2723Base
{
    public static function target() -> string
    {
        return "target";
    }

    public static function calledClass() -> string
    {
        return get_called_class();
    }

    public static function withArgument(var argument) -> string
    {
        return "argument " . argument;
    }

    public function instanceTarget() -> string
    {
        return "instance of " . get_class(this);
    }

    /**
     * The reported shape: the method name is a parameter.
     */
    public function viaParam(var className, var name)
    {
        return {className}::{name}();
    }

    public function viaLocal(var className, var name)
    {
        var local;

        let local = name;

        return {className}::{local}();
    }

    public function viaStringParam(var className, string name)
    {
        return {className}::{name}();
    }

    public function viaMixedParam(var className, mixed name)
    {
        return {className}::{name}();
    }

    public function viaMixedClass(mixed className)
    {
        return {className}::target();
    }

    public function viaParamWithArgument(var className, var name, var argument)
    {
        return {className}::{name}(argument);
    }

    public function assignViaParam(var className, var name)
    {
        var result;

        let result = {className}::{name}();

        return result;
    }

    public function statementDynamicName(var className, var name, var counter)
    {
        {className}::{name}();

        return counter->bump();
    }

    public function statementLiteralName(var className, var counter)
    {
        {className}::target();

        return counter->bump();
    }

    public function missingClassSkipsArguments(var className, var counter)
    {
        return {className}::withArgument(counter->bump());
    }

    /**
     * The result variable is the variable holding the name.
     */
    public function assignToName(var className, var name)
    {
        let name = {className}::{name}();

        return name;
    }

    public function assignToMethodName(var receiver, var name)
    {
        let name = receiver->{name}();

        return name;
    }

    public function methodViaParam(var receiver, var name)
    {
        return receiver->{name}();
    }

    public function methodViaMixedParam(var receiver, mixed name)
    {
        return receiver->{name}();
    }

    public function selfDynamic(var name)
    {
        return self::{name}();
    }

    public static function selfDynamicStatic(var name)
    {
        return self::{name}();
    }

    public function selfDynamicStatement(var name, var counter)
    {
        self::{name}();

        return counter->bump();
    }

    public function parentDynamic(var name)
    {
        return parent::{name}();
    }

    public function parentDynamicWithArgument(var name, var argument)
    {
        var result;

        let result = parent::{name}(argument);

        return result;
    }

    public function namedDynamic(var name)
    {
        return Issue2723Base::{name}();
    }

    public static function namedDynamicStatic(var name)
    {
        return Issue2723Base::{name}();
    }

    public function internalDynamic(var name, var argument)
    {
        return \DateTime::{name}("!Y-m-d", argument);
    }

    public function selfString()
    {
        return self::{"target"}();
    }

    public function staticString()
    {
        return static::{"calledClass"}();
    }

    public function namedString()
    {
        return Issue2723Base::{"baseTarget"}();
    }

    public function constantString()
    {
        return BASE2723::{"baseTarget"}();
    }

    public function dynamicClassString(var className)
    {
        return {className}::{"target"}();
    }

    public function parentResultIntoParameter(var result)
    {
        let result = parent::baseTarget();

        return result;
    }
}
