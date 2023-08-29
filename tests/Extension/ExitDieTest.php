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

namespace Extension;

use PHPUnit\Framework\TestCase;

use function constant;
use function defined;

final class ExitDieTest extends TestCase
{
    private string $phpBinary;

    /**
     * {@inheritdoc}
     */
    protected function setUp(): void
    {
        $this->phpBinary = constant('PHP_BINARY');
        /* If we use phpdbg, you need to add options -qrr */
        if (defined('PHP_SAPI') && 'phpdbg' == constant('PHP_SAPI')) {
            $this->phpBinary .= ' -qrr';
        }

        parent::setUp();
    }

    public function testShouldExitWithoutAnyMessage(): void
    {
        $testFile = __DIR__.'/../fixtures/exit.php';
        $command = "$this->phpBinary $testFile";
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

    public function testShouldExitWthProvidedStatusMessage(): void
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

    public function testShouldExitWthProvidedStatusCode(): void
    {
        $testFile = __DIR__.'/../fixtures/exit_int.php';
        $statusCode = 220;
        $command = "$this->phpBinary $testFile $statusCode";
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

    private function prepareOutput(array $output): string
    {
        if (empty($output)) {
            return '(empty output)';
        }

        if (!empty($output[0])) {
            return $output[0];
        }

        return json_encode($output, JSON_PRETTY_PRINT | JSON_UNESCAPED_SLASHES);
    }
}
