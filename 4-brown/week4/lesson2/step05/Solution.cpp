#include "Common.h"

using namespace std;

class Shape : public IShape
{
public:
    virtual std::unique_ptr<IShape> Clone() const = 0;

    void SetPosition(Point p) override
    {
        position_ = std::move(p);
    }

    Point GetPosition() const override
    {
        return position_;
    }

    void SetSize(Size size) override
    {
        size_ = std::move(size);
    }

    Size GetSize() const override
    {
        return size_;
    }

    void SetTexture(std::shared_ptr<ITexture> t) override
    {
        texture_ = std::move(t);
    }

    ITexture *GetTexture() const override
    {
        return texture_.get();
    }

    void Draw(Image &image) const override
    {
        for (int y = position_.y; y < std::min<int>(position_.y + size_.height, image.size()); ++y)
        {
            for (int x = position_.x; x < std::min<int>(position_.x + size_.width, image[y].size()); ++x)
            {
                if (IsPointInShape({x - position_.x, y - position_.y}, size_))
                    image[y][x] = '.';
            }
        }

        if (texture_)
        {
            const auto &texture_image = texture_->GetImage();
            const auto texture_size = texture_->GetSize();

            for (int y = 0; y < std::min(texture_size.height, size_.height); ++y)
            {
                int abs_y = position_.y + y;
                if (abs_y >= static_cast<int>(image.size()))
                    break;

                for (int x = 0; x < std::min(texture_size.width, size_.width); ++x)
                {
                    int abs_x = position_.x + x;
                    if (abs_x >= static_cast<int>(image[abs_y].size()))
                        break;

                    if (IsPointInShape({x, y}, size_))
                        image[abs_y][abs_x] = texture_image[y][x];
                }
            }
        }
    }

protected:
    std::shared_ptr<ITexture> texture_{};
    Point position_{};
    Size size_{};

    virtual bool IsPointInShape(Point p, Size size) const = 0;
};

class Rectangle : public Shape
{
public:
    std::unique_ptr<IShape> Clone() const override
    {
        return make_unique<Rectangle>(*this);
    }

private:
    bool IsPointInShape(Point p, Size size) const override
    {
        return p.x >= 0 && p.y >= 0 && p.x < size.width && p.y < size.height;
    }
};

class Ellipse : public Shape
{
public:
    std::unique_ptr<IShape> Clone() const override
    {
        return make_unique<Ellipse>(*this);
    }

private:
    bool IsPointInShape(Point p, Size size) const override
    {
        return IsPointInEllipse(std::move(p), std::move(size));
    }
};

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type)
{
    switch (shape_type)
    {
    case ShapeType::Rectangle:
        return make_unique<Rectangle>();
        break;

    case ShapeType::Ellipse:
        return make_unique<Ellipse>();
        break;

    default:
        return {};
        break;
    }
}
