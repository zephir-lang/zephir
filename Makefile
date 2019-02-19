MAKEFILE_PATH 	:= $(word $(words $(MAKEFILE_LIST)), $(MAKEFILE_LIST))
SHELL			?= /bin/bash
ARGS			= $(filter-out $@, $(MAKECMDGOALS))

.SILENT: ;               # no need for @
.ONESHELL: ;             # recipes execute in same shell
.NOTPARALLEL: ;          # wait for this target to finish
.EXPORT_ALL_VARIABLES: ; # send all vars to shell
Makefile: ;              # skip prerequisite discovery

# Run make help by default
.DEFAULT_GOAL = help

# Zephir predefined vars
php_ver     = $(shell php-config --vernum 2>/dev/null)
REPORT_PATH = ./.zephir/reports
VENDOR_DIR  = ./vendor/bin
PHPUNIT     = $(VENDOR_DIR)/simple-phpunit
CS_FIXER    = $(VENDOR_DIR)/php-cs-fixer
CODE_SNIFF  = $(VENDOR_DIR)/phpcs
TEST_OPT    = --colors=always --no-coverage
FILTER      = $(if $(filter-out $@, $(ARGS)), --filter $(filter-out $@, $(ARGS)), "")

# =================================================
include Makefile.common
# =================================================

.PHONY: init test-zephir test-extension

---: ## ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
init: ## Install all necessary Tools => [./vendor/bin/*]
	# PHP CS-Fixer
	$(call install_phar,php-cs-fixer.phar,https://cs.sensiolabs.org/download/php-cs-fixer-v2.phar)
	$(call install_phar,phpcs.phar,https://squizlabs.github.io/PHP_CodeSniffer/phpcs.phar)

---: ## ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
rebuild: ## Generate and Compile Zephir Extension
	zephir fullclean
	zephir generate
	zephir compile

---: ## ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
test-zephir: ## Run Zephir Tests Suit without Coverage and any Reports
	php $(PHPUNIT) $(TEST_OPT) $(FILTER) --testsuit Zephir

---: ## ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
cs-fixer: ## Run PHP CS-Fixer Dry-run
	$(CS_FIXER) --diff --dry-run -v fix

codesniffer: ## Run PHP CodeSniffer
	[ -d $(REPORT_PATH) ] || mkdir -p $(REPORT_PATH)
	$(CODE_SNIFF) \
	--report-full=$(REPORT_PATH)/phpcs.log \
	--report-summary \
	&& ([ $$? -eq 0 ] && echo "$(Green)SUCCSESS!$(NC)") \
	|| (cat $(REPORT_PATH)/phpcs.log && echo "$(Red)FAILURE!$(NC)")
