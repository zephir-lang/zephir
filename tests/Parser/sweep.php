<?php

/**
 * Dev harness: differential sweep of the pure-PHP parser against the C
 * extension over an arbitrary .zep tree (e.g. Phalcon). For each file, asserts
 * json_encode(zephir_parse_file()) === json_encode(PhpParser->parse()).
 * Not a committed test. Usage (inside a container with ext-zephir_parser):
 *   php tests/Parser/sweep.php /path/to/zep/tree [--show=N]
 */

declare(strict_types=1);

require dirname(__DIR__, 2) . '/vendor/autoload.php';

use Zephir\Parser\Php\PhpParser;

if (!function_exists('zephir_parse_file')) {
    fwrite(STDERR, "ext-zephir_parser not loaded\n");
    exit(1);
}

$root = $argv[1] ?? null;
$show = 5;
foreach ($argv as $a) {
    if (str_starts_with($a, '--show=')) {
        $show = (int) substr($a, 7);
    }
}
if ($root === null || !is_dir($root)) {
    fwrite(STDERR, "usage: php sweep.php <dir> [--show=N]\n");
    exit(1);
}

$files = [];
$it    = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($root, FilesystemIterator::SKIP_DOTS));
foreach ($it as $f) {
    if ($f->getExtension() === 'zep') {
        $files[] = $f->getPathname();
    }
}
sort($files);

$match = 0;
$diff  = 0;
$err   = 0;
$shown = 0;
foreach ($files as $path) {
    $src = file_get_contents($path);
    $rel = substr($path, strlen($root) + 1);

    $expected = json_encode(zephir_parse_file($src, $rel), JSON_PRETTY_PRINT);
    try {
        $actual = json_encode((new PhpParser())->parse($src, $rel), JSON_PRETTY_PRINT);
    } catch (\Throwable $e) {
        $err++;
        if ($shown < $show) {
            echo "EXC  $rel: " . get_class($e) . ' ' . $e->getMessage() . ' @ ' . basename($e->getFile()) . ':' . $e->getLine() . "\n";
            $shown++;
        }
        continue;
    }

    if ($actual === $expected) {
        $match++;
        continue;
    }
    $diff++;
    if ($shown < $show) {
        echo "DIFF $rel\n";
        $e = explode("\n", (string) $expected);
        $a = explode("\n", $actual);
        for ($i = 0, $n = max(count($e), count($a)); $i < $n; $i++) {
            if (($e[$i] ?? '') !== ($a[$i] ?? '')) {
                for ($j = max(0, $i - 3); $j <= $i; $j++) {
                    echo '  exp[' . $j . ']: ' . ($e[$j] ?? '<>') . "\n";
                    echo '  act[' . $j . ']: ' . ($a[$j] ?? '<>') . "\n";
                }
                break;
            }
        }
        $shown++;
    }
}

$total = $match + $diff + $err;
echo "\n==== {$match}/{$total} match, {$diff} diff, {$err} error ====\n";
