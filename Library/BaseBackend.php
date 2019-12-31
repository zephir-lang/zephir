<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

use Zephir\Fcall\FcallAwareInterface;
use Zephir\Fcall\FcallManagerInterface;

abstract class BaseBackend implements FcallAwareInterface
{
    /** @var Config */
    protected $config;

    /**
     * The name of the backend (e.g. ZendEngine2).
     *
     * @var string
     */
    protected $name;

    /** @var FcallManagerInterface */
    protected $fcallManager;

    /** @var string */
    protected $kernelsPath;

    /** @var string */
    protected $templatesPath;

    /**
     * BaseBackend constructor.
     *
     * @param Config $config
     * @param string $kernelsPath
     * @param string $templatesPath
     */
    public function __construct(Config $config, $kernelsPath, $templatesPath)
    {
        $this->config = $config;
        $this->kernelsPath = $kernelsPath;
        $this->templatesPath = $templatesPath;
    }

    /**
     * TODO: This should not be used, temporary (until its completely refactored).
     *
     * @return bool
     */
    abstract public function isZE3();

    public function getName()
    {
        return $this->name;
    }

    /**
     * Resolves the path to the source kernel files of the backend.
     *
     * @return string
     */
    public function getInternalKernelPath()
    {
        return "$this->kernelsPath/{$this->name}";
    }

    /**
     * Resolves the path to the source template files of the backend.
     *
     * @return string
     */
    public function getInternalTemplatePath()
    {
        return "$this->templatesPath/{$this->name}";
    }

    /**
     * Resolves the path to the source template file of the backend.
     *
     * @param string $filename
     *
     * @return string
     */
    public function getTemplateFileContents($filename)
    {
        $templatePath = rtrim($this->config->get('templatepath', 'backend'), '\\/');
        if (empty($templatepath)) {
            $templatePath = $this->templatesPath;
        }

        return file_get_contents(
            "{$templatePath}/{$this->name}/{$filename}"
        );
    }

    /**
     * @return StringsManager
     */
    abstract public function getStringsManager();

    abstract public function getTypeDefinition($type);

    /**
     * Checks the type of a variable using the ZendEngine constants.
     *
     * @param Variable           $variableVariable
     * @param string             $operator
     * @param string             $value
     * @param CompilationContext $context
     *
     * @return string
     */
    abstract public function getTypeofCondition(
        Variable $variableVariable,
        string $operator,
        string $value,
        CompilationContext $context
    ): string;

    /**
     * Initialize variable defaults.
     *
     * @param Variable[]         $variables
     * @param CompilationContext $compilationContext
     *
     * @return string
     */
    abstract public function initializeVariableDefaults($variables, CompilationContext $compilationContext): string;

    abstract public function generateInitCode(&$groupVariables, $type, $pointer, Variable $variable);

    /**
     * Returns the signature of an internal method.
     *
     * @param ClassMethod        $method
     * @param CompilationContext $context
     *
     * @return string
     */
    abstract public function getInternalSignature(ClassMethod $method, CompilationContext $context): string;

    abstract public function checkStrictType($type, $var, CompilationContext $context);

    abstract public function getBoolCode(Variable $variable, CompilationContext $context, $useCodePrinter = true);

    /**
     * Creates a temporary variable to be used to point to a heap variable.
     *
     * @param string             $type
     * @param CompilationContext $compilationContext
     * @param bool               $isLocal
     *
     * @return Variable
     */
    abstract public function getScalarTempVariable(
        string $type,
        CompilationContext $compilationContext,
        $isLocal = true
    ): Variable;

    abstract public function declareConstant($type, $name, $value, CompilationContext $context);

    /**
     * Assign value to variable.
     *
     * @param Variable           $variable
     * @param string|Variable    $value
     * @param CompilationContext $context
     * @param bool               $useCodePrinter
     *
     * @return string
     */
    abstract public function assignString(
        Variable $variable,
        $value,
        CompilationContext $context,
        bool $useCodePrinter = true
    ): string;

    abstract public function assignLong(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true);

    abstract public function assignDouble(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true);

    abstract public function assignBool(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true);

    abstract public function assignNull(Variable $variable, CompilationContext $context, $useCodePrinter = true);

    abstract public function assignZval(Variable $variable, $code, CompilationContext $context);

    abstract public function concatSelf(Variable $variable, Variable $itemVariable, CompilationContext $context);

    abstract public function returnString($value, CompilationContext $context, $useCodePrinter = true);

    abstract public function initArray(Variable $variable, CompilationContext $context, $size = null, $useCodePrinter = true);

    abstract public function createClosure(Variable $variable, $classDefinition, CompilationContext $context);

    abstract public function addArrayEntry(Variable $variable, $key, $value, CompilationContext $context, $statement = null, $useCodePrinter = true);

