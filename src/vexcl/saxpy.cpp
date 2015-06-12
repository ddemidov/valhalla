#include <vector>
#include <vexcl/vexcl.hpp>
#include <valhalla.hpp>

namespace vll {

    template<typename T>
    void saxpy(const int num, bool gen = true, int iter = 0)
    {
        static vex::Context ctx(vex::Filter::Env && vex::Filter::Count(1));
        static vex::vector<T> x;
        static vex::vector<T> y;
        static T alpha = 3.5;

        if (gen) {
            x.resize(ctx, num);
            y.resize(ctx, num);

            x = vex::Random<T>()(vex::element_index(), rand());
            y = vex::Random<T>()(vex::element_index(), rand());

            ctx.finish();
        }

        for (int i = 0; i < iter; i++)
            y += alpha * x;

        ctx.finish();
    }

    template<typename T>
    void generate(const int num)
    {
        try {
            saxpy<T>(num, true, 0);
        } catch(vex::backend::error &err) {
            std::cout << err << std::endl;
            throw;
        }
    }

    template<typename T>
    void run(const int iter)
    {
        try {
            saxpy<T>(-1, false, iter);
        } catch(vex::backend::error &err) {
            std::cout << err << std::endl;
            throw;
        }
    }
}

int main(int argc, const char **args)
{
    vll::launch(argc, args);
}
