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

Section &Document::AddSection(std::string name)
{
    return sections[std::move(name)];
}

const Section &Document::GetSection(const std::string &name) const
{
    return sections.at(name);
}

size_t Document::SectionCount() const
{
    return sections.size();
}

Document Load(std::istream &input)
{
    Document doc;
    Section *cur_section{};

    for (std::string line; std::getline(input, line);)
    {
        if (line.length() == 0)
        {
            continue;
        }

        std::string_view line_view(line);

        if (line.front() == '[' && line.back() == ']')
        {
            cur_section = &doc.AddSection(line.substr(1, line.length() - 2));
        }
        else
        {
            const size_t kv_separator = line_view.find('=');
            cur_section->emplace(line.substr(0, kv_separator), line.substr(kv_separator + 1));
        }
    }

    return doc;
}

} // namespace Ini
