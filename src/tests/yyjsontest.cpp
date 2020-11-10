#include "../test.h"
#include "yyjson/src/yyjson.h"

#include <cassert>

static void GenStat(Stat* s, const yyjson_doc* v) {
    switch (v->val_read) {
    case YYJSON_TYPE_OBJ:
//        for (yyjson_val* child = v->head; child != 0; child = child->next) {
//            GenStat(s, child);
//            ++s->memberCount;
//        }
//        ++s->objectCount;
        break;

    case YYJSON_TYPE_ARR:
//        for (int i = 0; i < v->size; ++i)
//            GenStat(s, v->adata[i]);
//        s->elementCount += v->size;
//        ++s->arrayCount;
        break;

    case YYJSON_TYPE_STR:
        ++s->stringCount;
        s->stringLength += v->dat_read;
        break;

    case YYJSON_TYPE_NUM:
        ++s->numberCount; 
        break;

    case YYJSON_TYPE_BOOL:
        v->root ? ++s->trueCount: ++s->falseCount;
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
    YYjsonStringResult() : s() {}
    ~YYjsonStringResult() { free(s); }

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


#ifdef TEST_STRINGIFY
#   undef TEST_STRINGIFY
#endif

#define TEST_STRINGIFY 0
#if TEST_STRINGIFY
    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const {
        const JusonParseResult* pr = static_cast<const JusonParseResult*>(parseResult);
        JusonStringResult* sr = new JusonStringResult;
        sr->s = cJSON_PrintUnformatted(pr->root);
        return sr;
    }
#endif

#ifdef TEST_PRETTIFY
#   undef TEST_PRETTIFY
#endif

#define TEST_PRETTIFY 0
#if TEST_PRETTIFY
    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const {
        const JusonParseResult* pr = static_cast<const JusonParseResult*>(parseResult);
        JusonStringResult* sr = new JusonStringResult;
        sr->s = cJSON_Print(pr->root);
        return sr;
    }
#endif

#if TEST_STATISTICS
    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const  override {
        const YYjsonParseResult* pr = static_cast<const YYjsonParseResult*>(parseResult);
        memset(stat, 0, sizeof(Stat));
        GenStat(stat, pr->doc);
        return true;
    }
#endif

#if TEST_CONFORMANCE
    virtual bool ParseDouble(const char* json, size_t jsize, double* d) const override {
        YYjsonParseResult pr;
        yyjson_read_flag flg=YYJSON_READ_NOFLAG;
        pr.doc = yyjson_read(json,jsize,flg);
        if (pr.doc==nullptr) {
             return false;
        }
        yyjson_val *root = yyjson_doc_get_root(pr.doc);
        size_t arrn = yyjson_arr_size(root);
        if( arrn == 0) {
            return false;
        }
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
        yyjson_read_flag flg=YYJSON_READ_NOFLAG;
        pr.doc = yyjson_read(json,jsize,flg);
        if (pr.doc==nullptr) {
             return false;
        }
        yyjson_val *root = yyjson_doc_get_root(pr.doc);
        size_t arrn = yyjson_arr_size(root);
        if( arrn == 0) {
            return false;
        }
        auto v = yyjson_arr_get_first(root);
        if( v->tag != YYJSON_TYPE_STR ) {
            return false;
        }
        s = std::move(std::string(yyjson_get_str(v)));
        return true;
    }
#endif
};

REGISTER_TEST(YYjsonTest);
