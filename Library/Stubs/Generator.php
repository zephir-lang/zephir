<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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
use Zephir\Exception;

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
        'internal',
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
        if ($this->config->get('indent', 'extra') === 'tabs') {
            $indent = "\t";
        } else {
            $indent = '    ';
        }

        $namespace = $this->config->get('namespace');

        foreach ($this->files as $file) {
            $class = $file->getClassDefinition();
            $source = $this->buildClass($class, $indent);

            $filename = ucfirst($class->getName()) . '.zep.php';
            $filePath = $path . str_replace(
                $namespace,
                '',
                str_replace($namespace . '\\\\', DIRECTORY_SEPARATOR, strtolower($class->getNamespace()))
            );
            $filePath = str_replace('\\', DIRECTORY_SEPARATOR, $filePath);
            $filePath = str_replace(DIRECTORY_SEPARATOR . DIRECTORY_SEPARATOR, DIRECTORY_SEPARATOR, $filePath);

            if (!is_dir($filePath)) {
                mkdir($filePath, 0777, true);
            }

            $filePath = realpath($filePath) . '/';
            file_put_contents($filePath . $filename, $source);
        }
    }

    /**
     * Build class
     *
     * @param ClassDefinition $class
     * @param string $indent
     * @return string
     */
    protected function buildClass(ClassDefinition $class, $indent)
    {
        $source = <<<EOF
<?php

namespace {$class->getNamespace()};


EOF;

        $source .= (new DocBlock($class->getDocBlock(), '')) . "\n";

        if ($class->isFinal()) {
            $source .= 'final ';
        } elseif ($class->isAbstract()) {
            $source .= 'abstract ';
        }

        $source .= $class->getType() . ' ' . $class->getName();

        if ($class->getExtendsClass()) {
            $extendsClassDefinition = $class->getExtendsClassDefinition();
            if (!$extendsClassDefinition) {
                throw new \RuntimeException('Class "' . $class->getName() . '" does not have a extendsClassDefinition');
            }

            $source .= ' extends ' . ($extendsClassDefinition->isBundled() ? '' : '\\') . $extendsClassDefinition->getCompleteName();
        }

        if ($implementedInterfaces = $class->getImplementedInterfaces()) {
            $interfaces = array_map(function ($val) {
                return '\\' . $val;
            }, $implementedInterfaces);

            $keyword = $class->getType() == 'interface' ? ' extends ' : ' implements ';
            $source .= $keyword . join(', ', $interfaces);
        }

        $source .= PHP_EOL . '{' . PHP_EOL;

        foreach ($class->getConstants() as $constant) {
            $source .= $this->buildConstant($constant, $indent) . PHP_EOL . PHP_EOL;
        }

        foreach ($class->getProperties() as $property) {
            $source .= $this->buildProperty($property, $indent) . PHP_EOL . PHP_EOL;
        }

        $source .= PHP_EOL;

        foreach ($class->getMethods() as $method) {
            $source .= $this->buildMethod($method, $class->getType() === 'interface', $indent) . "\n\n";
        }

        return $source . '}' . PHP_EOL;
    }

    /**
     * Build property
     *
     * @param ClassProperty $property
     * @param string $indent
     * @return string
     */
    protected function buildProperty(ClassProperty $property, $indent)
    {
        $visibility = $property->isPublic() ? 'public' : $property->isProtected() ? 'protected' : 'private';
        if ($property->isStatic()) {
            $visibility = 'static ' . $visibility;
        }

        $source = $visibility . ' $' . $property->getName();
        $original = $property->getOriginal();

        if (isset($original['default'])) {
            $source .= ' = ' . $this->wrapPHPValue(array(
                'default' => $original['default']
            ));
        }

        $docBlock = new DocBlock($property->getDocBlock(), $indent);
        return $docBlock . "\n" . $indent . $source . ';';
    }

    /**
     * @param ClassConstant $constant
     * @param string $indent
     *
     * @return string
     */
    protected function buildConstant(ClassConstant $constant, $indent)
    {
        $source = 'const ' . $constant->getName();

        $value = $this->wrapPHPValue(array(
            'default' => $constant->getValue()
        ));

        $docBlock = new DocBlock($constant->getDocBlock(), $indent);
        return $docBlock . "\n" . $indent . $source . ' = ' . $value . ';';
    }

    /**
     * @param ClassMethod $method
     * @param bool $isInterface
     * @param string $indent
     *
     * @return string
     */
    protected function buildMethod(ClassMethod $method, $isInterface, $indent)
    {
        $modifier = implode(' ', array_diff($method->getVisibility(), $this->ignoreModifiers));

        $methodParameters = $method->getParameters();
        $aliasManager = $method->getClassDefinition()->getAliasManager();
        $docBlock = new MethodDocBlock($method, $aliasManager, $indent);

        $parameters = array();

        if ($methodParameters) {
            foreach ($methodParameters->getParameters() as $parameter) {
                $paramStr = '';
                if (isset($parameter['cast'])) {
                    if ($aliasManager->isAlias($parameter['cast']['value'])) {
                        $cast = '\\' . $aliasManager->getAlias($parameter['cast']['value']);
                    } else {
                        $cast = $parameter['cast']['value'];
                    }
                    $paramStr .= $cast . ' ';
                }

                $paramStr .= '$' . $parameter['name'];

                if (isset($parameter['default'])) {
                    $paramStr .= ' = ' . $this->wrapPHPValue($parameter);
                }

                $parameters[] = $paramStr;
            }
        }

        $methodBody = $indent . $modifier . ' function ' . $method->getName() . '(' . implode(', ', $parameters) . ')';

        if ($isInterface || $method->isAbstract()) {
            $methodBody .= ';';
        } else {
            $methodBody .= ' {}';
        }

        return $docBlock . "\n" . $methodBody;
    }

    /**
     * Prepare AST default value to PHP code print
     *
     * @param $parameter
     * @return string
     * @throws Exception
     */
    protected function wrapPHPValue($parameter)
    {
        switch ($parameter['default']['type']) {
            case 'null':
                return 'null';
                break;

            case 'string':
            case 'char':
                return '"' . addslashes($parameter['default']['value']) . '"';
                break;

            case 'empty-array':
                return 'array()';
                break;

            case 'array':
                $parameters = array();

                foreach ($parameter['default']['left'] as $value) {
                    $source = '';

                    if (isset($value['key'])) {
                        $source .= $this->wrapPHPValue(array(
                            'default' => $value['key'],
                            'type' => $value['key']['type']
                        )) . ' => ';
                    }

                    $parameters[] = $source . $this->wrapPHPValue(array(
                        'default' => $value['value'],
                        'type' => $value['value']['type']
                    ));
                }

                return 'array(' . implode(', ', $parameters) . ')';
                break;

            case 'static-constant-access':
                return $parameter['default']['left']['value'] . '::' . $parameter['default']['right']['value'];
                break;

            case 'int':
            case 'double':
            case 'bool':
                return $parameter['default']['value'];
                break;

            default:
                throw new Exception('Stubs - value with type: ' . $parameter['default']['type'] . ' is not supported');
                break;
        }
    }
}
