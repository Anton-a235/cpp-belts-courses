#pragma warning(disable : 4710)
#pragma warning(disable : 4711)

#include <cstdint>
#include <list>
#include <string>

#include "test_runner.h"

using namespace std;

class Editor
{
public:
    Editor()
        : cursor_(text_.end())
    {
    }

    void Left()
    {
        if (cursor_ != text_.begin())
        {
            --cursor_;
        }
    }

    void Right()
    {
        if (cursor_ != text_.end())
        {
            ++cursor_;
        }
    }

    void Insert(char token)
    {
        text_.insert(cursor_, token);
    }

    void Cut(size_t tokens = 1)
    {
        auto old_cursor = cursor_;

        while (cursor_ != text_.end() && tokens-- > 0)
        {
            ++cursor_;
        }

        buffer_.clear();
        buffer_.splice(buffer_.end(), text_, old_cursor, cursor_);
    }

    void Copy(size_t tokens = 1)
    {
        auto copy_to = cursor_;

        while (copy_to != text_.end() && tokens-- > 0)
        {
            ++copy_to;
        }

        buffer_.clear();
        copy(cursor_, copy_to, back_inserter(buffer_));
    }

    void Paste()
    {
        text_.insert(cursor_, buffer_.begin(), buffer_.end());
    }

    string GetText() const
    {
        return {text_.cbegin(), text_.cend()};
    }

private:
    std::list<char> text_;
    std::list<char> buffer_;
    std::list<char>::iterator cursor_;
};

void TypeText(Editor &editor, const string &text)
{
    for (char c : text)
    {
        editor.Insert(c);
    }
}

void TestEditing()
{
    {
        Editor editor;

        const size_t text_len = 12;
        const size_t first_part_len = 7;
        TypeText(editor, "hello, world");
        for (size_t i = 0; i < text_len; ++i)
        {
            editor.Left();
        }
        editor.Cut(first_part_len);
        for (size_t i = 0; i < text_len - first_part_len; ++i)
        {
            editor.Right();
        }
        TypeText(editor, ", ");
        editor.Paste();
        editor.Left();
        editor.Left();
        editor.Cut(3);

        ASSERT_EQUAL(editor.GetText(), "world, hello");
    }
    {
        Editor editor;

        TypeText(editor, "misprnit");
        editor.Left();
        editor.Left();
        editor.Left();
        editor.Cut(1);
        editor.Right();
        editor.Paste();

        ASSERT_EQUAL(editor.GetText(), "misprint");
    }
}

void TestReverse()
{
    Editor editor;

    const string text = "esreveR";
    for (char c : text)
    {
        editor.Insert(c);
        editor.Left();
    }

    ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText()
{
    Editor editor;
    ASSERT_EQUAL(editor.GetText(), "");

    editor.Left();
    editor.Left();
    editor.Right();
    editor.Right();
    editor.Copy(0);
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer()
{
    Editor editor;

    editor.Paste();
    TypeText(editor, "example");
    editor.Left();
    editor.Left();
    editor.Paste();
    editor.Right();
    editor.Paste();
    editor.Copy(0);
    editor.Paste();
    editor.Left();
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "example");
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestEditing);
    RUN_TEST(tr, TestReverse);
    RUN_TEST(tr, TestNoText);
    RUN_TEST(tr, TestEmptyBuffer);
    return 0;
}
