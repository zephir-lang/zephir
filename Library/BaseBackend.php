<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir;

use Zephir\Fcall\FcallAwareInterface;
use Zephir\Fcall\FcallManagerInterface;

abstract class BaseBackend implements FcallAwareInterface
{
    /**
     * @var Config
     */
    protected $config;

    /**
     * The name of the backend (e.g. ZendEngine2)
     * @var string
     */
    protected $name;

    /**
     * @var FcallManagerInterface
     */
    protected $fcallManager;

    /**
     * BaseBackend constructor
     *
     * @param Config $config
     * @throws Exception
     */
    public function __construct(Config $config)
    {
        $this->config = $config;
    }

    public function getName()
    {
        return $this->name;
    }

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

    /**
     * Resolves the path to the source template file of the backend
     *
     * @param string $filename
     * @return string Absolute path to template file
     */
    public function getTemplateFileContents($filename)
    {
        $filepath = realpath(rtrim($this->config->get('templatepath', 'backend'), '/').'/'.$this->name.'/'.$filename);
        if (!file_exists($filepath)) {
            $filepath = realpath(__DIR__ . '/../templates/'.$this->name.'/'.$filename);
        }
        return file_get_contents($filepath);
    }

    /**
     * @return StringsManager
     */
    abstract public function getStringsManager();

    abstract public function getTypeDefinition($type);
    abstract public function getTypeofCondition(Variable $variableVariable, $operator, $value, CompilationContext $context);
    abstract public function generateInitCode(&$groupVariables, $type, $pointer, Variable $variable);
    abstract public function getInternalSignature(ClassMethod $method, CompilationContext $context);
    abstract public function checkStrictType($type, $var, CompilationContext $context);

    abstract public function getBoolCode(Variable $variable, CompilationContext $context, $useCodePrinter = true);

    abstract public function getScalarTempVariable($type, CompilationContext $compilationContext, $isLocal = true);

    abstract public function declareConstant($type, $name, $value, CompilationContext $context);

    /* Assign values to variables */
    abstract public function assignString(Variable $variable, $value, CompilationContext $context, $useCodePrinter = true, $doCopy = true);
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
    abstract public function assignArrayProperty(Variable $variable, $property, $key, $value, CompilationContext $context);

    abstract public function checkConstructor(Variable $var, CompilationContext $context);

    /* Method calling */
    abstract public function callDynamicFunction($symbolVariable, Variable $variable, CompilationContext $compilationContext, $params = array(), $cache = 'NULL', $cacheSlot = 0);
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

    abstract public function onPreInitVar($method, CompilationContext $context);
    abstract public function onPreCompile($method, CompilationContext $context);
    abstract public function onPostCompile($method, CompilationContext $context);

    /**
     * @param Variable $variable
     * @return string
     */
    abstract public function getVariableCode(Variable $variable);

    /**
     * Get a double pointer to the variable
     *
     * @param Variable $variable
     * @return string
     */
    abstract public function getVariableCodePointer(Variable $variable);

    abstract public function resolveValue($value, CompilationContext $context, $usePointer = false);

    public static function getActiveBackend()
    {
        if (version_compare(phpversion(), '7.0', '>=')) {
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
