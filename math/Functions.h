namespace alg::math {
    // -----------------------------------
    // vvv ---------Functions--------- vvv


    template <typename T1, typename T2>  // Constructors required: T1(T1); Operators required: *(T1, T1), =(T1, T1), %(T1, T2)
    T1 binary_exponentiation_modul(const T1& base, uint32_t degree, const T2& modul, const T1& one = T1(1)) {
        T1 result(one);
        for (size_t i = 8 * sizeof(uint32_t); i > 0; --i) {
            result = (result * result) % modul;

            if (degree & (static_cast<uint32_t>(1) << (i - 1))) {
                result = (result * base) % modul;
            }
        }
        return result;
    }

    template <typename T1, typename T2>  // Constructors required: T1(T1); Casts required: uint32_t(T2); Operators required: -(T2, int), *(T1, T1), =(T1, T1), %(T1, T2)
    T1 inverse(const T1& value, const T2& modul, const T1& one = T1(1)) {
        return binary_exponentiation_modul(value, uint32_t(modul - 2), modul, one);
    }

    std::vector<std::pair<uint64_t, uint32_t>> factorization(uint64_t number) {
        std::vector<std::pair<uint64_t, uint32_t>> result;
        result.reserve(8 * sizeof(number));
        for (uint64_t divider = 2; divider * divider <= number; ++divider) {
            if (number % divider != 0) {
                continue;
            }

            result.emplace_back(divider, 0);
            for (; number % divider == 0; number /= divider) {
                ++result.back().second;
            }
        }

        if (number > 1) {
            result.emplace_back(number, 1);
        }
        return result;
    }

    bool is_prime(uint64_t number) noexcept {
        if (number <= 1) {
            return false;
        }

        for (uint64_t divider = 2; divider * divider <= number; ++divider) {
            if (number % divider == 0) {
                return false;
            }
        }

        return true;
    }

    template <typename T>  // Operators required: %(uint64_t, T)
    uint64_t factorial(uint64_t number, const T& modul) {
        uint64_t result = 1;
        for (uint64_t i = 1; i <= number; ++i) {
            result = (result * i) % modul;
        }
        return result;
    }

    template <typename T>  // Operators required: %(uint64_t, T)
    std::vector<uint64_t> factorials(uint64_t number, const T& modul) {
        std::vector<uint64_t> result(1, 1);
        result.reserve(number + 1);
        for (uint64_t i = 1; i <= number; ++i) {
            result.push_back((result.back() * i) % modul);
        }
        return result;
    }

    uint64_t factorial(uint64_t number) noexcept {
        uint64_t result = 1;
        for (uint64_t i = 1; i <= number; ++i) {
            result *= i;
        }
        return result;
    }


    // ^^^ ---------Functions--------- ^^^
    // -----------------------------------
}   // Functions | Version: 1.0

using namespace alg::math;