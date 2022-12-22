#include "../Algorithms/Algorithms.h"
#define DEBUG

signed main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0); cout.precision(30);

#ifdef DEBUG
    Random gen(1);
    for (uint64_t seed = 1; true; ++seed) {
        gen.set_seed(seed);
        if (seed % 1 == 0) {
            std::cout << "Seed: " << seed << "\n";
        }



        if (!(false)) {
            std::cout << "DEBUG, invalid behavior detected on seed " << seed << ".\n";
            throw AlgRuntimeError(__FILE__, __LINE__, "DEBUG, invalid behavior detected.\n");
        }
    }
#else  // DEBUG

#endif  // NO DEBUG

    return 0;
}
