#include "../test.h"
#include "yyjson/src/yyjson.h"

#include <cassert>
#include <iostream>

static void GenStat(Stat* s, const yyjson_val* obj) {
    switch (yyjson_get_type(const_cast<yyjson_val*>(obj))) {
    case YYJSON_TYPE_OBJ: {
        size_t count = yyjson_obj_size(const_cast<yyjson_val*>(obj));
        for (size_t i = 0; i < count; ++i) {
            GenStat(s, yyjson_arr_get(const_cast<yyjson_val*>(obj),i));
        }
        size_t idx, max;
        yyjson_val *key, *val;
        yyjson_obj_foreach(const_cast<yyjson_val*>(obj), idx, max, key, val) {
            GenStat(s, const_cast<yyjson_val*>(val));
        }
        ++s->objectCount;
    }
    break;

    case YYJSON_TYPE_ARR: {
        size_t count = yyjson_arr_size(const_cast<yyjson_val*>(obj));
        for (size_t i = 0; i < count; ++i) {
            GenStat(s, yyjson_arr_get(const_cast<yyjson_val*>(obj),i));
        }
        s->elementCount += count;
        ++s->arrayCount;
    }
    break;

    case YYJSON_TYPE_STR:
        ++s->stringCount;
        s->stringLength += yyjson_get_len(const_cast<yyjson_val*>(obj));
    break;

    case YYJSON_TYPE_NUM:
        ++s->numberCount; 
    break;

    case YYJSON_TYPE_BOOL:
        yyjson_get_bool(const_cast<yyjson_val*>(obj)) ? ++s->trueCount: ++s->falseCount;
    break;

    case YYJSON_TYPE_NULL:
    case YYJSON_TYPE_NONE:
        ++s->nullCount;
    break;
    
    default:
        assert(false);
    }
}

class YYjsonParseResult : public ParseResultBase {
public:
    YYjsonParseResult() : doc(nullptr) {}
    ~YYjsonParseResult() { yyjson_doc_free(doc); }

    yyjson_doc *doc;
};

class YYjsonStringResult : public StringResultBase {
public:
    YYjsonStringResult()
        : s(nullptr)
    {}
    ~YYjsonStringResult()
    { free(s); }

    virtual const char* c_str() const { return s; }

    char* s;
};

class YYjsonTest : public TestBase {
public:
#if TEST_INFO
    virtual const char* GetName() const override { return "yyjson (C)"; }
    virtual const char* GetFilename() const override { return __FILE__; }
#endif
	
#if TEST_PARSE
    virtual ParseResultBase* Parse(const char* json, size_t length) const override {
        YYjsonParseResult* pr = new YYjsonParseResult;
        yyjson_read_flag flg=YYJSON_READ_NOFLAG;
        pr->doc = yyjson_read(json,length,flg);
        if (pr->doc==nullptr) {
            return 0;
        }
    	return pr;
    }
#endif

#if TEST_STRINGIFY
    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const override {
        const YYjsonParseResult* pr = static_cast<const YYjsonParseResult*>(parseResult);
        YYjsonStringResult* sr = new YYjsonStringResult;
        size_t len;
        yyjson_write_flag flg = YYJSON_WRITE_NOFLAG;
        sr->s = yyjson_write(pr->doc,flg, &len);
        return sr;
    }
#endif


#if TEST_PRETTIFY
    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const override {
        const YYjsonParseResult* pr = static_cast<const YYjsonParseResult*>(parseResult);
        YYjsonStringResult* sr = new YYjsonStringResult;
        size_t len;
        yyjson_write_flag flg = YYJSON_WRITE_PRETTY;
        sr->s = yyjson_write(pr->doc,flg, &len);
        return sr;
    }
#endif

#if TEST_STATISTICS
    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const  override {
        const YYjsonParseResult* pr = static_cast<const YYjsonParseResult*>(parseResult);
        memset(stat, 0, sizeof(Stat));
        yyjson_val * root = yyjson_doc_get_root(pr->doc);
        GenStat(stat, root);
        return true;
    }
#endif

#if TEST_CONFORMANCE
    virtual bool ParseDouble(const char* json, size_t jsize, double* d) const override {
        YYjsonParseResult pr;
        yyjson_read_flag flg=YYJSON_READ_STOP_WHEN_DONE;
        pr.doc = yyjson_read(json,jsize,flg);
        if (pr.doc==nullptr) {
            return false;
        }
        auto root = yyjson_doc_get_root(pr.doc);
        auto v = yyjson_arr_get_first(root);
        if( !yyjson_is_real(v)) {
            return false;
        }
        *d = yyjson_get_real(v);
        return true;
    }

    // const char* json should be std::string
    virtual bool ParseString(const char* json, size_t jsize, std::string& s) const override {
        YYjsonParseResult pr;
        yyjson_read_flag flg=YYJSON_READ_STOP_WHEN_DONE;
        yyjson_read_err err;
        pr.doc = yyjson_read_opts(const_cast<char*>(json),jsize,flg,nullptr, &err);
        if (pr.doc==nullptr) {
            return false;
        }
        yyjson_val *root = yyjson_doc_get_root(pr.doc);
        size_t arrn = yyjson_arr_size(root);
        if( arrn == 0) {
            return false;
        }
        auto v = yyjson_arr_get(root,0);
        if( ! yyjson_is_str(v) ) {
            return false;
        }
        s = std::string(yyjson_get_str( v ));
        return true;
    }
#endif
};

REGISTER_TEST(YYjsonTest);