    abstract public function updateArray(Variable $symbolVariable, $key, $value, CompilationContext $compilationContext, $flags = null);

    abstract public function initObject(Variable $variable, $ce, CompilationContext $context, $useCodePrinter = true);

    abstract public function initVar(Variable $variable, CompilationContext $context, $useCodePrinter = true, $second = false);

    abstract public function zvalOperator($zvalOperator, Variable $expected, Variable $variableLeft, Variable $variableRight, CompilationContext $compilationContext);

    abstract public function fetchGlobal(Variable $globalVar, CompilationContext $compilationContext, $useCodePrinter = true);

    abstract public function fetchClass(Variable $var, $name, $guarded, CompilationContext $context);

    abstract public function fetchProperty(Variable $symbolVariable, Variable $variableVariable, $property, $readOnly, CompilationContext $context, $useOptimized = false);

    abstract public function fetchStaticProperty(Variable $symbolVariable, $classDefinition, $property, $readOnly, CompilationContext $context);

    abstract public function updateProperty(Variable $symbolVariable, $propertyName, $value, CompilationContext $compilationContext);

    abstract public function updateStaticProperty($classEntry, $property, $value, CompilationContext $context);
    abstract public function addStaticProperty($classEntry, $property, $value, CompilationContext $context);
    abstract public function subStaticProperty($classEntry, $property, $value, CompilationContext $context);

    abstract public function assignArrayProperty(Variable $variable, $property, $key, $value, CompilationContext $context);

    abstract public function checkConstructor(Variable $var, CompilationContext $context);

    /* Method calling */
    abstract public function callDynamicFunction($symbolVariable, Variable $variable, CompilationContext $compilationContext, $params = [], $cache = 'NULL', $cacheSlot = 0);

    abstract public function callMethod($symbolVariable, Variable $variable, $methodName, $cachePointer, $params, CompilationContext $context);

    /* Read from array */
    abstract public function arrayFetch(Variable $var, Variable $src, $index, $flags, $arrayAccess, CompilationContext $context, $useCodePrinter = true);

    abstract public function arrayIsset(Variable $var, $resolvedExpr, $expression, CompilationContext $context);

    abstract public function arrayIssetFetch(Variable $target, Variable $var, $resolvedExpr, $flags, $expression, CompilationContext $context);

    abstract public function propertyIsset(Variable $var, $key, CompilationContext $context);

    /* Unset array */
    abstract public function arrayUnset(Variable $variable, $exprIndex, $flags, CompilationContext $context);

    /* Array update multi */
    abstract public function assignArrayMulti(Variable $variable, $symbolVariable, $offsetExprs, CompilationContext $context);

    abstract public function assignPropertyArrayMulti(Variable $variable, $valueVariable, $propertyName, $offsetExprs, CompilationContext $context);

    abstract public function assignStaticPropertyArrayMulti($classEntry, $valueVariable, $propertyName, $offsetExprs, CompilationContext $compilationContext);

    abstract public function maybeSeparate(Variable $variableTempSeparated, Variable $variable, CompilationContext $context);

    abstract public function setSymbolIfSeparated(Variable $variableTempSeparated, Variable $variable, CompilationContext $context);

    abstract public function fetchClassEntry($str);

    abstract public function copyOnWrite(Variable $target, $var, CompilationContext $context);

    abstract public function ifVariableValueUndefined(Variable $var, CompilationContext $context, $onlyBody = false, $useCodePrinter = true);

    /* For statement */
    abstract public function forStatement(Variable $exprVariable, $keyVariable, $variable, $duplicateKey, $duplicateHash, $statement, $statementBlock, CompilationContext $context);

    abstract public function forStatementIterator(Variable $iteratorVariable, Variable $targetVariable, CompilationContext $compilationContext);

    abstract public function destroyIterator(Variable $iteratorVariable, CompilationContext $context);

    abstract public function onPreInitVar(ClassMethod $method, CompilationContext $context): string;

    abstract public function onPreCompile(ClassMethod $method, CompilationContext $context);

    abstract public function onPostCompile(ClassMethod $method, CompilationContext $context);

    /**
     * @param Variable $variable
     *
     * @return string
     */
    abstract public function getVariableCode(Variable $variable): string;

    /**
     * Get a double pointer to the variable.
     *
     * @param Variable $variable
     *
     * @return string
     */
    abstract public function getVariableCodePointer(Variable $variable);

    abstract public function resolveValue($value, CompilationContext $context, $usePointer = false);

    public static function getActiveBackend()
    {
        if (version_compare(PHP_VERSION, '7.0', '>=')) {
            return 'ZendEngine3';
        }

        return 'ZendEngine2';
    }

    /**
     * {@inheritdoc}
     *
     * @param FcallManagerInterface $fcallManager
     */
    public function setFcallManager(FcallManagerInterface $fcallManager)
    {
        $this->fcallManager = $fcallManager;
    }
}
