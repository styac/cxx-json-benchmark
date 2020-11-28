#include <algorithm>
#include <cfloat>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "test.h"
#include "timer.h"
#include "resultfilename.h"
#include "rapidjson/internal/ieee754.h"

#include "reports.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#ifndef TEST_DATA_PATH
#error "TEST_DATA_PATH not defined"
#endif

#ifndef RESULT_PATH
#error "RESULT_PATH not defined"
#endif

#ifndef REPORT_PATH
#error "REPORT_PATH not defined"
#endif

#ifndef OBJ_PATH
#error "OBJ_PATH not defined"
#endif

constexpr unsigned cTrialCount = 10;

struct str {
    str()
    : m_data(nullptr)
    , m_size(0)
    {}

    auto size() const
    {
        return m_size;
    }

    auto data() const
    {
        return m_data;
    }

    void freedata()
    {
        free(m_data);
        m_data = nullptr;
        m_size = 0;
    }
    char *m_data;
    size_t m_size;
};

static str ReadJSON(const std::filesystem::path& p) {
    std::ifstream in(p);
    str res;
    if (in) {
        in.seekg(0, std::ios::end);
        std::streamsize len = in.tellg();
        in.seekg(0);
        res.m_data = (char*)malloc(len + 1);
        if( res.m_data == nullptr ) {
            return res;
        }
        in.read(res.m_data, len);
        res.m_data[len]='\0';
        res.m_size = len;
    }
    return res;
}


struct TestJson {
    TestJson()
        : m_filename()
        , m_json()
        , m_stat()
        , m_statUTF16()
    {}

    std::string m_filename;
    str m_json;
    Stat m_stat;           // Reference statistics
    Stat m_statUTF16;      // Reference statistics
};

typedef std::vector<TestJson> TestJsonList;

//static void PrintStat(const Stat& stat) {
//    printf("objectCount:  %10u\n", (unsigned)stat.objectCount);
//    printf("arrayCount:   %10u\n", (unsigned)stat.arrayCount);
//    printf("numberCount:  %10u\n", (unsigned)stat.numberCount);
//    printf("stringCount:  %10u\n", (unsigned)stat.stringCount);
//    printf("trueCount:    %10u\n", (unsigned)stat.trueCount);
//    printf("falseCount:   %10u\n", (unsigned)stat.falseCount);
//    printf("nullCount:    %10u\n", (unsigned)stat.nullCount);
//    printf("memberCount:  %10u\n", (unsigned)stat.memberCount);
//    printf("elementCount: %10u\n", (unsigned)stat.elementCount);
//    printf("stringLength: %10u\n", (unsigned)stat.stringLength);
//}

static void makeValidFilename(char *filename) {
    while (*filename) {
        switch (*filename) {
            case '/':
                *filename = '_';
                break;
        }
        filename++;
    }
}

static void EscapeString(FILE* fp, const char *s, size_t length) {
    fputc('"', fp);
    for (size_t j = 0; j < length; j++)
        if (s[j] != 0)
            fputc(s[j], fp);
        else
            fputs("\\0", fp);
    fputc('"', fp);
}

static bool ReadFiles(TestJsonList& testJsons) {
    // Currently use RapidJSON to generate reference statistics
    TestList& tests = TestManager::Instance().GetTests();
    const TestBase* referenceTest = 0;
    for (TestList::iterator itr = tests.begin(); itr != tests.end(); ++itr) {
        if (strcmp((*itr)->GetName(), "RapidJSON (C++)") == 0) {
            referenceTest = *itr;
           break;
        }
    }

    if (!referenceTest) {
        printf("Error: Cannot find RapidJSON as refernce test. Not reading any files.\n");
        return false;
    }

    std::filesystem::directory_iterator end_it;
    std::filesystem::path p(TEST_DATA_PATH );
    for (std::filesystem::directory_iterator it(p); it != end_it; ++it) {
        if( ! is_regular_file(it->path()) || it->path().extension() != ".json" ) {
            continue;
        }
        {
            auto json = ReadJSON(it->path());
            if (json.data() == nullptr )
                continue;
            TestJson t;
            t.m_filename = it->path().filename().stem();
            t.m_json = json;

            // Generate reference statistics
#if TEST_SAXSTATISTICS
            if (!referenceTest->SaxStatistics(t.m_json.data(), t.m_json.size(), &t.m_stat))
                printf("Failed to generate reference statistics\n");
#endif

#if TEST_SAXSTATISTICSUTF16
            if (!referenceTest->SaxStatisticsUTF16(t.json, t.length, &t.m_statUTF16))
                printf("Failed to generate reference UTF16 statistics\n");
#endif
            testJsons.push_back(t);
        }
    }
    printf("\n");
    return true;
}

static void FreeFiles(TestJsonList& testJsons) {
    for (TestJsonList::iterator itr = testJsons.begin(); itr != testJsons.end(); ++itr) {
        itr->m_json.freedata();
    }
}

// Normally use this at the end of MEMORYSTAT_SCOPE()
#if USE_MEMORYSTAT
#define MEMORYSTAT_CHECKMEMORYLEAK(t,p) ReportBase::get_instance().print_memory_leaks(t, p)
#else
#define MEMORYSTAT_CHECKMEMORYLEAK(t,p)
#endif

