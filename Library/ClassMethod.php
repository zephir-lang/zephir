<?php

/*
 +----------------------------------------------------------------------+
 | Zephir Language                                                      |
 +----------------------------------------------------------------------+
 | Copyright (c) 2013-2017 Zephir Team                                  |
 +----------------------------------------------------------------------+
 | This source file is subject to version 1.0 of the MIT license,       |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.zephir-lang.com/license                                   |
 |                                                                      |
 | If you did not receive a copy of the MIT license and are unable      |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
*/

namespace Zephir;

use Zephir\Compiler\CompilerException;
use Zephir\Passes\LocalContextPass;
use Zephir\Passes\StaticTypeInference;
use Zephir\Passes\CallGathererPass;
use Zephir\Detectors\WriteDetector;
use Zephir\Documentation\Docblock;
use Zephir\Documentation\DocblockParser;

/**
 * ClassMethod
 *
 * Represents a class method
 */
class ClassMethod
{
    /**
     * @var ClassDefinition
     */
    protected $classDefinition;

    /**
     * @var array
     */
    protected $visibility;

    /**
     * @var string
     */
    protected $name;

    /**
     * @var ClassMethodParameters
     */
    protected $parameters;

    protected $statements;

    /**
     * @var string
     */
    protected $docblock;

    /**
     * @var Documentation\Docblock
     */
    protected $parsedDocblock;

    /**
     * Types returned by the method
     *
     * @var array
     */
    protected $returnTypes = array();

    /**
     * Raw-types returned by the method
     *
     * @var array
     */
    protected $returnTypesRaw = array();

    /**
     * Class type hints returned by the method
     */
    protected $returnClassTypes = array();

    /**
     * Whether the variable is void
     *
     * @var boolean
     */
    protected $void = false;

    /**
     * Whether the method is public or not
     *
     * @var boolean
     */
    protected $isPublic = true;

    /**
     * Whether the method is static or not
     *
     * @var boolean
     */
    protected $isStatic = false;

    /**
     * Whether the method is final or not
     *
     * @var boolean
     */
    protected $isFinal = false;

    /**
     * Whether the method is abstract or not
     *
     * @var boolean
     */
    protected $isAbstract = false;

    /**
     * Whether the method is internal or not
     *
     * @var boolean
     */
    protected $isInternal = false;

    /**
     * Whether the method is bundled with PHP or not
     *
     * @var boolean
     */
    protected $isBundled = false;

    /**
     * Whether the method is an initializer or not
     *
     * @var boolean
     */
    protected $isInitializer = false;

    /**
     * @var array|null
     *
     * @var boolean
     */
    protected $expression;

    /**
     * LocalContextPass
     *
     * @var LocalContextPass
     */
    protected $localContext;

    /**
     * Static Type Inference Pass
     *
     * @var StaticTypeInference
     */
    protected $typeInference;

    /**
     * Call Gatherer Pass
     *
     * @var CallGathererPass
     */
    protected $callGathererPass;

    /**
     * ClassMethod constructor
     *
     * @param ClassDefinition $classDefinition
     * @param array $visibility
     * @param $name
     * @param $parameters
     * @param StatementsBlock $statements
     * @param null $docblock
     * @param null $returnType
     * @param array $original
     */
    public function __construct(ClassDefinition $classDefinition, array $visibility, $name, $parameters, StatementsBlock $statements = null, $docblock = null, $returnType = null, array $original = null)
    {
        $this->checkVisibility($visibility, $name, $original);

        $this->classDefinition = $classDefinition;
        $this->visibility = $visibility;
        $this->name = $name;
        $this->parameters = $parameters;
        $this->statements = $statements;
        $this->docblock = $docblock;
        $this->expression = $original;

        $this->setReturnTypes($returnType);
    }

    public function setReturnTypes($returnType)
    {
        $this->returnTypesRaw = $returnType;

        if (isset($returnType['void']) && $returnType['void']) {
            $this->void = true;
            return;
        }

        if (isset($returnType['list'])) {
            $types = array();
            $castTypes = array();
            foreach ($returnType['list'] as $returnTypeItem) {
                if (isset($returnTypeItem['cast'])) {
                    if (isset($returnTypeItem['cast']['collection'])) {
                        continue;
                    }
                    $castTypes[$returnTypeItem['cast']['value']] = $returnTypeItem['cast']['value'];
                } else {
                    $types[$returnTypeItem['data-type']] = $returnTypeItem;
                }
            }
            if (count($castTypes)) {
                $types['object'] = array();
                $this->returnClassTypes = $castTypes;
            }
            if (count($types)) {
                $this->returnTypes = $types;
            }
        }
    }

    /**
     * @return array
     */
    public function getReturnTypesRaw()
    {
        return $this->returnTypesRaw;
    }

    public function hasReturnTypesRaw()
    {
        return !empty($this->returnTypesRaw);
    }

    /**
     * Getter for statements block
     *
     * @return StatementsBlock $statements Statements block
     */
    public function getStatementsBlock()
    {
        return $this->statements;
    }

    /**
     * Setter for statements block
     *
     * @param StatementsBlock $statementsBlock
     */
    public function setStatementsBlock(StatementsBlock $statementsBlock)
    {
        $this->statements = $statementsBlock;
    }

    /**
     * Checks whether the method is empty
     *
     * @return boolean
     */
    public function isEmpty()
    {
        return $this->statements->isEmpty();
    }

    /**
     * Checks for visibility congruence
     *
     * @param array $visibility
     * @param string $name
     * @param array $original
     * @throws CompilerException
     */
    public function checkVisibility(array $visibility, $name, array $original = null)
    {
        if (count($visibility) > 1) {
            if (in_array('public', $visibility) && in_array('protected', $visibility)) {
                throw new CompilerException("Method '$name' cannot be 'public' and 'protected' at the same time", $original);
            }

            if (in_array('public', $visibility) && in_array('private', $visibility)) {
                throw new CompilerException("Method '$name' cannot be 'public' and 'private' at the same time", $original);
            }

            if (in_array('private', $visibility) && in_array('protected', $visibility)) {
                throw new CompilerException("Method '$name' cannot be 'protected' and 'private' at the same time", $original);
            }

            if (in_array('private', $visibility) && in_array('internal', $visibility)) {
                throw new CompilerException("Method '$name' cannot be 'internal' and 'private' at the same time", $original);
            }

            if (in_array('protected', $visibility) && in_array('internal', $visibility)) {
                throw new CompilerException("Method '$name' cannot be 'internal' and 'protected' at the same time", $original);
            }

            if (in_array('public', $visibility) && in_array('internal', $visibility)) {
                throw new CompilerException("Method '$name' cannot be 'internal' and 'public' at the same time", $original);
            }
        }

        if ($name == '__construct') {
            if (in_array('static', $visibility)) {
                throw new CompilerException("Constructors cannot be 'static'", $original);
            }
        } else {
            if ($name == '__destruct') {
                if (in_array('static', $visibility)) {
                    throw new CompilerException("Destructors cannot be 'static'", $original);
                }
            }
        }

        if (is_array($visibility)) {
            $this->isAbstract = in_array('abstract', $visibility);
            $this->isStatic = in_array('static', $visibility);
            $this->isFinal = in_array('final', $visibility);
            $this->isPublic = in_array('public', $visibility);
            $this->isInternal = in_array('internal', $visibility);
        }
    }

