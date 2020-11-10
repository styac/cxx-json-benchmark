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
    virtual const char* GetName() const { return "yyjson (C)"; }
    virtual const char* GetFilename() const { return __FILE__; }
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
    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const {
        const YYjsonParseResult* pr = static_cast<const YYjsonParseResult*>(parseResult);
        memset(stat, 0, sizeof(Stat));
        GenStat(stat, pr->doc);
        return true;
    }
#endif

#if TEST_CONFORMANCE
    virtual bool ParseDouble(const char* json, double* d) const override {
        YYjsonParseResult pr;
        yyjson_read_flag flg=YYJSON_READ_NOFLAG;
        pr.doc = yyjson_read(json,sizeof(double),flg);
        if (pr.doc==nullptr) {
             return false;
        }
//        juson_value_t* root = juson_parse(&pr.doc, json);
//        if (root && root->t == JUSON_ARRAY && root->size &&
//            (root->adata[0]->t == JUSON_FLOAT || root->adata[0]->t == JUSON_INTEGER)) {
//            if (root->adata[0]->t == JUSON_FLOAT)
//                *d = root->adata[0]->fval;
//            else
//                *d = root->adata[0]->ival;
//            return true;
//        }
//        else
            return false;
    }

    // const char* json should be std::string
    virtual bool ParseString(const char* json, std::string& s) const {
        YYjsonParseResult pr;
        yyjson_read_flag flg=YYJSON_READ_NOFLAG;
        // extra time added
        auto length = strlen(json);
        pr.doc = yyjson_read(json,length,flg);
        if (pr.doc==nullptr) {
             return false;
        }
//        juson_value_t* root = juson_parse(&pr.doc, json);
//        if (root && root->t == JUSON_ARRAY && root->size && root->adata[0]->t == JUSON_STRING) {
//            s = std::string(root->adata[0]->sval, root->adata[0]->len);
//            return true;
//        }
//        else
            return false;
    }
#endif
};

REGISTER_TEST(YYjsonTest);
