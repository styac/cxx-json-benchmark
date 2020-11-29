#pragma once

#include "test.h"

#include <string_view>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

#include <fmt/format.h>
#include <fmt/os.h>

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

    void print_memory_leaks( const char *test, const char *place );

    void print_statistics( std::string_view test_case, std::string_view test, const std::string &file, const Stat& stat, const Stat& stat_reference );

    void print_performance( std::string_view test_case, std::string_view test, const std::string &file, const Stat& stat, const Stat& stat_reference );

    void add_performance_statistics(std::string_view test, std::string_view test_case, std::string_view filename, double duration,
                                    uint64_t currentSize, uint64_t peakSize, uint64_t mallocCount );

    void add_conformance_statistics(std::string_view test, std::string_view test_case, std::string_view filename, bool result);


private:
    explicit ReportBase( std::string_view dir );
    std::string     m_dir;
    std::ofstream   m_memleaks;
    std::ofstream   m_performance;
    std::ofstream   m_conformance;
};