    /**
     * Sets if the method is internal or not
     *
     * @param boolean $static
     */
    public function setIsStatic($static)
    {
        $this->isStatic = $static;
    }

    /**
     * Sets if the method is internal or not
     *
     * @param boolean $internal
     */
    public function setIsInternal($internal)
    {
        $this->isInternal = $internal;
    }

    /**
     * Sets if the method is bundled or not
     *
     * @param boolean $bundled
     */
    public function setIsBundled($bundled)
    {
        $this->isBundled = $bundled;
    }

    /**
     * Sets if the method is an initializer or not
     *
     * @param boolean $initializer
     */
    public function setIsInitializer($initializer)
    {
        $this->isInitializer = $initializer;
    }

    /**
     * Returns the class definition where the method was declared
     *
     * @return ClassDefinition|null
     */
    public function getClassDefinition()
    {
        return $this->classDefinition;
    }

    /**
     * Generate internal method's based on the equivalent PHP methods,
     * allowing bypassing php userspace for internal method calls
     *
     * @param CompilationContext $compilationContext
     *
     * @return $this
     */
    public function setupOptimized(CompilationContext $compilationContext)
    {
        if (!$compilationContext->config->get('internal-call-transformation', 'optimizations')) {
            return $this;
        }
        $classDefinition = $this->getClassDefinition();
        /* Skip for closures */
        if ($this->getName() == '__invoke' || $classDefinition->isInterface()) {
            return $this;
        }
        if (!$this->isInternal() && !$classDefinition->isBundled()) {
            /* Not supported for now */
            if ($this->getNumberOfRequiredParameters() != $this->getNumberOfParameters()) {
                return $this;
            }
            if ($this->isConstructor()) {
                return $this;
            }
            $optimizedName = $this->getName() . '_zephir_internal_call';

            $visibility = array('internal');

            $statements = null;
            if ($this->statements) {
                $statements = new StatementsBlock(json_decode(json_encode($this->statements->getStatements()), true));
            }

            $optimizedMethod = new ClassMethod(
                $classDefinition,
                $visibility,
                $optimizedName,
                $this->parameters,
                $statements,
                $this->docblock,
                null,
                $this->expression
            );
            $optimizedMethod->typeInference = $this->typeInference;
            $optimizedMethod->setReturnTypes($this->returnTypes);
            $classDefinition->addMethod($optimizedMethod);
        }
    }
    public $optimizable = true;

    public function getOptimizedMethod()
    {
        $optimizedName = $this->getName() . '_zephir_internal_call';
        $optimizedMethod = $this->classDefinition->getMethod($optimizedName, false);
        if (!$optimizedMethod || !$this->optimizable) {
            return $this;
        }
        return $optimizedMethod;
    }

    /**
     * Sets the method name
     *
     * @param string $name
     */
    public function setName($name)
    {
        $this->name = $name;
    }

    /**
     * Returns the method name
     *
     * @return string
     */
    public function getName()
    {
        return $this->name;
    }

    /**
     * Returns the raw docblock
     *
     * @return string
     */
    public function getDocBlock()
    {
        return $this->docblock;
    }

    /**
     * Returns the parsed docblock
     *
     * @return Docblock
     */
    public function getParsedDocBlock()
    {
        if (!$this->parsedDocblock) {
            if (strlen($this->docblock) > 0) {
                $parser = new DocblockParser("/" . $this->docblock ."/");
                $this->parsedDocblock = $parser->parse();
            } else {
                return null;
            }
        }
        return $this->parsedDocblock;
    }

    /**
     * the starting line of the method in the source file
     * @return mixed
     */
    public function getLine()
    {
        return $this->expression["line"];
    }

    /**
     * the ending line of the method in the source file
     * @return mixed
     */
    public function getLastLine()
    {
        return $this->expression["last-line"];
    }

    /**
     * Returns the parameters
     *
     * @return ClassMethodParameters
     */
    public function getParameters()
    {
        return $this->parameters;
    }

    /**
     * Checks if the method has return-type or cast hints
     *
     * @return boolean
     */
    public function hasReturnTypes()
    {
        if (count($this->returnTypes)) {
            return true;
        }

        if (count($this->returnClassTypes)) {
            return true;
        }

        return false;
    }

    /**
     * Checks whether at least one return type hint is null compatible
     *
     * @param string $type
     * @return boolean
     */
    public function areReturnTypesNullCompatible($type = null)
    {
        if (count($this->returnTypes)) {
            foreach ($this->returnTypes as $returnType => $definition) {
                switch ($returnType) {
                    case 'null':
                        return true;
                }
            }
        }
        return false;
    }

    /**
     * Checks whether at least one return type hint is integer compatible
     *
     * @param string $type
     * @return boolean
     */
    public function areReturnTypesIntCompatible($type = null)
    {
        if (count($this->returnTypes)) {
            foreach ($this->returnTypes as $returnType => $definition) {
                switch ($returnType) {
                    case 'int':
                    case 'uint':
                    case 'char':
                    case 'uchar':
                    case 'long':
                    case 'ulong':
                        return true;
                }
            }
        }
        return false;
    }

    /**
     * Checks whether at least one return type hint is double compatible
     *
     * @param string $type
     * @return boolean
     */
    public function areReturnTypesDoubleCompatible($type = null)
    {
        if (count($this->returnTypes)) {
            foreach ($this->returnTypes as $returnType => $definition) {
                switch ($returnType) {
                    case 'double':
                        return true;
                }
            }
        }
        return false;
    }

    /**
     * Checks whether at least one return type hint is integer compatible
     *
     * @param string $type
     * @return boolean
     */
    public function areReturnTypesBoolCompatible($type = null)
    {
        if (count($this->returnTypes)) {
            foreach ($this->returnTypes as $returnType => $definition) {
                switch ($returnType) {
                    case 'bool':
                        return true;
                }
            }
        }
        return false;
    }

    /**
     * Checks whether at least one return type hint is integer compatible
     *
     * @param string $type
     * @return boolean
     */
    public function areReturnTypesStringCompatible($type = null)
    {
        if (count($this->returnTypes)) {
            foreach ($this->returnTypes as $returnType => $definition) {
                switch ($returnType) {
                    case 'string':
                        return true;
                }
            }
        }
        return false;
    }

    /**
     * Returned type hints by the method
     *
     * @return array
     */
    public function getReturnTypes()
    {
        return $this->returnTypes;
    }

    /**
     * Returned class-type hints by the method
     *
     * @return array
     */
    public function getReturnClassTypes()
    {
        return $this->returnClassTypes;
    }

    /**
     * Returns the number of parameters the method has
     *
     * @return boolean
     */
    public function hasParameters()
    {
        if (is_object($this->parameters)) {
            return count($this->parameters->getParameters()) > 0;
        }
        return false;
    }

    /**
     * Returns the number of parameters the method has
     *
     * @return int
     */
    public function getNumberOfParameters()
    {
        if (is_object($this->parameters)) {
            return count($this->parameters->getParameters());
        }
        return 0;
    }

    /**
     * Returns the number of required parameters the method has
     *
     * @return int
     */
    public function getNumberOfRequiredParameters()
    {
        if (is_object($this->parameters)) {
            $parameters = $this->parameters->getParameters();
            if (count($parameters)) {
                $required = 0;
                foreach ($parameters as $parameter) {
                    if (!isset($parameter['default'])) {
                        $required++;
                    }
                }
                return $required;
            }
        }
        return 0;
    }

