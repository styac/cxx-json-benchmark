#include <string_view>
#include <string>
#include <vector>

typedef std::vector<std::string_view> sviewvec;

class ReportBase {
public:
    static constexpr char const * conformance_dir = "conformance";
    static constexpr char const * performance_dir = "performance";
    static constexpr char const * memory_leaks_dir = "memory_leaks";

    static auto get_instance()
    {
        static ReportBase instance(REPORT_PATH);
        return instance;
    }

    ReportBase() = delete;
    ~ReportBase();

    void print_test_names(sviewvec& test_vec);

    void print_memory_leaks();
    void print_conformance_statistics();
    void print_performance_statistics();
    void print_conformance_case();

    void add_memory_leaks();
    void add_conformance_statistics();
    void add_performance_statistics();
    void add_conformance_case();

private:
    explicit ReportBase( std::string_view dir );
    std::string m_dir;
};
