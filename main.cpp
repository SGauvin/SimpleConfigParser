#include <iostream>
#include "SimpleConfigParser.h"

int main()
{
    Config config("test.txt");   

    // If the file opened correctly
    if (config)
    {        
        // If 'key1' exists in config
        if (std::optional<const std::variant<long, double, bool, std::string>*> optionalValue = config.get("key1"))
        {
            // If the optional value is a long
            if (const long* value = std::get_if<long>(*optionalValue))
            {                
                // Note that it is possible to use std::get instead of std::get_if but an exception
                // will be thrown if the conversion does not work
                std::cout << "The value of 'key1' is a long: " << *value << std::endl;
            }
        }
        
        /// Etc
        if (auto optionalValue = config.get("key2"))
        {
            if (const auto value = std::get_if<double>(*optionalValue))
            {
                std::cout << "The value of 'key2' is a double: " << *value << std::endl;
            }
        }
        
        if (auto optionalValue = config.get("key3"))
        {
            if (const auto value = std::get_if<bool>(*optionalValue))
            {
                std::cout << "The value of 'key3' is a bool: " << *value << std::endl;
            }
        }

        if (auto optionalValue = config.get("key4"))
        {
            if (const auto value = std::get_if<long>(*optionalValue))
            {
                std::cout << "The value of 'key4' is a bool: " << *value << std::endl;
            }
            else
            {
                std::cerr << "The value of key4 is not a bool! Unknown value!" << std::endl;
            }
        }

        if (auto optionalValue = config.get("key5"))
        {
            if (const auto value = std::get_if<std::string>(*optionalValue))
            {
                std::cout << "The value of 'key5' is a string: " << *value << std::endl;
            }
        }

        if (auto optionalValue = config.get("key6"))
        {
            if (const auto value = std::get_if<std::string>(*optionalValue))
            {
                std::cout << "The value of 'key6' is a string: " << *value << std::endl;
            }
        }

        if (const auto optionalValue = config.get("key7"))
        {
            std::cout << "This line should not execute since there is no key7 in the input file." << std::endl;
        }
        else
        {
            std::cerr << "Missing value!! There is no key7 in the input file" << std::endl;
        }
    }
} 