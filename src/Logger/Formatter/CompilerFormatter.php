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

namespace Zephir\Logger\Formatter;

use Monolog\Formatter\LineFormatter;
use Zephir\Config;

use function array_key_exists;
use function count;
use function file;
use function file_exists;
use function is_array;
use function preg_replace;
use function str_repeat;
use function str_replace;
use function strtolower;
use function ucfirst;

use const PHP_EOL;

/**
 * Formatter for warnings/notices/errors generated in compilation.
 */
final class CompilerFormatter extends LineFormatter
{
    public const SIMPLE_FORMAT = " %level_name%: %message% in %file% on line %line% %type%\n";

    /**
     * The contents of the files that are involved in the log message.
     */
    private array $filesContent = [];

    public function __construct(private Config $config)
    {
        parent::__construct();
    }

    public function format(array $record): string
    {
        if ($this->config->get('silent')) {
            return '';
        }

        $vars = parent::normalize($record);

        $output = $this->format;

        // unused
        unset($vars['extra']);
        $output = str_replace('%extra%', '', $output);
        $output = str_replace('%context%', '', $output);

        // ignore empty context or invalid format
        if (
            !empty($vars['context']) &&
            is_array($vars['context']) &&
            2 == count($vars['context'])
        ) {
            $type = $vars['context'][0];
            $node = $vars['context'][1];

            if (!$this->config->get($type, 'warnings')) {
                return '';
            }

            $vars['type'] = "[$type]";

            if (!isset($node['file'])) {
                $vars['file'] = 'unknown';
                $vars['line'] = '0';
            } else {
                $vars['file'] = $node['file'];
                $vars['line'] = $node['line'];
                $output       .= PHP_EOL;

                $lines = $this->getFileContents($node['file']);
                if (isset($lines[$node['line'] - 1])) {
                    $line   = $lines[$node['line'] - 1];
                    $output .= "\t" . str_replace("\t", ' ', $line);
                    if (($node['char'] - 1) > 0) {
                        $output .= PHP_EOL . "\t" . str_repeat('-', $node['char'] - 1) . '^' . PHP_EOL;
                    }
                }
            }

            $output = str_replace('%file%', $this->stringify($vars['file']), $output);
            $output = str_replace('%line%', $this->stringify($vars['line']), $output);
            $output = str_replace('%type%', $this->stringify($vars['type']), $output);
        }

        $output = $this->replacePlaceholders($vars, $output);

        return $this->cleanExtraPlaceholders($output);
    }

    /**
     * Remove leftover %extra.xxx% and %context.xxx% (if any).
     */
    private function cleanExtraPlaceholders(string $output): string
    {
        if (str_contains($output, '%')) {
            $output = preg_replace('/%(?:extra|context)\..+?%/', '', $output);
            $output = preg_replace('/ %type%\n/', "\n", $output);
            $output = preg_replace('/on line %line%/', '', $output);
            $output = preg_replace('/ in %file% /', '', $output);
        }

        return $output;
    }

    /**
     * Gets the contents of the files that are involved in the log message.
     */
    private function getFileContents(string $file): array
    {
        if (!isset($this->filesContent[$file])) {
            $this->filesContent[$file] = file_exists($file) ? file($file) : [];
        }

        return $this->filesContent[$file];
    }

    private function replacePlaceholders(array $vars, $output)
    {
        // WARNING -> Warning
        if (array_key_exists('level_name', $vars)) {
            $vars['level_name'] = ucfirst(strtolower($vars['level_name']));
        }

        foreach ($vars as $var => $val) {
            $placeholder = '%' . $var . '%';
            $realValue   = $this->stringify($val);

            if (!str_contains($output, $placeholder)) {
                continue;
            }

            // Strip level name from entries like "Info: Installing..."
            if ('%level_name%' === $placeholder) {
                switch ($realValue) {
                    case 'Debug':
                    case 'Info':
                        $output = str_replace('%level_name%: ', '', $output);
                        continue 2;
                }
            }

            $output = str_replace('%' . $var . '%', $realValue, $output);
        }

        return $output;
    }
}
