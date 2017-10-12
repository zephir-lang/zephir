<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Stubs;

/**
 * @author Gusakov Nikita <dev@nkt.me>
 */
class DocBlock
{
    /**
     * @var string
     */
    protected $indent;

    /**
     * @var string
     */
    protected $description = '';

    /**
     * @var array
     */
    protected $lines = [];

    /**
     * @param string $source Raw doc-block
     * @param string $indent Indent, 4 spaces by default
     */
    public function __construct($source, $indent = '    ')
    {
        $this->indent = $indent;
        $lines = explode("\n", trim($source));
        $count = count($lines);

        foreach ($lines as $i => $line) {
            $line = preg_replace('#^([\s\t]+)?/?([*]+)([\s\t]+)?$#im', '', rtrim($line));
            $line = preg_replace('#^([\s\t]+)?([*]+)([\s\t]+)?/?$#im', '', rtrim($line));

            if (($i === 0 || $i === $count - 1) && empty($line)) {
                continue;
            }

            $cleaned = trim($line, "\t*\0 ");

            if (strpos($cleaned, '@') === 0) {
                $this->lines[] = $line = $cleaned;
            } else {
                $line = preg_replace('#([\s\t]+)?[*]#', '', $line);
                $line = preg_replace('#([\s\t]+)?[*]([\s\t]){1,2}#', '$1* ', ' * ' . $line);
                $line = preg_replace('#[*]([\s\t]){1,}$#', '*', $line);
                $line = preg_replace('#\t#', $indent, $line);

                $this->lines[] = array_pop($this->lines) . "\n{$this->indent}" . $line;
            }
        }

        if (!empty($this->lines) && strpos(trim($this->lines[0], "\t*\0 "), '@') !== 0) {
            $description = array_shift($this->lines);
            $description = explode("\n", $description);

            $cleaned = [];
            $empty = 0;
            foreach ($description as $i => $line) {
                if (preg_match('#^([\s\t]+)?[*]([\s\t]+)?$#', $line)) {
                    $empty++;
                } else {
                    $empty = 0;
                }

                if ($empty > 1) {
                    continue;
                }

                $cleaned[] = $line;
            }

            $reversed = array_reverse($cleaned);
            if (empty($reversed[0]) || trim($reversed[0], "\t*\0 ") === '') {
                unset($reversed[0]);
            }

            $this->description = implode("\n", array_reverse($reversed));
        }
    }

    /**
     * @return string
     */
    public function __toString()
    {
        $doc = '';
        $indent = $this->indent;

        if (!empty($this->description)) {
            $doc = $this->description;
        }

        if (!empty($this->lines)) {
            $lines = array_map(function ($line) use ($indent) {
                return "$indent * $line";
            }, $this->lines);

            if (!empty($doc)) {
                $doc .= "\n$indent *";
            }

            $doc .= "\n" . implode("\n", $lines);
        }

        return $doc === '' ? '' : "$indent/**$doc\n$indent */";
    }
}