static void Verify(const TestBase& test, const TestJsonList& testJsons) {
    (void)testJsons;

    printf(" ***** Verifying %s ... ", test.GetName());
    fflush(stdout);

    bool failed = false;

#if TEST_PARSE && TEST_STATISTICS
    for (TestJsonList::const_iterator itr = testJsons.begin(); itr != testJsons.end(); ++itr) {
        MEMORYSTAT_SCOPE();

        test.SetUp();
        ParseResultBase* dom1 = test.Parse(itr->m_json.data(), itr->m_json.size());
        if (!dom1) {
            printf("\nFailed to parse '%s'\n", itr->m_filename.data());
            failed = true;
            test.TearDown();
            continue;
        }

        Stat stat1;
        if (!test.Statistics(dom1, &stat1)) {
            printf("Not support Statistics\n");
            delete dom1;
            test.TearDown();
            continue;
        }

        StringResultBase* json1 = test.Stringify(dom1);
        delete dom1;

        if (!json1) {
            // Some libraries may not support stringify, but still check statistics
            if (memcmp(&stat1, &itr->m_stat, sizeof(Stat)) != 0 &&
                memcmp(&stat1, &itr->m_statUTF16, sizeof(Stat)) != 0)
            {
//                printf("\nStatistics of '%s' is different from reference.\n\n", itr->m_filename.data());
//                printf("Reference\n---------\n");
//                PrintStat(itr->m_stat);
//                printf("\nStat 1\n--------\n");
//                PrintStat(stat1);
//                printf("\n");
                failed = true;
                ReportBase::get_instance().print_statistics( "Stringify1-", test.GetName(), itr->m_filename, stat1, itr->m_stat);
            }
            test.TearDown();
            continue;
        }

        ParseResultBase* dom2 = test.Parse(json1->c_str(), strlen(json1->c_str()));
        if (!dom2) {
            printf("\nFailed to parse '%s' 2nd time\n", itr->m_filename.data());
            failed = true;

            // Write out json1 for diagnosis
            char filename[FILENAME_MAX];
            sprintf(filename, "%s_%s", test.GetName(), itr->m_filename.data());
            makeValidFilename(filename);
            FILE* fp = fopen(filename, "wb");
            fwrite(json1->c_str(), strlen(json1->c_str()), 1, fp);
            fclose(fp);

            delete json1;
            test.TearDown();
            continue;
        }

        Stat stat2;
        test.Statistics(dom2, &stat2);

        StringResultBase* json2 = test.Stringify(dom2);
        delete dom2;

        Stat* statProblem = 0;
        int statProblemWhich = 0;
        if (memcmp(&stat1, &itr->m_stat,      sizeof(Stat)) != 0 &&
            memcmp(&stat1, &itr->m_statUTF16, sizeof(Stat)) != 0)
        {
            statProblem = &stat1;
            statProblemWhich = 1;
        }
        else if (memcmp(&stat2, &itr->m_stat,      sizeof(Stat)) != 0 &&
                 memcmp(&stat2, &itr->m_statUTF16, sizeof(Stat)) != 0)
        {
            statProblem = &stat2;
            statProblemWhich = 2;
        }

        if (statProblem != 0) {
//            printf("\nStatistics of '%s' is different from reference.\n\n", itr->m_filename.data());
//            printf("Reference\n---------\n");
//            PrintStat(itr->m_stat);
//            printf("\nStat #%d\n--------\n", statProblemWhich);
//            PrintStat(*statProblem);
//            printf("\n");
            ReportBase::get_instance().print_statistics( "Stringify2-", test.GetName(), itr->m_filename, *statProblem, itr->m_stat);

            // Write out json1 for diagnosis
            char filename[FILENAME_MAX];
            sprintf(filename, "%s_%s", test.GetName(), itr->m_filename.data());
            makeValidFilename(filename);
            FILE* fp = fopen(filename, "wb");
            fwrite(json1->c_str(), strlen(json1->c_str()), 1, fp);
            fclose(fp);

            failed = true;
        }

        delete json1;
        delete json2;
        test.TearDown();

        MEMORYSTAT_CHECKMEMORYLEAK(test.GetName(),"verify");
    }
#endif

#if TEST_SAXSTATISTICS
    // Verify SaxStatistics()
    for (TestJsonList::const_iterator itr = testJsons.begin(); itr != testJsons.end(); ++itr) {
        MEMORYSTAT_SCOPE();
        Stat stat1;
        if (test.SaxStatistics(itr->m_json.data(), itr->m_json.size(), &stat1)) {
            if (memcmp(&stat1, &itr->m_stat, sizeof(Stat)) != 0 &&
                memcmp(&stat1, &itr->m_statUTF16, sizeof(Stat)) != 0)
            {
//                printf("\nSaxStatistics of '%s' is different from reference.\n\n", itr->m_filename.data());
//                printf("Reference\n---------\n");
//                PrintStat(itr->m_stat);
//                printf("\nStat #%d\n--------\n", 1);
//                PrintStat(stat1);
//                printf("\n");
                ReportBase::get_instance().print_statistics( "SAX-", test.GetName(), itr->m_filename, stat1, itr->m_stat);

            }
        }
    }
#endif

    printf(failed ? " **** Failed\n" : "OK\n");
}

static void VerifyAll(const TestJsonList& testJsons) {
    TestList& tests = TestManager::Instance().GetTests();
    for (TestList::iterator itr = tests.begin(); itr != tests.end(); ++itr)
        Verify(**itr, testJsons);
    printf("\n");
}

#if USE_MEMORYSTAT
#define BENCH_MEMORYSTAT_INIT()             MemoryStat memoryStat = MemoryStat()
#define BENCH_MEMORYSTAT_ITERATION(trial)   if (trial == 0) memoryStat = Memory::Instance().GetStat()
#ifdef _MSC_VER
#define BENCH_MEMORYSTAT_OUTPUT(fp)         fprintf(fp, ",%Iu,%Iu,%Iu", memoryStat.currentSize, memoryStat.peakSize, memoryStat.mallocCount + memoryStat.reallocCount)
#else
#define BENCH_MEMORYSTAT_OUTPUT(fp)         fprintf(fp, ",%zu,%zu,%zu", memoryStat.currentSize, memoryStat.peakSize, memoryStat.mallocCount + memoryStat.reallocCount)
#endif
#else
#define BENCH_MEMORYSTAT_INIT()
#define BENCH_MEMORYSTAT_ITERATION(trial)
#define BENCH_MEMORYSTAT_OUTPUT(fp)
#endif

