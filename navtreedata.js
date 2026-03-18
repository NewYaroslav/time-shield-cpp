/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Time Shield Library", "index.html", [
    [ "Introduction", "index.html#intro_sec", null ],
    [ "Why Time Shield?", "index.html#motivation_sec", null ],
    [ "Features", "index.html#features_sec", null ],
    [ "Configuration", "index.html#config_sec", null ],
    [ "API Invariants", "index.html#invariants_sec", null ],
    [ "Examples", "index.html#examples_sec", null ],
    [ "NTP client, pool, and time service", "index.html#ntp_sec", [
      [ "OA date and astronomy helpers", "index.html#oa_and_astronomy", null ],
      [ "Workday helpers", "index.html#workday_helpers", null ]
    ] ],
    [ "Installation", "index.html#install_sec", [
      [ "Install and <span class=\"tt\">find_package</span>", "index.html#install_pkg", null ],
      [ "Git submodule with <span class=\"tt\">add_subdirectory</span>", "index.html#install_submodule", null ],
      [ "vcpkg overlay", "index.html#install_vcpkg", null ]
    ] ],
    [ "Tested Platforms", "index.html#tested_sec", null ],
    [ "Online Documentation", "index.html#docs_sec", null ],
    [ "Repository", "index.html#repo_sec", null ],
    [ "License", "index.html#license_sec", null ],
    [ "Рекомендации по владению заголовками и файлами реализации", "md_docs_2header-implementation-guidelines-RU.html", [
      [ "Канонический язык", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md35", null ],
      [ "Назначение", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md36", null ],
      [ "Роли файлов", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md37", [
        [ "<span class=\"tt\">.hpp</span>", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md38", null ],
        [ "<span class=\"tt\">.tpp</span>", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md39", null ],
        [ "<span class=\"tt\">.ipp</span>", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md40", null ]
      ] ],
      [ "Модель владения", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md41", [
        [ "Реализация, всегда подключаемая из заголовка", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md42", null ],
        [ "Реализация с явным ownership", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md43", null ]
      ] ],
      [ "Aggregate entry header и leaf header", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md44", [
        [ "Aggregate-first подсистема", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md45", null ],
        [ "Standalone-leaf-first подсистема", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md46", null ]
      ] ],
      [ "Размещение зависимостей", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md47", [
        [ "Общие STL и доменные include'ы", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md48", null ],
        [ "Implementation-only helper'ы", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md49", null ]
      ] ],
      [ "Forward declarations", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md50", null ],
      [ "Границы private / vendor реализации", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md51", null ],
      [ "Как тестировать include-контракт", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md52", null ],
      [ "Практический checklist", "md_docs_2header-implementation-guidelines-RU.html#autotoc_md53", null ]
    ] ],
    [ "Header / Implementation Ownership Guidelines", "md_docs_2header-implementation-guidelines.html", [
      [ "Canonical Language", "md_docs_2header-implementation-guidelines.html#autotoc_md55", null ],
      [ "Purpose", "md_docs_2header-implementation-guidelines.html#autotoc_md56", null ],
      [ "File Roles", "md_docs_2header-implementation-guidelines.html#autotoc_md57", [
        [ "<span class=\"tt\">.hpp</span>", "md_docs_2header-implementation-guidelines.html#autotoc_md58", null ],
        [ "<span class=\"tt\">.tpp</span>", "md_docs_2header-implementation-guidelines.html#autotoc_md59", null ],
        [ "<span class=\"tt\">.ipp</span>", "md_docs_2header-implementation-guidelines.html#autotoc_md60", null ]
      ] ],
      [ "Ownership Model", "md_docs_2header-implementation-guidelines.html#autotoc_md61", [
        [ "Always-included implementation", "md_docs_2header-implementation-guidelines.html#autotoc_md62", null ],
        [ "Ownership-controlled implementation", "md_docs_2header-implementation-guidelines.html#autotoc_md63", null ]
      ] ],
      [ "Aggregate Entry Header vs Leaf Header", "md_docs_2header-implementation-guidelines.html#autotoc_md64", [
        [ "Aggregate-first subsystem", "md_docs_2header-implementation-guidelines.html#autotoc_md65", null ],
        [ "Standalone-leaf-first subsystem", "md_docs_2header-implementation-guidelines.html#autotoc_md66", null ]
      ] ],
      [ "Dependency Placement", "md_docs_2header-implementation-guidelines.html#autotoc_md67", [
        [ "Shared STL and domain includes", "md_docs_2header-implementation-guidelines.html#autotoc_md68", null ],
        [ "Implementation-only helpers", "md_docs_2header-implementation-guidelines.html#autotoc_md69", null ]
      ] ],
      [ "Forward Declarations", "md_docs_2header-implementation-guidelines.html#autotoc_md70", null ],
      [ "Private / Vendor Boundaries", "md_docs_2header-implementation-guidelines.html#autotoc_md71", null ],
      [ "Test Contract Guidance", "md_docs_2header-implementation-guidelines.html#autotoc_md72", null ],
      [ "Practical Checklist", "md_docs_2header-implementation-guidelines.html#autotoc_md73", null ]
    ] ],
    [ "Рекомендации по хранению singleton / service storage в header-only C++ коде", "md_docs_2singleton-storage-guidelines-RU.html", [
      [ "Канонический язык", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md78", null ],
      [ "Назначение", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md79", null ],
      [ "Базовое правило", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md80", null ],
      [ "C++17 и новее", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md81", null ],
      [ "C++11/14", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md82", null ],
      [ "Границы ownership", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md83", null ],
      [ "Именование макросов", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md84", null ],
      [ "Сценарии ошибок", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md85", null ],
      [ "Практический checklist", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md86", null ]
    ] ],
    [ "Header-only Singleton / Service Storage Guidelines", "md_docs_2singleton-storage-guidelines.html", [
      [ "Canonical Language", "md_docs_2singleton-storage-guidelines.html#autotoc_md88", null ],
      [ "Purpose", "md_docs_2singleton-storage-guidelines.html#autotoc_md89", null ],
      [ "Core Rule", "md_docs_2singleton-storage-guidelines.html#autotoc_md90", null ],
      [ "C++17 and newer", "md_docs_2singleton-storage-guidelines.html#autotoc_md91", null ],
      [ "C++11/14", "md_docs_2singleton-storage-guidelines.html#autotoc_md92", null ],
      [ "Ownership Boundaries", "md_docs_2singleton-storage-guidelines.html#autotoc_md93", null ],
      [ "Macro Naming", "md_docs_2singleton-storage-guidelines.html#autotoc_md94", null ],
      [ "Failure Modes", "md_docs_2singleton-storage-guidelines.html#autotoc_md95", null ],
      [ "Practical Checklist", "md_docs_2singleton-storage-guidelines.html#autotoc_md96", null ]
    ] ],
    [ "Topics", "topics.html", "topics" ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", "namespacemembers_dup" ],
        [ "Functions", "namespacemembers_func.html", "namespacemembers_func" ],
        [ "Variables", "namespacemembers_vars.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ],
        [ "Enumerations", "namespacemembers_enum.html", null ],
        [ "Enumerator", "namespacemembers_eval.html", null ]
      ] ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", "functions_vars" ],
        [ "Typedefs", "functions_type.html", null ],
        [ "Enumerations", "functions_enum.html", null ],
        [ "Related Symbols", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"CpuTickTimer_8hpp.html",
"classtime__shield_1_1DateTime.html#afd3ae5c2b4a50d3997b139bd152c0886",
"classtime__shield_1_1Timer.html#a27d7a174235fbd7e4d4f9a9709a22c95",
"classtime__shield_1_1detail_1_1FakeNtpRunner.html#ad4af303ea14f5c1655e23d8f0fbd0ef6",
"group__time__constants.html#ga77f5cd93d7f33492f325a240c113a579",
"group__time__conversions.html#ga572de0047cbf71f2e69b7eca85520d83",
"group__time__conversions.html#gacd1601d44252ccf331a6c11449e71bac",
"group__time__enums.html#ggac2ffd79e06ab94cf1bb50fec432c68a8a939f4fa15cc31f9b2ec0fc54fed08595",
"group__time__structures.html#ga361b7f574fa40955ec4a6b7bfac97ba3",
"namespacemembers_f.html",
"structtime__shield_1_1astronomy_1_1MoonPhaseResult.html#a7b675de383072b1bedebe3bf0159bbad"
];

var SYNCONMSG = 'click to disable panel synchronization';
var SYNCOFFMSG = 'click to enable panel synchronization';
var LISTOFALLMEMBERS = 'List of all members';