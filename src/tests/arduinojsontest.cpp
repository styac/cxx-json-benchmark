#include "../test.h"
#include "../memorystat_c.h"

#include <string>
#include <sstream>

//#define ARDUINOJSON_ENABLE_STD_STREAM 1
#include "ArduinoJson/src/ArduinoJson.h"

using namespace ArduinoJson;

static void GenStat(Stat& stat, const JsonVariant& v)
{
    if (v.is<const JsonArray>()) {
        JsonArray a = v.to<JsonArray>();
        for (auto itr = a.begin(); itr != a.end(); ++itr)
            GenStat(stat, *itr);
        stat.arrayCount++;
        stat.elementCount += a.size();
    } else if (v.is<const JsonObject&>()) {
        JsonObject o = v.to<JsonObject>();
        for (auto itr = o.begin(); itr != o.end(); ++itr) {
            GenStat(stat, itr->value());
            stat.stringLength += strlen(itr->key().c_str());
        }
        stat.objectCount++;
        stat.memberCount += o.size();
        stat.stringCount += o.size();
    } else if (v.is<const char*>()) {
            stat.stringCount++;
            stat.stringLength += strlen(v.as<const char*> ());
    } else if(v.isNull()) {
        stat.nullCount++; // JSON null value is treat as string null pointer
    } else if (v.is<long>() || v.is<double>()) {
        stat.numberCount++;
    } else if (v.is<bool>()) {
        if ((bool)v) {
            stat.trueCount++;
        } else {
            stat.falseCount++;
        }
    }
}

class ArduinojsonParseResult : public ParseResultBase {
public:
    ArduinojsonParseResult()
        : buffer(nullptr)
        , jsonBuffer(6553600)
    {}
    ~ArduinojsonParseResult() { free(buffer); }

    char* buffer;
    DynamicJsonDocument jsonBuffer;
    JsonVariant root;
};

class ArduinojsonStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return s.c_str(); }

    std::string s;
};

class ArduinojsonTest : public TestBase {
public:
#if TEST_INFO
    virtual const char* GetName() const override { return "Arduino Json (C++)"; }
    virtual const char* GetFilename() const override { return __FILE__; }
#endif

#if TEST_PARSE
    virtual ParseResultBase* Parse(const char* json, size_t length) const override
    {
        (void)length;
        ArduinojsonParseResult* pr = new ArduinojsonParseResult;
        pr->buffer = (char*)malloc(length);
        memcpy(pr->buffer, json, length);
        // Determine object or array
        for (size_t i = 0; i < length; i++) {            
            switch (json[i]) {
                case '{':
                    {
                        DeserializationError error = deserializeJson(pr->jsonBuffer,pr->buffer);
                        if (error) {
                            free(pr);
                            return 0;
                        }                        
                        pr->root = pr->jsonBuffer.as<JsonObject>();
                    }
                    return pr;
                case '[':
                    {
                        DeserializationError error = deserializeJson(pr->jsonBuffer,pr->buffer);
                        if (error) {
                            free(pr);
                            return 0;
                        }
                        pr->root = pr->jsonBuffer.as<JsonArray>();
                    }
                    return pr;
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    continue;
            }
            // Unknown first non-whitespace character
            break;
        }
        free(pr);
        return 0;
    }
#endif

#if TEST_STRINGIFY
    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const override
    {
        const ArduinojsonParseResult* pr = static_cast<const ArduinojsonParseResult*>(parseResult);
        ArduinojsonStringResult* sr = new ArduinojsonStringResult;
        serializeJson(pr->jsonBuffer, sr->s);
        return sr;
    }
#endif

#if TEST_PRETTIFY
    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const override
    {
        const ArduinojsonParseResult* pr = static_cast<const ArduinojsonParseResult*>(parseResult);
        ArduinojsonStringResult* sr = new ArduinojsonStringResult;
        serializeJsonPretty(pr->jsonBuffer, sr->s);
        return sr;
    }
#endif

#if TEST_STATISTICS
    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const override
    {
        const ArduinojsonParseResult* pr = static_cast<const ArduinojsonParseResult*>(parseResult);
        memset(stat, 0, sizeof(Stat));
        GenStat(*stat, pr->root);
        return true;
    }
#endif

#if TEST_CONFORMANCE
    virtual bool ParseDouble(const char* json, size_t length, double* d) const override
    {
        ArduinojsonParseResult pr;
        DeserializationError error = deserializeJson(pr.jsonBuffer,json,length);
        if (error) {
            return false;
        }
        auto a = pr.jsonBuffer.as<JsonArray>();
        if (a.size() == 1) {
            *d = a[0].as<double>();
            return true;
        }
        else
            return false;
    }

    virtual bool ParseString(const char* json, size_t length, std::string& s) const override
    {
        ArduinojsonParseResult pr;
        DeserializationError error = deserializeJson(pr.jsonBuffer,json,length);
        if (error) {
            return false;
        }
        auto a = pr.jsonBuffer.as<JsonArray>();
        if (a.size() == 1) {
            s = a[0].as<std::string>();
            return true;
        }
        else
            return false;
    }
#endif
};

REGISTER_TEST(ArduinojsonTest);
