#include "../test.h"

#if !defined(_MSC_VER) && !defined(__i386__) // VC have compilation error, crash in 32-bit

#include "nbsdxSimpleJSON/json.hpp"

using namespace json;

static void GenStat(Stat& stat, const JSON& v) {
    switch (const_cast<JSON&>(v).JSONType()) {
    case JSON::Class::Array:
        for (auto& i : const_cast<JSON&>(v).ArrayRange()) {
            GenStat(stat, i);
            stat.elementCount++;
        }
        stat.arrayCount++;
        break;

    case JSON::Class::Object:
        for (auto& i : const_cast<JSON&>(v).ObjectRange()) {
            GenStat(stat, i.second);
            stat.stringLength += i.first.size();
            stat.stringCount++;
            stat.elementCount++;
        }
        stat.objectCount++;
        break;

    case JSON::Class::String:
        stat.stringCount++;
        stat.stringLength += v.ToString().size();
        break;

    case JSON::Class::Integral:
    case JSON::Class::Floating:
        stat.numberCount++;
        break;

    case JSON::Class::Boolean:
        if (v.ToBool())
            stat.trueCount++;
        else
            stat.falseCount++;
        break;

    case JSON::Class::Null:
        stat.nullCount++;
        break;
    }
}

class NbsdxsimplejsonParseResult : public ParseResultBase {
public:
    JSON root;
};

class NbsdxsimplejsonStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return s.c_str(); }

    std::string s;
};
class NbsdxsimplejsonTest : public TestBase {
public:
#if TEST_INFO
    virtual const char* GetName() const override { return "nbsdx SimpleJSON (C++11)"; }
    virtual const char* GetFilename() const override { return __FILE__; }
#endif

#if TEST_PARSE
    virtual ParseResultBase* Parse(const char* json, size_t length) const override
    {
        (void)length;
        NbsdxsimplejsonParseResult* pr = new NbsdxsimplejsonParseResult;
        try {
            pr->root = JSON::Load(std::string(json, length));
        }
        catch (...) {
            delete pr;
            return 0;
        }
    	return pr;
    }
#endif

#if TEST_STRINGIFY
    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const override
    {
        const NbsdxsimplejsonParseResult* pr = static_cast<const NbsdxsimplejsonParseResult*>(parseResult);
        NbsdxsimplejsonStringResult* sr = new NbsdxsimplejsonStringResult;
        sr->s = pr->root.dump();
        return sr;
    }
#endif

#if TEST_PRETTIFY
    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const override
    {
        const NbsdxsimplejsonParseResult* pr = static_cast<const NbsdxsimplejsonParseResult*>(parseResult);
        NbsdxsimplejsonStringResult* sr = new NbsdxsimplejsonStringResult;
        sr->s = pr->root.dump(1, "    ");
        return sr;
    }
#endif

#if TEST_STATISTICS
    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const override
    {
        const NbsdxsimplejsonParseResult* pr = static_cast<const NbsdxsimplejsonParseResult*>(parseResult);
        memset(stat, 0, sizeof(Stat));
        GenStat(*stat, pr->root);
        return true;
    }
#endif

#if TEST_CONFORMANCE
    virtual bool ParseDouble(const char* json, size_t length, double* d) const override
    {
        try {
            JSON root = JSON::Load(std::string(json, length));
            *d = root[0].ToFloat();
            return true;
        }
        catch (...) {
        }
        return false;
    }

    virtual bool ParseString(const char* json, size_t length, std::string& s) const override
    {
        try {
            JSON root = JSON::Load(std::string(json, length));
            s = root[0].ToString();
            return true;
        }
        catch (...) {
        }
        return false;
    }
#endif
};

REGISTER_TEST(NbsdxsimplejsonTest);

#endif