#if TEST_PARSE
static void BenchParse(const TestBase& test, const TestJsonList& testJsons, FILE *fp) {
    MEMORYSTAT_SCOPE();
    for (TestJsonList::const_iterator itr = testJsons.begin(); itr != testJsons.end(); ++itr) {
        printf("%15s %-20s ... ", "Parse", itr->m_filename.data());
        fflush(stdout);

        double minDuration = DBL_MAX;

        BENCH_MEMORYSTAT_INIT();
        bool supported = true;
        for (unsigned trial = 0; trial < cTrialCount; trial++) {
            test.SetUp();
            Timer timer;
            ParseResultBase* dom;
            {
                MEMORYSTAT_SCOPE();

                timer.start();
                dom = test.Parse(itr->m_json.data(), itr->m_json.size());
                timer.stop();

                BENCH_MEMORYSTAT_ITERATION(trial);
            }

            delete dom;

            if (!dom) {
                supported = false;
                test.TearDown();
                break;
            }

            double duration = timer.diff_time();
            minDuration = std::min(minDuration, duration);
            test.TearDown();
        }

        if (!supported) {
            printf("Not support\n");
        } else {
            double throughput = itr->m_json.size() / (1024.0 * 1024.0) / (minDuration * 0.001);
            printf("%6.3f ms  %3.3f MB/s\n", minDuration, throughput);

            ReportBase::get_instance().add_performance_statistics("1. Parse", test.GetName(), itr->m_filename, minDuration,
                memoryStat.currentSize, memoryStat.peakSize, memoryStat.mallocCount + memoryStat.reallocCount);

        }
    }
    MEMORYSTAT_CHECKMEMORYLEAK(test.GetName(),"bench parse");
}
#else
static void BenchParse(const TestBase&, const TestJsonList&, FILE *) {
}
#endif

#if TEST_PARSE && TEST_STRINGIFY
static void BenchStringify(const TestBase& test, const TestJsonList& testJsons, FILE *fp) {
    MEMORYSTAT_SCOPE();
    for (TestJsonList::const_iterator itr = testJsons.begin(); itr != testJsons.end(); ++itr) {
        printf("%15s %-20s ... ", "Stringify", itr->m_filename.data());
        fflush(stdout);

        test.SetUp();
        double minDuration = DBL_MAX;
        ParseResultBase* dom = test.Parse(itr->m_json.data(), itr->m_json.size());

        BENCH_MEMORYSTAT_INIT();
        bool supported = true;
        for (unsigned trial = 0; trial < cTrialCount; trial++) {
            Timer timer;
            StringResultBase* json;
            {
                MEMORYSTAT_SCOPE();

                timer.start();
                json = test.Stringify(dom);
                timer.stop();

                BENCH_MEMORYSTAT_ITERATION(trial);
            }

            delete json;

            if (!json) {
                supported = false;
                break;
            }

            double duration = timer.diff_time();
            minDuration = std::min(minDuration, duration);
        }

        delete dom;
        test.TearDown();

        if (!supported) {
            printf("Not support\n");
        } else {
            double throughput = itr->m_json.size() / (1024.0 * 1024.0) / (minDuration * 0.001);
            printf("%6.3f ms  %3.3f MB/s\n", minDuration, throughput);

            ReportBase::get_instance().add_performance_statistics("2. Stringify", test.GetName(), itr->m_filename, minDuration,
                memoryStat.currentSize, memoryStat.peakSize, memoryStat.mallocCount + memoryStat.reallocCount);

        }
    }
    MEMORYSTAT_CHECKMEMORYLEAK(test.GetName(),"bench stringify");
}
#else
static void BenchStringify(const TestBase&, const TestJsonList&, FILE *) {
}
#endif

#if TEST_PARSE && TEST_PRETTIFY
static void BenchPrettify(const TestBase& test, const TestJsonList& testJsons, FILE *fp) {
    MEMORYSTAT_SCOPE();
    for (TestJsonList::const_iterator itr = testJsons.begin(); itr != testJsons.end(); ++itr) {
        printf("%15s %-20s ... ", "Prettify", itr->m_filename.data());
        fflush(stdout);

        test.SetUp();
        double minDuration = DBL_MAX;
        ParseResultBase* dom = test.Parse(itr->m_json.data(), itr->m_json.size());

        BENCH_MEMORYSTAT_INIT();
        bool supported = true;
        for (unsigned trial = 0; trial < cTrialCount; trial++) {
            Timer timer;
            StringResultBase* json;
            {
                MEMORYSTAT_SCOPE();

                timer.start();
                json = test.Prettify(dom);
                timer.stop();

                BENCH_MEMORYSTAT_ITERATION(trial);
            }

            delete json;

            if (!json) {
                supported = false;
                break;
            }

            double duration = timer.diff_time();
            minDuration = std::min(minDuration, duration);
        }

        delete dom;
        test.TearDown();

        if (!supported) {
            printf("Not support\n");
        } else {
            double throughput = itr->m_json.size() / (1024.0 * 1024.0) / (minDuration * 0.001);
            printf("%6.3f ms  %3.3f MB/s\n", minDuration, throughput);
            fprintf(fp, "3. Prettify,%s,%s,%f", test.GetName(), itr->m_filename.data(), minDuration);
            BENCH_MEMORYSTAT_OUTPUT(fp);
            fprintf(fp, ",0");  // Code size
            fputc('\n', fp);
            ReportBase::get_instance().add_performance_statistics("3. Prettify", test.GetName(), itr->m_filename, minDuration,
                memoryStat.currentSize, memoryStat.peakSize, memoryStat.mallocCount + memoryStat.reallocCount);

        }
    }
    MEMORYSTAT_CHECKMEMORYLEAK(test.GetName(),"bench prettify");
}
#else
static void BenchPrettify(const TestBase&, const TestJsonList&, FILE *) {
}
#endif

