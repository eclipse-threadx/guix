# Contributing to Eclipse ThreadX GUIX

Thanks for your interest in this project.

## Project description

GUIX is the embedded graphical user interface library for ThreadX applications. It provides drawing, widgets, animation, text, and display-driver interfaces. This repository also contains GUIX Studio, the Windows design tool used to create GUIX screens and resources.

Eclipse ThreadX provides a vendor-neutral, open source, safety-certified OS for real-time applications, published under a permissive license. The Eclipse ThreadX suite encompasses:

* **ThreadX** - advanced real-time operating system (RTOS) designed specifically for deeply embedded applications
* **NetX Duo** - advanced, industrial-grade TCP/IP network stack designed specifically for deeply embedded real-time and IoT applications
* **FileX** - high-performance, FAT-compatible file system that is fully integrated with the ThreadX kernel
* **GUIX** - complete, embedded graphical user interface (GUI) library
* **GUIX Studio** - design environment, facilitating the creation and maintenance of all graphical elements for GUIX
* **USBX** - high-performance USB host, device, and on-the-go (OTG) embedded stack, fully integrated with the ThreadX kernel
* **LevelX** - flash wear levelling for FileX and stand-alone purposes
* **TraceX** - analysis tool that provides a graphical view of real-time system events to better understand the behaviour of real-time systems
* **ZoneX** - deterministic partitioning hypervisor for mixed-criticality embedded systems
* **SampleX** - samples and demos for the components above

Project websites:

* https://threadx.io
* https://projects.eclipse.org/projects/iot.threadx

This file describes how to contribute to **GUIX and GUIX Studio**. General contribution conventions are shared across the Eclipse ThreadX repositories; build and test instructions differ by component.

## Terms of Use

This repository is subject to the Terms of Use of the Eclipse Foundation: https://www.eclipse.org/legal/termsofuse.php

## New contributors

Welcome. Here is the shortest path from zero to a merged pull request.

1. **Create an Eclipse Foundation account.** https://accounts.eclipse.org/user/register

   The email address on that account matters: it must be the same address you use as the `Author` of your Git commits. If the two do not match, the ECA check on your pull request will fail.

2. **Sign the Eclipse Contributor Agreement (ECA).** See the section below. This is a one-time step and covers every Eclipse Foundation project.

   <br>

3. **Configure Git accordingly.**

   ```
   git config user.name "Your Name"
   git config user.email "the-address-on-your-eclipse-account@example.org"
   ```

4. **Pick something to work on.** Open issues are fair game, including ones nobody has assigned. Please leave a comment on the issue saying you intend to work on it, so two people do not solve the same problem twice. If an issue carries a `good first issue` label, it is a reasonable starting point.

