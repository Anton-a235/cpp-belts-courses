#include <string>

#pragma warning(disable : 5267)

class Animal
{
public:
    virtual std::string Voice() const
    {
        return "Not implemented yet";
    }
    virtual ~Animal()
    {
    }
};

class Tiger : public Animal
{
    std::string Voice() const override
    {
        return "Rrrr";
    }
};

class Wolf : public Animal
{
    std::string Voice() const override
    {
        return "Wooo";
    }
};

class Fox : public Animal
{
    std::string Voice() const override
    {
        return "Tyaf";
    }
};

#pragma warning(error : 5267)