#if TEST_PARSE && TEST_STATISTICS
static void BenchStatistics(const TestBase& test, const TestJsonList& testJsons, FILE *fp) {
    MEMORYSTAT_SCOPE();
    for (TestJsonList::const_iterator itr = testJsons.begin(); itr != testJsons.end(); ++itr) {
        printf("%15s %-20s ... ", "Statistics", itr->m_filename.data());
        fflush(stdout);

        test.SetUp();
        double minDuration = DBL_MAX;
        ParseResultBase* dom = test.Parse(itr->m_json.data(), itr->m_json.size());

        BENCH_MEMORYSTAT_INIT();
        bool supported = true;
        for (unsigned trial = 0; trial < cTrialCount; trial++) {
            Timer timer;
            {
                MEMORYSTAT_SCOPE();

                timer.start();
                Stat stat;
                supported = test.Statistics(dom, &stat);
                timer.stop();

                BENCH_MEMORYSTAT_ITERATION(trial);
            }

            if (!supported)
                break;

            double duration = timer.diff_time();
            minDuration = std::min(minDuration, duration);
        }

        delete dom;
        test.TearDown();

        if (!supported) {
            printf("Not support\n");
        } else {
            double throughput = itr->m_json.size() / (1024.0 * 1024.0) / (minDuration * 0.001);
            printf("%6.3f ms  %3.3f MB/s\n", minDuration, throughput);

            ReportBase::get_instance().add_performance_statistics("4. Statistics", test.GetName(), itr->m_filename, minDuration,
                memoryStat.currentSize, memoryStat.peakSize, memoryStat.mallocCount + memoryStat.reallocCount);

        }
    }
    MEMORYSTAT_CHECKMEMORYLEAK(test.GetName(),"bench stat");
}
#else
static void BenchStatistics(const TestBase&, const TestJsonList&, FILE *) {
}
#endif

#if TEST_SAXROUNDTRIP
static void BenchSaxRoundtrip(const TestBase& test, const TestJsonList& testJsons, FILE *fp) {
    MEMORYSTAT_SCOPE();
    for (TestJsonList::const_iterator itr = testJsons.begin(); itr != testJsons.end(); ++itr) {
        printf("%15s %-20s ... ", "SaxRoundtrip", itr->m_filename.data());
        fflush(stdout);

        double minDuration = DBL_MAX;

        BENCH_MEMORYSTAT_INIT();
        bool supported = true;
        for (unsigned trial = 0; trial < cTrialCount; trial++) {
            test.SetUp();
            Timer timer;
            StringResultBase* json;
            {
                MEMORYSTAT_SCOPE();

                timer.start();
                json = test.SaxRoundtrip(itr->m_json.data(), itr->m_json.size());
                timer.stop();

                BENCH_MEMORYSTAT_ITERATION(trial);
            }

            delete json;

            if (!json) {
                supported = false;
                test.TearDown();
                break;
            }

            double duration = timer.diff_time();
            minDuration = std::min(minDuration, duration);
            test.TearDown();
        }

        if (!supported) {
            printf("Not support\n");
        } else {
            double throughput = itr->m_json.size() / (1024.0 * 1024.0) / (minDuration * 0.001);
            printf("%6.3f ms  %3.3f MB/s\n", minDuration, throughput);

            ReportBase::get_instance().add_performance_statistics("5. Sax Round-trip", test.GetName(), itr->m_filename, minDuration,
                memoryStat.currentSize, memoryStat.peakSize, memoryStat.mallocCount + memoryStat.reallocCount);

        }
    }
    MEMORYSTAT_CHECKMEMORYLEAK(test.GetName(),"bench sax roundtrip");
}
#else
static void BenchSaxRoundtrip(const TestBase&, const TestJsonList&, FILE*) {
}
#endif

#if TEST_SAXSTATISTICS
static void BenchSaxStatistics(const TestBase& test, const TestJsonList& testJsons, FILE *fp) {
    MEMORYSTAT_SCOPE();
    for (TestJsonList::const_iterator itr = testJsons.begin(); itr != testJsons.end(); ++itr) {
        printf("%15s %-20s ... ", "Sax Statistics", itr->m_filename.data());
        fflush(stdout);

        double minDuration = DBL_MAX;

        BENCH_MEMORYSTAT_INIT();
        bool supported = true;
        for (unsigned trial = 0; trial < cTrialCount; trial++) {
            test.SetUp();
            Timer timer;
            {
                MEMORYSTAT_SCOPE();

                timer.start();
                Stat stat;
                supported = test.SaxStatistics(itr->m_json.data(), itr->m_json.size(), &stat);
                timer.stop();

                BENCH_MEMORYSTAT_ITERATION(trial);
            }

            if (!supported) {
                test.TearDown();
                break;
            }

            double duration = timer.diff_time();
            minDuration = std::min(minDuration, duration);
            test.TearDown();
        }

        if (!supported) {
            printf("Not support\n");
        } else {
            double throughput = itr->m_json.size() / (1024.0 * 1024.0) / (minDuration * 0.001);
            printf("%6.3f ms  %3.3f MB/s\n", minDuration, throughput);

            ReportBase::get_instance().add_performance_statistics("6. SaxStatistics", test.GetName(), itr->m_filename, minDuration,
                memoryStat.currentSize, memoryStat.peakSize, memoryStat.mallocCount + memoryStat.reallocCount);

        }
    }
    MEMORYSTAT_CHECKMEMORYLEAK(test.GetName(),"bench sax stat");
}
#else
static void BenchSaxStatistics(const TestBase&, const TestJsonList&, FILE*) {
}
#endif

static void BenchPerformance(const TestBase& test, const TestJsonList& testJsons, FILE *fp) {
    printf("Benchmarking Performance of %s\n", test.GetName());

    BenchParse(test, testJsons, fp);
    BenchStringify(test, testJsons, fp);
    BenchPrettify(test, testJsons, fp);
    BenchStatistics(test, testJsons, fp);
    BenchSaxRoundtrip(test, testJsons, fp);
    BenchSaxStatistics(test, testJsons, fp);
    printf("\n");
}

static void BenchAllPerformance(const TestJsonList& testJsons) {
    FILE *fp = fopen(RESULT_PATH "performance_" RESULT_FILENAME, "w");

    fputs("Type,Library,Filename,Time (ms)", fp);
#if USE_MEMORYSTAT
    fputs(",Memory (byte),MemoryPeak (byte),AllocCount", fp);
#endif
    fputs(",FileSize (byte)\n", fp);

    TestList& tests = TestManager::Instance().GetTests();
    for (TestList::iterator itr = tests.begin(); itr != tests.end(); ++itr)
        BenchPerformance(**itr, testJsons, fp);

    fclose(fp);
}

#if TEST_CONFORMANCE

