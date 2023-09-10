<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Documentation\File;

use Zephir\Class\Definition\Definition;
use Zephir\Documentation\FileInterface;

class ClassesFile implements FileInterface
{
    public function __construct(protected Definition $classes)
    {
    }

    public function getTemplateName(): string
    {
        return 'classes.phtml';
    }

    public function getData(): array
    {
        return [
            'classes' => $this->classes,
        ];
    }

    public function getOutputFile(): string
    {
        return 'classes.html';
    }
}
