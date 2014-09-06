Zephir is an open source project and a volunteer effort. Contributions are welcome!

## General

*We only accept bug reports, new feature requests and pull requests in GitHub*.

If you are not familiar with markdown and also
[GitHub markdown implementation](https://help.github.com/articles/github-flavored-markdown),
then read this. This is important, because we write code, and all our questions are about the code.
So lets respect each other and wrap code-blocks by specified tags.

If you have a change or new feature in mind, please fill an [NFR](https://github.com/phalcon/zephir/wiki/NFR).

## Pull request procedure

Pull requests should be targeted at Zephir's `master` branch.
Before pushing to your Github repo and issuing the pull request,
please do two things:

1. [Rebase](http://git-scm.com/book/en/Git-Branching-Rebasing) your
   local changes against the `master` branch. Resolve any conflicts
   that arise.

2. Run the full Zephir test suite with the `./vendor/bin/phpunit` command.  You're
   not off the hook even if you just stick to documentation; code
   examples in the docs are tested as well!
3. If your pull request fix or add some functionality - you have to **add tests**.
   That is very important. When you add tests, you are helping other developers,
   and also yourself, do not make mistakes in the future.

Pull requests will be treated as "review requests", and we will give
feedback we expect to see corrected on
[coding style](https://github.com/php-fig/fig-standards/blob/master/accepted/PSR-2-coding-style-guide.md) and
substance before pulling.  Changes contributed via pull request should
focus on a single issue at a time, like any other.  We will not accept
pull-requests that try to "sneak" unrelated changes in.

Normally, all pull requests must include regression tests
that test your change.  Occasionally, a change will be very difficult
to test for.  In those cases, please include a note in your commit
message explaining why.

In the licensing header at the beginning of any files you change,
please make sure the listed date range includes the current year.

## Bugs

Many bugs are raised because users are not running the latest version. Please visit the download page, download/clone the latest available version (or compile it) and confirm if the issue you are experiencing is indeed a bug. When submitting bugs to the issue tracker make sure your issue includes:

 - Code to reproduce the bug
 - Your environment: OS, compiler
 - Zephir version, if your version is outdated - try updating the latest version

Thanks! <br />
Zephir Team