static void BenchConformance(const TestBase& test, FILE* fp) {
    std::filesystem::directory_iterator end_it;
    printf("Benchmarking Conformance of %s\n", test.GetName());

    // Output markdown
    FILE* md;
    char testname[FILENAME_MAX];
    strcpy(testname, test.GetName());
    makeValidFilename(testname);

    char mdFilename[FILENAME_MAX];
    sprintf(mdFilename, RESULT_PATH "conformance_%s.md", testname);
    md = fopen(mdFilename, "w");
    if (md)
        fprintf(md, "# Conformance of %s\n\n", test.GetName());

#if TEST_PARSE
    // Parse Validation, JsonChecker pass

    if (md)
        fprintf(md, "## 1. Parse Validation\n\n");


    int parseValidationCorrect = 0, parseValidationTotal = 0;

    std::filesystem::path p_pass(TEST_DATA_PATH "jsonchecker_pass/");
    for (std::filesystem::directory_iterator it(p_pass); it != end_it; ++it) {
        if( ! is_regular_file(it->path()) || it->path().extension() != ".json" ) {
            continue;
        }
        MEMORYSTAT_SCOPE();
        auto json = ReadJSON(it->path());
        if (json.data() != nullptr ) {

            test.SetUp();
            ParseResultBase* pr = test.Parse(json.data(), json.size());
            bool result = pr != 0;
            delete pr;
            auto const& n = it->path().filename().stem().string();
            ReportBase::get_instance().add_conformance_statistics("1. Parse Validation", test.GetName(), n, result);
            test.TearDown();
            if (!result) {
                if (md)
                    fprintf(md, "* `%s` is valid but was mistakenly deemed invalid.\n~~~js\n%s\n~~~\n\n", n.data(), json.data());
            } else {
                parseValidationCorrect++;
            }
            parseValidationTotal++;
            json.freedata();
        }
        MEMORYSTAT_CHECKMEMORYLEAK(test.GetName(),"check pass");
    }

    // Parse Validation, JsonChecker fail
    std::filesystem::path p_fail(TEST_DATA_PATH "jsonchecker_fail/");
    for (std::filesystem::directory_iterator it(p_fail); it != end_it; ++it) {
        if( ! is_regular_file(it->path()) || it->path().extension() != ".json" ) {
            continue;
        }
        MEMORYSTAT_SCOPE();
        auto json = ReadJSON(it->path());
        if (json.data() != nullptr ) {
            test.SetUp();
            ParseResultBase* pr = test.Parse(json.data(), json.size());
            bool result = pr == 0;
            delete pr;
            const auto& n = it->path().filename().stem().string();
            ReportBase::get_instance().add_conformance_statistics("1. Parse Validation", test.GetName(), n, result);

            test.TearDown();
            if (!result) {
                if (md)
                    fprintf(md, "* `%s` is valid but was mistakenly deemed invalid.\n~~~js\n%s\n~~~\n\n", n.data(), json.data());
            } else {
                parseValidationCorrect++;
            }
            parseValidationTotal++;
            json.freedata();
        }
        MEMORYSTAT_CHECKMEMORYLEAK(test.GetName(),"check fail");
    }

//    {
//        std::filesystem::path p(TEST_DATA_PATH "transform_extra/");
//        for (std::filesystem::directory_iterator it(p); it != end_it; ++it) {
//            if( ! is_regular_file(it->path()) || it->path().extension() != ".json" ) {
//                continue;
//            }
//            MEMORYSTAT_SCOPE();
//            auto json = ReadJSON(it->path());
//            if (json.data() != nullptr ) {
//                test.SetUp();
//                ParseResultBase* pr = test.Parse(json.data(), json.size());
//                bool result = pr != 0;
//                delete pr;
//                auto const& n = it->path().filename().stem().string();
//                fprintf(fp, "1. Parse Validation,%s,%s,%s\n", test.GetName(), n.data(), result ? "true" : "false");
//                test.TearDown();
//                if (!result) {
//                    if (md)
//                        fprintf(md, "* `%s` is valid but was mistakenly deemed invalid.\n~~~js\n%s\n~~~\n\n", n.data(), json.data());
//                } else {
//                    parseValidationCorrect++;
//                }
//                parseValidationTotal++;
//                json.freedata();
//            }
//            MEMORYSTAT_CHECKMEMORYLEAK(test.GetName(),"transform");
//        }
//    }


    if (md)
        fprintf(md, "\nSummary: %d of %d are correct.\n\n", parseValidationCorrect, parseValidationTotal);

#endif // TEST_PARSE

    parseValidationTotal = 0;
    parseValidationCorrect = 0;

    if (md)
        fprintf(md, "## 2. Parse Double\n\n");

    // Parse Double
    {
        using rapidjson::internal::Double;
        #define TEST_DOUBLE(tname,json,expect)\
        {\
            bool result = false;\
            double actual = 0.0;\
            test.SetUp();\
            if (test.ParseDouble(json, sizeof(json)-1, &actual)) \
                result = Double(expect).Uint64Value() == Double(actual).Uint64Value();\
            if (!result) {\
                if (md)\
                    fprintf(md, "* `%s`\n  * expect: `%.17g (0x016%" PRIX64 ")`\n  * actual: `%.17g (0x016%" PRIX64 ")`\n\n",\
                        json, expect, Double(expect).Uint64Value(), actual, Double(actual).Uint64Value());\
            }\
            else\
                parseValidationCorrect++;\
            parseValidationTotal++;\
            ReportBase::get_instance().add_conformance_statistics("2. Parse Double", test.GetName(), tname, result);\
            test.TearDown();\
        }
        TEST_DOUBLE("0.0","[0.0]", 0.0);
        TEST_DOUBLE("-0.0","[-0.0]", -0.0);
        TEST_DOUBLE("1.0","[1.0]", 1.0);
        TEST_DOUBLE("-1.0","[-1.0]", -1.0);
        TEST_DOUBLE("1.5","[1.5]", 1.5);
        TEST_DOUBLE("-1.5","[-1.5]", -1.5);
        TEST_DOUBLE("3.1416","[3.1416]", 3.1416);
        TEST_DOUBLE("1E10","[1E10]", 1E10);
        TEST_DOUBLE("1e10","[1e10]", 1e10);
        TEST_DOUBLE("1E+10","[1E+10]", 1E+10);
        TEST_DOUBLE("1E-10","[1E-10]", 1E-10);
        TEST_DOUBLE("-1E10","[-1E10]", -1E10);
        TEST_DOUBLE("-1e10","[-1e10]", -1e10);
        TEST_DOUBLE("-1E+10]","[-1E+10]", -1E+10);
        TEST_DOUBLE("-1E-10","[-1E-10]", -1E-10);
        TEST_DOUBLE("1.234E+10","[1.234E+10]", 1.234E+10);
        TEST_DOUBLE("1.234E-10","[1.234E-10]", 1.234E-10);
        TEST_DOUBLE("1.79769e+308","[1.79769e+308]", 1.79769e+308);
        TEST_DOUBLE("2.22507e-308","[2.22507e-308]", 2.22507e-308);
        TEST_DOUBLE("-1.79769e+308","[-1.79769e+308]", -1.79769e+308);
        TEST_DOUBLE("-2.22507e-308","[-2.22507e-308]", -2.22507e-308);
        TEST_DOUBLE("min denormal","[4.9406564584124654e-324]", 4.9406564584124654e-324);   // minimum denormal
        TEST_DOUBLE("max subnormal","[2.2250738585072009e-308]", 2.2250738585072009e-308);  // Max subnormal double
        TEST_DOUBLE("min normal","[2.2250738585072014e-308]", 2.2250738585072014e-308);     // Min normal positive double
        TEST_DOUBLE("max double","[1.7976931348623157e+308]", 1.7976931348623157e+308);     // Max double
        TEST_DOUBLE("must underflow","[1e-10000]", 0.0);                                    // must underflow
        TEST_DOUBLE("2^64","[18446744073709551616]", 18446744073709551616.0);               // 2^64 (max of uint64_t + 1, force to use double)
        TEST_DOUBLE("-2^63-1","[-9223372036854775809]", -9223372036854775809.0);            // -2^63 - 1(min of int64_t + 1, force to use double)
        TEST_DOUBLE("issues/120","[0.9868011474609375]", 0.9868011474609375);               // https://github.com/miloyip/rapidjson/issues/120
        TEST_DOUBLE("123e34","[123e34]", 123e34);                                           // Fast Path Cases In Disguise
        TEST_DOUBLE("45913141877270640000.0","[45913141877270640000.0]", 45913141877270640000.0);
        TEST_DOUBLE("2.2250738585072011e-308","[2.2250738585072011e-308]", 2.2250738585072011e-308);               // http://www.exploringbinary.com/php-hangs-on-numeric-value-2-2250738585072011e-308/
        //TEST_DOUBLE("[1e-00011111111111]", 0.0);
        //TEST_DOUBLE("[-1e-00011111111111]", -0.0);
        TEST_DOUBLE("1e-214748363","[1e-214748363]", 0.0);
        TEST_DOUBLE("1e-214748364","[1e-214748364]", 0.0);
        //TEST_DOUBLE("[1e-21474836311]", 0.0);
        TEST_DOUBLE("nax double/other","[0.017976931348623157e+310]", 1.7976931348623157e+308); // Max double in another form

        // Since
        // abs((2^-1022 - 2^-1074) - 2.2250738585072012e-308) = 3.109754131239141401123495768877590405345064751974375599... ¡Á 10^-324
        // abs((2^-1022) - 2.2250738585072012e-308) = 1.830902327173324040642192159804623318305533274168872044... ¡Á 10 ^ -324
        // So 2.2250738585072012e-308 should round to 2^-1022 = 2.2250738585072014e-308
        TEST_DOUBLE("java-hang","[2.2250738585072012e-308]", 2.2250738585072014e-308);       // http://www.exploringbinary.com/java-hangs-when-converting-2-2250738585072012e-308/

        // More closer to normal/subnormal boundary
        // boundary = 2^-1022 - 2^-1075 = 2.225073858507201136057409796709131975934819546351645648... ¡Á 10^-308
        TEST_DOUBLE("2...564e-308","[2.22507385850720113605740979670913197593481954635164564e-308]", 2.2250738585072009e-308);
        TEST_DOUBLE("2...565e-308","[2.22507385850720113605740979670913197593481954635164565e-308]", 2.2250738585072014e-308);

        // 1.0 is in (1.0 - 2^-54, 1.0 + 2^-53)
        // 1.0 - 2^-54 = 0.999999999999999944488848768742172978818416595458984375
        TEST_DOUBLE("0.99...84375","[0.999999999999999944488848768742172978818416595458984375]", 1.0); // round to even
        TEST_DOUBLE("0.99...84374","[0.999999999999999944488848768742172978818416595458984374]", 0.99999999999999989); // previous double
        TEST_DOUBLE("0.99...84376","[0.999999999999999944488848768742172978818416595458984376]", 1.0); // next double
        // 1.0 + 2^-53 = 1.00000000000000011102230246251565404236316680908203125
        TEST_DOUBLE("1.00...03125","[1.00000000000000011102230246251565404236316680908203125]", 1.0); // round to even
        TEST_DOUBLE("1.00...03124","[1.00000000000000011102230246251565404236316680908203124]", 1.0); // previous double
        TEST_DOUBLE("1.00...03126","[1.00000000000000011102230246251565404236316680908203126]", 1.00000000000000022); // next double

        // Numbers from https://github.com/floitsch/double-conversion/blob/master/test/cctest/test-strtod.cc

        TEST_DOUBLE("720...28.0","[72057594037927928.0]", 72057594037927928.0);
        TEST_DOUBLE("720...36.0","[72057594037927936.0]", 72057594037927936.0);
        TEST_DOUBLE("720...32.0","[72057594037927932.0]", 72057594037927936.0);
        TEST_DOUBLE("720...99e-5","[7205759403792793199999e-5]", 72057594037927928.0);
        TEST_DOUBLE("720...01e-5","[7205759403792793200001e-5]", 72057594037927936.0);

        TEST_DOUBLE("922...84.0","[9223372036854774784.0]", 9223372036854774784.0);
        TEST_DOUBLE("922...08.0","[9223372036854775808.0]", 9223372036854775808.0);
        TEST_DOUBLE("922...96.0","[9223372036854775296.0]", 9223372036854775808.0);
        TEST_DOUBLE("922...9e-5","[922337203685477529599999e-5]", 9223372036854774784.0);
        TEST_DOUBLE("922...1e-5","[922337203685477529600001e-5]", 9223372036854775808.0);

        TEST_DOUBLE("101...384","[10141204801825834086073718800384]", 10141204801825834086073718800384.0);
        TEST_DOUBLE("101...008","[10141204801825835211973625643008]", 10141204801825835211973625643008.0);
        TEST_DOUBLE("101...696","[10141204801825834649023672221696]", 10141204801825835211973625643008.0);
        TEST_DOUBLE("101...9e-5","[1014120480182583464902367222169599999e-5]", 10141204801825834086073718800384.0);
        TEST_DOUBLE("101...1e-5","[1014120480182583464902367222169600001e-5]", 10141204801825835211973625643008.0);

        TEST_DOUBLE("570...808","[5708990770823838890407843763683279797179383808]", 5708990770823838890407843763683279797179383808.0);
        TEST_DOUBLE("570...496","[5708990770823839524233143877797980545530986496]", 5708990770823839524233143877797980545530986496.0);
        TEST_DOUBLE("570...152","[5708990770823839207320493820740630171355185152]", 5708990770823839524233143877797980545530986496.0);
        TEST_DOUBLE("570...9e-3","[5708990770823839207320493820740630171355185151999e-3]", 5708990770823838890407843763683279797179383808.0);
        TEST_DOUBLE("570...1e-3","[5708990770823839207320493820740630171355185152001e-3]", 5708990770823839524233143877797980545530986496.0);

        {
            char n1e308[312];   // '1' followed by 308 '0'
            n1e308[0] = '[';
            n1e308[1] = '1';
            for (int j = 2; j < 310; j++)
                n1e308[j] = '0';
            n1e308[310] = ']';
            n1e308[311] = '\0';
            TEST_DOUBLE("n1e308",n1e308, 1E308);
        }

        // Cover trimming
        TEST_DOUBLE("trimming",
            "[2.22507385850720113605740979670913197593481954635164564802342610972482222202107694551652952390813508"
            "7914149158913039621106870086438694594645527657207407820621743379988141063267329253552286881372149012"
            "9811224514518898490572223072852551331557550159143974763979834118019993239625482890171070818506906306"
            "6665599493827577257201576306269066333264756530000924588831643303777979186961204949739037782970490505"
            "1080609940730262937128958950003583799967207254304360284078895771796150945516748243471030702609144621"
            "5722898802581825451803257070188608721131280795122334262883686223215037756666225039825343359745688844"
            "2390026549819838548794829220689472168983109969836584681402285424333066033985088644580400103493397042"
            "7567186443383770486037861622771738545623065874679014086723327636718751234567890123456789012345678901"
            "e-308]",
            2.2250738585072014e-308);
    }

    if (md)
        fprintf(md, "\nSummary: %d of %d are correct.\n\n", parseValidationCorrect, parseValidationTotal);

    // Parse String

    parseValidationTotal = 0;
    parseValidationCorrect = 0;

    if (md)
        fprintf(md, "## 3. Parse String\n\n");

    {
        int i = 1;
        #define TEST_STRING(tname,json, expect)\
        {\
            bool result = false;\
            size_t expectLength = sizeof(expect) - 1;\
            std::string actual;\
            test.SetUp();\
            if (test.ParseString(json, sizeof(json)-1, actual)) \
                result = (expectLength == actual.size()) && (memcmp(expect, actual.c_str(), expectLength) == 0);\
            if (!result) {\
                if (md) {\
                    fprintf(md, "* `%s`\n  * expect: `", json);\
                    EscapeString(md, expect, expectLength);\
                    fprintf(md, "` (length: %d)\n  * actual: `", (int)expectLength);\
                    EscapeString(md, actual.c_str(), actual.size());\
                    fprintf(md, "` (length: %d)\n\n", (int)actual.size());\
                }\
            }\
            else\
                parseValidationCorrect++;\
            parseValidationTotal++;\
            ReportBase::get_instance().add_conformance_statistics("3. Parse String", test.GetName(), tname, result);\
            test.TearDown();\
            i++;\
        }

        TEST_STRING("empty","[\"\"]", "");
        TEST_STRING("ascii","[\"Hello\"]", "Hello");
        TEST_STRING("newline","[\"Hello\\nWorld\"]", "Hello\nWorld");
        TEST_STRING("utf16 zero","[\"Hello\\u0000World\"]", "Hello\0World");
        TEST_STRING("whitesp","[\"\\\"\\\\/\\b\\f\\n\\r\\t\"]", "\"\\/\b\f\n\r\t");
        TEST_STRING("utf16 dollar","[\"\\u0024\"]", "\x24");         // Dollar sign U+0024
        TEST_STRING("utf16 cent","[\"\\u00A2\"]", "\xC2\xA2");     // Cents sign U+00A2
        TEST_STRING("utf16 euro","[\"\\u20AC\"]", "\xE2\x82\xAC"); // Euro sign U+20AC
        TEST_STRING("utf16 gclef","[\"\\uD834\\uDD1E\"]", "\xF0\x9D\x84\x9E");  // G clef sign U+1D11E
    }

    if (md)
        fprintf(md, "\nSummary: %d of %d are correct.\n\n", parseValidationCorrect, parseValidationTotal);

#if TEST_PARSE && TEST_STRINGIFY
    parseValidationTotal = 0;
    parseValidationCorrect = 0;

    if (md)
        fprintf(md, "## 4. Roundtrip\n\n");

    std::filesystem::path p_rtrip(TEST_DATA_PATH "roundtrip/");
    for (std::filesystem::directory_iterator it(p_rtrip); it != end_it; ++it) {
        if( ! is_regular_file(it->path()) || it->path().extension() != ".json" ) {
            continue;
        }
        MEMORYSTAT_SCOPE();
        auto json = ReadJSON(it->path());
        if (json.data() != nullptr ) {
            test.SetUp();
            ParseResultBase* pr = test.Parse(json.data(), json.size());
            bool result = false;
            bool terminate = false;
            if (pr) {
                StringResultBase* sr = test.Stringify(pr);
                delete pr;

                if (sr) {
                    // Some libraries must produce newlines/tabs, skip them in comparison.
                    const char* s = sr->c_str();
                    result = true;
                    for (size_t j = 0; j < json.size(); ++j, ++s) {
                        while (*s == '\n' || *s == '\t')
                            ++s;
                        if (json.data()[j] != *s) {
                            result = false;
                            break;
                        }
                    }
                    // if (!result)
                    //     printf("Expect: %s\nActual: %s\n\n", json, sr->c_str());
                    if (!result) {
                        if (md)
                            fprintf(md, "* Fail:\n~~~js\n%s\n~~~\n\n~~~js\n%s\n~~~\n\n", json.data(), sr ? sr->c_str() : "N/A");\
                    } else {
                        parseValidationCorrect++;
                    }
                    parseValidationTotal++;
                    delete sr;
                } else {
                    terminate = true; // This library does not support stringify, terminate this test
                }
            }

            test.TearDown();
            json.freedata();
            if (terminate)
                break;
            auto const& n = it->path().filename().stem().string();
            ReportBase::get_instance().add_conformance_statistics("4. Roundtrip", test.GetName(), n, result);
        }
        MEMORYSTAT_CHECKMEMORYLEAK(test.GetName(),"roundtrip");
    }
    if (md)
        fprintf(md, "\nSummary: %d of %d are correct.\n\n", parseValidationCorrect, parseValidationTotal);

#endif // TEST_PARSE && TEST_STRINGIFY

    printf("\n");
}

