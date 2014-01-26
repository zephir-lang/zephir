<?php

$b = new Test\BranchPrediction();

assert($b->testLikely1() === true);
assert($b->testLikely2(false) === false);
assert($b->testUnlikely1() === true);
assert($b->testUnlikely2(false) === false);
