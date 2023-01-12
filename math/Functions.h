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

    template <typename T>  // Constructors required: T(T); Operators required: -(T, int), *(T, T), =(T, T), %(T, T)
    T inverse(const T& value, const T& modul, const T& one = T(1)) {
        return binary_exponentiation_modul(value, modul - 2, modul, one);
    }


    // ^^^ ---------Functions--------- ^^^
    // -----------------------------------
}   // Functions

using namespace alg::math;