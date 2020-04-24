;;; Directory Local Variables
;; For more information see (info "(emacs) Directory Variables")

((nil
  (indent-tabs-mode . nil))
 (zephir-mode
  (fill-column . 120))
 (c-mode
  (fill-column . 80)
  (c-file-style . "k&r")
  (tab-width . 4)
  (c-basic-offset . 4)
  (indent-tabs-mode . t)
  (flycheck-checker . c/c++-gcc)
  (flycheck-disabled-checkers . (c/c++-clang))
  (flycheck-gcc-language-standard . "gnu99"))
 (php-mode
  (fill-column . 120)
  (c-file-style . nil)
  (indent-tabs-mode . nil)
  (flycheck-disabled-checkers . (php-phpmd php-phpcs))))
