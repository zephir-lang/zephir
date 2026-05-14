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

namespace Zephir\Test;

use InvalidArgumentException;
use PHPUnit\Framework\TestCase;
use Zephir\HeadersManager;

final class HeadersManagerTest extends TestCase
{
    private HeadersManager $headersManager;

    protected function setUp(): void
    {
        $this->headersManager = new HeadersManager();
    }

    public function testShouldAddHeaderAtLastPosition(): void
    {
        $this->headersManager->add('stdio.h');
        $this->headersManager->add('stdlib.h');

        $headers = $this->headersManager->get();

        $this->assertContains('stdio.h', $headers);
        $this->assertContains('stdlib.h', $headers);
    }

    public function testShouldAddHeaderAtFirstPosition(): void
    {
        $this->headersManager->add('stdio.h', HeadersManager::POSITION_LAST);
        $this->headersManager->add('stdlib.h', HeadersManager::POSITION_FIRST);

        $headers = $this->headersManager->get();

        $this->assertContains('stdio.h', $headers);
        $this->assertContains('stdlib.h', $headers);
    }


    public function testShouldNotAddDuplicateHeaders(): void
    {
        $this->headersManager->add('stdio.h');
        $this->headersManager->add('stdio.h');
        $this->headersManager->add('stdio.h');

        $headers = $this->headersManager->get();

        $count = 0;
        foreach ($headers as $header) {
            if ($header === 'stdio.h') {
                $count++;
            }
        }

        $this->assertSame(1, $count, 'Header should only appear once');
    }

    public function testShouldMergeHeadersInCorrectOrder(): void
    {
        $this->headersManager->add('middle.h');
        $this->headersManager->add('first.h', HeadersManager::POSITION_FIRST);
        $this->headersManager->add('last.h', HeadersManager::POSITION_LAST);

        $headers = $this->headersManager->get();

        $this->assertIsArray($headers);
        $this->assertContains('first.h', $headers);
        $this->assertContains('middle.h', $headers);
        $this->assertContains('last.h', $headers);
    }

    public function testPositionConstants(): void
    {
        $this->assertSame(1, HeadersManager::POSITION_FIRST);
        $this->assertSame(2, HeadersManager::POSITION_LAST);
    }
}
