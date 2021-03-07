<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension\Integral;

use PHPUnit\Framework\TestCase;
use Stub\RegexDNA;

final class RegexDNATest extends TestCase
{
    public function testProcess(): void
    {
        $test = new RegexDNA();

        ob_start();
        $test->process(\dirname(__DIR__).'/../fixtures/regexdna/input.txt');
        $content = ob_get_clean();

        $this->assertSame(
            $content,
            file_get_contents(\dirname(__DIR__).'/../fixtures/regexdna/output.txt')
        );
    }
}
