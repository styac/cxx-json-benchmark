#include <string_view>

class ReportBase {
public:
    ReportBase() = delete;
    explicit ReportBase( std::string_view dir );
    ~ReportBase();

private:
    std::string m_dir;
};
