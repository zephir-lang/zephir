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

    abstract public function getStringsManager();
    abstract public function getTypeDefinition($type);
    abstract public function generateInitCode(&$groupVariables, $type, $pointer, Variable $variable);

    /* Assign values to variables */
    abstract public function assignString(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true);
    abstract public function assignLong(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true);
    abstract public function assignDouble(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true);
    abstract public function assignBool(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true);
    abstract public function initArray(Variable $variable, CompilationContext $context, $size = null, $useCodePrinter = true);
    abstract public function addArrayEntry(Variable $variable, $key, $value, CompilationContext $context, $useCodePrinter = true);
    abstract public function initObject(Variable $variable, $ce, CompilationContext $context, $useCodePrinter = true);
    abstract public function fetchClass(Variable $var, $name, $guarded, CompilationContext $context);

    abstract public function checkConstructor(Variable $var, CompilationContext $context);

    /* Method calling */
    abstract public function callMethod($symbolVariable, Variable $variable, $methodName, $cachePointer, $params, CompilationContext $context);

    /* Read from array */
    abstract public function arrayFetch(Variable $var, Variable $src, $index, $flags, $arrayAccess, CompilationContext $context, $useCodePrinter = true);

    /* Array update multi */
    abstract public function assignArrayMulti(Variable $variable, $symbolVariable, $offsetExprs, CompilationContext $context);

    public static function getActiveBackend()
    {
        if (version_compare(phpversion(), '7.0', '>=')) {
            return 'ZendEngine3';
        }
        return 'ZendEngine2';
    }
}
