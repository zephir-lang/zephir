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

namespace Zephir\Stubs;

use function array_map;
use function array_pop;
use function array_reverse;
use function array_shift;
use function count;
use function explode;
use function implode;
use function preg_match;
use function preg_replace;
use function rtrim;
use function str_replace;
use function trim;

/**
 * @author Gusakov Nikita <dev@nkt.me>
 */
class DocBlock
{
    /**
     * @var string
     */
    protected string $description = '';
    /**
     * @var string
     */
    protected string $indent;
    /**
     * @var array
     */
    protected array $lines = [];

    /**
     * @param string|null $source Raw doc-block
     * @param string      $indent Indent, 4 spaces by default
     */
    public function __construct(?string $source, string $indent = '    ')
    {
        $this->indent = $indent;
        $lines        = explode("\n", trim($source ?: ''));
        $count        = count($lines);

        foreach ($lines as $i => $line) {
            $line = preg_replace('#^([\s\t]+)?/?([*]+)([\s\t]+)?$#im', '', rtrim($line));
            $line = preg_replace('#^([\s\t]+)?([*]+)([\s\t]+)?/?$#im', '', rtrim($line));

            if ((0 === $i || $i === $count - 1) && empty($line)) {
                continue;
            }

            $cleaned = trim($line, "\t*\0 ");
            $cleaned = str_replace('$$', '$', $cleaned);

            if (str_starts_with($cleaned, '@')) {
                $this->lines[] = $cleaned;
            } else {
                $line = preg_replace('#([\s\t]+)?[*]#', '', $line);
                $line = preg_replace('#([\s\t]+)?[*]([\s\t]){1,2}#', '$1* ', ' * ' . $line);
                $line = preg_replace('#[*]([\s\t])+$#', '*', $line);
                $line = preg_replace('#\t#', $indent, $line);

                $this->lines[] = array_pop($this->lines) . "\n{$this->indent}" . $line;
            }
        }

        if (!empty($this->lines) && !str_starts_with(trim($this->lines[0], "\t*\0 "), '@')) {
            $description = array_shift($this->lines);
            $description = explode("\n", $description);

            $cleaned = [];
            $empty   = 0;
            foreach ($description as $i => $line) {
                if (preg_match('#^([\s\t]+)?[*]([\s\t]+)?$#', $line)) {
                    ++$empty;
                } else {
                    $empty = 0;
                }

                if ($empty > 1) {
                    continue;
                }

                $cleaned[] = $line;
            }

            $reversed = array_reverse($cleaned);
            if (empty($reversed[0]) || '' === trim($reversed[0], "\t*\0 ")) {
                unset($reversed[0]);
            }

            $this->description = implode("\n", array_reverse($reversed));
        }
    }

    /**
     * @return string
     */
    public function __toString(): string
    {
        $doc    = '';
        $indent = $this->indent;

        if (!empty($this->description)) {
            $doc = $this->description;
        }

        if (!empty($this->lines)) {
            $lines = array_map(fn($line) => "$indent * $line", $this->lines);

            if (!empty($doc)) {
                $doc .= "\n$indent *";
            }

            $doc .= "\n" . implode("\n", $lines);
        }

        return '' === $doc ? '' : "$indent/**$doc\n$indent */";
    }
}
