#include <fmt/format.h>
#include "reports.h"

ReportBase::ReportBase( std::string_view dir )
    : m_dir(dir)
    , m_memleaks()
{
    std::filesystem::path p(dir);
    m_memleaks.open( p / "memory_leaks.md" );
}

ReportBase::~ReportBase()
{
    m_memleaks.close();
}

void ReportBase::print_test_names(sviewvec& test_vec)
{
    std::filesystem::path p(m_dir);
    p /= "active_test_names.md";

    std::ofstream logf(p);
    if( logf) {
        logf << "# List of active tests\n\n" << std::endl;
        size_t index = 0;
        for(auto const & n : test_vec) {
            logf << ++index << ".   \"" << n.data() << "\"" << std::endl;
        }
    }
}

void ReportBase::print_memory_leaks(const MemoryStat& stat, const char *test, const char *place)
{
    if (stat.currentSize != 0) {

        int64_t alloc_count = (int64_t)stat.mallocCount + stat.reallocCount - stat.freeCount;
        m_memleaks
                << "\n## potential memory leak allocation count " << alloc_count
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
