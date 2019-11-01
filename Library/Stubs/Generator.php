<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Stubs;

use Zephir\ClassConstant;
use Zephir\ClassDefinition;
use Zephir\ClassMethod;
use Zephir\ClassProperty;
use Zephir\CompilerFile;
use Zephir\Exception;

/**
 * Stubs Generator.
 */
class Generator
{
    /**
     * Not php visible style variants.
     *
     * @var array
     */
    protected $ignoreModifiers = [
        'inline',
        'internal',
        'scoped',
        'deprecated',
    ];

    /**
     * @var CompilerFile[]
     */
    protected $files;

    /**
     * @param CompilerFile[] $files
     */
    public function __construct(array $files)
    {
        $this->files = $files;
    }

    /**
     * Generates stubs.
     *
     * @param string $namespace
     * @param string $path
     * @param string $indent
     *
     * @throws Exception\LogicException
     */
    public function generate(string $namespace, string $path, string $indent)
    {
        if (empty($path)) {
            throw new Exception\LogicException(
                'The stubs path must not be empty.'
            );
        }

        $indent = 'tabs' === $indent ? "\t" : '    ';

        foreach ($this->files as $file) {
            $class = $file->getClassDefinition();
            $source = $this->buildClass($class, $indent);

            $filename = ucfirst($class->getName()).'.zep.php';
            $filePath = $path.str_replace(
                $namespace,
                '',
                str_replace($namespace.'\\\\', \DIRECTORY_SEPARATOR, strtolower($class->getNamespace()))
            );
            $filePath = str_replace('\\', \DIRECTORY_SEPARATOR, $filePath);
            $filePath = str_replace(\DIRECTORY_SEPARATOR.\DIRECTORY_SEPARATOR, \DIRECTORY_SEPARATOR, $filePath);

            if (!is_dir($filePath)) {
                mkdir($filePath, 0777, true);
            }

            $filePath = realpath($filePath).'/';
            file_put_contents($filePath.$filename, $source);
        }
    }

    /**
     * Build class.
     *
     * @param ClassDefinition $class
     * @param string          $indent
     *
     * @throws Exception\RuntimeException
     *
     * @return string
     */
    protected function buildClass(ClassDefinition $class, string $indent): string
    {
        $source = '<?php'.PHP_EOL.PHP_EOL;
        $source .= "namespace {$class->getNamespace()};".PHP_EOL;

        /** @var Zephir\AliasManager $aliasManager */
        $aliasManager = $class->getAliasManager();
        $aliases = $aliasManager->getAliases();

        if (!empty($aliases)) {
            $source .= PHP_EOL;

            foreach ($aliases as $alias => $fqn) {
                $isAliased = $aliasManager->isUseStatementAliased($alias);
                $asAlias = $isAliased ? ' as '.$alias : '';
                $source .= 'use '.$fqn.$asAlias.';'.PHP_EOL;
            }
        }

        $source .= PHP_EOL;

        $docBlock = (new DocBlock($class->getDocBlock(), ''));

        if ('' !== (string) $docBlock) {
            $source .= $docBlock.PHP_EOL;
        }

        if ($class->isFinal()) {
            $source .= 'final ';
        } elseif ($class->isAbstract()) {
            $source .= 'abstract ';
        }

        $source .= $class->getType().' '.$class->getName();

        if ($class->getExtendsClass()) {
            $extendsClassDefinition = $class->getExtendsClassDefinition();
            if (!$extendsClassDefinition) {
                throw new Exception\RuntimeException(
                    sprintf(
                        'Class "%s" does not have a extendsClassDefinition',
                        $class->getName()
                    )
                );
            }

            $hasAliasForExtends = $aliasManager->isAlias($extendsClassDefinition->getShortName());

            $source .= ' extends '.($hasAliasForExtends || $extendsClassDefinition->isBundled() ? '' : '\\');
            $source .= $hasAliasForExtends ? $extendsClassDefinition->getShortName() : $extendsClassDefinition->getCompleteName();
        }

        if ($interfaces = $class->getImplementedInterfaces()) {
            foreach ($interfaces as $key => $interface) {
                $interfaces[$key] = '\\'.trim($interface, '\\');

                if ($aliasManager->isNamespaceAliased($interface)) {
                    $interfaces[$key] = $aliasManager->getAliasForNamespace($interface);
                }
            }

            $keyword = 'interface' == $class->getType() ? ' extends ' : ' implements ';
            $source .= $keyword.implode(', ', $interfaces);
        }

        $source .= PHP_EOL.'{'.PHP_EOL;

        foreach ($class->getConstants() as $constant) {
            $source .= $this->buildConstant($constant, $indent).PHP_EOL.PHP_EOL;
        }

        foreach ($class->getProperties() as $property) {
            $source .= $this->buildProperty($property, $indent).PHP_EOL.PHP_EOL;
        }

        $source .= PHP_EOL;

        foreach ($class->getMethods() as $method) {
            if ($method->isInternal()) {
                continue;
            }

            $source .= $this->buildMethod(
                $method,
                'interface' === $class->getType(),
                $indent
            );

            $source .= PHP_EOL.PHP_EOL;
        }

        return $source.'}'.PHP_EOL;
    }

