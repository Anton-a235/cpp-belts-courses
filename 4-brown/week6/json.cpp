#include "json.h"

using namespace std;

namespace Json
{

Document::Document(Node root)
    : root(move(root))
{
}

const Node& Document::GetRoot() const
{
    return root;
}

Node LoadNode(istream& input);

Node LoadArray(istream& input)
{
    vector<Node> result;

    for (char c; input >> c && c != ']';)
    {
        if (c != ',')
        {
            input.putback(c);
        }
        result.push_back(LoadNode(input));
    }

    return Node(move(result));
}

Node LoadBool(istream& input)
{
    bool result;
    input >> std::boolalpha >> result;
    return Node(result);
}

Node LoadInt(istream& input)
{
    int result = 0;
    input >> result;
    return Node(result);
}

Node LoadDouble(istream& input, int int_part)
{
    double result = 0.0;
    input >> result;
    return Node(result + int_part);
}

Node LoadString(istream& input)
{
    string line;
    getline(input, line, '"');
    return Node(move(line));
}

Node LoadDict(istream& input)
{
    map<string, Node> result;

    for (char c; input >> c && c != '}';)
    {
        if (c == ',')
        {
            input >> c;
        }

        string key = LoadString(input).AsString();
        input >> c;
        result.emplace(move(key), LoadNode(input));
    }

    return Node(move(result));
}

Node LoadNode(istream& input)
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
    else if (c == 't' || c == 'f')
    {
        input.putback(c);
        return LoadBool(input);
    }
    else
    {
        input.putback(c);
        Node result = LoadInt(input);

        if (input.peek() == '.')
            result = LoadDouble(input, result.AsInt());

        return result;
    }
}

Document Load(istream& input)
{
    return Document{LoadNode(input)};
}

} // namespace Json
