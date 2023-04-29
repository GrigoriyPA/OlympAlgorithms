namespace alg {
    // -----------------------------------
    // vvv ----------Complex---------- vvv


    namespace geom {
        struct AngleRepresentation {
            long double length = 0.0;
            long double angle = 0.0;
        };

        template <typename ValueType = long double>  // ValueType - standard numeric type
        class Complex {
        public:
            ValueType re = ValueType(0.0);
            ValueType im = ValueType(0.0);

            Complex() noexcept {
            }

            explicit Complex(ValueType value) noexcept {
                re = value;
            }

            // Casts required: ValueType(long double)
            Complex(const AngleRepresentation& complex) {
                re = ValueType(complex.length * std::cosl(complex.angle));
                im = ValueType(complex.length * std::sinl(complex.angle));
            }

            template <typename T>  // Casts required: ValueType(T)
            explicit Complex(const Vec2<T>& vect) {
                re = ValueType(vect.x);
                im = ValueType(vect.y);
            }

            Complex(ValueType re, ValueType im) noexcept {
                this->re = re;
                this->im = im;
            }

            template <typename T>  // Casts required: ValueType(T)
            Complex(const std::initializer_list<T>& init) {
                size_t comp_id = 0;
                for (const T& element : init) {
                    (*this)[comp_id] = ValueType(element);

                    if (++comp_id == 2) {
                        break;
                    }
                }
            }

            template <typename T>  // Casts required: ValueType(T)
            explicit Complex(const std::vector<T>& init) {
                for (size_t comp_id = 0; comp_id < std::min(static_cast<size_t>(2), init.size()); ++comp_id) {
                    (*this)[comp_id] = ValueType(init[comp_id]);
                }
            }

            operator AngleRepresentation() const {
                long double length = module();
                if (func::equality(length, static_cast<long double>(0.0))) {
                    throw func::AlgDomainError(__FILE__, __LINE__, "operator AngleRepresentation, zero number cannot be represented in the form of an angle.\n\n");
                }

                return { .length = length, .angle = atan2(im, re) };
            }

            template <typename T>  // Constructors required: T(T), T(ValueType)
            explicit operator Vec2<T>() const {
                return Vec2<T>(T(re), T(im));
            }

            template <typename T>  // Constructors required: T(T), T(ValueType)
            explicit operator std::vector<T>() const {
                return { T(re), T(im) };
            }

            explicit operator std::string() const {
                return func::to_string(*this);
            }

            ValueType& operator[](size_t index) {
                if (index == 0) {
                    return re;
                }
                if (index == 1) {
                    return im;
                }
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            const ValueType& operator[](size_t index) const {
                if (index == 0) {
                    return re;
                }
                if (index == 1) {
                    return im;
                }
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            bool operator==(const Complex<ValueType>& other) const noexcept {
                return func::equality(re, other.re) && func::equality(im, other.im);
            }

            bool operator!=(const Complex<ValueType>& other) const noexcept {
                return !func::equality(re, other.re) || !func::equality(im, other.im);
            }

            Complex<ValueType>& operator+=(const Complex<ValueType>& other)& noexcept {
                re += other.re;
                im += other.im;
                return *this;
            }

            Complex<ValueType>& operator+=(const ValueType& other)& noexcept {
                re += other;
                return *this;
            }

            Complex<ValueType>& operator-=(const Complex<ValueType>& other)& noexcept {
                re -= other.re;
                im -= other.im;
                return *this;
            }

            Complex<ValueType>& operator-=(const ValueType& other)& noexcept {
                re -= other;
                return *this;
            }

            Complex<ValueType>& operator*=(const Complex<ValueType>& other)& noexcept {
                *this = *this * other;
                return *this;
            }

            Complex<ValueType>& operator*=(const ValueType& other)& noexcept {
                re *= other;
                im *= other;
                return *this;
            }

            Complex<ValueType>& operator/=(const Complex<ValueType>& other)& {
                ValueType divisor = other.module_sqr();
                if (func::equality(divisor, ValueType(0.0))) {
                    throw func::AlgDomainError(__FILE__, __LINE__, "operator/=, division by zero.\n\n");
                }

                *this = *this * (~other) / divisor;
                return *this;
            }

            Complex<ValueType>& operator/=(const ValueType& other)& {
                if (func::equality(other, ValueType(0.0))) {
                    throw func::AlgDomainError(__FILE__, __LINE__, "operator/=, division by zero.\n\n");
                }

                re /= other;
                im /= other;
                return *this;
            }

            Complex<ValueType>& operator^=(int32_t other)& noexcept {
                if (other < 0) {
                    *this = ValueType(1.0) / func::binary_exponentiation(*this, static_cast<uint32_t>(std::abs(other)));
                }
                else {
                    *this = func::binary_exponentiation(*this, static_cast<uint32_t>(other));
                }
                return *this;
            }

            Complex<ValueType> operator-() const noexcept {
                return Complex<ValueType>(-re, -im);
            }

            Complex<ValueType> operator~() const noexcept {
                return Complex<ValueType>(re, -im);
            }

            Complex<ValueType> operator+(const Complex<ValueType>& other) const noexcept {
                return Complex<ValueType>(re + other.re, im + other.im);
            }

            Complex<ValueType> operator+(const ValueType& other) const noexcept {
                return Complex<ValueType>(re + other, im);
            }

            Complex<ValueType> operator-(const Complex<ValueType>& other) const noexcept {
                return Complex<ValueType>(re - other.re, im - other.im);
            }

            Complex<ValueType> operator-(const ValueType& other) const noexcept {
                return Complex<ValueType>(re - other, im);
            }

            Complex<ValueType> operator*(const Complex<ValueType>& other) const noexcept {
                return Complex<ValueType>(re * other.re - im * other.im, im * other.re + re * other.im);
            }

            Complex<ValueType> operator*(const ValueType& other) const noexcept {
                return Complex<ValueType>(re * other, im * other);
            }

            Complex<ValueType> operator/(const Complex<ValueType>& other) const {
                ValueType divisor = other.module_sqr();
                if (func::equality(divisor, ValueType(0.0))) {
                    throw func::AlgDomainError(__FILE__, __LINE__, "operator/, division by zero.\n\n");
                }

                return *this * (~other) / divisor;
            }

            Complex<ValueType> operator/(const ValueType& other) const {
                if (func::equality(other, ValueType(0.0))) {
                    throw func::AlgDomainError(__FILE__, __LINE__, "operator/, division by zero.\n\n");
                }

                return Complex<ValueType>(re / other, im / other);
            }

            Complex<ValueType> operator^(int32_t other) const noexcept {
                if (other < 0) {
                    return ValueType(1.0) / func::binary_exponentiation(*this, static_cast<uint32_t>(std::abs(other)));
                }
                return func::binary_exponentiation(*this, static_cast<uint32_t>(other));
            }

            Complex<ValueType> swap() const noexcept {
                return Complex(im, re);
            }

            Complex<ValueType> exponentiation(int32_t degree) const {
                try {
                    auto complex = AngleRepresentation(*this);
                    
                    if (degree < 0) {
                        complex.length = ValueType(1.0) / func::binary_exponentiation(complex.length, static_cast<uint32_t>(std::abs(degree)));
                    }
                    else {
                        complex.length = func::binary_exponentiation(complex.length, static_cast<uint32_t>(degree));
                    }

                    complex.angle *= degree;
                    return Complex<ValueType>(complex);
                }
                catch (func::AlgDomainError) {
                    return Complex<ValueType>(0.0, 0.0);
                }
            }

            std::vector<Complex<ValueType>> root(uint32_t degree) const {
                try {
                    auto complex = AngleRepresentation(*this);
                    complex.length = std::powl(complex.length, static_cast<long double>(1.0) / static_cast<long double>(degree));
                    complex.angle /= degree;

                    std::vector<Complex<ValueType>> result;
                    result.reserve(degree);
                    for (size_t i = 0; i < degree; ++i, complex.angle += 2.0 * func::PI / static_cast<long double>(degree)) {
                        result.push_back(Complex<ValueType>(complex));
                    }
                    return result;
                }
                catch (func::AlgDomainError) {
                    return std::vector<Complex<ValueType>>(degree, Complex<ValueType>(0.0, 0.0));
                }
            }

            ValueType module_sqr() const noexcept {
                return re * re + im * im;
            }

            long double module() const {
                return std::sqrtl(re * re + im * im);
            }

            static Complex<ValueType> zip_map(const Complex<ValueType>& v1, const Complex<ValueType>& v2, std::function<ValueType(ValueType, ValueType)> zip_func) {
                return Complex<ValueType>(zip_func(v1.re, v2.re), zip_func(v1.im, v2.im));
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
        bool equality(const Complex<T1>& left, const Complex<T2>& right, long double eps = EPS) noexcept {
            return func::equality(left.re, right.re, eps) && func::equality(left.im, right.im, eps);
        }
    }


    // ^^^ ----------Complex---------- ^^^
    // -----------------------------------
}   // Complex | Version: 1.0

using namespace alg::geom;