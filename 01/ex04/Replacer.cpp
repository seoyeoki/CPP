#include "Replacer.hpp"

Replacer::Replacer(const std::string &filename, const std::string &s1, const std::string &s2) : _filename(filename), _s1(s1), _s2(s2)
{}

Replacer::~Replacer(void)
{}

bool Replacer::run(void) const
{
    if (_filename == "" || _s1 == "")
    {
        std::cout << "Invalid input" << std::endl;

        return false;
    }

    std::ifstream in(_filename.c_str());
    if (!in.is_open())
    {
        std::cout << "Cannot open input file" << std::endl;
        return false;
    }

    in.seekg(0, std::ios::end);
    std::streamoff size = in.tellg();
    in.seekg(0, std::ios::beg);

    std::string content;
    std::string line;

    while (std::getline(in, line))
    {
        content += line;
        if (!in.eof())
            content += '\n';
    }
    // A directory opens fine but reads as empty: its reported size never matches what was read.
    if (in.bad() || (size > 0 && static_cast<std::streamoff>(content.length()) != size))
    {
        std::cout << "Cannot read input file" << std::endl;
        return false;
    }

    std::ofstream out((_filename + ".replace").c_str());
    if (!out.is_open())
    {
        std::cout << "Cannot create output file" << std::endl;
        return false;
    }

    out << replaceAll(content);

    return true;
}

std::string Replacer::replaceAll(const std::string &content) const
{
    std::string result;
    std::string::size_type pos = 0;
    std::string::size_type found;

    while ((found = content.find(_s1, pos)) != std::string::npos)
    {
        result += content.substr(pos, found - pos);
        result += _s2;
        pos = found + _s1.length();
    }
    result += content.substr(pos);

    return result;
}