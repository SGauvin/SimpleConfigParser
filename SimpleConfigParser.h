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
    Config();
    
    std::optional<const std::variant<long, double, bool, std::string>*> get(const std::string& key) const;

    bool parse(const std::string& filePath);
    bool save(const std::string& filePath) const;

    template<typename T>
    auto& operator[](const T& key) {return m_values[key];}

    template<typename T>
    auto& operator[](T&& key) {return m_values[key];}

private:

    std::unordered_map<std::string, std::variant<long, double, bool, std::string>> m_values;
};