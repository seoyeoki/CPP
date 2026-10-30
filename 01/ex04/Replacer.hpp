#ifndef REPLACER_HPP
#define REPLACER_HPP

#include <string>
#include <iostream>
#include <fstream>

class Replacer
{
public:
    Replacer(const std::string &filename, const std::string &s1, const std::string &s2);
    ~Replacer(void);

    bool run(void) const;

private:
    std::string _filename;
    std::string _s1;
    std::string _s2;

    std::string replaceAll(const std::string &content) const;
};

#endif