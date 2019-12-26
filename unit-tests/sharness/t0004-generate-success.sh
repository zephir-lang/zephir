#!/usr/bin/env bash

# shellcheck disable=SC2034
test_description="Test generate command for success"

# shellcheck disable=SC1091
source ./setup.sh

test_expect_success "Should not use the same cache when working with different projects" "
  cd $OUTPUTDIR &&
  rm -rf ./gen1 &&
  zephirc init gen1 &&
  cd ./gen1 &&
  echo 'namespace Gen1;' > ./gen1/test.zep &&
  echo 'class Test { }' >> ./gen1/test.zep &&
  zephirc generate &&
  test -d ./.zephir &&
  rm -r ./.zephir &&

  cd $OUTPUTDIR &&
  rm -rf ./gen2 &&
  zephirc init gen2 &&
  cd ./gen2 &&
  echo 'namespace Gen2;' > ./gen2/test.zep &&
  echo 'class Test { }' >> ./gen2/test.zep &&
  zephirc generate &&
  test -d ./.zephir &&
  test ! -d $OUTPUTDIR/gen1/.zephir
"

test_done
