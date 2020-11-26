#pragma once

#include "test.h"

#include <string_view>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

typedef std::vector<std::string_view> sviewvec;

class ReportBase {
public:
    static constexpr char const * conformance_dir = "conformance";
    static constexpr char const * performance_dir = "performance";

    static auto& get_instance()
    {
        static ReportBase instance(REPORT_PATH);
        return instance;
    }

    ReportBase() = delete;

    ~ReportBase();

    void print_test_names(sviewvec& test_vec);

    void print_memory_leaks(const MemoryStat& stat, const char *test, const char *place);

    void print_conformance_statistics();
    void print_performance_statistics();
    void print_conformance_case();

    void add_conformance_statistics();
    void add_performance_statistics();
    void add_conformance_case();

private:
    explicit ReportBase( std::string_view dir );
    std::string     m_dir;
    std::ofstream   m_memleaks;
};
