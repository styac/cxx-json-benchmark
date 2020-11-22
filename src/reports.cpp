#include <fmt/format.h>
#include "reports.h"
#include <filesystem>
#include <fstream>

ReportBase::ReportBase( std::string_view dir )
    : m_dir(dir)
{}

ReportBase::~ReportBase()
{

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

