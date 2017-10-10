<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2017 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | https://zephir-lang.com/license.html                                     |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Parser;

use Zephir\Utils;
use Zephir\Logger;
use Zephir\Parser;
use Zephir\Compiler\CompilerException;

/**
 * Zephir\Parser\Manager
 */
class Manager
{
    /**
     * Zephir Parser
     * @var Parser
     */
    protected $parser;

    /**
     * Is the Zephir Parser enabled
     * @var bool
     */
    protected $parserEnabled = false;

    /**
     * Should we recompile the Zephir Parser
     * @var bool
     */
    protected $forceCompileParser = false;

    /**
     * The Zephir Logger
     * @var Logger
     */
    protected $logger;

    /**
     * Manager constructor.
     *
     * @param Parser $parser     The Zephir Parser
     * @param Logger $logger     The Zephir Logger
     * @param array  $parameters Additional parameters [Optional]
     */
    public function __construct(Parser $parser, Logger $logger, array $parameters = [])
    {
        $this->parser = $parser;
        $this->logger = $logger;

        if (isset($parameters['parser-compiled'])) {
            if ($parameters['parser-compiled'] === 'force') {
                $this->forceCompileParser = true;
            } else {
                $this->parserEnabled = true;
            }
        } else {
            $this->parserEnabled = $parser->isAvailable();
        }
    }

    /**
     * Get Zephir Parser.
     *
     * @return Parser
     */
    public function getParser()
    {
        return $this->parser;
    }

    /**
     * Check if Zephir Parser available.
     *
     * @return bool
     */
    public function isAvailable()
    {
        return $this->parserEnabled;
    }

    /**
     * Should we recompile the Zephir Parser or not?
     *
     * @return bool
     */
    public function hasToRecompileParser()
    {
        return $this->forceCompileParser;
    }

    /**
     * Checks if the Zephir parser already compiled and stored in the local path.
     *
     * @return bool
     */
    public function isAlreadyCompiled()
    {
        $parserPath = $this->getParserFilePath();

        return file_exists($parserPath);
    }

    /**
     * Gets real path to the parser extension.
     *
     * @return string
     */
    public function getParserFilePath()
    {
        $modulesPath = $this->getLocalParserPath()  . 'modules' . DIRECTORY_SEPARATOR;

        if (Utils::isWindows()) {
            return  $modulesPath . 'zephir_parser.dll';
        }

        return $modulesPath . 'zephir_parser.so';
    }

    protected function compileLinuxParser($destination)
    {
        $oldCwd = getcwd();
        chdir($destination);

        echo shell_exec('cd parser && /bin/sh ./build_linux.sh');

        exec('make clean && phpize --clean', $output, $exit);
        exec('phpize', $output, $exit);
        exec('export CC="gcc" && ./configure --enable-zephir_parser');

        $this->logger->output('Compiling the parser...');

        exec(
            '(make -s -j2 2> ./compile-errors.log 1> ./compile.log)',
            $output,
            $exit
        );

        echo file_get_contents('compile-errors.log') . PHP_EOL;
        echo file_get_contents('compile.log') . PHP_EOL;

        copy('modules/zephir_parser.so', 'zephir_parser.so');
        chdir($oldCwd);

        return $destination . 'zephir_parser.so';
    }

    protected function compileWin32Parser($destination)
    {
        $oldCwd = getcwd();
        chdir($destination);

        echo shell_exec('cd parser && cmd /c build_win32.bat');

        exec('%PHP_DEVPACK%\\phpize --clean', $output, $exit);
        if (file_exists('Release')) {
            exec('rd /s /q "' . $destination . '"Release', $output, $exit);
        }

        exec('%PHP_DEVPACK%\\phpize', $output, $exit);

        // fix until https://github.com/php/php-src/commit/9a3af83ee2aecff25fd4922ef67c1fb4d2af6201
        // hits all supported PHP builds
        $fixMarker = "/* zephir_phpize_fix */";
        $configureFile = file_get_contents("configure.js");
        $configureFix = ["var PHP_ANALYZER = 'disabled';", "var PHP_PGO = 'no';", "var PHP_PGI = 'no';"];

        if (strpos($configureFile, $fixMarker) === false) {
            file_put_contents(
                "configure.js",
                $fixMarker . PHP_EOL . implode($configureFix, PHP_EOL) . PHP_EOL . $configureFile
            );
        }

        exec('configure --enable-zephir_parser');

        $this->logger->output('Compiling the parser...');

        exec('nmake 2> compile-errors.log 1> compile.log', $output, $exit);

        echo file_get_contents('compile-errors.log') . PHP_EOL;
        $buildLog = file_get_contents('compile.log');
        echo $buildLog . PHP_EOL;

        $buildType = 'Release';
        if (strpos($buildLog, 'Debug_TS\\') !== false) {
            $buildType = 'Debug_TS';
        } else {
            if (strpos($buildLog, 'Release_TS\\') !== false) {
                $buildType = 'Release_TS';
            } else {
                if (strpos($buildLog, 'Debug\\') !== false) {
                    $buildType = 'Debug';
                }
            }
        }

        if (strpos($buildLog, 'x64\\'.$buildType) !== false) {
            $buildType = 'x64/' . $buildType;
        }

        copy($buildType . '/php_zephir_parser.dll', 'php_zephir_parser.dll');
        chdir($oldCwd);

        return $destination . 'php_zephir_parser.dll';
    }

    /**
     * Compile the parser PHP extension
     */
    public function compileParser()
    {
        $extFile = null;

        $parserDir = $this->getLocalParserPath();

        $this->logger->output('Preparing for parser compilation...');

        if (Utils::isWindows()) {
            $extFile = $this->compileWin32Parser($parserDir);
        } else {
            $extFile = $this->compileLinuxParser($parserDir);
        }

        if (!$extFile || !file_exists($extFile)) {
            throw new CompilerException('The Zephir Parser extension could not be found or compiled!');
        }

        return $extFile;
    }

    protected function getLocalParserPath()
    {
        return realpath(dirname(dirname(__DIR__))) . DIRECTORY_SEPARATOR . 'parser' . DIRECTORY_SEPARATOR;
    }
}