static void BenchAllConformance() {
    FILE *fp = fopen(RESULT_PATH "conformance.csv", "w");
    fputs("Type,Library,Test,Result\n", fp);

    TestList& tests = TestManager::Instance().GetTests();
    for (TestList::iterator itr = tests.begin(); itr != tests.end(); ++itr) {
        if (strcmp((*itr)->GetName(), "strdup (C)") == 0
            || strcmp((*itr)->GetName(), "pjson (C)") == 0)
            continue;
        BenchConformance(**itr, fp);
    }
    fclose(fp);
}
#endif // TEST_CONFORMANCE


bool CheckAbort( TestBase const * test_abort,  const char * fn )
{
    std::error_code ec;
    std::filesystem::path ptest( TEST_DATA_PATH );
    std::filesystem::path rep( REPORT_PATH );
    ptest = ptest / "jsonchecker_aborts" / fn;
    ptest += ".json";
    rep = rep / "abort_test";
    rep = rep / "test ";
    rep += fn;
    rep += "~";
    rep += test_abort->GetName();
    auto rep_parsed = rep.string() + ".parsed";
    auto rep_failed = rep.string() + ".failed";
    auto rep_aborted = rep.string() + ".aborted";
    auto rep_file_not_found = rep.string() + ".file_not_found";
    std::filesystem::remove(rep_parsed,ec);
    std::filesystem::remove(rep_file_not_found,ec);
    std::ofstream ofs(rep_aborted);
    ofs << "x";
    ofs.close();
    std::cout << "Check abort " << test_abort->GetName() << " with file " << ptest;
    auto json = ReadJSON(ptest);
    if (json.data() == nullptr ) {
        std::filesystem::rename(rep_aborted,rep_file_not_found,ec);
        std::cout << " not found" << std::endl;
        return false;
    }

    ParseResultBase * pr = test_abort->Parse(json.data(), json.size()); // may abort
    if (pr==nullptr) {
        std::filesystem::rename(rep_aborted,rep_failed,ec);
        std::cout << " failed" << std::endl;
        return true; // test was executed
    }

    std::filesystem::rename(rep_aborted,rep_parsed,ec);
    std::cout <<  " parsed" << std::endl;
    return true; // test was executed
}

