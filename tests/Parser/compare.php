<?php

/**
 * Dev harness: run the pure-PHP parser over every golden fixture and report
 * byte-identical parity vs the C extension's dump. Not a committed test — used
 * to drive the slice toward parity. Usage:
 *   php tests/Parser/compare.php [--show=N] [filter]
 */

declare(strict_types=1);

require dirname(__DIR__, 2) . '/vendor/autoload.php';

use Zephir\Parser\Php\PhpParser;

$fixtures = __DIR__ . '/fixtures';
$show     = 3;
$filter   = '';
foreach ($argv as $a) {
    if (str_starts_with($a, '--show=')) {
        $show = (int) substr($a, 7);
    } elseif ($a !== $argv[0]) {
        $filter = $a;
    }
}

$match = 0;
$diff  = 0;
$err   = 0;
$shown = 0;
$diffSlugs = [];

$zeps = glob($fixtures . '/*.zep');
sort($zeps);
foreach ($zeps as $zep) {
    $slug = basename($zep, '.zep');
    if ($filter !== '' && !str_contains($slug, $filter)) {
        continue;
    }
    $src      = file_get_contents($zep);
    $expected = file_get_contents($fixtures . '/' . $slug . '.json');

    try {
        $ir     = (new PhpParser())->parse($src, $slug . '.zep');
        $actual = json_encode($ir, JSON_PRETTY_PRINT);
    } catch (\Throwable $e) {
        $err++;
        if ($shown < $show) {
            echo "ERROR  $slug: " . get_class($e) . ': ' . $e->getMessage() . "\n";
            echo '  at ' . $e->getFile() . ':' . $e->getLine() . "\n";
            $shown++;
        }
        continue;
    }

    if ($actual === $expected) {
        $match++;
        continue;
    }

    $diff++;
    $diffSlugs[] = $slug;
    if ($shown < $show) {
        echo "DIFF   $slug\n";
        printFirstDiff($expected, $actual);
        $shown++;
    }
}

$total = $match + $diff + $err;
echo "\n==== $match/$total match, $diff diff, $err error ====\n";
if ($diffSlugs !== [] && $filter === '') {
    echo "diff slugs (first 40): " . implode(' ', array_slice($diffSlugs, 0, 40)) . "\n";
}

function printFirstDiff(string $expected, string $actual): void
{
    $e = explode("\n", $expected);
    $a = explode("\n", $actual);
    $n = max(count($e), count($a));
    for ($i = 0; $i < $n; $i++) {
        $el = $e[$i] ?? '<none>';
        $al = $a[$i] ?? '<none>';
        if ($el !== $al) {
            $from = max(0, $i - 2);
            for ($j = $from; $j <= $i; $j++) {
                echo '  exp[' . $j . ']: ' . ($e[$j] ?? '<none>') . "\n";
                echo '  act[' . $j . ']: ' . ($a[$j] ?? '<none>') . "\n";
            }

            return;
        }
    }
}
