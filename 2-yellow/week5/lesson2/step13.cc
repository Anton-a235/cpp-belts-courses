#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person
{
public:
    Person(const string &name, const string &type)
        : Name(name), Type(type)
    {
    }

    virtual ~Person() = default;

    virtual void Walk(const string &destination) const = 0;

    const string &GetName() const
    {
        return Name;
    }

    const string &GetType() const
    {
        return Type;
    }

protected:
    string Name;
    string Type;
};

class Student : public Person
{
public:
    Student(const string &name, const string &favouriteSong)
        : Person(name, "Student"), FavouriteSong(favouriteSong)
    {
    }

    void Learn() const
    {
        cout << "Student: " << Name << " learns" << endl;
    }

    void Walk(const string &destination) const override
    {
        cout << "Student: " << Name << " walks to: " << destination << endl;
        cout << "Student: " << Name << " sings a song: " << FavouriteSong << endl;
    }

    void SingSong() const
    {
        cout << "Student: " << Name << " sings a song: " << FavouriteSong << endl;
    }

private:
    string FavouriteSong;
};

class Teacher : public Person
{
public:
    Teacher(const string &name, const string &subject)
        : Person(name, "Teacher"), Subject(subject)
    {
    }

    void Teach() const
    {
        cout << "Teacher: " << Name << " teaches: " << Subject << endl;
    }

    void Walk(const string &destination) const override
    {
        cout << "Teacher: " << Name << " walks to: " << destination << endl;
    }

private:
    string Subject;
};

class Policeman : public Person
{
public:
    Policeman(const string &name)
        : Person(name, "Policeman")
    {
    }

    void Check(const Person &p) const
    {
        cout << "Policeman: " << Name << " checks " << p.GetType() << ". "
             << p.GetType() << "'s name is: " << p.GetName() << endl;
    }

    void Walk(const string &destination) const override
    {
        cout << "Policeman: " << Name << " walks to: " << destination << endl;
    }
};

void VisitPlaces(const Person &t, const vector<string> &places)
{
    for (const auto &p : places)
    {
        t.Walk(p);
    }
}

int main()
{
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    VisitPlaces(s, {"Moscow", "London"});

    return 0;
}