5. **Have an idea for a new feature? Discuss it first.** Open a thread in [GitHub Discussions](https://github.com/orgs/eclipse-threadx/discussions) or raise it on the [developer mailing list](https://accounts.eclipse.org/mailing-list/threadx-dev) before writing code. Widget, rendering, and resource-format changes can affect existing GUIX applications and Studio projects, so discuss the shape of a feature before submitting a finished implementation.

Bug fixes and documentation corrections need no prior discussion. Send them straight in.

## Eclipse Development Process

This Eclipse Foundation open project is governed by the Eclipse Foundation Development Process and operates under the terms of the Eclipse IP Policy.

* https://eclipse.org/projects/dev_process
* https://www.eclipse.org/org/documents/Eclipse_IP_Policy.pdf

## Eclipse Contributor Agreement

In order to be able to contribute to Eclipse Foundation projects you must electronically sign the Eclipse Contributor Agreement (ECA): https://www.eclipse.org/legal/ECA.php

The ECA provides the Eclipse Foundation with a permanent record that you agree that each of your contributions will comply with the commitments documented in the Developer Certificate of Origin (DCO). Having an ECA on file associated with the email address matching the "Author" field of your contribution's Git commits fulfills the DCO's requirement that you sign-off on your contributions.

For more information, please see the Eclipse Committer Handbook: https://www.eclipse.org/projects/handbook/#resources-commit

## Required tooling

Build and test GUIX with CMake and Ninja. The root and regression projects require CMake 3.13 or later. The Linux regression suite uses GCC 14 with matching gcov and compiles GUIX sources as C99. It builds 32-bit by default, so install the GCC multilib toolchain. On Ubuntu, `scripts/install.sh` installs the host tools and pins `gcovr` 8.6 in `.venv-ci/`.

The GUIX CMake target links ThreadX. The Linux regression runner obtains a pinned ThreadX revision under `test/guix_test/cmake/threadx/` and builds it for the host tests. Embedded ports need their target compiler and toolchain files; use GCC syntax for assembly built with GNU toolchains.

GUIX Studio tests run on Windows with the Visual Studio 2022 MSBuild tools, CMake, CTest, and Python. The CI jobs use `windows-2022`. A full Visual Studio IDE is not required for the scripts, but the matching Build Tools and Windows SDK must be available.

## Building and testing

The GUIX library regression suite lives under `test/guix_test/cmake/`. The top-level scripts build and test all 18 configurations:

```sh
scripts/install.sh
scripts/build.sh all
scripts/test.sh all
```

For a focused run, use the same configuration in both steps:

```sh
scripts/build.sh default_build_coverage
scripts/test.sh default_build_coverage
```

The configurations cover error checking, UTF-8 and extended Unicode, mouse and font options, bidirectional text, legacy compatibility, palette modes, deprecated APIs, and partial canvas refresh. The runner uses GCC 14 and instruments every selected configuration. Select the configurations affected by your change and run the full suite before review when feasible. [The GUIX regression test guide](test/README.md) describes golden-file comparisons, individual tests, and debugging.

GUIX Studio has three Windows validation paths. Run the pair relevant to your change from a Windows command prompt with the MSBuild environment available:

| Scripts under `scripts/` | What they check |
| ------------------------ | --------------- |
| `build_guix_studio.cmd`, `test_studio_demo.cmd` | GUIX Studio demo generation. |
| `build_guix.cmd`, `test_studio_demo_compile.cmd` | Compilation of generated demos. |
| `install_studio_view.cmd`, `test_studio_view.cmd` | GUIX Studio View. |

All Linux regression configurations produce JSON, XML and HTML coverage reports under `test/guix_test/cmake/coverage_report/`. A full run merges the 18 reports; a selected subset produces a partial report. The merged report measures GUIX sources across configurations without dropping optional or untested code. CI requires at least 98.3% line and 96.0% branch coverage. These floors do not replace the project's 100% coverage goal: add or update regression tests for new behaviour and explain any relevant gaps. Studio jobs do not collect this Linux coverage. Host tests do not replace testing a changed display driver on the affected hardware.

## Continuous integration

Four GitHub Actions workflows run on pushes and pull requests to `dev` and `master`, and by manual dispatch:

| Workflow | What it checks |
| -------- | -------------- |
| `regression_test.yml` | All 18 Linux GUIX configurations, merged coverage, and the coverage floors through the pinned ThreadX reusable workflow. |
| `studio_demo_test.yml` | GUIX Studio demo generation on Windows. |
| `studio_demo_test_compile.yml` | Compilation of GUIX Studio generated demos on Windows. |
| `studio_view_test.yml` | GUIX Studio View tests on Windows. |

`studio_msix_package.yml` builds a Studio package only when manually dispatched; it is not an automatic pull-request check. The Studio View workflow does not publish packages. Run changed display drivers on the affected hardware where available and report the result separately. Do not describe an unrun test or hardware target as verified.

## Pull request acceptance criteria

**Pull requests must target the `dev` branch.** `master` holds the latest release; see [Release model and support](#release-model-and-support) below. A pull request opened against `master` will be asked to retarget.

Before requesting a review, check your contribution against this list.

**Process**

* The branch is a feature branch based on `dev`. Never commit directly to `master` or `dev`.
* Your ECA is signed and the commit `Author` email matches your Eclipse account.
* The pull request is one logical change. Unrelated fixes belong in separate pull requests.
* Commit subject lines start with a past-tense verb, for example `Fixed the memory allocator`.
* The pull request explains what changed and why, and how you verified it.

**Code**

* GUIX C code is C99-compatible.
* It follows the coding style of the surrounding code.
* New functions and structures are documented in comments, as in existing code.
* MISRA C rules are followed as closely as practical, taking MISRA C 2004, 2012 and 2023 into account. Any deviation is explicit, names the rule being circumvented, and justifies it in a comment.
* `goto` is not used.
* Do not add new external dependencies. This is a hard rule. ThreadX is an existing library dependency for the configured GUIX build.
* When implementing an industry standard, code is not copied from an existing implementation. Existing implementations may inform your work, but you must identify those sources clearly.
* Code is written with the suite's priorities in mind: runtime speed and small code size.
* New C and assembly files, and edited C and assembly files, carry the appropriate header and attribution (see below).

**Verification**

* All applicable CI checks are green, including the Linux regression and GUIX Studio workflows on a pull request to `dev`.
* The change builds without new warnings on the reference toolchains.
* Regression tests covering the change are added or updated. The project targets 100% test coverage; a pull request that lowers coverage needs a stated reason. State which GUIX configurations or Studio validation paths ran and any relevant coverage gap.
* API or behaviour changes come with a matching documentation pull request against [rtos-docs-asciidoc](https://github.com/eclipse-threadx/rtos-docs-asciidoc).

**Security**

If you discover a security issue while working on a change, do not describe it in a public pull request. Follow [SECURITY.md](SECURITY.md) instead.

## AI-assisted contributions

**AI-assisted contributions are welcome**, provided they are attributed.

Two things are non-negotiable:

1. **Attribution.** Mark AI-assisted C and assembly files as described below, and identify the tool and model in the commit message.
2. **Human responsibility.** The human contributor submitting the pull request is responsible for the contribution - technically and legally. Signing the ECA means *you* certify the contribution's provenance. An AI tool cannot sign the ECA and cannot hold that responsibility. Review what the tool produced, understand it, and verify that it is correct and that you have the right to contribute it. "The model wrote it" is not a defence.

This is consistent with the Eclipse Foundation's [Generative AI Usage Guidelines](https://www.eclipse.org/projects/guidelines/genai/) and the [Eclipse Project Handbook](https://www.eclipse.org/projects/handbook/#genai). Please read them before submitting AI-assisted work.

### Using a coding agent

Give a coding agent this contribution guide and the build instructions for the area it changes. Review its output against the same acceptance criteria as any other contribution. In particular, check C99 compatibility for GUIX C code, surrounding style, test coverage, and the affected rendering path, widget behaviour, resource, or Studio workflow.

### Header for new files

Add this header when creating a new C or assembly (`.S`) file:

```c
/***************************************************************************
 * Copyright (c) <current_year> Eclipse ThreadX contributors
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * AI Disclosure: This file was largely AI-generated by <product> (<model_and_version>).
 * The AI-generated portions may be considered public domain (CC0-1.0)
 * and not subject to the project's licence. The human contributor has
 * reviewed and verified that the code is correct.
 *
 * SPDX-License-Identifier: MIT and CC0-1.0
 **************************************************************************/
```

Substitute the current year, product name, and model and version. If a file was written without AI assistance, omit the AI Disclosure paragraph and use `SPDX-License-Identifier: MIT`.

### Header for existing files

When editing an existing C or assembly file whose copyright is from 2025 or earlier, and which does not already mention Eclipse ThreadX contributors, add this line to the header:

```c
 * Copyright (c) <current_year> Eclipse ThreadX contributors
```

It goes *below* the older copyright. All copyright lines must stay in chronological order. For example:

```c
/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/
```

If the edit was AI-assisted and the file does not already carry an AI disclosure line, add this line just under the header:

```c
// Portions of this file were generated with AI assistance.
```

Use the comment marker already used by the file. Keep an existing disclosure line unchanged and never add a second one. If you copy an existing file to get started on a new one, treat the result as a new file: use the new-file header, do not carry the old one over.

### Commit attribution

Attribute AI assistance with an `Assisted-by` trailer in each commit that contains AI-assisted changes:

```text
Fixed the widget event handling status

A completed widget event could report an out-of-date status.

The event path now returns the status set by the completed operation.

The affected GUIX regression configurations passed all tests.

Assisted-by: <product> (<model_and_version>) <email>
```

Use the product's accepted name (`Claude Code`, `Copilot`, `Codex`, or `Gemini`) and the model string reported by the tool. Do not use `Co-Authored-By` for AI attribution. Commit subjects and pull request titles start with a past-tense verb and stay within 72 characters. The message body states the cause, fix, and test result in that order; wrap commit bodies at 88 characters. Pull request descriptions use one line per paragraph.

## Release model and support

This section summarises the project's [Release Model and Support Policy](https://github.com/eclipse-threadx/rtos-docs-asciidoc/blob/main/rtos-docs/home/modules/ROOT/pages/releases-and-support.adoc), which is the authoritative version.

### Version numbers

Eclipse ThreadX releases generally follow [Semantic Versioning](https://semver.org/). Given a version number **X.Y.Z.Bh**, for example `6.5.0.202601a`:

* **X** increases for a *milestone release*. Components currently stay at version 6.
* **Y** increases for a *feature release* adding a major feature.
* **Z** increases for a *maintenance release* of minor fixes and improvements.
* **B** is a *build number* identifying the quarter of publication - `202601` is Q1 2026.
* **h** denotes a *hotfix release*, identified by a letter. Hotfix releases are component-specific.

### Release cadence

The project adopted a predictable quarterly release model in September 2025 and has published a release every quarter since.

Quarterly releases ship new versions of every component, whether or not that component saw code changes, so that version numbers do not drift apart between components. A release shipping no code changes for a component says so in its release notes.

Urgent fixes between quarterly releases ship as *hotfix* releases. Those contain only security fixes, or fixes for serious problems that prevent building or testing applications.

### Branches

The project runs a time-based release train on trunk-based development. There are no long-term support branches and no backports to older releases.

* `master` always contains the latest quarterly or hotfix release.
* `dev` is the integration branch. Pull requests are merged there during the quarter. Track `dev` if you want to test new features and fixes ahead of a release.
* A quarterly release is performed by merging `dev` into `master`.

### Support policy

Issues in the codebase are fixed on a best-effort basis, and pull requests fixing existing or new issues are gladly accepted.

Security vulnerabilities disclosed confidentially are handled under the [Eclipse Foundation's security policy](https://www.eclipse.org/security/policy/). Once a candidate vulnerability is confirmed, the team works to deliver a fix as soon as possible, shipping it in the next quarterly release or as a hotfix depending on timing. Resolved vulnerabilities are disclosed immediately after a release containing the fix becomes available. The team aims to resolve newly confirmed vulnerabilities within three months; that period may be extended by the Project Leadership Chain together with the Eclipse Foundation Security team where appropriate.

See [SECURITY.md](SECURITY.md) for how to report a vulnerability.

### Roadmap

The project team plans its work on a [public GitHub project board](https://github.com/orgs/eclipse-threadx/projects/2/views/2).

### A note on older releases

Microsoft contributed only the Azure RTOS codebase v6.x to the Eclipse Foundation. Older ThreadX releases (v5.x and lower) sold by Express Logic were never made open source, and Microsoft has discontinued sales and support for them. Users of ThreadX 5.x and lower should upgrade to the latest release of Eclipse ThreadX as soon as possible.

## Documentation

The documentation is published at **https://threadx.io**. PDF manuals for every component, in A4 and US Letter formats, are attached as assets to each GitHub release.

### The stack

Documentation is written in [AsciiDoc](https://asciidoc.org/) and built with [Antora](https://antora.org/). PDF manuals are produced by the [Antora Assembler](https://docs.antora.org/assembler/latest/) with `asciidoctor-pdf`.

### The workflow

1. The single source of truth is [rtos-docs-asciidoc](https://github.com/eclipse-threadx/rtos-docs-asciidoc). All documentation changes are made there, as pull requests, following the same branch rules as the code repositories.
2. At release time the site is generated from that source into [rtos-docs-html](https://github.com/eclipse-threadx/rtos-docs-html), which holds the rendered HTML and exists solely for website integration. Do not edit it by hand - your changes will be overwritten by the next build.
3. The website then serves the generated content from `rtos-docs-html`.

The older `rtos-docs` repository, which held the documentation in Markdown, is archived and superseded by `rtos-docs-asciidoc`. Do not send changes there.

If your contribution adds or changes an API or a feature, open a matching pull request against `rtos-docs-asciidoc`.

## Developer resources

Information regarding source code management, builds, coding standards, and more: https://projects.eclipse.org/projects/iot.threadx/developer

The project maintains the following repositories:

**Components**

* https://github.com/eclipse-threadx/threadx
* https://github.com/eclipse-threadx/netxduo
* https://github.com/eclipse-threadx/filex
* https://github.com/eclipse-threadx/guix
* https://github.com/eclipse-threadx/usbx
* https://github.com/eclipse-threadx/levelx
* https://github.com/eclipse-threadx/tracex
* https://github.com/eclipse-threadx/zonex

**Samples and platforms**

* https://github.com/eclipse-threadx/samplex
* https://github.com/eclipse-threadx/supported-platforms

**Documentation**

* https://github.com/eclipse-threadx/rtos-docs-asciidoc
* https://github.com/eclipse-threadx/rtos-docs-html

**Community and process**

* https://github.com/eclipse-threadx/discussions
* https://github.com/eclipse-threadx/trustedx
* https://github.com/eclipse-threadx/.github

## Contact

### GitHub Discussions

https://github.com/orgs/eclipse-threadx/discussions

Q&A, feedback, and announcements. Decisions taken by the project team are documented here as well. This is usually the fastest way to reach both the team and other users.

### Main ThreadX mailing list

https://accounts.eclipse.org/mailing-list/threadx

News and updates about the ThreadX project and the ThreadX Alliance.

### Developer mailing list

https://accounts.eclipse.org/mailing-list/threadx-dev

Project team conversations. Feel free to jump in and ask non-technical questions there.

### User mailing list

https://accounts.eclipse.org/mailing-list/threadx-users

Ask your technical questions and discuss issues here.
