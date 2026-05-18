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
use Stub\Issue2522;

/**
 * @see https://github.com/zephir-lang/zephir/issues/2522
 */
final class Issue2522Test extends TestCase
{
    public function testInlineClosureInitProducesACallable(): void
    {
        $sut = new Issue2522();
        $this->assertTrue($sut->inlineClosureIsCallable());
    }

    public function testInlineClosureInitProducesAClosureInstance(): void
    {
        $sut = new Issue2522();
        $this->assertTrue($sut->inlineClosureReturnsClosureInstance());
    }

    public function testInlineFnKeywordInitProducesACallable(): void
    {
        $sut = new Issue2522();
        $this->assertTrue($sut->inlineFnKeywordIsCallable());
    }

    public function testTwoStepFormStillWorks(): void
    {
        $sut = new Issue2522();
        $this->assertTrue($sut->twoStepClosureIsCallable());
    }
}
