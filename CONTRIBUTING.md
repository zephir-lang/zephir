# Contributing to Zephir

Zephir is an open source project and a volunteer effort. Zephir welcomes
contribution from everyone. The following summarizes the process for
contributing changes. Please take a moment to review this document in order
to make the contribution process easy and effective for everyone involved.

## Community

Zephir is maintained and supported by [Phalcon Team][team] and developed in
collaboration with a productive community of contributors. Please use the
[GitHub issues][issues] to raise discussion of development topics.

We use the GitHub issues for tracking bugs and feature requests and have
limited bandwidth to address all of them. Thus we only accept bug reports,
new feature requests and pull requests in GitHub. Our great community and
contributors are happy to help you though! Please use these community
resources for obtaining help.

_Please use the [Documentation][docs] before anything else. You can also use
the search feature in our documents to find what you are looking for. If your
question is still not answered, there are more options below._

## Contributions

Contributions to Zephir should be made in the form of GitHub pull requests.
Each pull request will be reviewed by a core contributor (someone with
permission to merge patches). Feedback can be provided and potentially changes
requested or the pull request will be merged. All contributions should follow
this format, even those from core contributors.

## Code style conventions

- PHP: PSR-1/PSR-2
- Bash/Sh: https://github.com/icy/bash-coding-style

## Pull request procedure

Pull requests should be targeted at Zephir's `master` branch.
Before pushing to your Github repo and issuing the pull request,
please do two things:

1. [Rebase][git rebase] your local changes against the `master` branch.
   Resolve any conflicts that arise.
2. Run the full Zephir test suite with the `./vendor/bin/phpunit` command.
   You're not off the hook even if you just stick to documentation; code
   examples in the docs are tested as well!
3. If your pull request fix or add some functionality - you have to **add tests**.
   That is very important. When you add tests, you are helping other developers,
   and also yourself, do not make mistakes in the future.

Pull requests will be treated as "review requests", and we will give
feedback we expect to see corrected on [coding style][psr-2]
and substance before pulling.  Changes contributed via pull request should
focus on a single issue at a time, like any other.  We will not accept
pull-requests that try to "sneak" unrelated changes in.

Normally, all pull requests must include regression tests
that test your change.  Occasionally, a change will be very difficult
to test for.  In those cases, please include a note in your commit
message explaining why.

In the licensing header at the beginning of any files you change,
please make sure the listed date range includes the current year.

## Bugs

Many bugs are raised because users are not running the latest version.
Please visit the download page, download/clone the latest available version
(or compile it) and confirm if the issue you are experiencing is indeed a bug.
When submitting bugs to the issue tracker make sure your issue includes:

 - Code to reproduce the bug
 - Your environment: OS, compiler
 - Zephir version, if your version is outdated - try updating the latest version

Thanks! <br />
Phalcon Team

[team]: https://phalcon.io/en-us/team
[issues]: https://github.com/phalcon/zephir/issues
[docs]: https://docs.zephir-lang.com
[git rebase]: http://git-scm.com/book/en/Git-Branching-Rebasing
[psr-2]: https://github.com/php-fig/fig-standards/blob/master/accepted/PSR-2-coding-style-guide.md
