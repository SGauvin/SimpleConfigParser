#include <optional>
#include <variant>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <sstream>

class Config
{
public:
    Config(const std::string filepath);
    ~Config();

    operator bool() const;
    std::optional<const std::variant<long, double, bool, std::string>*> get(const std::string& key) const;

private:

    void parse();

    std::unordered_map<std::string, std::variant<long, double, bool, std::string>> m_values;
    std::ifstream m_inputFile;
};