    /**
     * Returns the number of required parameters the method has
     *
     * @return string
     */
    public function getInternalParameters()
    {
        if (is_object($this->parameters)) {
            $parameters = $this->parameters->getParameters();
            if (count($parameters)) {
                return count($parameters) . ', ...';
            }
        }
        return "";
    }

    /**
     * Checks whether the method has a specific modifier
     *
     * @param string $modifier
     * @return boolean
     */
    public function hasModifier($modifier)
    {
        foreach ($this->visibility as $visibility) {
            if ($visibility == $modifier) {
                return true;
            }
        }
        return false;
    }

    /**
     * Returns method visibility modifiers
     *
     * @return array
     */
    public function getVisibility()
    {
        return $this->visibility;
    }

    public function isDeprecated()
    {
        return !empty($this->visibility) && in_array('deprecated', $this->visibility, true);
    }

    /**
     * Returns the C-modifier flags
     *
     * @return string
     * @throws Exception
     */
    public function getModifiers()
    {
        $modifiers = array();
        foreach ($this->visibility as $visibility) {
            switch ($visibility) {
                case 'public':
                    $modifiers['ZEND_ACC_PUBLIC'] = $visibility;
                    break;

                case 'protected':
                    $modifiers['ZEND_ACC_PROTECTED'] = $visibility;
                    break;

                case 'private':
                    $modifiers['ZEND_ACC_PRIVATE'] = $visibility;
                    break;

                case 'static':
                    $modifiers['ZEND_ACC_STATIC'] = $visibility;
                    break;

                case 'final':
                    $modifiers['ZEND_ACC_FINAL'] = $visibility;
                    break;

                case 'abstract':
                    $modifiers['ZEND_ACC_ABSTRACT'] = $visibility;
                    break;

                case 'deprecated':
                    $modifiers['ZEND_ACC_DEPRECATED'] = $visibility;
                    break;

                case 'inline':
                    break;

                case 'scoped':
                    break;

                case 'internal':
                    break;

                default:
                    throw new Exception('Unknown modifier "' . $visibility . '"');
            }
        }

        if ($this->name == '__construct') {
            $modifiers['ZEND_ACC_CTOR'] = true;
        } else {
            if ($this->name == '__destruct') {
                $modifiers['ZEND_ACC_DTOR'] = true;
            }
        }

        return join('|', array_keys($modifiers));
    }

    /**
     * Checks if the method must not return any value
     *
     * @return boolean
     */
    public function isVoid()
    {
        return $this->void;
    }

    /**
     * Checks if the method is inline
     *
     * @return boolean
     */
    public function isInline()
    {
        if (is_array($this->visibility)) {
            return in_array('inline', $this->visibility);
        }
        return false;
    }

    /**
     * Checks if the method is private
     *
     * @return boolean
     */
    public function isPrivate()
    {
        if (is_array($this->visibility)) {
            return in_array('private', $this->visibility);
        }
        return false;
    }

    /**
     * Checks if the method is protected
     *
     * @return boolean
     */
    public function isProtected()
    {
        if (is_array($this->visibility)) {
            return in_array('protected', $this->visibility);
        }
        return false;
    }

    /**
     * Checks if the method is public
     *
     * @return boolean
     */
    public function isPublic()
    {
        return $this->isPublic;
    }

    /**
     * Checks is abstract method?
     *
     * @return bool
     */
    public function isAbstract()
    {
        return $this->isAbstract;
    }

    /**
     * Checks whether the method is static
     *
     * @return boolean
     */
    public function isStatic()
    {
        return $this->isStatic;
    }

    /**
     * Checks whether the method is final
     *
     * @return boolean
     */
    public function isFinal()
    {
        return $this->isFinal;
    }

    /**
     * Checks whether the method is internal
     *
     * @return boolean
     */
    public function isInternal()
    {
        return $this->isInternal;
    }

    /**
     * Checks whether the method is bundled
     *
     * @return boolean
     */
    public function isBundled()
    {
        return $this->isBundled;
    }

    /**
     * Checks whether the method is an initializer
     *
     * @return boolean
     */
    public function isInitializer()
    {
        return $this->isInitializer;
    }

    /**
     * Check whether the current method is a constructor
     *
     * @return boolean
     */
    public function isConstructor()
    {
        return $this->name == '__construct';
    }

    /**
     * Checks if method is a shortcut
     *
     * @return bool
     */
    public function isShortcut()
    {
        return $this->expression && $this->expression['type'] == 'shortcut';
    }

    /**
     * Return shortcut method name
     *
     * @return mixed
     */
    public function getShortcutName()
    {
        return $this->expression['name'];
    }

    /**
     * Returns the local context pass information
     *
     * @return LocalContextPass
     */
    public function getLocalContextPass()
    {
        return $this->localContext;
    }

    /**
     * Returns the static type inference pass information
     *
     * @return StaticTypeInference
     */
    public function getStaticTypeInferencePass()
    {
        return $this->typeInference;
    }

    /**
     * Returns the call gatherer pass information
     *
     * @return CallGathererPass
     */
    public function getCallGathererPass()
    {
        return $this->callGathererPass;
    }

    /**
     * Replace macros
     *
     * @param SymbolTable $symbolTable
     * @param string $containerCode
     *
     * @return mixed
     */
    public function removeMemoryStackReferences(SymbolTable $symbolTable, $containerCode)
    {
        if (!$symbolTable->getMustGrownStack()) {
            $containerCode = str_replace('ZEPHIR_THROW_EXCEPTION_STR', 'ZEPHIR_THROW_EXCEPTION_STRW', $containerCode);
            $containerCode = str_replace('ZEPHIR_THROW_EXCEPTION_DEBUG_STR', 'ZEPHIR_THROW_EXCEPTION_DEBUG_STRW', $containerCode);
            $containerCode = str_replace('ZEPHIR_THROW_EXCEPTION_ZVAL', 'ZEPHIR_THROW_EXCEPTION_ZVALW', $containerCode);
            $containerCode = str_replace('RETURN_THIS', 'RETURN_THISW', $containerCode);
            $containerCode = str_replace('RETURN_LCTOR', 'RETURN_LCTORW', $containerCode);
            $containerCode = str_replace('RETURN_CTOR', 'RETURN_CTORW', $containerCode);
            $containerCode = str_replace('RETURN_NCTOR', 'RETURN_NCTORW', $containerCode);
            $containerCode = str_replace('RETURN_CCTOR', 'RETURN_CCTORW', $containerCode);
            $containerCode = str_replace('RETURN_MM_NULL', 'RETURN_NULL', $containerCode);
            $containerCode = str_replace('RETURN_MM_BOOL', 'RETURN_BOOL', $containerCode);
            $containerCode = str_replace('RETURN_MM_FALSE', 'RETURN_FALSE', $containerCode);
            $containerCode = str_replace('RETURN_MM_TRUE', 'RETURN_TRUE', $containerCode);
            $containerCode = str_replace('RETURN_MM_STRING', 'RETURN_STRING', $containerCode);
            $containerCode = str_replace('RETURN_MM_LONG', 'RETURN_LONG', $containerCode);
            $containerCode = str_replace('RETURN_MM_DOUBLE', 'RETURN_DOUBLE', $containerCode);
            $containerCode = str_replace('RETURN_MM_FALSE', 'RETURN_FALSE', $containerCode);
            $containerCode = str_replace('RETURN_MM_EMPTY_STRING', 'RETURN_MM_EMPTY_STRING', $containerCode);
            $containerCode = str_replace('RETURN_MM_EMPTY_ARRAY', 'RETURN_EMPTY_ARRAY', $containerCode);
            $containerCode = str_replace('RETURN_MM_MEMBER', 'RETURN_MEMBER', $containerCode);
            $containerCode = str_replace('RETURN_MM()', 'return', $containerCode);
            $containerCode = preg_replace('/[ \t]+ZEPHIR_MM_RESTORE\(\);' . PHP_EOL . '/s', '', $containerCode);
        }
        return $containerCode;
    }

