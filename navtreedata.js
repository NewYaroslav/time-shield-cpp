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
    [ "Рекомендации по интеграции библиотеки", "md_docs_2library-integration-guidelines-RU.html", [
      [ "Канонический язык", "md_docs_2library-integration-guidelines-RU.html#autotoc_md75", null ],
      [ "Поддерживаемые варианты интеграции", "md_docs_2library-integration-guidelines-RU.html#autotoc_md76", null ],
      [ "Несколько статических библиотек", "md_docs_2library-integration-guidelines-RU.html#autotoc_md77", null ],
      [ "Контракт NTP сервиса", "md_docs_2library-integration-guidelines-RU.html#autotoc_md78", null ],
      [ "Линковка на Windows", "md_docs_2library-integration-guidelines-RU.html#autotoc_md79", null ],
      [ "Согласованность конфигурации", "md_docs_2library-integration-guidelines-RU.html#autotoc_md80", null ],
      [ "Практический checklist", "md_docs_2library-integration-guidelines-RU.html#autotoc_md81", null ]
    ] ],
    [ "Library Integration Guidelines", "md_docs_2library-integration-guidelines.html", [
      [ "Canonical Language", "md_docs_2library-integration-guidelines.html#autotoc_md83", null ],
      [ "Supported Integration Shapes", "md_docs_2library-integration-guidelines.html#autotoc_md84", null ],
      [ "Multiple Static Libraries", "md_docs_2library-integration-guidelines.html#autotoc_md85", null ],
      [ "NTP Service Contract", "md_docs_2library-integration-guidelines.html#autotoc_md86", null ],
      [ "Windows Linking", "md_docs_2library-integration-guidelines.html#autotoc_md87", null ],
      [ "Configuration Consistency", "md_docs_2library-integration-guidelines.html#autotoc_md88", null ],
      [ "Practical Checklist", "md_docs_2library-integration-guidelines.html#autotoc_md89", null ]
    ] ],
    [ "Рекомендации по хранению singleton / service storage в header-only C++ коде", "md_docs_2singleton-storage-guidelines-RU.html", [
      [ "Канонический язык", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md94", null ],
      [ "Назначение", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md95", null ],
      [ "Базовое правило", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md96", null ],
      [ "C++17 и новее", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md97", null ],
      [ "C++11/14", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md98", null ],
      [ "Исключение для late-teardown-safe сервисов в C++11/14", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md99", null ],
      [ "Границы ownership", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md100", null ],
      [ "Именование макросов", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md101", null ],
      [ "Сценарии ошибок", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md102", null ],
      [ "Практический checklist", "md_docs_2singleton-storage-guidelines-RU.html#autotoc_md103", null ]
    ] ],
    [ "Header-only Singleton / Service Storage Guidelines", "md_docs_2singleton-storage-guidelines.html", [
      [ "Canonical Language", "md_docs_2singleton-storage-guidelines.html#autotoc_md105", null ],
      [ "Purpose", "md_docs_2singleton-storage-guidelines.html#autotoc_md106", null ],
      [ "Core Rule", "md_docs_2singleton-storage-guidelines.html#autotoc_md107", null ],
      [ "C++17 and newer", "md_docs_2singleton-storage-guidelines.html#autotoc_md108", null ],
      [ "C++11/14", "md_docs_2singleton-storage-guidelines.html#autotoc_md109", null ],
      [ "C++11/14 exception for late-teardown-safe services", "md_docs_2singleton-storage-guidelines.html#autotoc_md110", null ],
      [ "Ownership Boundaries", "md_docs_2singleton-storage-guidelines.html#autotoc_md111", null ],
      [ "Macro Naming", "md_docs_2singleton-storage-guidelines.html#autotoc_md112", null ],
      [ "Failure Modes", "md_docs_2singleton-storage-guidelines.html#autotoc_md113", null ],
      [ "Practical Checklist", "md_docs_2singleton-storage-guidelines.html#autotoc_md114", null ]
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
"classtime__shield_1_1NtpTimeServiceT.html#aae4854c5b039941c1241b3d5ba279871",
"classtime__shield_1_1detail_1_1FakeNtpRunner.html#a9241312ff92a213da9624f01f479e50b",
"group__time__constants.html#ga1519887a8b884dd4cc0b45c2ed7a98f1",
"group__time__conversions.html#ga48e95bb999e9f63ec736bde9c3047f7c",
"group__time__conversions.html#gaba3ac826a6f37a8d1609e204f69fa970",
"group__time__enums.html#gga5fa51beb76e014adfffcd028fe1804acabbc64be78b01604edf18005eb126443f",
"group__time__formatting.html#ga9c3b567d4309ef41bee267c65a4751a2",
"group__time__utils.html#gad6e2af7c1ddd04c8f8f170c329b4e492",
"namespacetime__shield.html#a7015ffc21ef1c6b62acf3fc54bb7f585",
"structtime__shield_1_1detail_1_1DaySplit.html#a6e556b9f12b3380e5df12aae10f26c03"
];

var SYNCONMSG = 'click to disable panel synchronization';
var SYNCOFFMSG = 'click to enable panel synchronization';
var LISTOFALLMEMBERS = 'List of all members';