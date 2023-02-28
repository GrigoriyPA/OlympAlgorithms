namespace alg::math {
    // -----------------------------------
    // vvv ---------Functions--------- vvv


    template <typename T>  // Constructors required: T(T); Operators required: *(T, T), =(T, T), %(T, T)
    T binary_exponentiation_modul(const T& base, uint32_t degree, const T& modul, const T& one = T(1)) {
        T result(one);
        for (size_t i = 8 * sizeof(uint32_t); i > 0; --i) {
            result = (result * result) % modul;

            if (degree & (static_cast<uint32_t>(1) << (i - 1))) {
                result = (result * base) % modul;
            }
        }
        return result;
    }

    template <typename T>  // Constructors required: T(T); Casts required: uint32_t(T); Operators required: -(T, int), *(T, T), =(T, T), %(T, T)
    T inverse(const T& value, const T& modul, const T& one = T(1)) {
        return binary_exponentiation_modul(value, uint32_t(modul - 2), modul, one);
    }

    std::vector<std::pair<uint64_t, uint32_t>> factorization(uint64_t value) {
        std::vector<std::pair<uint64_t, uint32_t>> result;
        result.reserve(8 * sizeof(value));
        for (uint64_t divider = 2; divider * divider <= value; ++divider) {
            if (value % divider != 0) {
                continue;
            }

            result.emplace_back(divider, 0);
            for (; value % divider == 0; value /= divider) {
                ++result.back().second;
            }
        }

        if (value > 1) {
            result.emplace_back(value, 1);
        }
        return result;
    }


    // ^^^ ---------Functions--------- ^^^
    // -----------------------------------
}   // Functions | Version: 1.0

using namespace alg::math;