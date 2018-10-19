<?php

/*
 * This file is part of the Zephir package.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use Zephir\Support\TestCase;
use PHPUnit\Framework\SkippedTestError;

class ExitDieTest extends TestCase
{
    /** @var string */
    private $phpBinary;

    /**
     * {@inheritdoc}
     *
     * @return void
     */
    public static function setUpBeforeClass()
    {
        if (PHP_VERSION_ID >= 70300) {
            throw new SkippedTestError("Skip test on unstable PHP versions");
        }
    }

    /**
     * {@inheritdoc}
     *
     * @return void
     */
    public function setUp()
    {
        $this->phpBinary = constant('PHP_BINARY');
        /* If we use phpdbg, you need to add options -qrr */
        if (defined('PHP_SAPI') && constant('PHP_SAPI') == 'phpdbg') {
            $this->phpBinary .= ' -qrr';
        }

        $this->phpBinary .= " -d 'enable_dl=true'";
        $extension = realpath( __DIR__ . '/../../ext/modules/test.so');

        if (file_exists($extension)) {
            $this->phpBinary .= sprintf(" -d 'extension=%s'", $extension);
        }

        parent::setUp();
    }

    /** @test */
    public function shouldExitWthoutAnyMessage()
    {
        $testfile = __DIR__ . '/../fixtures/exit.php';
        $command = "$this->phpBinary $testfile";
        $output = [];

        exec($command, $output, $exitStatus);

        $this->assertEmpty($output, sprintf(
            'Exit message does not match with expected value. Output was: %s. Executed command: %s',
            $this->prepareOutput($output),
            $command
        ));

        $this->assertSame(0, $exitStatus, sprintf(
            'Exit code does not match with expected value. Output was: %s. Executed command: %s',
            $this->prepareOutput($output),
            $command
        ));
    }

    /** @test */
    public function shouldExitWthProvidedStatusMessage()
    {
        $testfile = __DIR__ . '/../fixtures/exit_string.php';
        $statusMessage = 'Hello World';
        $command = "$this->phpBinary $testfile \"$statusMessage\"";
        $output = [];

        exec($command, $output, $exitStatus);

        $this->assertSame($statusMessage, $output[0], sprintf(
            'Exit message does not match with expected value. Output was: %s. Executed command: %s',
            $this->prepareOutput($output),
            $command
        ));

        $this->assertSame(0, $exitStatus, sprintf(
            'Exit code does not match with expected value. Output was: %s. Executed command: %s',
            $this->prepareOutput($output),
            $command
        ));
    }

    /** @test */
    public function shouldExitWthProvidedStatusCode()
    {
        $testfile = __DIR__ . '/../fixtures/exit_int.php';
        $statusCode = 220;
        $command = "$this->phpBinary $testfile $statusCode";
        $output = [];

        exec($command, $output, $exitStatus);

        $this->assertEmpty($output, sprintf(
            'Exit message does not match with expected value. Output was: %s. Executed command: %s',
            $this->prepareOutput($output),
            $command
        ));

        $this->assertSame($statusCode, $exitStatus, sprintf(
            'Exit code does not match with expected value. Output was: %s. Executed command: %s',
            $this->prepareOutput($output),
            $command
        ));
    }

    private function prepareOutput($output)
    {
        if (empty($output)) {
            return '(empty output)';
        }

        if (isset($output[0]) && !empty($output[0])) {
            return $output[0];
        }

        return json_encode($output, JSON_PRETTY_PRINT | JSON_UNESCAPED_SLASHES);
    }
}