void info()
{
    std::cerr
        << "\noptions: "
        << "\n  --verify-only"
        << "\n  --performance-only"
        << "\n  --conformance-only"
        << "\n  --aborts <test_name> <file_name_without_extension>\n"
        << std::endl;
}

int main(int argc, char* argv[]) {
    bool doVerify = true;
    bool doPerformance = true;
    bool doConformance = true;
    bool doAborts = false;
    bool printHelp = true;
    auto& reports = ReportBase::get_instance();

    const TestBase * test_abort=nullptr;
    if (argc == 2) {
        if (strcmp(argv[1], "--verify-only") == 0) {
            doVerify = true;
            printHelp = false;
            doPerformance = doConformance = false;
        } else if (strcmp(argv[1], "--performance-only") == 0) {
            doPerformance = true;
            printHelp = false;
            doVerify = doConformance = false;
        } else if (strcmp(argv[1], "--conformance-only") == 0) {
            doConformance = true;
            printHelp = false;
            doVerify = doPerformance = false;
        } else {
            fprintf(stderr, "Invalid option\n");
            exit(1);
        }
    } else if(argc == 4) { // --aborts <test_name> <file_name>.json // in data/jsonchecker_aborts/
        if (strcmp(argv[1], "--aborts") == 0 ) {
            doConformance = false;
            doVerify = false;
            doPerformance = false;
            doConformance = false;
            doAborts = true;
            printHelp = false;

        }
    }

    if(printHelp) {
        info();
    }

    {
        // sort tests
        TestList& tests = TestManager::Instance().GetTests();
        std::sort(tests.begin(), tests.end());
        {
            sviewvec names;
            for(auto const &t : tests) {
                if( 0 != strcmp(t->GetName(), "strdup")) {
                    names.push_back(t->GetName());
                    if( doAborts && 0 == strcmp(t->GetName(), argv[2])) {
                        test_abort = t;
                    }
                }
            }
            reports.print_test_names(names);
        }

        if( doAborts ) {
            if( test_abort != nullptr ) {
                CheckAbort(test_abort, argv[3]);
                exit(0);
            } else {
                info();
                exit(-1);
            }
        }

        // Read files
        TestJsonList testJsons;
        ReadFiles(testJsons);

        if (doVerify)
            VerifyAll(testJsons);

        if (doPerformance)
            BenchAllPerformance(testJsons);

#if TEST_CONFORMANCE
        if (doConformance)
            BenchAllConformance();
#endif

        printf("\n");
        FreeFiles(testJsons);
    }
}
