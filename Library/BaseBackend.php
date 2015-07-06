<?php
namespace Zephir;

abstract class BaseBackend
{
    /**
     * The name of the backend (e.g. ZendEngine2)
     * @var string
     */
    protected $name;

    /**
     * Resolves the path to the source kernel files of the backend
     * @return string Absolute path to kernel files
     */
    public function getInternalKernelPath()
    {
        return realpath(__DIR__ . '/../kernels/' . $this->name);
    }

    /**
     * Resolves the path to the source template files of the backend
     * @return string Absolute path to template files
     */
    public function getInternalTemplatePath()
    {
        return realpath(__DIR__ . '/../templates/' . $this->name);
    }

    abstract function getStringsManager();
    abstract function getTypeDefinition($type);
    abstract function generateInitCode(&$groupVariables, $type, $pointer, Variable $variable);

    /* Assign values to variables */
    abstract function assignString(Variable $variable, $value, CompilationContext $context, $useCodePrinter=true);
    abstract function assignLong(Variable $variable, $value, CompilationContext $context, $useCodePrinter=true);
    abstract function assignDouble(Variable $variable, $value, CompilationContext $context, $useCodePrinter=true);
    abstract function assignBool(Variable $variable, $value, CompilationContext $context, $useCodePrinter=true);
    abstract function initArray(Variable $variable, CompilationContext $context, $size=null, $useCodePrinter=true);
    abstract function addArrayEntry(Variable $variable, $key, $value, CompilationContext $context, $useCodePrinter=true);
    abstract function initObject(Variable $variable, $ce, CompilationContext $context, $useCodePrinter=true);
    abstract function fetchClass(Variable $var, $name, $guarded, CompilationContext $context);

    abstract function checkConstructor(Variable $var, CompilationContext $context);

    /* Method calling */
    abstract function callMethod($symbolVariable, Variable $variable, $methodName, $cachePointer, $params, CompilationContext $compilationContext);

    /* Read from array */
    abstract public function arrayFetch(Variable $var, Variable $src, $index, $flags, $arrayAccess, CompilationContext $context, $useCodePrinter=true);

    public static function getActiveBackend()
    {
        if (version_compare(phpversion(), '7.0', '>=')) {
            return 'ZendEngine3';
        }
        return 'ZendEngine2';
    }
}
