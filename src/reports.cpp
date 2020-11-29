
#include "reports.h"
#include "resultfilename.h"

ReportBase::ReportBase( std::string_view dir )
    : m_dir(dir)
    , m_memleaks()
    , m_performance()
    , m_conformance()

{
    std::filesystem::path p(dir);
    m_memleaks.open( p / "memory_leaks.md" );
    m_performance.open( p / "performance_" RESULT_FILENAME );
    m_performance << fmt::format("Type,Library,Filename,Time (ms),Memory (byte),MemoryPeak (byte),AllocCount,FileSize (byte)\n");
    m_conformance.open( p / "conformance.csv" );
    m_conformance << fmt::format("Type,Library,Test,Result\n");
}

ReportBase::~ReportBase()
{
    m_memleaks.close();
    m_performance.close();
    m_conformance.close();
}

void ReportBase::print_test_names(sviewvec& test_vec)
{
    std::filesystem::path p(m_dir);
    p /= "active_test_names.md";

    auto logf = fmt::output_file(p.string());
    logf.print("# List of active tests\n\n");
    size_t index = 0;
    for(auto const & n : test_vec) {
        logf.print( "{}.  \"{}\"\n", ++index, n.data() );
    }
}

void ReportBase::print_memory_leaks( const char *test, const char *place)
{
    const MemoryStat& stat = Memory::Instance().GetStat();
    if (stat.currentSize != 0) {
        int64_t alloc_count = (int64_t)stat.mallocCount + stat.reallocCount - stat.freeCount;
        m_memleaks <<
            fmt::format("\n## potential memory leak - allocation count {} "
                " for {} in test {} / place {}\n"
                "    mallocCount  {}\n"
                "    reallocCount {}\n"
                "    freeCount    {}\n"
                "    currentSize  {}\n"
                "    peakSize     {}\n"
                , alloc_count
                , stat.currentSize
                , test
                , place
                , stat.mallocCount
                , stat.reallocCount
                , stat.freeCount
                , stat.currentSize
                , stat.peakSize
             );
    }
}

void ReportBase::print_statistics( std::string_view test_case, std::string_view test, const std::string &file, const Stat& stat, const Stat& stat_reference )
{
    std::filesystem::path p(m_dir);
    std::string filename(test_case);
    filename += test;
    filename += "-";
    filename += file;
    filename += ".md";
    p /= "statistics";
    p /= filename;

    auto logf = fmt::output_file(p.string());
    logf.print("# Statistics count is different from reference\n\n");
    logf.print( " objectCount  {:10}  ref: {:10}\n", stat.objectCount, stat_reference.objectCount );
    logf.print( " arrayCount   {:10}  ref: {:10}\n", stat.arrayCount, stat_reference.arrayCount );
    logf.print( " numberCount  {:10}  ref: {:10}\n", stat.numberCount, stat_reference.numberCount );
    logf.print( " stringCount  {:10}  ref: {:10}\n", stat.stringCount, stat_reference.stringCount );
    logf.print( " trueCount    {:10}  ref: {:10}\n", stat.trueCount, stat_reference.trueCount );
    logf.print( " falseCount   {:10}  ref: {:10}\n", stat.falseCount, stat_reference.falseCount );
    logf.print( " nullCount    {:10}  ref: {:10}\n", stat.nullCount, stat_reference.nullCount );
    logf.print( " memberCount  {:10}  ref: {:10}\n", stat.memberCount, stat_reference.memberCount );
    logf.print( " elementCount {:10}  ref: {:10}\n", stat.elementCount, stat_reference.elementCount );
    logf.print( " stringLength {:10}  ref: {:10}\n", stat.stringLength, stat_reference.stringLength );
}

void ReportBase::add_performance_statistics(std::string_view test, std::string_view test_case, std::string_view filename, double duration,
                                uint64_t currentSize, uint64_t peakSize, uint64_t mallocCount )
{
    m_performance <<
        fmt::format("{},{},{},{:f},{},{},{},0\n"
            , test
            , test_case
            , filename
            , duration
            , currentSize
            , peakSize
            , mallocCount
            // object size
         );
}

void ReportBase::add_conformance_statistics(std::string_view test, std::string_view test_case, std::string_view filename, bool result)
{
    m_conformance <<
        fmt::format("{},{},{},{}\n"
            , test
            , test_case
            , filename
            , result
         );
}
