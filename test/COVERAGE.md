# Linux regression measurements

The matrix contains 18 configurations. The baseline at `ab87d54f238141331f1295d957f68350c3ecf546`, using GCC 14 and ThreadX `44d7c95c582d415c4ad84527180b29c93c3bf664`, registered 1,849 tests. Four configurations built completely; 14 had compilation failures. CTest executed 488 tests successfully and could not execute 1,361 missing binaries. These are incomplete baseline results, not a passing regression run.

The baseline coverage was 25,581/44,475 lines and 11,874/24,432 branches for the default configuration, and 3,700/44,330 lines and 1,498/24,352 branches without UTF-8. Other configurations were not instrumented. The following durations are CTest wall times in seconds; unavailable tests are included in the registered count.

| Configuration | Baseline executed/registered | CTest seconds |
|---|---:|---:|
| default_build_coverage | 216/734 | 120.638 |
| disable_error_check_build | 216/734 | 35.942 |
| no_utf8_build_coverage | 14/135 | 28.204 |
| no_utf8_no_checking_build | 14/135 | 8.987 |
| ex_unicode_build | 1/2 | 0.010 |
| ex_unicode_no_checking_build | 1/2 | 0.011 |
| mouse_support_build | 2/56 | 8.641 |
| font_kerning_support_build | 3/3 | 0.146 |
| dynamic_bidi_text_build | 1/3 | 0.034 |
| dynamic_bidi_text_no_checking_build | 1/3 | 0.024 |
| _5_4_0_compatible_no_checking_build | 0/1 | 0.004 |
| synergy_font_support_build | 15/16 | 0.490 |
| thai_glyph_shaping_support_build | 1/1 | 0.028 |
| palette_mode_aa_text_colors_16_build | 2/2 | 0.796 |
| disable_deprecated_string_api_build | 1/1 | 1.024 |
| partial_canvas_support_build | 0/7 | 0.007 |
| partial_canvas_support_vertical_refresh_build | 0/7 | 0.006 |
| partial_canvas_support_horizontal_refresh_build | 0/7 | 0.011 |

## Complete instrumented matrix

GCC 14, CMake/Ninja and CTest passed 1,849/1,849 tests across all 18 configurations, with no retries, skipped tests, or disabled tests. The summed per-profile CTest wall time was 2,337.18 seconds. The reviewed ThreadX dependency and reusable workflow are pinned to `b37cd4a81a1cb8c2ebefc438220ab7f009e13362`; coverage uses gcovr 8.6 and gcov 14.

The merged report covers **46,115/46,888 lines (98.3514%)** and **25,635/26,675 branches (96.1012%)**. Its source-line and covered-line sets exactly equal the respective raw-trace unions. Every expected JSON, XML, HTML index, and linked source page is nonempty, and source paths are repository-relative. The harness enforces floors of **98.3% lines** and **96.0% branches**; the reusable summary action also enforces its integer line floor of 98%.

“Added covered lines” counts lines covered by a profile but not by the default profile. These contributions overlap and must not be summed. “Additional source lines” includes newly exposed but unexecuted code, which remains in the merged denominator.

| Configuration | Passed | CTest seconds | Lines covered/total | Branches covered/total | Added covered lines | Additional source lines |
|---|---:|---:|---:|---:|---:|---:|
| default_build_coverage | 734 | 202.18 | 44379/44475 | 24282/24432 | 0 | 0 |
| disable_error_check_build | 734 | 202.06 | 41317/44468 | 20373/24428 | 0 | 0 |
| no_utf8_build_coverage | 135 | 65.56 | 22107/44330 | 10364/24352 | 31 | 34 |
| no_utf8_no_checking_build | 135 | 65.23 | 20969/44323 | 9504/24348 | 31 | 34 |
| ex_unicode_build | 2 | 0.08 | 2176/44477 | 827/24434 | 1 | 2 |
| ex_unicode_no_checking_build | 2 | 0.08 | 1972/44470 | 661/24430 | 1 | 2 |
| mouse_support_build | 56 | 56.26 | 14215/45073 | 6196/24739 | 77 | 598 |
| font_kerning_support_build | 3 | 0.26 | 3374/44600 | 1239/24510 | 119 | 125 |
| dynamic_bidi_text_build | 3 | 739.61 | 5076/45760 | 2161/25246 | 1168 | 1285 |
| dynamic_bidi_text_no_checking_build | 3 | 737.16 | 4595/45753 | 1811/25242 | 1163 | 1285 |
| _5_4_0_compatible_no_checking_build | 1 | 0.18 | 5973/44507 | 2005/24458 | 37 | 39 |
| synergy_font_support_build | 16 | 3.61 | 9863/44566 | 4127/24488 | 90 | 91 |
| thai_glyph_shaping_support_build | 1 | 0.07 | 2002/44607 | 804/24561 | 121 | 132 |
| palette_mode_aa_text_colors_16_build | 2 | 5.25 | 3055/44475 | 1151/24432 | 1 | 2 |
| disable_deprecated_string_api_build | 1 | 4.26 | 2134/43892 | 812/23986 | 1 | 8 |
| partial_canvas_support_build | 7 | 76.47 | 10098/44545 | 4364/24478 | 70 | 77 |
| partial_canvas_support_vertical_refresh_build | 7 | 76.02 | 10130/44547 | 4392/24482 | 72 | 79 |
| partial_canvas_support_horizontal_refresh_build | 7 | 102.84 | 10168/44548 | 4403/24482 | 73 | 80 |

## Remaining coverage

The complete denominator leaves **773 lines and 1,040 branches** uncovered. The following counts retain all source code, including optional feature paths and defensive checks. They are gaps to investigate, not claims of unreachable code. Source-level HTML and the merged JSON identify each line and branch in the downloadable CI artifacts.

| Area | Uncovered lines | Uncovered branches |
|---|---:|---:|
| Animation drag, landing and update decisions | 8 | 20 |
| Bidi allocation, shaping, explicit levels and reordering | 101 | 146 |
| Binary resource and image-decoder error paths | 20 | 43 |
| Canvas refresh, driver setup, blending and drawing | 108 | 112 |
| Font formats, glyph shaping and drawing | 20 | 73 |
| Mouse cursor definition, visibility, capture and restore | 453 | 281 |
| Widget, text-editing, scrolling and system decisions | 63 | 365 |

The mouse configuration covers 77 lines beyond the default but leaves most cursor capture/restore and checked mouse APIs unexercised. Dedicated cursor and framebuffer assertions are needed for those paths. Bidi and font follow-up work needs allocation-failure cases, shaping boundaries and text-editing decisions; canvas and driver follow-up needs refresh-direction boundaries and rotated blending. Widget, animation, binary-resource and decoder gaps require focused state and invalid-input cases. None are excluded to meet the floors.

New assertions exercise view-pool exhaustion, checked cursor-height rejection, the core cursor-height reset, and invalid checkbox validation. Checkout failure, dependency-build failure, a failing CTest case, and missing/empty coverage input probes all returned nonzero. A failing test retained partial coverage; missing or empty traces could not produce a merged report. Report tests also verify stale-artifact removal, partial selections, exact unions, repository-relative paths, all required formats, and both coverage floors.