    /**
     * Assigns a default value
     *
     * @param array $parameter
     * @param CompilationContext $compilationContext
     * @return string
     * @throws CompilerException
     */
    public function assignDefaultValue(array $parameter, CompilationContext $compilationContext)
    {
        if (isset($parameter['data-type'])) {
            $dataType = $parameter['data-type'];
        } else {
            $dataType = 'variable';
        }

        /**
         * Class-Hinted parameters only can be null?
         */
        if (isset($parameter['cast'])) {
            if ($parameter['default']['type'] != 'null') {
                throw new CompilerException('Class-Hinted parameters only can have "null" as default parameter', $parameter);
            }
        }

        $oldCodePrinter = $compilationContext->codePrinter;
        $codePrinter = new CodePrinter();
        $codePrinter->increaseLevel();
        $codePrinter->increaseLevel();
        $compilationContext->codePrinter = $codePrinter;

        $paramVariable = $compilationContext->symbolTable->getVariableForWrite($parameter['name'], $compilationContext);
        /**
         * @todo Refactoring this place, move to one - static-constant-access
         */
        switch ($dataType) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                switch ($parameter['default']['type']) {
                    case 'static-constant-access':
                        /**
                         * Now I can write code for easy use on Expression because code in this method don't write with codePrinter ;(
                         * @todo Rewrite all to codePrinter
                         */
                        $symbolVariable = $compilationContext->symbolTable->getVariableForWrite($parameter['name'], $compilationContext, $parameter['default']);
                        $expression = new Expression($parameter['default']);
                        $expression->setExpectReturn(true, $symbolVariable);
                        $compiledExpression = $expression->compile($compilationContext);

                        if ($compiledExpression->getType() != 'int') {
                            throw new CompilerException("Default parameter value type: " . $compiledExpression->getType() . " cannot be assigned to variable(int)", $parameter);
                        }

                        $parameter['default']['type'] = $compiledExpression->getType();
                        $parameter['default']['value'] = $compiledExpression->getCode();

                        $compilationContext->codePrinter = $oldCodePrinter;
                        return $this->assignDefaultValue($parameter, $compilationContext);
                        break;

                    case 'null':
                        $codePrinter->output($parameter['name'] . ' = 0;');
                        break;

                    case 'int':
                    case 'uint':
                    case 'long':
                        $codePrinter->output($parameter['name'] . ' = ' . $parameter['default']['value'] . ';');
                        break;

                    case 'double':
                        $codePrinter->output($parameter['name'] . ' = (int) ' . $parameter['default']['value'] . ';');
                        break;

                    default:
                        throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(int)", $parameter);
                }
                break;

            case 'double':
                switch ($parameter['default']['type']) {
                    case 'static-constant-access':
                        /**
                         * Now I can write code for easy use on Expression because code in this method don't write with codePrinter ;(
                         * @todo Rewrite all to codePrinter
                         */
                        $symbolVariable = $compilationContext->symbolTable->getVariableForWrite($parameter['name'], $compilationContext, $parameter['default']);
                        $expression = new Expression($parameter['default']);
                        $expression->setExpectReturn(true, $symbolVariable);
                        $compiledExpression = $expression->compile($compilationContext);


                        if ($compiledExpression->getType() != 'double') {
                            throw new CompilerException("Default parameter value type: " . $compiledExpression->getType() . " cannot be assigned to variable(double)", $parameter);
                        }

                        $parameter['default']['type'] = $compiledExpression->getType();
                        $parameter['default']['value'] = $compiledExpression->getCode();

                        $compilationContext->codePrinter = $oldCodePrinter;
                        return $this->assignDefaultValue($parameter, $compilationContext);
                        break;

                    case 'null':
                        $codePrinter->output($parameter['name'] . ' = 0;');
                        break;

                    case 'int':
                    case 'uint':
                    case 'long':
                        $codePrinter->output($parameter['name'] . ' = (double) ' . $parameter['default']['value'] . ';');
                        break;

                    case 'double':
                        $codePrinter->output($parameter['name'] . ' = ' . $parameter['default']['value'] . ';');
                        break;

                    default:
                        throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(double)", $parameter);
                }
                break;

            case 'bool':
                switch ($parameter['default']['type']) {
                    case 'static-constant-access':
                        /**
                         * Now I can write code for easy use on Expression because code in this method don't write with codePrinter ;(
                         * @todo Rewrite all to codePrinter
                         */
                        $symbolVariable = $compilationContext->symbolTable->getVariableForWrite($parameter['name'], $compilationContext, $parameter['default']);
                        $expression = new Expression($parameter['default']);
                        $expression->setExpectReturn(true, $symbolVariable);
                        $compiledExpression = $expression->compile($compilationContext);


                        if ($compiledExpression->getType() != 'bool') {
                            throw new CompilerException("Default parameter value type: " . $compiledExpression->getType() . " cannot be assigned to variable(bool)", $parameter);
                        }

                        $parameter['default']['type'] = $compiledExpression->getType();
                        $parameter['default']['value'] = $compiledExpression->getCode();

                        $compilationContext->codePrinter = $oldCodePrinter;
                        return $this->assignDefaultValue($parameter, $compilationContext);
                        break;

                    case 'null':
                        $codePrinter->output($parameter['name'] . ' = 0;');
                        break;

                    case 'bool':
                        if ($parameter['default']['value'] == 'true') {
                            $codePrinter->output($parameter['name'] . ' = 1;');
                        } else {
                            $codePrinter->output($parameter['name'] . ' = 0;');
                        }
                        break;

                    default:
                        throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(bool)", $parameter);
                }
                break;

            case 'string':
                $compilationContext->symbolTable->mustGrownStack(true);
                $compilationContext->headersManager->add('kernel/memory');

                switch ($parameter['default']['type']) {
                    case 'static-constant-access':
                        /**
                         * Now I can write code for easy use on Expression because code in this method don't write with codePrinter ;(
                         * @todo Rewrite all to codePrinter
                         */
                        $symbolVariable = $compilationContext->symbolTable->getVariableForWrite($parameter['name'], $compilationContext, $parameter['default']);
                        $expression = new Expression($parameter['default']);
                        $expression->setExpectReturn(true, $symbolVariable);
                        $compiledExpression = $expression->compile($compilationContext);

                        if ($compiledExpression->getType() != 'string') {
                            throw new CompilerException("Default parameter value type: " . $compiledExpression->getType() . " cannot be assigned to variable(string)", $parameter);
                        }

                        $parameter['default']['type'] = $compiledExpression->getType();
                        $parameter['default']['value'] = $compiledExpression->getCode();

                        $compilationContext->codePrinter = $oldCodePrinter;
                        return $this->assignDefaultValue($parameter, $compilationContext);
                        break;

                    case 'null':
                        $compilationContext->backend->initVar($paramVariable, $compilationContext);
                        $compilationContext->backend->assignString($paramVariable, null, $compilationContext);
                        break;

                    case 'string':
                        $compilationContext->backend->initVar($paramVariable, $compilationContext);
                        $compilationContext->backend->assignString($paramVariable, Utils::addSlashes($parameter['default']['value'], true), $compilationContext);
                        break;

                    default:
                        throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(string)", $parameter);
                }
                break;

            case 'array':
                $compilationContext->symbolTable->mustGrownStack(true);
                $compilationContext->headersManager->add('kernel/memory');
                switch ($parameter['default']['type']) {
                    case 'null':
                        $compilationContext->backend->initVar($paramVariable, $compilationContext);
                        $compilationContext->backend->initArray($paramVariable, $compilationContext, null);
                        break;

                    case 'empty-array':
                    case 'array':
                        $compilationContext->backend->initVar($paramVariable, $compilationContext);
                        $compilationContext->backend->initArray($paramVariable, $compilationContext, null);
                        break;

                    default:
                        throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(array)", $parameter);
                }
                break;

            case 'variable':
                $symbolVariable = $compilationContext->symbolTable->getVariableForWrite($parameter['name'], $compilationContext, $parameter['default']);
                switch ($parameter['default']['type']) {
                    case 'static-constant-access':
                        /**
                         * Now I can write code for easy use on Expression because code in this method don't write with codePrinter ;(
                         * @todo Rewrite all to codePrinter
                         */
                        $expression = new Expression($parameter['default']);
                        $expression->setExpectReturn(true, $symbolVariable);
                        $compiledExpression = $expression->compile($compilationContext);

                        $parameter['default']['type'] = $compiledExpression->getType();
                        $parameter['default']['value'] = $compiledExpression->getCode();

                        $compilationContext->codePrinter = $oldCodePrinter;
                        return $this->assignDefaultValue($parameter, $compilationContext);
                        break;

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/memory');
                        $compilationContext->backend->initVar($symbolVariable, $compilationContext);
                        $compilationContext->backend->assignLong($symbolVariable, $parameter['default']['value'], $compilationContext);
                        break;

                    case 'double':
                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/memory');
                        $compilationContext->backend->initVar($symbolVariable, $compilationContext);
                        $compilationContext->backend->assignDouble($symbolVariable, $parameter['default']['value'], $compilationContext);
                        break;

                    case 'string':
                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/memory');
                        $compilationContext->backend->initVar($symbolVariable, $compilationContext);
                        $compilationContext->backend->assignString($paramVariable, Utils::addSlashes($parameter['default']['value'], true), $compilationContext);
                        break;

                    case 'bool':
                        $expectedMutations = $compilationContext->symbolTable->getExpectedMutations($parameter['name']);
                        if ($expectedMutations < 2) {
                            if ($parameter['default']['value'] == 'true') {
                                $compilationContext->backend->assignZval($paramVariable, $compilationContext->backend->resolveValue('true', $compilationContext), $compilationContext);
                            } else {
                                $compilationContext->backend->assignZval($paramVariable, $compilationContext->backend->resolveValue('false', $compilationContext), $compilationContext);
                            }
                        } else {
                            $compilationContext->symbolTable->mustGrownStack(true);
                            $compilationContext->headersManager->add('kernel/memory');
                            if ($parameter['default']['value'] == 'true') {
                                $compilationContext->backend->copyOnWrite($paramVariable, $compilationContext->backend->resolveValue('true', $compilationContext), $compilationContext);
                            } else {
                                $compilationContext->backend->copyOnWrite($paramVariable, $compilationContext->backend->resolveValue('false', $compilationContext), $compilationContext);
                            }
                        }
                        break;

                    case 'null':
                        $expectedMutations = $compilationContext->symbolTable->getExpectedMutations($parameter['name']);
                        if ($expectedMutations < 2) {
                            $compilationContext->backend->assignZval($symbolVariable, $compilationContext->backend->resolveValue('null', $compilationContext), $compilationContext);
                        } else {
                            $compilationContext->symbolTable->mustGrownStack(true);
                            $compilationContext->headersManager->add('kernel/memory');
                            $compilationContext->backend->copyOnWrite($paramVariable, $compilationContext->backend->resolveValue('null', $compilationContext), $compilationContext);
                        }
                        break;

                    case 'empty-array':
                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/memory');
                        $compilationContext->backend->initVar($symbolVariable, $compilationContext);
                        $compilationContext->backend->initArray($symbolVariable, $compilationContext, null);
                        break;

                    default:
                        throw new CompilerException("Default parameter value type: " . $parameter['default']['type'] . " cannot be assigned to variable(variable)", $parameter);
                }
                break;

            default:
                throw new CompilerException("Default parameter type: " . $dataType, $parameter);
        }

