<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;

class ExitDieTest extends TestCase
{
    /** @var string */
    private $phpBinary;

    /**
     * {@inheritdoc}
     */
    protected function setUp(): void
    {
        $this->phpBinary = \constant('PHP_BINARY');
        /* If we use phpdbg, you need to add options -qrr */
        if (\defined('PHP_SAPI') && 'phpdbg' == \constant('PHP_SAPI')) {
            $this->phpBinary .= ' -qrr';
        }

        $this->phpBinary .= " -d 'enable_dl=true'";
        $extension = realpath(__DIR__.'/../../ext/modules/stub.so');

        if (file_exists($extension)) {
            $this->phpBinary .= sprintf(" -d 'extension=%s'", $extension);
        }

        parent::setUp();
    }

    public function testShouldExitWthoutAnyMessage()
    {
        $testfile = __DIR__.'/../fixtures/exit.php';
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

    public function testShouldExitWthProvidedStatusMessage()
    {
        $testfile = __DIR__.'/../fixtures/exit_string.php';
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

    public function testShouldExitWthProvidedStatusCode()
    {
        $testfile = __DIR__.'/../fixtures/exit_int.php';
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
