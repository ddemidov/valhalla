#include <vector>
#include <vexcl/vexcl.hpp>
#include <valhalla.hpp>

namespace vll {

    template<typename T>
    void scan(const int num, bool gen = true, int iter = 0)
    {
        static vex::Context ctx(vex::Filter::Env && vex::Filter::Count(1));
        static vex::vector<T> d_in;
        static vex::vector<T> d_out;

        if (gen) {
            d_in.resize(ctx, num);
            d_out.resize(ctx, num);

            d_in = vex::Random<T>()(vex::element_index(), rand());

            ctx.finish();
        }

        for (int i = 0; i < iter; i++)
            vex::inclusive_scan(d_in, d_out);

        ctx.finish();
    }

    template<typename T>
    void generate(const int num)
    {
        try {
            scan<T>(num, true, 0);
        } catch(vex::backend::error &err) {
            std::cout << err << std::endl;
            throw;
        }
    }

    template<typename T>
    void run(const int iter)
    {
        try {
            scan<T>(-1, false, iter);
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
