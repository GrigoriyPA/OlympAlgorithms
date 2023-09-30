namespace alg::math {
    // -----------------------------------
    // vvv ------FastFourierTrick----- vvv


    class FastFourierTrick {
        static void fast_fourier_trick(std::vector<geom::Complex<double>>& vector, bool invert) {
            if (vector.size() == 1) {
                return;
            }

            std::vector<geom::Complex<double>> left(vector.size() / 2);
            std::vector<geom::Complex<double>> right(vector.size() / 2);
            for (size_t src = 0, dst = 0; src < vector.size(); src += 2, ++dst) {
                left[dst] = vector[src];
                right[dst] = vector[src + 1];
            }

            fast_fourier_trick(left, invert);
            fast_fourier_trick(right, invert);

            double angle = 2.0 * func::PI / (static_cast<double>(vector.size()) * (invert ? -1.0 : 1.0));
            geom::Complex<double> delta(geom::AngleRepresentation{ .length = 1.0, .angle = angle });
            geom::Complex<double> current(1.0);
            for (size_t i = 0; i < vector.size() / 2; ++i, current *= delta) {
                vector[i] = left[i] + current * right[i];
                vector[i + vector.size() / 2] = left[i] - current * right[i];

                if (invert) {
                    vector[i] /= 2.0;
                    vector[i + vector.size() / 2] /= 2.0;
                }
            }
        }

    public:
        template <typename T>  // Constructors required: T(0.0); Casts required: Complex<long double>(T)
        static std::vector<T> multiply(const std::vector<T>& left, const std::vector<T>& right) {
            std::vector<geom::Complex<double>> fourier_left(left.begin(), left.end());
            std::vector<geom::Complex<double>> fourier_right(right.begin(), right.end());

            size_t size = 1;
            for (; size < max(left.size(), right.size()); size *= 2) {}
            size *= 2;

            fourier_left.resize(size, geom::Complex<double>(0.0));
            fourier_right.resize(size, geom::Complex<double>(0.0));

            fast_fourier_trick(fourier_left, false);
            fast_fourier_trick(fourier_right, false);

            for (size_t i = 0; i < size; ++i) {
                fourier_left[i] *= fourier_right[i];
            }

            fast_fourier_trick(fourier_left, true);

            std::vector<T> result;
            result.reserve(size);
            for (size_t i = 0; i < size; ++i) {
                result.emplace_back(fourier_left[i].re);
            }

            return result;
        }
    };


    // ^^^------FastFourierTrick------ ^^^
    // -----------------------------------
}   // FastFourierTrick | Version: 0.0

using namespace alg::math;