<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Stubs;

use Zephir\AliasManager;
use Zephir\Class\ClassConstant;
use Zephir\Class\ClassDefinition;
use Zephir\Class\ClassMethod;
use Zephir\Class\ClassMethodParameters;
use Zephir\ClassProperty;
use Zephir\CompilerFile;
use Zephir\Exception;
use function array_key_exists;
use function in_array;

class Generator
{
    /**
     * Not php visible style variants.
     *
     * @var array
     */
    protected array $ignoreModifiers = [
        'inline',
        'internal',
        'scoped',
        'deprecated',
    ];
    /**
     * @param CompilerFile[] $files
     */
    public function __construct(protected array $files)
    {
    }

    /**
     * Generates stubs.
     *
     * @param string $namespace
     * @param string $path
     * @param string $indent
     * @param string $banner
     *
     * @throws Exception\LogicException
     */
    public function generate(string $namespace, string $path, string $indent, string $banner): void
    {
        if (empty($path)) {
            throw new Exception\LogicException(
                'The stubs path must not be empty.'
            );
        }

        $indent = 'tabs' === $indent ? "\t" : '    ';

        foreach ($this->files as $file) {
            $class = $file->getClassDefinition();
            $source = $this->buildClass($class, $indent, $banner);

            $filename = ucfirst($class->getName()).'.php';
            $filePath = $path.str_ireplace(
                $namespace,
                '',
                str_replace($namespace.'\\\\', \DIRECTORY_SEPARATOR, $class->getNamespace())
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
     * @param string          $banner
     *
     * @throws Exception\RuntimeException
     *
     * @return string
     */
    protected function buildClass(ClassDefinition $class, string $indent, string $banner): string
    {
        $source = '<?php'.PHP_EOL.PHP_EOL;
        $source .= '' === $banner ? '' : $banner.PHP_EOL;
        $source .= "namespace {$class->getNamespace()};".PHP_EOL;

        /** @var AliasManager $aliasManager */
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

                if ($aliasManager->isAliasPresentFor($interface)) {
                    $interfaces[$key] = $aliasManager->getAliasForClassName($interface);
                }
            }

            $keyword = 'interface' == $class->getType() ? ' extends ' : ' implements ';
            $source .= $keyword.implode(', ', $interfaces);
        }

        $source .= PHP_EOL.'{'.PHP_EOL;

        /**
         * Build Class constants
         */
        $constants = [];
        foreach ($class->getConstants() as $constant) {
            $constants[] = $this->buildConstant($constant, $indent).PHP_EOL;
        }

        $source .= implode(PHP_EOL, $constants).PHP_EOL;
        unset($constants);

        /**
         * Build Class properties
         */
        $properties = [];
        foreach ($class->getProperties() as $property) {
            $properties[] = $this->buildProperty($property, $indent).PHP_EOL;
        }

        $source .= implode(PHP_EOL, $properties).PHP_EOL;
        unset($properties);

        /**
         * Build Class methods
         */
        $methods = [];
        foreach ($class->getMethods() as $method) {
            if ($method->isInternal()) {
                continue;
            }

            $methods[] = $this->buildMethod($method, 'interface' === $class->getType(), $indent).PHP_EOL;
        }

        $source .= implode(PHP_EOL, $methods);
        unset($methods);

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
    protected function buildProperty(ClassProperty $property, string $indent): string
    {
        $visibility = 'public';

        if (false === $property->isPublic()) {
            $visibility = $property->isProtected() ? 'protected' : 'private';
        }

        if ($property->isStatic()) {
            $visibility = 'static '.$visibility;
        }

        $source = $indent.$visibility.' $'.$property->getName();
        $original = $property->getOriginal();

        if (isset($original['default'])) {
            $source .= ' = '.$this->wrapPHPValue([
                'default' => $original['default'],
            ]);
        }

        return $this->fetchDocBlock($property->getDocBlock(), $indent).$source.';';
    }

    /**
     * @param ClassConstant $constant
     * @param string        $indent
     *
     * @return string
     */
    protected function buildConstant(ClassConstant $constant, string $indent): string
    {
        $source = 'const '.$constant->getName();

        $value = $this->wrapPHPValue([
            'default' => $constant->getValue(),
        ]);

        return $this->fetchDocBlock($constant->getDocBlock(), $indent).$indent.$source.' = '.$value.';';
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

        if ($methodParameters instanceof ClassMethodParameters) {
            foreach ($methodParameters->getParameters() as $parameter) {
                $paramStr = '';
                if (isset($parameter['cast'])) {
                    if ($aliasManager->isAlias($parameter['cast']['value'])) {
                        $cast = '\\'.$aliasManager->getAlias($parameter['cast']['value']);
                    } else {
                        $cast = $parameter['cast']['value'];
                    }

                    $paramStr .= $cast.' ';
                } elseif (isset($parameter['data-type']) && 'array' === $parameter['data-type']) {
                    $paramStr .= 'array ';
                } elseif (isset($parameter['data-type'])) {
                    if (in_array($parameter['data-type'], ['bool', 'boolean'])) {
                        $paramStr .= 'bool ';
                    } elseif ('double' == $parameter['data-type']) {
                        $paramStr .= 'float ';
                    } elseif (in_array($parameter['data-type'], ['int', 'uint', 'long', 'ulong', 'uchar'])) {
                        $paramStr .= 'int ';
                    } elseif (in_array($parameter['data-type'], ['char', 'string'])) {
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
        if ($method->hasReturnTypes()) {
            $supported = 0;

            if (array_key_exists('object', $method->getReturnTypes())) {
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

            if (array_key_exists('array', $method->getReturnTypes())) {
                $return = 'array';
                ++$supported;
            }

            if (!empty($return) && $method->areReturnTypesNullCompatible()) {
                $return = '?'.$return;
            }

            // PHP doesn't support multiple return types (yet?)
            if ($supported > 1 || array_key_exists('variable', $method->getReturnTypes())) {
                $return = '';
            }
        } elseif ($method->isVoid()) {
            $return = 'void';
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

        $docs = $docBlock->processMethodDocBlock();
        $docs = $docs ? $docs.PHP_EOL : '';

        return $docs.$methodBody;
    }

    /**
     * Prepare AST default value to PHP code print.
     *
     * @param array $parameter
     *
     * @throws Exception\NotImplementedException
     *
     * @return string
     */
    protected function wrapPHPValue(array $parameter): string
    {
        switch ($parameter['default']['type']) {
            case 'null':
                $returnValue = 'null';
                break;

            case 'string':
            case 'char':
                $returnValue = '\''.addslashes($parameter['default']['value']).'\'';
                break;

            case 'empty-array':
                $returnValue = '[]';
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

                $returnValue = '['.implode(', ', $parameters).']';
                break;

            case 'static-constant-access':
                $returnValue = $parameter['default']['left']['value'].'::'.$parameter['default']['right']['value'];
                break;

            case 'int':
            case 'double':
            case 'bool':
                $returnValue = $parameter['default']['value'];
                break;

            default:
                throw new Exception\LogicException(
                    sprintf(
                        'Stubs - value with type: %s is not supported',
                        $parameter['default']['type']
                    )
                );
        }

        return (string) $returnValue;
    }

    private function fetchDocBlock(?string $docBlock, string $indent): string
    {
        $docBlock = (new DocBlock($docBlock, $indent))->__toString();

        return $docBlock ? $docBlock.PHP_EOL : '';
    }
}