    /**
     * Build property.
     *
     * @param ClassProperty $property
     * @param string        $indent
     *
     * @return string
     */
    protected function buildProperty(ClassProperty $property, $indent)
    {
        $visibility = 'public';

        if (false === $property->isPublic()) {
            $visibility = $property->isProtected() ? 'protected' : 'private';
        }

        if ($property->isStatic()) {
            $visibility = 'static '.$visibility;
        }

        $source = $visibility.' $'.$property->getName();
        $original = $property->getOriginal();

        if (isset($original['default'])) {
            $source .= ' = '.$this->wrapPHPValue([
                'default' => $original['default'],
            ]);
        }

        $docBlock = new DocBlock($property->getDocBlock(), $indent);

        return $docBlock."\n".$indent.$source.';';
    }

    /**
     * @param ClassConstant $constant
     * @param string        $indent
     *
     * @return string
     */
    protected function buildConstant(ClassConstant $constant, $indent)
    {
        $source = 'const '.$constant->getName();

        $value = $this->wrapPHPValue([
            'default' => $constant->getValue(),
        ]);

        $docBlock = new DocBlock($constant->getDocBlock(), $indent);

        return $docBlock."\n".$indent.$source.' = '.$value.';';
    }

    /**
     * @param ClassMethod $method
     * @param bool        $isInterface
     * @param string      $indent
     *
     * @return string
     */
    protected function buildMethod(ClassMethod $method, bool $isInterface, string $indent): string
    {
        $modifier = implode(' ', array_diff($method->getVisibility(), $this->ignoreModifiers));

        $methodParameters = $method->getParameters();
        $aliasManager = $method->getClassDefinition()->getAliasManager();
        $docBlock = new MethodDocBlock($method, $aliasManager, $indent);

        $parameters = [];

        if ($methodParameters) {
            foreach ($methodParameters->getParameters() as $parameter) {
                $paramStr = '';
                if (isset($parameter['cast'])) {
                    if ($aliasManager->isAlias($parameter['cast']['value'])) {
                        $cast = '\\'.$aliasManager->getAlias($parameter['cast']['value']);
                    } else {
                        $cast = $parameter['cast']['value'];
                    }
                    $paramStr .= $cast.' ';
                } elseif (isset($parameter['data-type']) && 'array' == $parameter['data-type']) {
                    $paramStr .= 'array ';
                } elseif (isset($parameter['data-type']) && version_compare(PHP_VERSION, '7.0.0', '>=')) {
                    if (\in_array($parameter['data-type'], ['bool', 'boolean'])) {
                        $paramStr .= 'bool ';
                    } elseif ('double' == $parameter['data-type']) {
                        $paramStr .= 'float ';
                    } elseif (\in_array($parameter['data-type'], ['int', 'uint', 'long', 'ulong', 'uchar'])) {
                        $paramStr .= 'int ';
                    } elseif (\in_array($parameter['data-type'], ['char', 'string'])) {
                        $paramStr .= 'string ';
                    }
                }

                $paramStr .= '$'.$parameter['name'];

                if (isset($parameter['default'])) {
                    $paramStr .= ' = '.$this->wrapPHPValue($parameter);
                }

                $parameters[] = $paramStr;
            }
        }

        $return = '';
        /**
         * TODO: Add $method->isVoid() check after removing PHP 7.0 support.
         *
         * @see https://github.com/phalcon/zephir/issues/1977
         * @see https://github.com/phalcon/zephir/pull/1978
         */
        if (version_compare(PHP_VERSION, '7.0.0', '>=') && $method->hasReturnTypes()) {
            $supported = 0;

            if (\array_key_exists('object', $method->getReturnTypes()) && 1 == \count($method->getReturnClassTypes())) {
                $return = key($method->getReturnClassTypes());
                ++$supported;
            }

            if ($method->areReturnTypesIntCompatible()) {
                $return = 'int';
                ++$supported;
            }
            if ($method->areReturnTypesDoubleCompatible()) {
                $return = 'float';
                ++$supported;
            }
            if ($method->areReturnTypesBoolCompatible()) {
                $return = 'bool';
                ++$supported;
            }
            if ($method->areReturnTypesStringCompatible()) {
                $return = 'string';
                ++$supported;
            }
            if (\array_key_exists('array', $method->getReturnTypes())) {
                $return = 'array';
                ++$supported;
            }

            if (!empty($return) && $method->areReturnTypesNullCompatible()) {
                if (version_compare(PHP_VERSION, '7.1.0', '>=')) {
                    $return = '?'.$return;
                } else {
                    $return = '';
                }
            }

            // PHP doesn't support multiple return types (yet?)
            if ($supported > 1) {
                $return = '';
            }
        }
        if (!empty($return)) {
            $return = ': '.$return;
        }

        $function = trim($modifier.' function', ' ').' ';
        $methodBody = $indent.$function.$method->getName().'('.implode(', ', $parameters).')'.$return;

        if ($isInterface || $method->isAbstract()) {
            $methodBody .= ';';
        } else {
            $methodBody .= PHP_EOL.$indent.'{'.PHP_EOL.$indent.'}';
        }

        return $docBlock->processMethodDocBlock().PHP_EOL.$methodBody;
    }

