#include <vector>
#include <vexcl/vexcl.hpp>
#include <valhalla.hpp>

namespace vll {

    template<typename T>
    void reduce(const int num, bool gen = true, int iter = 0)
    {
        static vex::Context ctx(vex::Filter::Env && vex::Filter::Count(1));
        static vex::Reductor<T> sum(ctx);
        static vex::vector<T> d_vec;

        if (gen) {
            d_vec.resize(ctx, num);

            d_vec = vex::Random<T>()(vex::element_index(), rand());

            ctx.finish();
        }

        for (int i = 0; i < iter; i++)
            T x = sum(d_vec);

        ctx.finish();
    }

    template<typename T>
    void generate(const int num)
    {
        try {
            reduce<T>(num, true, 0);
        } catch(vex::backend::error &err) {
            std::cout << err << std::endl;
            throw;
        }
    }

    template<typename T>
    void run(const int iter)
    {
        try {
            reduce<T>(-1, false, iter);
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
