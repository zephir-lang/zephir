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
     * @var CompilerFile[]
     */
    protected $files;

    /**
     * @var Config
     */
    protected $config;

    /**
     * @param CompilerFile[] $files
     * @param Config $config
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

            $filename = ucfirst($class->getName()) . '.php';
            $filePath = $path . str_replace($namespace, '', str_replace($namespace . '\\', '', strtolower($class->getNamespace())));

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
            if ($extendsClassDefinition instanceof \ReflectionClass) {
                $source .= ' extends \\' . $extendsClassDefinition->getName();
            } else {
                $source .= ' extends \\' . $extendsClassDefinition->getCompleteName();
            }
        }

        if ($implementedInterfaces = $class->getImplementedInterfaces()) {
            $source .= ' implements ' . implode(', ', array_map(function ($val) {
                return '\\' . $val;
            }, $implementedInterfaces));
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
            $source .= $this->buildMethod($method) . PHP_EOL . PHP_EOL;
        }

        return $source . '}' . PHP_EOL;
    }

    /**
     * @param string $docBlock
     * @return string
     */
    protected function buildDocBlock($docBlock)
    {
        $code = '';
        $docBlock = '/' . $docBlock . '/';

        foreach (explode("\n", $docBlock) as $line) {
            $code .= "\t" . preg_replace('/^[ \t]+/', ' ', $line) . PHP_EOL;
        }

        return $code;
    }

    /**
     * @param ClassProperty $property
     * @return string
     */
    protected function buildProperty(ClassProperty $property)
    {
        $source = '';
        $docBlock = $property->getDocBlock();
        if ($docBlock) {
            $source .= $this->buildDocBlock($docBlock);
        }

        $visibility = $property->isProtected() ? 'protected' : $property->isPublic() ? 'public' : 'private';
        if ($property->isStatic()) {
            $visibility = 'static ' . $visibility;
        }

        $source .= "\t" . $visibility . ' $' . $property->getName();

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

        return $source . ';';
    }

    /**
     * @param ClassConstant $constant
     * @return string
     */
    protected function buildConstant(ClassConstant $constant)
    {
        $source = '';
        $docBlock = $constant->getDocBlock();
        if ($docBlock) {
            $source .= $this->buildDocBlock($docBlock);
        }

        $source .= "\t" . 'const ' . $constant->getName();

        $value = $constant->getValueValue();

        switch ($constant->getType()) {
            case 'null':
                $value .= 'null';
                break;
            case 'string':
                $value = '"' . $value . '"';
                break;
        }

        return $source . ' = ' . $value . ';';
    }

    /**
     * @param ClassMethod $method
     * @return string
     */
    protected function buildMethod(ClassMethod $method)
    {
        $modifier = implode(' ', $method->getVisibility());
        $modifier = str_replace(' inline', '', $modifier);

        $docBlock = $method->getDocBlock();
        if ($docBlock) {
            $docBlock = $this->buildDocBlock($docBlock);
        }
        $methodBody = <<<EOL
    $modifier function {$method->getName()}()
    {
    }
EOL;

        return $docBlock . $methodBody;
    }
}
