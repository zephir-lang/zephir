<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Stubs;

use Zephir\ClassConstant;
use Zephir\ClassDefinition;
use Zephir\ClassMethod;
use Zephir\ClassProperty;
use Zephir\CompilerFile;
use Zephir\Config;

/**
 * Stubs Generator
 */
class Generator
{
    /**
     * Not php visible style variants
     * @var array
     */
    protected $ignoreModifiers = array(
        'inline',
        'scoped'
    );

    /**
     * @var CompilerFile[]
     */
    protected $files;

    /**
     * @var Config
     */
    protected $config;

    /**
     * @param CompilerFile[] $files
     * @param Config         $config
     */
    public function __construct(array $files, Config $config)
    {
        $this->files = $files;
        $this->config = $config;
    }

    /**
     * Generates stubs
     *
     * @param string $path
     */
    public function generate($path)
    {
        $namespace = $this->config->get('namespace');

        foreach ($this->files as $file) {
            $class = $file->getClassDefinition();
            $source = $this->buildClass($class);

            $filename = ucfirst($class->getName()) . '.zep.php';
            $filePath = $path . str_replace($namespace, '', str_replace($namespace . '\\\\', DIRECTORY_SEPARATOR, strtolower($class->getNamespace())));
            $filePath = str_replace('\\', DIRECTORY_SEPARATOR, $filePath);
            $filePath = str_replace(DIRECTORY_SEPARATOR . DIRECTORY_SEPARATOR, DIRECTORY_SEPARATOR, $filePath);

            if (!is_dir($filePath)) {
                mkdir($filePath, 0777, true);
            }

            $filePath = realpath($filePath) . '/';
            file_put_contents($filePath . $filename, $source);
        }
    }

    protected function buildClass(ClassDefinition $class)
    {
        $source = <<<EOF
<?php

namespace {$class->getNamespace()};


EOF;
        if ($class->isFinal()) {
            $source .= 'final ';
        } elseif ($class->isAbstract()) {
            $source .= 'abstract ';
        }

        $source .= $class->getType() . ' ' . $class->getName();

        if ($extendsClassDefinition = $class->getExtendsClassDefinition()) {
                $source .= ' extends \\' . $extendsClassDefinition->getCompleteName();
        }

        if ($implementedInterfaces = $class->getImplementedInterfaces()) {
            $interfaces = array_map(function ($val) {
                return '\\' . $val;
            }, $implementedInterfaces);
            $source .= ' implements ' . join(', ', $interfaces);
        }

        $source .= PHP_EOL . '{' . PHP_EOL;

        foreach ($class->getConstants() as $constant) {
            $source .= $this->buildConstant($constant) . PHP_EOL;
        }

        foreach ($class->getProperties() as $property) {
            $source .= $this->buildProperty($property) . PHP_EOL;
        }

        $source .= PHP_EOL;

        foreach ($class->getMethods() as $method) {
            $source .= $this->buildMethod($method, $class->getType() === 'interface') . "\n\n";
        }

        return $source . '}' . PHP_EOL;
    }

    /**
     * @param ClassProperty $property
     *
     * @return string
     */
    protected function buildProperty(ClassProperty $property)
    {
        $visibility = $property->isPublic() ? 'public' : $property->isProtected() ? 'protected' : 'private';
        if ($property->isStatic()) {
            $visibility = 'static ' . $visibility;
        }

        $source = $visibility . ' $' . $property->getName();

        switch ($property->getType()) {
            case 'null':
                // @TODO: Fix getting value
            case 'static-constant-access':
                break;
            case 'string':
                $source .= ' = "' . $property->getValue() . '"';
                break;
            case 'empty-array':
                $source .= ' = array()';
                break;
            case 'array':
                $source .= ' = array(' . implode(', ', $property->getValue()) . ')';
                break;
            default:
                $source .= ' = ' . $property->getValue();
                break;
        }
        $docBlock = new DocBlock($property->getDocBlock(), 4);

        return $docBlock . "\n    " . $source . ';';
    }

    /**
     * @param ClassConstant $constant
     *
     * @return string
     */
    protected function buildConstant(ClassConstant $constant)
    {
        $source = 'const ' . $constant->getName();

        $value = $constant->getValueValue();

        switch ($constant->getType()) {
            case 'null':
                $value .= 'null';
                break;
            case 'string':
                $value = '"' . $value . '"';
                break;
        }
        $docBlock = new DocBlock($constant->getDocBlock(), 4);

        return $docBlock . "\n    " . $source . ' = ' . $value . ';';
    }

    /**
     * @param ClassMethod $method
     *
     * @param bool        $isInterface
     *
     * @return string
     */
    protected function buildMethod(ClassMethod $method, $isInterface)
    {
        $modifier = implode(' ', array_diff($method->getVisibility(), $this->ignoreModifiers));
        $docBlock = new MethodDocBlock($method, 4);

        $parameters = array();
        $methodParameters = $method->getParameters();

        if ($methodParameters) {
            foreach ($methodParameters->getParameters() as $parameter) {
                $paramStr = '$' . $parameter['name'];

                if (isset($parameter['default'])) {
                    $paramStr .= ' = ';
                    $paramStr .= $this->wrapPHPValue($parameter);
                }

                $parameters[] = $paramStr;
            }
        }

        $methodBody = "\t" . $modifier . ' function ' . $method->getName() . '(' . implode(', ', $parameters) . ')';

        if ($isInterface || $method->isAbstract()) {
            $methodBody .= ';';
        } else {
            $methodBody .= ' {}';
        }

        return $docBlock . "\n" . $methodBody;
    }

    /**
     * Prepare AST defalut value to PHP code print
     *
     * @param $parameter
     * @return string
     */
    private function wrapPHPValue($parameter)
    {
        switch ($parameter['default']['type']) {
            case 'null':
                return 'null';
                break;
            case 'string':
                return '"' . $parameter['default']['value'] . '"';
                break;
            case 'empty-array':
                return 'array()';
                break;
            case 'array':
                $parameters = array();

                array_walk($parameter['default']['left'], function($value) use (&$parameters) {
                    $parameters[] = $this->wrapPHPValue(array(
                        'default' => $value['value'],
                        'type' => $value['value']['type']
                    ));
                });

                if (count($parameters) > 0) {
                    return 'array('.implode(', ', $parameters).')';
                }

                return 'array()';
                break;
            case 'static-constant-access':
                return $parameter['default']['left']['value'] . '::' . $parameter['default']['right']['value'];
                break;
            default:
                return $parameter['default']['value'];
                break;
        }
    }
}
