#pragma once
#include "config.h"

#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

#include "memorystat.h"

class TestBase;
typedef std::vector<const TestBase *> TestList;

class TestManager {
public:
    static TestManager& Instance() {
        static TestManager* singleton = new TestManager;
        return *singleton;
    }

    static void DestroyInstance() {
        delete &Instance();
    }

    void AddTest(const TestBase* test) {
        mTests.push_back(test);
    }

    const TestList& GetTests() const {
        return mTests;
    }

    TestList& GetTests() {
        return mTests;
    }

private:
    TestList mTests;
};

struct Stat {
    size_t objectCount;
    size_t arrayCount;
    size_t numberCount;
    size_t stringCount;
    size_t trueCount;
    size_t falseCount;
    size_t nullCount;

    size_t memberCount;   // Number of members in all objects
    size_t elementCount;  // Number of elements in all arrays
    size_t stringLength;  // Number of code units in all strings
};

// Each test can customize what to be stored in parse result, 
// which will be passed to Stringify()/Prettify()/Statistics()
class ParseResultBase {
public:
    virtual ~ParseResultBase() {}
};

// Stringify()/Prettify() returns object derived from this class.
// So that it can prevents unncessary strdup().
class StringResultBase {
public:
    virtual ~StringResultBase() {}

    // The test framework call this function to get a null-terminated string.
    virtual const char* c_str() const = 0;
};

class TestBase {
public:
    TestBase() {
        TestManager::Instance().AddTest(this);
    }

    // For each operation, call SetUp() before and TearDown() after.
    // It is mainly for libraries require huge initialize time (e.g. Creating Isolate in V8).
    virtual void SetUp() const {}
    virtual void TearDown() const {}

#if TEST_INFO
    virtual const char* GetName() const = 0;
    virtual const char* GetFilename() const = 0;
#endif

#if TEST_PARSE

    // const char* json should be std::string
    virtual ParseResultBase* Parse(const char* json, size_t length) const
    {
        (void)json;
        (void)length;
        return 0;
    }
#endif

#if TEST_STRINGIFY
    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const
    {
        (void)parseResult;
        return 0;
    }
#endif

#if TEST_PRETTIFY
    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const
    {
        (void)parseResult;
        return 0;
    }
#endif

#if TEST_STATISTICS
    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const
    {
        (void)parseResult;
        (void)stat;
        return false;
    }
#endif

#if TEST_SAXROUNDTRIP
    virtual StringResultBase* SaxRoundtrip(const char* json, size_t length) const
    {
        (void)json;
        (void)length;
        return 0;
    }
#endif

#if TEST_SAXSTATISTICS
    virtual bool SaxStatistics(const char* json, size_t length, Stat* stat) const
    {
        (void)json;
        (void)length;
        (void)stat;
        return false;
    }
#endif

#if TEST_SAXSTATISTICSUTF16
    virtual bool SaxStatisticsUTF16(const char* json, size_t length, Stat* stat) const
    {
        (void)json;
        (void)length;
        (void)stat;
        return false;
    }
#endif

#if TEST_CONFORMANCE
    // Parse a JSON of an array containing one double
    // Return false if it is failed to parse.
    // E.g. json = "[1.0]" -> d = 1.0

    // const char* json should be std::string
    virtual bool ParseDouble(const char* json, size_t length, double* d) const
    {
        (void)json;
        (void)d;
        return false;
    }

    // const char* json should be std::string
    virtual bool ParseString(const char* json, size_t length, std::string& s) const
    {
        (void)json;
        (void)s;
        return false;
    }
#endif

};


#define REGISTER_TEST(cls) static cls gRegister##cls
