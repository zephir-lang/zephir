<?php

$t = new Test\Fannkuch();

assert($t->process(5) === array(11, 5, 7));
