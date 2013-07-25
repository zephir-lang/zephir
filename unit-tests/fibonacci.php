<?php

$t = new Test\Fibonnaci();

assert($t->fibArray() === array(
  0, 1, 1, 2, 3, 5, 8, 13, 21, 34
));
