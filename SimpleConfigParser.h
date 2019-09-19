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
    /// Constructor
    Config();
    
    /// Search for an element with a key equivalent to key and return it if it exists
    /// \param key  Key to be searched for
    std::optional<const std::variant<long, double, bool, std::string>*> get(const std::string& key) const;

    /// Parse file. Note that any previous data is erased and any pointer to old data is invalidated.
    /// \param filePath Filepath of the config
    /// \return True if the file opened correctly
    bool parse(const std::string& filePath);

    /// Save the config in a file.
    /// \param filePath Filepath of the config file
    /// \return True if the file opened correctly
    bool save(const std::string& filePath) const;

    /// Clear the config
    void clear() {m_values.clear();}

    /// Remove from config a single element identified by its key
    /// \param key The key identifying the element to be removed
    void erase(const std::string& key) {m_values.erase(key);}

    /// Access an element in the config. If key matches the key of an element in the config,
    /// the function returns a reference to its mapped value. If k does not match the key of
    /// any element in the config, the function inserts a new element with that key and returns
    /// a reference to its mapped value.
    /// \param key  Key value of the element whose mapped value is accessed
    /// \return     A reference to the mapped value of the element with a key value equals to key
    template<typename T>
    auto& operator[](const T& key) {return m_values[key];}

    /// Access an element in the config. If key matches the key of an element in the config,
    /// the function returns a reference to its mapped value. If k does not match the key of
    /// any element in the config, the function inserts a new element with that key and returns
    /// a reference to its mapped value.
    /// \param key  Key value of the element whose mapped value is accessed
    /// \return     A reference to the mapped value of the element with a key value equals to key
    template<typename T>
    auto& operator[](T&& key) {return m_values[std::forward<T>(key)];}

private:

    std::unordered_map<std::string, std::variant<long, double, bool, std::string>> m_values;
};