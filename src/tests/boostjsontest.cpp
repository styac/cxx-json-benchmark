#if defined(__clang__) || defined(__GNUC__)

#include "../test.h"

#define BOOST_JSON_STANDALONE
#include "boost/json.hpp"
#include "boost/json/src.hpp"

namespace json = boost::json;

static void GenStat(Stat& stat, const json::value& v)
{
    switch (v.kind()) {
    case json::kind::array:
        for (auto& element : v.as_array() ) {
            GenStat(stat, element);
        }
        stat.arrayCount++;
        stat.elementCount += v.as_array().size();
        break;

    case json::kind::object:
        for (auto& element : v.as_object() ) {
            GenStat(stat, element.value());
            stat.stringLength += element.key().size();
        }
        stat.objectCount++;
        stat.memberCount += v.as_object().size();
        break;

    case json::kind::string:
        stat.stringCount++;
        stat.stringLength += v.as_string().size();
        break;

    case json::kind::int64:
    case json::kind::uint64:
    case json::kind::double_:
        stat.numberCount++;
        break;

    case json::kind::bool_:
        if (v.as_bool())
            stat.trueCount++;
        else
            stat.falseCount++;
        break;

    case json::kind::null:
        stat.nullCount++;
        break;

    default:
        break;
    }
}

class BoostjsonParseResult : public ParseResultBase {
public:
    json::value root;
};

class BoostjsonStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return s.c_str(); }

    std::string s;
};

class BoostjsonTest : public TestBase {
public:
#if TEST_INFO
    virtual const char* GetName() const override { return "boostjson (C++11)"; }
    virtual const char* GetFilename() const override { return __FILE__; }
#endif

#if TEST_PARSE
    virtual ParseResultBase* Parse(const char* jstr, size_t length) const override
    {
        (void)length;
        BoostjsonParseResult* pr = new BoostjsonParseResult;
        json::parser p;
        json::error_code ec;
        p.write(jstr,length,ec);
        if(ec) {
            delete pr;
            return 0;
        }
        pr->root = p.release();
        return pr;
    }
#endif

#if TEST_STRINGIFY
    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const override
    {
        const BoostjsonParseResult* pr = static_cast<const BoostjsonParseResult*>(parseResult);
        BoostjsonStringResult* sr = new BoostjsonStringResult;
        sr->s = json::serialize(pr->root);
        return sr;
    }
#endif

//#if TEST_PRETTIFY
//    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const override
//    {
//        const BoostjsonParseResult* pr = static_cast<const BoostjsonParseResult*>(parseResult);
//        BoostjsonStringResult* sr = new BoostjsonStringResult;
//        sr->s = json::serialize(pr->root);
//        return sr;
//    }
//#endif

#if TEST_STATISTICS
    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const override
    {
        const BoostjsonParseResult* pr = static_cast<const BoostjsonParseResult*>(parseResult);
        memset(stat, 0, sizeof(Stat));
        GenStat(*stat, pr->root);
        return true;
    }
#endif

#if TEST_CONFORMANCE
    virtual bool ParseDouble(const char* jstr, size_t length, double* d) const override
    {
        json::parser p;
        json::error_code ec;
        p.write(jstr,length,ec);
        if(ec) {
            return false;
        }
        auto v = p.release();
        try {
            *d = v.as_array()[0].as_double();
        }  catch (...) {
            return false;
        }
        return true;
    }

    virtual bool ParseString(const char* jstr, size_t length, std::string& s) const override
    {
        json::parser p;
        json::error_code ec;
        p.write(jstr,length,ec);
        if(ec) {
            return false;
        }
        auto v = p.release();
        try {
            auto x = v.as_array()[0].as_string();
            s = { x.begin(), x.end() };
        }  catch (...) {
            return false;
        }
        return true;
    }
#endif
};

REGISTER_TEST(BoostjsonTest);

#endif
