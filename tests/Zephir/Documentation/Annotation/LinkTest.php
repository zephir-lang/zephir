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

namespace Zephir\Test\Documentation\Annotation;

use PHPUnit\Framework\TestCase;
use Zephir\Documentation\Annotation\Link;

final class LinkTest extends TestCase
{
    private const LINK_NAME = 'link';

    public function testShouldGetLink(): void
    {
        $link = new Link(self::LINK_NAME, '  sample with leading space  ');

        $this->assertSame(self::LINK_NAME, $link->getName());
        $this->assertSame(
            'sample with leading space',
            $link->getString(),
            'Link should be without leading spaces'
        );

        $link->setString('another link');
        $this->assertSame('another link', $link->getString());
    }

    public function testShouldGetUri(): void
    {
        $testString = 'https://github.com/zephir-lang/zephir Zephir SRC';

        $link = new Link(self::LINK_NAME, $testString);

        $this->assertSame(self::LINK_NAME, $link->getName());
        $this->assertSame($testString, $link->getString());
        $this->assertSame('https://github.com/zephir-lang/zephir', $link->getUri());
    }

    public function testShouldGetLinkText(): void
    {
        $testString = 'https://github.com/zephir-lang/zephir Zephir SRC';

        $link = new Link(self::LINK_NAME, $testString);

        $this->assertSame(self::LINK_NAME, $link->getName());
        $this->assertSame($testString, $link->getString());
        $this->assertSame('Zephir SRC', $link->getLinkText());
    }
}
