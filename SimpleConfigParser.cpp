#include "SimpleConfigParser.h"
#include <iomanip>

namespace
{
    bool quotePositionsAreValid(std::size_t openeningQuotePosition, std::size_t closingQuotePosition)
    {
        return openeningQuotePosition != std::string::npos && closingQuotePosition != openeningQuotePosition;
    }

    const char* blankChars = "\t\n\v\f\r ";
    void trim(std::string& str)
    {
        str.erase(0, str.find_first_not_of(blankChars));
        str.erase(str.find_last_not_of(blankChars) + 1);
    }

    void removeComment(std::string& str, std::size_t colonPosition)
    {
        // No need to remove comment from invalid line since
        // invalid lines are ignored
        if (colonPosition == std::string::npos)
        {
            return;
        }

        auto commentIndexStart = str.find('#');
        auto firstQuotationMarkPos = str.find('"', colonPosition);
        auto lastQuotationMarkPos = str.find_last_of('"');
        
        // No need to check for other '#' outsides quotes since it is ignored
        if (quotePositionsAreValid(firstQuotationMarkPos, lastQuotationMarkPos))
        {
            if (commentIndexStart > firstQuotationMarkPos && commentIndexStart < lastQuotationMarkPos)
            {
                // Dont erase comment if its inside quote
                return;
            }
        }

        // If there is a comment
        if (commentIndexStart != std::string::npos)
        {
            str.erase(commentIndexStart);
        }
    }

    struct OutpoutValue
    {
        OutpoutValue(std::ofstream& outputfile) : m_outputFile(outputfile) {}
        void operator()(long value) const {m_outputFile << value;}
        void operator()(double value) const {m_outputFile << value;}
        void operator()(bool value) const
        {
            switch (value)
            {
                case true:
                    m_outputFile << "true";
                    break;
                case false:
                    m_outputFile << "false";
                    break;
            }
        }
        void operator()(const std::string& value) const {m_outputFile << "\"" << value << "\"";}

        std::ofstream& m_outputFile;
    };

} // namespace

Config::Config()
    : m_values{}
{
}

std::optional<const std::variant<long, double, bool, std::string>*> Config::get(const std::string& key) const
{
    auto it = m_values.find(key);
    if (it != m_values.cend())
    {
        return {&(it->second)};
    }
    return {};
}

bool Config::parse(const std::string& filePath)
{
    std::ifstream inputFile(filePath);
    if (inputFile)
    {
        std::string line;
        while (std::getline(inputFile, line))
        {
            std::size_t colonPosition = line.find(":");
            removeComment(line, colonPosition);
            // Ignore empty lines
            if (line.empty() || colonPosition == std::string::npos)
            {
                continue;
            }

            std::string key = line.substr(0, colonPosition);
            std::string value = line.substr(colonPosition + 1);

            trim(key);
            trim(value);

            // Try to find " to see if value is string
            std::size_t openeningQuotePosition = value.find("\"");
            std::size_t closingQuotePosition = value.find_last_of("\"");
            // If there is two quotation marks in value
            if (quotePositionsAreValid(openeningQuotePosition, closingQuotePosition))
            {
                m_values.emplace(key, value.substr(openeningQuotePosition + 1, closingQuotePosition - openeningQuotePosition - 1));
                continue;
            }
            // Check if value is "true"
            if (value == "true")
            {
                m_values.emplace(key, true);
                continue;
            }
            // Check if value is "false"
            if (value == "false")
            {
                m_values.emplace(key, false);
                continue;
            }
            
            std::stringstream stream(value);

            // Check for long
            if (value.find('.') == std::string::npos)
            {
                // Could be a long
                long testLong;
                if (stream >> testLong)
                {
                    m_values.emplace(key, testLong);
                    continue;
                }
            }
            // Check for double
            else
            {
                // Could be a double
                double testDouble;
                if (stream >> testDouble)
                {
                    m_values.emplace(key, testDouble);
                    continue;
                }
            }

            // If nothing was found, default to string
            m_values.emplace(key, value);
        }
        return true;
    }
    return false;
}

bool Config::save(const std::string& filePath) const
{
    std::ofstream outputFile(filePath);
    if (outputFile)
    {
        outputFile << std::fixed << std::setprecision(10);
        OutpoutValue outputFunctions(outputFile);

        for(auto [key, val] : m_values)
        {
            outputFile << key << ": ";
            std::visit(outputFunctions, val);
            outputFile << '\n';
        }
        return true;
    }
    return false;
}