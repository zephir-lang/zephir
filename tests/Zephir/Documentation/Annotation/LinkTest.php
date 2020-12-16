<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\Documentation\Annotation\Link;

class LinkTest extends TestCase
{
    const LINK_NAME = 'link';

    public function testShouldGetLink()
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

    public function testShouldGetUri()
    {
        $testString = 'https://github.com/phalcon/zephir Zephir SRC';

        $link = new Link(self::LINK_NAME, $testString);

        $this->assertSame(self::LINK_NAME, $link->getName());
        $this->assertSame($testString, $link->getString());
        $this->assertSame('https://github.com/phalcon/zephir', $link->getUri());
    }

    public function testShouldGetLinkText()
    {
        $testString = 'https://github.com/phalcon/zephir Zephir SRC';

        $link = new Link(self::LINK_NAME, $testString);

        $this->assertSame(self::LINK_NAME, $link->getName());
        $this->assertSame($testString, $link->getString());
        $this->assertSame('Zephir SRC', $link->getLinkText());
    }
}
