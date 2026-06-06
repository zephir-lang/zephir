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

/*
 * Golden-fixture generator for the pure-PHP Zephir parser.
 *
 * Runs the REFERENCE C extension (ext-zephir_parser) over a corpus of inputs
 * and dumps `json_encode(zephir_parse_file($src, $path), JSON_PRETTY_PRINT)`
 * — the exact same encoding `CompilerFile::genIR()` writes to its `.json`
 * cache — as committed golden files.  The differential parity test
 * (tests/Zephir/Parser/PhpParserParityTest.php) then asserts the pure-PHP
 * parser reproduces each byte-for-byte.
 *
 * Inputs:
 *   1. Every stub/ *.zep in this repo.
 *   2. Every heredoc/nowdoc Zephir snippet assigned to a variable in the
 *      --FILE-- section of php-zephir-parser/tests/ *.phpt.
 *
 * For each input we emit two files under tests/Parser/fixtures/:
 *   <slug>.zep   — the exact source bytes fed to the parser
 *   <slug>.json  — the golden IR (pretty-printed JSON)
 * The file-path argument passed to the parser is always "<slug>.zep", so the
 * IR's "file" field is deterministic and machine-independent.  Both this
 * generator and the parity test must use the same path.
 *
 * Usage (inside a container that has ext-zephir_parser loaded):
 *   php tests/Parser/generate-golden.php
 */

if (!function_exists('zephir_parse_file')) {
    fwrite(STDERR, "ext-zephir_parser is not loaded; cannot generate golden fixtures.\n");
    exit(1);
}

$repoRoot     = dirname(__DIR__, 2);
$stubDir      = $repoRoot . '/stub';
$phptDir      = getenv('PZP_TESTS_DIR') ?: ($repoRoot . '/../php-zephir-parser/tests');
$fixturesDir  = __DIR__ . '/fixtures';

if (!is_dir($fixturesDir) && !mkdir($fixturesDir, 0775, true) && !is_dir($fixturesDir)) {
    fwrite(STDERR, "Unable to create fixtures dir: {$fixturesDir}\n");
    exit(1);
}

/** @var array<string, string> slug => source */
$inputs = [];

/* ---- 1. stub/*.zep ------------------------------------------------------- */
$it = new RecursiveIteratorIterator(
    new RecursiveDirectoryIterator($stubDir, FilesystemIterator::SKIP_DOTS)
);
foreach ($it as $file) {
    /** @var SplFileInfo $file */
    if ($file->getExtension() !== 'zep') {
        continue;
    }
    $rel  = substr($file->getPathname(), strlen($stubDir) + 1);
    $slug = 'stub__' . slugify(substr($rel, 0, -4));
    $inputs[$slug] = file_get_contents($file->getPathname());
}

/* ---- 2. *.phpt heredoc snippets ----------------------------------------- */
$skipped = [];
if (is_dir($phptDir)) {
    $it = new RecursiveIteratorIterator(
        new RecursiveDirectoryIterator($phptDir, FilesystemIterator::SKIP_DOTS)
    );
    foreach ($it as $file) {
        /** @var SplFileInfo $file */
        if ($file->getExtension() !== 'phpt') {
            continue;
        }
        $rel     = substr($file->getPathname(), strlen($phptDir) + 1);
        $relSlug = slugify(substr($rel, 0, -5));
        $body    = file_get_contents($file->getPathname());

        // Isolate the --FILE-- ... --(next section)-- block.
        if (!preg_match('/--FILE--\R(.*?)\R--[A-Z_]+--/s', $body, $m)) {
            continue;
        }
        $fileSection = $m[1];

        $snippets = extractHeredocs($fileSection);
        if ($snippets === []) {
            $skipped[] = $rel;
            continue;
        }
        foreach ($snippets as $i => $src) {
            $slug = 'phpt__' . $relSlug . ($i > 0 ? '__' . $i : '');
            $inputs[$slug] = $src;
        }
    }
}

ksort($inputs);

$count    = 0;
$unusable = [];
foreach ($inputs as $slug => $src) {
    $path = $slug . '.zep';
    $ir   = zephir_parse_file($src, $path);
    $json = json_encode($ir, JSON_PRETTY_PRINT);

    // Some pathological inputs (e.g. invalid UTF-8 bytes) make the C extension
    // emit an error message that is itself not JSON-encodable; there is then no
    // valid golden to assert against, so skip them.
    if ($json === false) {
        $unusable[] = $slug;
        continue;
    }

    file_put_contents($fixturesDir . '/' . $slug . '.zep', $src);
    file_put_contents($fixturesDir . '/' . $slug . '.json', $json);
    $count++;
}

printf("Wrote %d golden fixtures to %s\n", $count, $fixturesDir);
if ($unusable !== []) {
    printf(
        "Skipped %d input(s) with non-JSON-encodable C-extension output:\n  %s\n",
        count($unusable),
        implode("\n  ", $unusable)
    );
}
if ($skipped !== []) {
    printf(
        "Skipped %d .phpt files with no extractable heredoc:\n  %s\n",
        count($skipped),
        implode("\n  ", $skipped)
    );
}

/**
 * Turn a relative path into a filesystem-safe slug.
 */
function slugify(string $s): string
{
    return preg_replace('/[^A-Za-z0-9]+/', '_', $s);
}

/**
 * Extract Zephir source snippets from a phpt --FILE-- section by evaluating
 * each `$var =<<<LABEL ... LABEL;` heredoc/nowdoc with PHP's own escape
 * semantics (so \$, \\, etc. resolve exactly as the test fed the extension).
 *
 * @return list<string>
 */
function extractHeredocs(string $fileSection): array
{
    // Match: $name = <<<LABEL  or  $name =<<<'LABEL'  ... newline ... LABEL;
    $re = '/\$\w+\s*=\s*<<<(["\']?)(\w+)\1\R(.*?)\R[ \t]*\2;/s';
    if (!preg_match_all($re, $fileSection, $matches, PREG_SET_ORDER)) {
        return [];
    }

    $out = [];
    foreach ($matches as $m) {
        [$full, $quote, $label, $body] = $m;

        // Re-evaluate the heredoc/nowdoc into a fixed variable so PHP applies
        // the same escape semantics the test fed the extension (avoids a
        // variable-variable while preserving heredoc vs nowdoc behavior).
        $resolved = null;
        try {
            eval("\$resolved = <<<{$quote}{$label}{$quote}\n{$body}\n{$label};");
        } catch (\Throwable $e) {
            $resolved = null;
        }
        if (is_string($resolved)) {
            $out[] = $resolved;
        }
    }

    return $out;
}