        $compilationContext->codePrinter = $oldCodePrinter;
        return $codePrinter->getOutput();
    }

    /**
     * Assigns a zval value to a static low-level type
     *
     * @todo rewrite this to build ifs and throw from builders
     *
     * @param array $parameter
     * @param CompilationContext $compilationContext
     * @return string
     * @throws CompilerException
     */
    public function checkStrictType(array $parameter, CompilationContext $compilationContext)
    {
        if (isset($parameter['data-type'])) {
            $dataType = $parameter['data-type'];
        } else {
            $dataType = 'variable';
        }

        $compilationContext->headersManager->add('ext/spl/spl_exceptions');
        $compilationContext->headersManager->add('kernel/exception');

        $codePrinter = new CodePrinter();
        $codePrinter->increaseLevel();
        $oldCodePrinter = $compilationContext->codePrinter;
        $compilationContext->codePrinter = $codePrinter;
        $compilationContext->backend->checkStrictType($dataType, $parameter, $compilationContext);

        $compilationContext->codePrinter = $oldCodePrinter;
        return $codePrinter->getOutput();
    }

    /**
     * Assigns a zval value to a static low-level type
     *
     * @param array $parameter
     * @param CompilationContext $compilationContext
     * @return string
     * @throws CompilerException
     */
    public function assignZvalValue(array $parameter, CompilationContext $compilationContext)
    {
        if (isset($parameter['data-type'])) {
            $dataType = $parameter['data-type'];
        } else {
            $dataType = 'variable';
        }

        if (in_array($dataType, array('variable', 'callable', 'object', 'resource'))) {
            return;
        }

        $compilationContext->headersManager->add('kernel/operators');
        $parameterVariable = $compilationContext->symbolTable->getVariableForWrite($parameter['name'] . '_param', $compilationContext);
        $parameterCode = $compilationContext->backend->getVariableCode($parameterVariable);
        $inputParamVar = $compilationContext->symbolTable->getVariableForWrite($parameter['name'], $compilationContext);
        $inputParamCode = $compilationContext->backend->getVariableCode($inputParamVar);
        switch ($dataType) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                return "\t" . $parameter['name'] . ' = zephir_get_intval(' . $parameterCode . ');' . PHP_EOL;

            case 'char':
                return "\t" . $parameter['name'] . ' = zephir_get_charval(' . $parameterCode . ');' . PHP_EOL;

            case 'bool':
                return "\t" . $parameter['name'] . ' = zephir_get_boolval(' . $parameterCode . ');' . PHP_EOL;

            case 'double':
                return "\t" . $parameter['name'] . ' = zephir_get_doubleval(' . $parameterCode . ');' . PHP_EOL;

            case 'string':
                $compilationContext->symbolTable->mustGrownStack(true);
                return "\t" . 'zephir_get_strval(' . $inputParamCode . ', ' . $parameterCode . ');' . PHP_EOL;

            case 'array':
                $compilationContext->symbolTable->mustGrownStack(true);
                return "\t" . 'zephir_get_arrval(' . $inputParamCode . ', ' . $parameterCode . ');' . PHP_EOL;

            default:
                throw new CompilerException("Parameter type: " . $dataType, $parameter);
        }
    }

    /**
     * Pre-compiles the method making compilation pass data (static inference, local-context-pass) available to other methods
     *
     * @param CompilationContext $compilationContext
     * @return null
     * @throws CompilerException
     */
    public function preCompile(CompilationContext $compilationContext)
    {
        $localContext = null;
        $typeInference = null;
        $callGathererPass = null;

        if (is_object($this->statements)) {
            $compilationContext->currentMethod = $this;

            /**
             * This pass checks for zval variables than can be potentially
             * used without allocating memory and track it
             * these variables are stored in the stack
             */
            if ($compilationContext->config->get('local-context-pass', 'optimizations')) {
                $localContext = new LocalContextPass();
                $localContext->pass($this->statements);
            }

            /**
             * This pass tries to infer types for dynamic variables
             * replacing them by low level variables
             */
            if ($compilationContext->config->get('static-type-inference', 'optimizations')) {
                $typeInference = new StaticTypeInference();
                $typeInference->pass($this->statements);
                if ($compilationContext->config->get('static-type-inference-second-pass', 'optimizations')) {
                    $typeInference->reduce();
                    $typeInference->pass($this->statements);
                }
            }

            /**
             * This pass counts how many times a specific
             */
            if ($compilationContext->config->get('call-gatherer-pass', 'optimizations')) {
                $callGathererPass = new CallGathererPass($compilationContext);
                $callGathererPass->pass($this->statements);
            }
        }

        $this->localContext = $localContext;
        $this->typeInference = $typeInference;
        $this->callGathererPass = $callGathererPass;
    }

    /**
     * Compiles the method
     *
     * @param CompilationContext $compilationContext
     * @return null
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext)
    {
        /**
         * Set the method currently being compiled
         */
        $compilationContext->currentMethod = $this;

        /**
         * Initialize the method warm-up to null
         */
        $compilationContext->methodWarmUp = null;

        /**
         * Assign pre-made compilation passes
         */
        $localContext = $this->localContext;
        $typeInference = $this->typeInference;
        $callGathererPass = $this->callGathererPass;

        /**
         * Every method has its own symbol table
         */
        $symbolTable = new SymbolTable($compilationContext);
        if ($localContext) {
            $symbolTable->setLocalContext($localContext);
        }

        /**
         * Parameters has an additional extra mutation
         */
        $parameters = $this->parameters;
        if ($localContext) {
            if (is_object($parameters)) {
                foreach ($parameters->getParameters() as $parameter) {
                    $localContext->increaseMutations($parameter['name']);
                }
            }
        }

        /**
         * Initialization of parameters happens in a fictitious external branch
         */
        $branch = new Branch();
        $branch->setType(Branch::TYPE_EXTERNAL);

        /**
         * BranchManager helps to create graphs of conditional/loop/root/jump branches
         */
        $branchManager = new BranchManager();
        $branchManager->addBranch($branch);

        /**
         * Cache Manager manages function calls, method calls and class entries caches
         */
        $cacheManager = new CacheManager();
        $cacheManager->setGatherer($callGathererPass);

        $compilationContext->branchManager = $branchManager;
        $compilationContext->cacheManager  = $cacheManager;
        $compilationContext->typeInference = $typeInference;
        $compilationContext->symbolTable   = $symbolTable;

        $oldCodePrinter = $compilationContext->codePrinter;

        /**
         * Change the code printer to a single method instance
         */
        $codePrinter = new CodePrinter();
        $compilationContext->codePrinter = $codePrinter;

        /**
         * Set an empty function cache
         */
        $compilationContext->functionCache = null;

        /**
         * Reset try/catch and loop counter
         */
        $compilationContext->insideCycle = 0;
        $compilationContext->insideTryCatch = 0;
        $compilationContext->currentTryCatch = 0;

        if (is_object($parameters)) {
            /**
             * Round 1. Create variables in parameters in the symbol table
             */
            $classCastChecks = array();
            $substituteVars = array();
            foreach ($parameters->getParameters() as $parameter) {
                /**
                 * Change dynamic variables to low level types
                 */
                if ($typeInference) {
                    if (isset($parameter['data-type'])) {
                        if ($parameter['data-type'] == 'variable') {
                            $type = $typeInference->getInferedType($parameter['name']);
                            if (is_string($type)) {
                                /* promote polymorphic parameters to low level types */
                            }
                        }
                    } else {
                        $type = $typeInference->getInferedType($parameter['name']);
                        if (is_string($type)) {
                            /* promote polymorphic parameters to low level types */
                        }
                    }
                }

                $symbolParam = null;

                if (isset($parameter['data-type'])) {
                    switch ($parameter['data-type']) {
                        case 'object':
                        case 'callable':
                        case 'resource':
                        case 'variable':
                            $symbol = $symbolTable->addVariable($parameter['data-type'], $parameter['name'], $compilationContext);
                            /* TODO: Move this to the respective backend, which requires refactoring how this works */
                            if ($compilationContext->backend->isZE3()) {
                                $symbol->setIsDoublePointer(true);
                                $substituteVars[$parameter['name']] = $symbolTable->addVariable('variable', $parameter['name'] . '_sub', $compilationContext);
                            }
                            break;

                        default:
                            $symbol = $symbolTable->addVariable($parameter['data-type'], $parameter['name'], $compilationContext);
                            $symbolParam = $symbolTable->addVariable('variable', $parameter['name'] . '_param', $compilationContext);
                            /* TODO: Move this to the respective backend, which requires refactoring how this works */
                            if ($compilationContext->backend->isZE3()) {
                                $symbolParam->setIsDoublePointer(true);
                            }
                            if ($parameter['data-type'] == 'string' || $parameter['data-type'] == 'array') {
                                $symbol->setMustInitNull(true);
                            }
                            break;
                    }
                } else {
                    $symbol = $symbolTable->addVariable('variable', $parameter['name'], $compilationContext);
                }

                /* ZE3 only */
                if (isset($substituteVars[$parameter['name']])) {
                    $substituteVar = $substituteVars[$parameter['name']];
                    $substituteVar->increaseUses();
                }

                /**
                 * Some parameters can be read-only
                 */
                if (isset($parameter['const']) && $parameter['const']) {
                    $symbol->setReadOnly(true);
                    if (is_object($symbolParam)) {
                        $symbolParam->setReadOnly(true);
                    }
                }

                if (is_object($symbolParam)) {
                    /**
                     * Parameters are marked as 'external'
                     */
                    $symbolParam->setIsExternal(true);

                    /**
                     * Assuming they're initialized
                     */
                    $symbolParam->setIsInitialized(true, $compilationContext, $parameter);

                    /**
                     * Initialize auxiliary parameter zvals to null
                     */
                    $symbolParam->setMustInitNull(true);

                    /**
                     * Increase uses
                     */
                    $symbolParam->increaseUses();
                } else {
                    if (isset($parameter['default'])) {
                        if (isset($parameter['data-type'])) {
                            if ($parameter['data-type'] == 'variable') {
                                $symbol->setMustInitNull(true);
                            }
                        } else {
                            $symbol->setMustInitNull(true);
                        }
                    }
                }

                /**
                 * Original node where the variable was declared
                 */
                $symbol->setOriginal($parameter);

                /**
                 * Parameters are marked as 'external'
                 */
                $symbol->setIsExternal(true);

                /**
                 * Assuming they're initialized
                 */
                $symbol->setIsInitialized(true, $compilationContext);

                /**
                 * Variables with class/type must be objects across the execution
                 */
                if (isset($parameter['cast'])) {
                    $symbol->setDynamicTypes('object');
                    $symbol->setClassTypes($compilationContext->getFullName($parameter['cast']['value']));
                    $classCastChecks[] = array($symbol, $parameter);
                } else {
                    if (isset($parameter['data-type'])) {
                        if ($parameter['data-type'] == 'variable') {
                            $symbol->setDynamicTypes('undefined');
                        }
                    } else {
                        $symbol->setDynamicTypes('undefined');
                    }
                }
            }
        }

        $compilationContext->backend->onPreCompile($this, $compilationContext);

        /**
         * Compile the block of statements if any
         */
        if (is_object($this->statements)) {
            if ($this->hasModifier('static')) {
                $compilationContext->staticContext = true;
            } else {
                $compilationContext->staticContext = false;
            }

            /**
             * Compile the statements block as a 'root' branch
             */
            $this->statements->compile($compilationContext, false, Branch::TYPE_ROOT);
        }

        /**
         * Initialize variable default values
         */
        $initVarCode = $compilationContext->backend->initializeVariableDefaults($symbolTable->getVariables(), $compilationContext);

        /**
         * Fetch parameters from vm-top
         */
        $initCode = "";
        $code = "";
        if (is_object($parameters)) {
            /**
             * Round 2. Fetch the parameters in the method
             */
            $params = array();
            $requiredParams = array();
            $optionalParams = array();
            $numberRequiredParams = 0;
            $numberOptionalParams = 0;
            foreach ($parameters->getParameters() as $parameter) {
                if (isset($parameter['data-type'])) {
                    $dataType = $parameter['data-type'];
                } else {
                    $dataType = 'variable';
                }

                switch ($dataType) {
                    case 'object':
                    case 'callable':
                    case 'resource':
                    case 'variable':
                        if (!$this->isInternal()) {
                            $params[] = '&' . $parameter['name'];
                        } else {
                            $params[] = $parameter['name'];
                        }
                        break;

                    default:
                        if (!$this->isInternal()) {
                            $params[] = '&' . $parameter['name'] . '_param';
                        } else {
                            $params[] = $parameter['name'] . '_param';
                        }
                        break;
                }

                if (isset($parameter['default'])) {
                    $optionalParams[] = $parameter;
                    $numberOptionalParams++;
                } else {
                    $requiredParams[] = $parameter;
                    $numberRequiredParams++;
                }
            }

            /**
             * Pass the write detector to the method statement block to check if the parameter
             * variable is modified so as do the proper separation
             */
            $parametersToSeparate = array();
            if (is_object($this->statements)) {
                /**
                 * If local context is not available
                 */
                if (!$localContext) {
                    $writeDetector = new WriteDetector();
                }

                foreach ($parameters->getParameters() as $parameter) {
                    if (isset($parameter['data-type'])) {
                        $dataType = $parameter['data-type'];
                    } else {
                        $dataType = 'variable';
                    }

                    switch ($dataType) {
                        case 'variable':
                        case 'string':
                        case 'array':
                        case 'resource':
                        case 'object':
                        case 'callable':
                            $name = $parameter['name'];
                            if (!$localContext) {
                                if ($writeDetector->detect($name, $this->statements->getStatements())) {
                                    $parametersToSeparate[$name] = true;
                                }
                            } else {
                                if ($localContext->getNumberOfMutations($name) > 1) {
                                    $parametersToSeparate[$name] = true;
                                }
                            }
                            break;
                    }
                }
            }

            /**
             * Initialize required parameters
             */
            foreach ($requiredParams as $parameter) {
                if (isset($parameter['mandatory'])) {
                    $mandatory = $parameter['mandatory'];
                } else {
                    $mandatory = 0;
                }

                if (isset($parameter['data-type'])) {
                    $dataType = $parameter['data-type'];
                } else {
                    $dataType = 'variable';
                }

                if ($dataType != 'variable') {
                    /**
                     * Assign value from zval to low level type
                     */
                    if ($mandatory) {
                        $initCode .= $this->checkStrictType($parameter, $compilationContext);
                    } else {
                        $initCode .= $this->assignZvalValue($parameter, $compilationContext);
                    }
                }

                switch ($dataType) {
                    case 'variable':
                    case 'resource':
                    case 'object':
                    case 'callable':
                        if (isset($parametersToSeparate[$parameter['name']])) {
                            $symbolTable->mustGrownStack(true);
                            $initCode .= "\t" . "ZEPHIR_SEPARATE_PARAM(" . $parameter['name'] . ");" . PHP_EOL;
                        }
                        break;
                }
            }

            /**
             * Initialize optional parameters
             */
            foreach ($optionalParams as $parameter) {
                if (isset($parameter['mandatory'])) {
                    $mandatory = $parameter['mandatory'];
                } else {
                    $mandatory = 0;
                }

                if (isset($parameter['data-type'])) {
                    $dataType = $parameter['data-type'];
                } else {
                    $dataType = 'variable';
                }

                switch ($dataType) {
                    case 'object':
                    case 'callable':
                    case 'resource':
                    case 'variable':
                        $name = $parameter['name'];
                        break;

                    default:
                        $name = $parameter['name'] . '_param';
                        break;
                }

                /**
                 * Assign the default value according to the variable's type
                 */
                $targetVar = $compilationContext->symbolTable->getVariableForWrite($name, $compilationContext);
                $initCode .= "\t" . $compilationContext->backend->ifVariableValueUndefined($targetVar, $compilationContext, false, false) . PHP_EOL;

                if ($compilationContext->backend->isZE3()) {
                    if ($targetVar->isDoublePointer() && isset($substituteVars[$parameter['name']])) {
                        $substituteVar = $substituteVars[$parameter['name']];
                        $initCode .= "\t\t" . $targetVar->getName() . ' = &' . $substituteVar->getName() . ';' . PHP_EOL;
                    }
                }
                $initCode .= $this->assignDefaultValue($parameter, $compilationContext);

                if (isset($parametersToSeparate[$name]) || $dataType != 'variable') {
                    $initCode .= "\t" . '} else {' . PHP_EOL;
                    if (isset($parametersToSeparate[$name])) {
                        $initCode .= "\t\t" . "ZEPHIR_SEPARATE_PARAM(" . $name . ");" . PHP_EOL;
                    } else {
                        if ($mandatory) {
                            $initCode .= $this->checkStrictType($parameter, $compilationContext, $mandatory);
                        } else {
                            $initCode .= "\t" . $this->assignZvalValue($parameter, $compilationContext);
                        }
                    }
                }

                $initCode .= "\t" . '}' . PHP_EOL;
            }

            /**
             * Fetch the parameters to zval pointers
             */
            $codePrinter->preOutputBlankLine();

            if (!$this->isInternal()) {
                $compilationContext->headersManager->add('kernel/memory');
                if ($symbolTable->getMustGrownStack()) {
                    $code .= "\t" . 'zephir_fetch_params(1, ' . $numberRequiredParams . ', ' . $numberOptionalParams . ', ' . join(', ', $params) . ');' . PHP_EOL;
                } else {
                    $code .= "\t" . 'zephir_fetch_params(0, ' . $numberRequiredParams . ', ' . $numberOptionalParams . ', ' . join(', ', $params) . ');' . PHP_EOL;
                }
            } else {
                foreach ($params as $param) {
                    /* TODO: Migrate all this code to codeprinter, get rid of temp code printer */
                    $tempCodePrinter = new CodePrinter();
                    $realCodePrinter = $compilationContext->codePrinter;
                    $compilationContext->codePrinter = $tempCodePrinter;
                    $paramVar = $compilationContext->symbolTable->getVariableForRead($param, $compilationContext);
                    $compilationContext->backend->assignZval($paramVar, $param . '_ext', $compilationContext);
                    $code .= "\t" . $tempCodePrinter->getOutput() . PHP_EOL;
                    $compilationContext->codePrinter = $realCodePrinter;
                }
            }
            $code .= PHP_EOL;
        }

        $code .= $initCode . $initVarCode;
        $codePrinter->preOutput($code);

        /**
         * Fetch used superglobals
         */
        foreach ($symbolTable->getVariables() as $name => $variable) {
            if ($variable->isSuperGlobal()) {
                $globalVar = $symbolTable->getVariable($name);
                $codePrinter->preOutput("\t" . $compilationContext->backend->fetchGlobal($globalVar, $compilationContext, false));
            }
        }

        /**
         * Grow the stack if needed
         */
        if ($symbolTable->getMustGrownStack()) {
            $compilationContext->headersManager->add('kernel/memory');
            $codePrinter->preOutput("\t" . 'ZEPHIR_MM_GROW();');
        }

        /**
         * Check if there are unused variables
         */
        $usedVariables = array();
        $classDefinition = $this->getClassDefinition();
        if ($classDefinition) {
            $completeName = $classDefinition->getCompleteName();
        } else {
            $completeName = '[unknown]';
        }

        foreach ($symbolTable->getVariables() as $variable) {
            if ($variable->getNumberUses() <= 0) {
                if ($variable->isExternal() == false) {
                    $compilationContext->logger->warning('Variable "' . $variable->getName() . '" declared but not used in ' . $completeName . '::' . $this->getName(), "unused-variable", $variable->getOriginal());
                    continue;
                }
                $compilationContext->logger->warning('Variable "' . $variable->getName() . '" declared but not used in ' . $completeName . '::' . $this->getName(), "unused-variable-external", $variable->getOriginal());
            }

            if ($variable->getName() != 'this_ptr' && $variable->getName() != 'return_value' && $variable->getName() != 'return_value_ptr') {
                $type = $variable->getType();
                if (!isset($usedVariables[$type])) {
                    $usedVariables[$type] = array();
                }
                $usedVariables[$type][] = $variable;
            }
        }

        /**
         * Check if there are assigned but not used variables
         * Warn whenever a variable is unused aside from its declaration.
         */
        foreach ($symbolTable->getVariables() as $variable) {
            if ($variable->isExternal() == true || $variable->isTemporal()) {
                continue;
            }

            if ($variable->getName() == 'this_ptr' || $variable->getName() == 'return_value' || $variable->getName() == 'return_value_ptr' || $variable->getName() == 'ZEPHIR_LAST_CALL_STATUS') {
                continue;
            }

            if (!$variable->isUsed()) {
                $node = $variable->getLastUsedNode();
                if (is_array($node)) {
                    $compilationContext->logger->warning('Variable "' . $variable->getName() . '" assigned but not used in ' . $completeName . '::' . $this->getName(), "unused-variable", $node);
                } else {
                    $compilationContext->logger->warning('Variable "' . $variable->getName() . '" assigned but not used in ' . $completeName . '::' . $this->getName(), "unused-variable", $variable->getOriginal());
                }
            }
        }

        if (count($usedVariables)) {
            $codePrinter->preOutputBlankLine();
        }

        /**
         * Generate the variable definition for variables used
         */
        $initCode = "\t".implode(PHP_EOL."\t", $compilationContext->backend->declareVariables($this, $usedVariables, $compilationContext));
        if ($initCode) {
            $codePrinter->preOutput($initCode);
        }

        /**
         * Finalize the method compilation
         */
        if (is_object($this->statements)) {
            /**
             * If the last statement is not a 'return' or 'throw' we need to
             * restore the memory stack if needed
             */
            $lastType = $this->statements->getLastStatementType();

            if ($lastType != 'return' && $lastType != 'throw' && !$this->hasChildReturnStatementType($this->statements->getLastStatement())) {
                if ($symbolTable->getMustGrownStack()) {
                    $compilationContext->headersManager->add('kernel/memory');
                    $codePrinter->output("\t" . 'ZEPHIR_MM_RESTORE();');
                }

                /**
                 * If a method has return-type hints we need to ensure the last statement is a 'return' statement
                 */
                if ($this->hasReturnTypes()) {
                    throw new CompilerException('Reached end of the method without returning a valid type specified in the return-type hints', $this->expression['return-type']);
                }
            }
        }

        $compilationContext->backend->onPostCompile($this, $compilationContext);

        /**
         * Remove macros that grow/restore the memory frame stack if it wasn't used
         */
        $code = $this->removeMemoryStackReferences($symbolTable, $codePrinter->getOutput());

        /**
         * Restore the compilation context
         */
        $oldCodePrinter->output($code);
        $compilationContext->codePrinter = $oldCodePrinter;

        $compilationContext->branchManager = null;
        $compilationContext->cacheManager = null;
        $compilationContext->typeInference = null;

        $codePrinter->clear();

        return null;
    }

    /**
     * Simple method to check if one of the paths are returning the right expected type
     *
     * @param array $statement
     * @return boolean
     */
    public function hasChildReturnStatementType($statement)
    {
        if (!isset($statement['statements']) || !is_array($statement['statements'])) {
            return false;
        }

        if ($statement['type'] == 'if') {
            $ret = false;

            $statements = $statement['statements'];
            foreach ($statements as $item) {
                $type = isset($item['type']) ? $item['type'] : null;
                if ($type == 'return' || $type == 'throw') {
                    $ret = true;
                } else {
                    $ret = $this->hasChildReturnStatementType($item);
                }
            }

            if (!$ret || !isset($statement['else_statements'])) {
                return false;
            }

            $statements = $statement['else_statements'];
            foreach ($statements as $item) {
                $type = isset($item['type']) ? $item['type'] : null;
                if ($type == 'return' || $type == 'throw') {
                    return true;
                } else {
                    return $this->hasChildReturnStatementType($item);
                }
            }
        } else {
            $statements = $statement['statements'];
            foreach ($statements as $item) {
                $type = isset($item['type']) ? $item['type'] : null;
                if ($type == 'return' || $type == 'throw') {
                    return true;
                } else {
                    return $this->hasChildReturnStatementType($item);
                }
            }
        }

        return false;
    }

    /**
     * @return string
     */
    public function getInternalName()
    {
        $classDefinition = $this->getClassDefinition();
        return 'zep_' . $classDefinition->getCNamespace() . '_' . $classDefinition->getName() . '_' . $this->getName();
    }
}