    /**
     * Prepare AST default value to PHP code print.
     *
     * @param $parameter
     *
     * @throws Exception\NotImplementedException
     *
     * @return string
     */
    protected function wrapPHPValue($parameter)
    {
        switch ($parameter['default']['type']) {
            case 'null':
                return 'null';
                break;

            case 'string':
            case 'char':
                return '\''.addslashes($parameter['default']['value']).'\'';
                break;

            case 'empty-array':
                return 'array()';
                break;

            case 'array':
                $parameters = [];

                foreach ($parameter['default']['left'] as $value) {
                    $source = '';

                    if (isset($value['key'])) {
                        $source .= $this->wrapPHPValue([
                            'default' => $value['key'],
                            'type' => $value['key']['type'],
                        ]).' => ';
                    }

                    $parameters[] = $source.$this->wrapPHPValue([
                        'default' => $value['value'],
                        'type' => $value['value']['type'],
                    ]);
                }

                return 'array('.implode(', ', $parameters).')';
                break;

            case 'static-constant-access':
                return $parameter['default']['left']['value'].'::'.$parameter['default']['right']['value'];
                break;

            case 'int':
            case 'double':
            case 'bool':
                return $parameter['default']['value'];
                break;

            default:
                throw new Exception\LogicException(
                    sprintf(
                        'Stubs - value with type: %s is not supported',
                        $parameter['default']['type']
                    )
                );
                break;
        }
    }
}
