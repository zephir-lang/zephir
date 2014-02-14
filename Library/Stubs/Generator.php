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

use Zephir\CompilerFile;
use Zephir\Config;

/**
 * Class Generator
 * @package Stubs
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

    public function __construct(array $files, Config $config)
    {
        $this->files = $files;
        $this->config = $config;
    }

    /**
     * Generates stubs
     *
     * @param $path
     */
    public function generate($path)
    {
        foreach ($this->files as $file) {
            $class = $file->getClassDefinition();

            $source = '<?php' . PHP_EOL;

            $source .= PHP_EOL;
            $source .= 'namespace '.$class->getNamespace().';'.PHP_EOL;
            $source .= PHP_EOL;

            if ($class->isFinal()) {
                $source .= 'final ';
            }

            if ($class->isAbstract()) {
                $source .= 'abstract ';
            }

            $source .= 'class ' . $class->getName();

            $extendsClassDefinition = $class->getExtendsClassDefinition();
            if ($extendsClassDefinition) {
                $source .= ' extends '.$extendsClassDefinition->getName();
            }

            $implementedInterfaces = $class->getImplementedInterfaces();
            if ($implementedInterfaces) {
                $source .= ' implements '.implode(', ', $implementedInterfaces);
            }

            $source .= ' {' . PHP_EOL;

            $constants = $class->getConstants();
            foreach ($constants as $constant) {
                $source .= "\t" . 'const '.$constant->getName();

                $value = $constant->getValueValue();

                switch($constant->getType()) {
                    case 'null':
                        $value .= 'null';
                        break;
                    case 'string':
                        $value = '"'.$value.'"';
                        break;
                }

                $source .= ' = '.$value.';'.PHP_EOL;
            }

            $methods = $class->getMethods();
            $countMethods = count($methods);

            $i = 0;

            foreach ($methods as $method) {
                $modifier = implode(' ', $method->getVisibility());
                $modifier = str_replace(' inline', '', $modifier);

                $docBlock = $method->getDocBlock();
                if ($docBlock) {
                    $source .= $this->outputDocBlock($docBlock);
                }
                $source .= "\t" . $modifier .' function '.$method->getName() . '() {}' . PHP_EOL;

                if (++$i != $countMethods) {
                    $source .= PHP_EOL;
                }
            }

            $source .= '}';

            $filename = ucfirst($class->getName()).'.php';
            $filePath = $path . str_replace($this->config->get('namespace'), '', str_replace($this->config->get('namespace').'\\', '', strtolower($class->getNamespace())));

            if (!is_dir($filePath)) {
                mkdir($filePath, 0777, true);
            }

            $filePath = realpath($filePath) . '/';
            file_put_contents($filePath.$filename, $source);
        }
    }

    protected function outputDocBlock($docblock)
    {
        $code = '';
        $docblock = '/' . $docblock . '/';

        foreach (explode("\n", $docblock) as $line) {
            $code .= "\t".preg_replace('/^[ \t]+/', ' ', $line) . PHP_EOL;
        }

        return $code;
    }
}
