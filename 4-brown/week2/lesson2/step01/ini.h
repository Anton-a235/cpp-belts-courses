#pragma once

#include <istream>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Ini
{

using Section = std::unordered_map<std::string, std::string>;

class Document
{
public:
    Section &AddSection(std::string name);
    const Section &GetSection(const std::string &name) const;
    size_t SectionCount() const;

private:
    std::unordered_map<std::string, Section> sections;
};

Document Load(std::istream &input);

} // namespace Ini
