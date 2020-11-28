
#include "reports.h"

ReportBase::ReportBase( std::string_view dir )
    : m_dir(dir)
    , m_memleaks()
    , m_statistics()
{
    std::filesystem::path p(dir);
    m_memleaks.open( p / "memory_leaks.md" );
    m_statistics.open( p / "statistics.md" );
}

ReportBase::~ReportBase()
{
    m_memleaks.close();
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
        m_memleaks
                << "\n## pofilenametential memory leak allocation count " << alloc_count
                << " for " << stat.currentSize
                << " in test " << test
                << " / " << place
                << "\n    mallocCount  " << stat.mallocCount
                << "\n    reallocCount " << stat.reallocCount
                << "\n    freeCount    " << stat.freeCount
                << "\n    currentSize  " << stat.currentSize
                << "\n    peakSize     " << stat.peakSize
                << "\n";
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
    logf.print( "objectCount  {:10}  ref: {:10}\n", stat.objectCount, stat_reference.objectCount );
    logf.print( "arrayCount   {:10}  ref: {:10}\n", stat.arrayCount, stat_reference.arrayCount );
    logf.print( "numberCount  {:10}  ref: {:10}\n", stat.numberCount, stat_reference.numberCount );
    logf.print( "stringCount  {:10}  ref: {:10}\n", stat.stringCount, stat_reference.stringCount );
    logf.print( "trueCount    {:10}  ref: {:10}\n", stat.trueCount, stat_reference.trueCount );
    logf.print( "falseCount   {:10}  ref: {:10}\n", stat.falseCount, stat_reference.falseCount );
    logf.print( "nullCount    {:10}  ref: {:10}\n", stat.nullCount, stat_reference.nullCount );
    logf.print( "memberCount  {:10}  ref: {:10}\n", stat.memberCount, stat_reference.memberCount );
    logf.print( "elementCount {:10}  ref: {:10}\n", stat.elementCount, stat_reference.elementCount );
    logf.print( "stringLength {:10}  ref: {:10}\n", stat.stringLength, stat_reference.stringLength );
}
