#pragma once

#include <istream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace Json
{

class Node
{
public:
    explicit Node(std::vector<Node> array);
    explicit Node(std::map<std::string, Node> map);
    explicit Node(int value);
    explicit Node(std::string value);

    const std::vector<Node> &AsArray() const;
    const std::map<std::string, Node> &AsMap() const;
    int AsInt() const;
    const std::string &AsString() const;

private:
    std::vector<Node> as_array;
    std::map<std::string, Node> as_map;
    int as_int;
    std::string as_string;
};

class Document
{
public:
    explicit Document(Node root);

    const Node &GetRoot() const;

private:
    Node root;
};

Document Load(std::istream &input);

Node::Node(std::vector<Node> array) : as_array(std::move(array))
{
}

Node::Node(std::map<std::string, Node> map) : as_map(std::move(map))
{
}

Node::Node(int value) : as_int(value)
{
}

Node::Node(std::string value) : as_string(std::move(value))
{
}

const std::vector<Node> &Node::AsArray() const
{
    return as_array;
}

const std::map<std::string, Node> &Node::AsMap() const
{
    return as_map;
}

int Node::AsInt() const
{
    return as_int;
}

const std::string &Node::AsString() const
{
    return as_string;
}

Document::Document(Node root) : root(std::move(root))
{
}

const Node &Document::GetRoot() const
{
    return root;
}

Node LoadNode(std::istream &input);

Node LoadArray(std::istream &input)
{
    std::vector<Node> result;

    for (char c; input >> c && c != ']';)
    {
        if (c != ',')
        {
            input.putback(c);
        }
        result.push_back(LoadNode(input));
    }

    return Node(std::move(result));
}

Node LoadInt(std::istream &input)
{
    int result = 0;
    while (isdigit(input.peek()))
    {
        result *= 10;
        result += input.get() - '0';
    }
    return Node(result);
}

Node LoadString(std::istream &input)
{
    std::string line;
    getline(input, line, '"');
    return Node(std::move(line));
}

Node LoadDict(std::istream &input)
{
    std::map<std::string, Node> result;

    for (char c; input >> c && c != '}';)
    {
        if (c == ',')
        {
            input >> c;
        }

        std::string key = LoadString(input).AsString();
        input >> c;
        result.insert({std::move(key), LoadNode(input)});
    }

    return Node(std::move(result));
}

Node LoadNode(std::istream &input)
{
    char c;
    input >> c;

    if (c == '[')
    {
        return LoadArray(input);
    }
    else if (c == '{')
    {
        return LoadDict(input);
    }
    else if (c == '"')
    {
        return LoadString(input);
    }
    else
    {
        input.putback(c);
        return LoadInt(input);
    }
}

Document Load(std::istream &input)
{
    return Document{LoadNode(input)};
}

} // namespace Json
