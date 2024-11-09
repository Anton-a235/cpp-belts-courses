#include <iostream>

#include <algorithm>
#include <numeric>
#include <vector>

class FunctionPart
{
public:
    FunctionPart(char op, double val) noexcept
        : op_(op), val_(val)
    {
    }

    void Invert() noexcept
    {
        switch (op_)
        {
        case '+':
            op_ = '-';
            break;

        case '-':
            op_ = '+';
            break;

        case '*':
            op_ = '/';
            break;

        case '/':
            op_ = '*';
            break;

        default:
            // Invalid op_
            break;
        }
    }

    double Apply(double arg) const noexcept
    {
        switch (op_)
        {
        case '+':
            return arg + val_;
            break;

        case '-':
            return arg - val_;
            break;

        case '*':
            return arg * val_;
            break;

        case '/':
            return arg / val_;
            break;

        default:
            // Invalid op_
            return 0.0;
        }
    }

private:
    char op_;
    double val_;
};

class Function
{
public:
    void AddPart(char op, double val)
    {
        parts_.emplace_back(op, val);
    }

    void Invert()
    {
        for (auto &part : parts_)
        {
            part.Invert();
        }

        std::reverse(parts_.begin(), parts_.end());
    }

    double Apply(double init) const noexcept
    {
        return std::accumulate(parts_.cbegin(), parts_.cend(), init,
                               [](double res, const FunctionPart &f)
                               {
                                   return f.Apply(res);
                               });
    }

private:
    std::vector<FunctionPart> parts_;
};

// struct Image
// {
//     double quality;
//     double freshness;
//     double rating;
// };

// struct Params
// {
//     double a;
//     double b;
//     double c;
// };

// Function MakeWeightFunction(const Params &params,
//                             const Image &image)
// {
//     Function function;
//     function.AddPart('*', params.a);
//     function.AddPart('-', image.freshness * params.b);
//     function.AddPart('+', image.rating * params.c);
//     return function;
// }

// double ComputeImageWeight(const Params &params, const Image &image)
// {
//     Function function = MakeWeightFunction(params, image);
//     return function.Apply(image.quality);
// }

// double ComputeQualityByWeight(const Params &params,
//                               const Image &image,
//                               double weight)
// {
//     Function function = MakeWeightFunction(params, image);
//     function.Invert();
//     return function.Apply(weight);
// }

// int main()
// {
//     Image image = {10, 2, 6};
//     Params params = {4, 2, 6};
//     std::cout << ComputeImageWeight(params, image) << std::endl;
//     std::cout << ComputeQualityByWeight(params, image, 52) << std::endl;
//     return 0;
// }
