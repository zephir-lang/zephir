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

namespace Zephir\Traits;

use Zephir\Class\Definition\Definition;
use Zephir\CompilationContext;

use function dirname;
use function explode;
use function implode;
use function is_dir;
use function mkdir;
use function str_repeat;
use function str_replace;
use function strpos;
use function strtolower;

use const DIRECTORY_SEPARATOR;
use const PHP_EOL;

trait CompilerTrait
{
    protected function calculatePaths(string $completeName): array
    {
        $path = str_replace('\\', DIRECTORY_SEPARATOR, strtolower($completeName));

        $filePath       = 'ext/' . $path . '.zep.c';
        $filePathHeader = 'ext/' . $path . '.zep.h';

        if (strpos($path, DIRECTORY_SEPARATOR)) {
            $dirname = dirname($filePath);
            if (!is_dir($dirname)) {
                mkdir($dirname, 0755, true);
            }
        }

        return [$path, $filePath, $filePathHeader];
    }

    protected function generateClassHeadersPost(
        string $code,
        ?Definition $classDefinition,
        CompilationContext $compilationContext
    ): void {
        $code .= PHP_EOL;
        $code .= '#include "kernel/main.h"' . PHP_EOL;

        if ('class' === $classDefinition->getType()) {
            foreach ($compilationContext->headersManager->get() as $header => $one) {
                $code .= '#include "' . $header . '.h"' . PHP_EOL;
            }
        }

        $code .= PHP_EOL;

        if (!$this->headerCBlocks->isEmpty()) {
            $code .= implode(PHP_EOL, $this->headerCBlocks->get()) . PHP_EOL;
            $code .= PHP_EOL;
        }

        /**
         * Prepend the required files to the header
         */
        $compilationContext->codePrinter->preOutput($code);
    }

    protected function generateCodeHeadersPre(?Definition $classDefinition): string
    {
        $separators = str_repeat('../', count(explode('\\', $classDefinition->getCompleteName())) - 1);

        $code = PHP_EOL;
        $code .= '#ifdef HAVE_CONFIG_H' . PHP_EOL;
        $code .= '#include "' . $separators . 'ext_config.h"' . PHP_EOL;
        $code .= '#endif' . PHP_EOL;
        $code .= PHP_EOL;

        $code .= '#include <php.h>' . PHP_EOL;
        $code .= '#include "' . $separators . 'php_ext.h"' . PHP_EOL;
        $code .= '#include "' . $separators . 'ext.h"' . PHP_EOL;
        $code .= PHP_EOL;

        return $code;
    }
}
