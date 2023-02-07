namespace alg {
    // -----------------------------------
    // vvv ----------Complex---------- vvv


    namespace geom {
        struct AngleRepresentation {
            long double length = 0.0;
            long double angle = 0.0;
        };

        template <typename VT = long double>  // VT - standard numeric type
        class Complex {
        public:
            VT re = VT(0.0);
            VT im = VT(0.0);

            Complex() noexcept {
            }

            explicit Complex(VT value) noexcept {
                re = value;
            }

            // Casts required: VT(long double)
            Complex(const AngleRepresentation& complex) {
                re = VT(complex.length * std::cosl(complex.angle));
                im = VT(complex.length * std::sinl(complex.angle));
            }

            template <typename T>  // Casts required: VT(T)
            explicit Complex(const Vec2<T>& vect) {
                re = VT(vect.x);
                im = VT(vect.y);
            }

            Complex(VT re, VT im) noexcept {
                this->re = re;
                this->im = im;
            }

            template <typename T>  // Casts required: VT(T)
            Complex(const std::initializer_list<T>& init) {
                size_t comp_id = 0;
                for (const T& element : init) {
                    (*this)[comp_id] = VT(element);

                    if (++comp_id == 2) {
                        break;
                    }
                }
            }

            template <typename T>  // Casts required: VT(T)
            explicit Complex(const std::vector<T>& init) {
                for (size_t comp_id = 0; comp_id < std::min(static_cast<size_t>(2), init.size()); ++comp_id) {
                    (*this)[comp_id] = VT(init[comp_id]);
                }
            }

            operator AngleRepresentation() const {
                long double length = module();
                if (func::equality(length, static_cast<long double>(0.0))) {
                    throw func::AlgDomainError(__FILE__, __LINE__, "operator AngleRepresentation, zero number cannot be represented in the form of an angle.\n\n");
                }

                return { .length = length, .angle = atan2(im, re) };
            }

            template <typename T>  // Constructors required: T(T), T(VT)
            explicit operator Vec2<T>() const {
                return Vec2<T>(T(re), T(im));
            }

            template <typename T>  // Constructors required: T(T), T(VT)
            explicit operator std::vector<T>() const {
                return { T(re), T(im) };
            }

            explicit operator std::string() const {
                return func::to_string(*this);
            }

            VT& operator[](size_t index) {
                if (index == 0) {
                    return re;
                }
                if (index == 1) {
                    return im;
                }
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            const VT& operator[](size_t index) const {
                if (index == 0) {
                    return re;
                }
                if (index == 1) {
                    return im;
                }
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            bool operator==(const Complex<VT>& other) const noexcept {
                return func::equality(re, other.re) && func::equality(im, other.im);
            }

            bool operator!=(const Complex<VT>& other) const noexcept {
                return !func::equality(re, other.re) || !func::equality(im, other.im);
            }

            Complex<VT>& operator+=(const Complex<VT>& other)& noexcept {
                re += other.re;
                im += other.im;
                return *this;
            }

            Complex<VT>& operator+=(const VT& other)& noexcept {
                re += other;
                return *this;
            }

            Complex<VT>& operator-=(const Complex<VT>& other)& noexcept {
                re -= other.re;
                im -= other.im;
                return *this;
            }

            Complex<VT>& operator-=(const VT& other)& noexcept {
                re -= other;
                return *this;
            }

            Complex<VT>& operator*=(const Complex<VT>& other)& noexcept {
                *this = *this * other;
                return *this;
            }

            Complex<VT>& operator*=(const VT& other)& noexcept {
                re *= other;
                im *= other;
                return *this;
            }

            Complex<VT>& operator/=(const Complex<VT>& other)& {
                VT divisor = other.module_sqr();
                if (func::equality(divisor, VT(0.0))) {
                    throw func::AlgDomainError(__FILE__, __LINE__, "operator/=, division by zero.\n\n");
                }

                *this = *this * (~other) / divisor;
                return *this;
            }

            Complex<VT>& operator/=(const VT& other)& {
                if (func::equality(other, VT(0.0))) {
                    throw func::AlgDomainError(__FILE__, __LINE__, "operator/=, division by zero.\n\n");
                }

                re /= other;
                im /= other;
                return *this;
            }

            Complex<VT>& operator^=(int32_t other)& noexcept {
                if (other < 0) {
                    *this = VT(1.0) / func::binary_exponentiation(*this, static_cast<uint32_t>(std::abs(other)));
                }
                else {
                    *this = func::binary_exponentiation(*this, static_cast<uint32_t>(other));
                }
                return *this;
            }

            Complex<VT> operator-() const noexcept {
                return Complex<VT>(-re, -im);
            }

            Complex<VT> operator~() const noexcept {
                return Complex<VT>(re, -im);
            }

            Complex<VT> operator+(const Complex<VT>& other) const noexcept {
                return Complex<VT>(re + other.re, im + other.im);
            }

            Complex<VT> operator+(const VT& other) const noexcept {
                return Complex<VT>(re + other, im);
            }

            Complex<VT> operator-(const Complex<VT>& other) const noexcept {
                return Complex<VT>(re - other.re, im - other.im);
            }

            Complex<VT> operator-(const VT& other) const noexcept {
                return Complex<VT>(re - other, im);
            }

            Complex<VT> operator*(const Complex<VT>& other) const noexcept {
                return Complex<VT>(re * other.re - im * other.im, im * other.re + re * other.im);
            }

            Complex<VT> operator*(const VT& other) const noexcept {
                return Complex<VT>(re * other, im * other);
            }

            Complex<VT> operator/(const Complex<VT>& other) const {
                VT divisor = other.module_sqr();
                if (func::equality(divisor, VT(0.0))) {
                    throw func::AlgDomainError(__FILE__, __LINE__, "operator/, division by zero.\n\n");
                }

                return *this * (~other) / divisor;
            }

            Complex<VT> operator/(const VT& other) const {
                if (func::equality(other, VT(0.0))) {
                    throw func::AlgDomainError(__FILE__, __LINE__, "operator/, division by zero.\n\n");
                }

                return Complex<VT>(re / other, im / other);
            }

            Complex<VT> operator^(int32_t other) const noexcept {
                if (other < 0) {
                    return VT(1.0) / func::binary_exponentiation(*this, static_cast<uint32_t>(std::abs(other)));
                }
                return func::binary_exponentiation(*this, static_cast<uint32_t>(other));
            }

            Complex<VT> swap() const noexcept {
                return Complex(im, re);
            }

            Complex<VT> exponentiation(int32_t degree) const {
                try {
                    auto complex = AngleRepresentation(*this);
                    
                    if (degree < 0) {
                        complex.length = VT(1.0) / func::binary_exponentiation(complex.length, static_cast<uint32_t>(std::abs(degree)));
                    }
                    else {
                        complex.length = func::binary_exponentiation(complex.length, static_cast<uint32_t>(degree));
                    }

                    complex.angle *= degree;
                    return Complex<VT>(complex);
                }
                catch (func::AlgDomainError) {
                    return Complex<VT>(0.0, 0.0);
                }
            }

            std::vector<Complex<VT>> root(uint32_t degree) const {
                try {
                    auto complex = AngleRepresentation(*this);
                    complex.length = std::powl(complex.length, static_cast<long double>(1.0) / static_cast<long double>(degree));
                    complex.angle /= degree;

                    std::vector<Complex<VT>> result;
                    result.reserve(degree);
                    for (size_t i = 0; i < degree; ++i, complex.angle += 2.0 * func::PI / static_cast<long double>(degree)) {
                        result.push_back(Complex<VT>(complex));
                    }
                    return result;
                }
                catch (func::AlgDomainError) {
                    return std::vector<Complex<VT>>(degree, Complex<VT>(0.0, 0.0));
                }
            }

            VT module_sqr() const noexcept {
                return re * re + im * im;
            }

            long double module() const {
                return std::sqrtl(re * re + im * im);
            }

            static Complex<VT> zip_map(const Complex<VT>& v1, const Complex<VT>& v2, std::function<VT(VT, VT)> zip_func) {
                return Complex<VT>(zip_func(v1.re, v2.re), zip_func(v1.im, v2.im));
            }
        };

        template <typename T>
        struct std::hash<alg::geom::Complex<T>> {
            size_t operator()(const alg::geom::Complex<T>& vector) const noexcept {
                size_t result = 0;
                alg::func::hash_combine(result, vector.re);
                alg::func::hash_combine(result, vector.im);
                return result;
            }
        };

        template <typename T>
        std::istream& operator>>(std::istream& fin, Complex<T>& complex) noexcept {
            fin >> complex.re >> complex.im;
            return fin;
        }

        template <typename T>
        std::ostream& operator<<(std::ostream& fout, const Complex<T>& complex) noexcept {
            if (func::equality(complex.im, T(0.0))) {
                fout << complex.re;
                return fout;
            }

            if (!func::equality(abs(complex.im), T(1.0))) {
                if (func::equality(complex.re, T(0.0))) {
                    fout << complex.im << "i";
                }
                else if (less_equality(complex.im, T(0.0))) {
                    fout << complex.re << " - " << abs(complex.im) << "i";
                }
                else {
                    fout << complex.re << " + " << complex.im << "i";
                }
            }
            else {
                if (func::equality(complex.re, T(0.0))) {
                    fout << "i";
                }
                else if (less_equality(complex.im, T(0.0))) {
                    fout << complex.re << " - i";
                }
                else {
                    fout << complex.re << " + i";
                }
            }
            return fout;
        }

        std::ostream& operator<<(std::ostream& fout, const AngleRepresentation& complex) noexcept {
            fout << "module = " << complex.length << " | angle = " << complex.angle;
            return fout;
        }

        template <typename T>
        Complex<T> operator+(T value, const Complex<T>& complex) noexcept {
            return Complex<T>(complex.re + value, complex.im);
        }

        template <typename T>
        Complex<T> operator-(T value, const Complex<T>& complex) noexcept {
            return Complex<T>(-complex.re + value, -complex.im);
        }

        template <typename T>
        Complex<T> operator*(T value, const Complex<T>& complex) noexcept {
            return Complex<T>(complex.re * value, complex.im * value);
        }

        template <typename T>
        Complex<T> operator/(T value, const Complex<T>& complex) {
            T divisor = complex.module_sqr();
            if (func::equality(divisor, T(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/, division by zero.\n\n");
            }

            return value * (~complex) / divisor;
        }
    }

    namespace func {
        template <typename T1, typename T2>  // T1, T2 - standard numeric types
        bool equality(const Complex<T1>& left, const Complex<T2>& right, long double eps = func::EPS) noexcept {
            return func::equality(left.re, right.re, eps) && func::equality(left.im, right.im, eps);
        }
    }


    // ^^^ ----------Complex---------- ^^^
    // -----------------------------------
}   // Complex | Version: 1.0

using namespace alg::geom;