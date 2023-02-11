namespace alg::geom {
    // -----------------------------------
    // vvv ------------Vec2----------- vvv


    template <typename ValueType = long double>  // ValueType - standard numeric type
    class Vec2 {
    public:
        ValueType x = ValueType(0.0);
        ValueType y = ValueType(0.0);

        Vec2() noexcept {
        }

        explicit Vec2(ValueType value) noexcept {
            x = value;
            y = value;
        }

        Vec2(ValueType x, ValueType y) noexcept {
            this->x = x;
            this->y = y;
        }

        template <typename T>  // Casts required: ValueType(T)
        Vec2(const std::initializer_list<T>& init) {
            size_t comp_id = 0;
            for (const T& element : init) {
                (*this)[comp_id] = ValueType(element);

                if (++comp_id == 2) {
                    break;
                }
            }
        }

        template <typename T>  // Casts required: ValueType(T)
        explicit Vec2(const std::vector<T>& init) {
            for (size_t comp_id = 0; comp_id < std::min(static_cast<size_t>(2), init.size()); ++comp_id) {
                (*this)[comp_id] = ValueType(init[comp_id]);
            }
        }

        template <typename T>  // Constructors required: T(T), T(ValueType)
        explicit operator std::vector<T>() const {
            return { T(x), T(y) };
        }

        explicit operator std::string() const {
            return func::to_string(*this);
        }

        ValueType& operator[](size_t index) {
            if (index == 0) {
                return x;
            }
            if (index == 1) {
                return y;
            }
            throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
        }

        const ValueType& operator[](size_t index) const {
            if (index == 0) {
                return x;
            }
            if (index == 1) {
                return y;
            }
            throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
        }

        bool operator==(const Vec2<ValueType>& other) const noexcept {
            return func::equality(x, other.x) && func::equality(y, other.y);
        }

        bool operator!=(const Vec2<ValueType>& other) const noexcept {
            return !func::equality(x, other.x) || !func::equality(y, other.y);
        }

        Vec2<ValueType>& operator+=(const Vec2<ValueType>& other)& noexcept {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vec2<ValueType>& operator-=(const Vec2<ValueType>& other)& noexcept {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vec2<ValueType>& operator*=(ValueType other)& noexcept {
            x *= other;
            y *= other;
            return *this;
        }

        Vec2<ValueType>& operator/=(ValueType other)& {
            if (func::equality(other, ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/=, division by zero.\n\n");
            }

            x /= other;
            y /= other;
            return *this;
        }

        Vec2<ValueType>& operator^=(ValueType other)& {
            if (x < ValueType(0.0) || y < ValueType(0.0)) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator^=, raising a negative number to a power.\n\n");
            }

            x = std::pow(x, other);
            y = std::pow(y, other);
            return *this;
        }

        Vec2<ValueType>& operator^=(uint32_t other)& noexcept {
            x = func::binary_exponentiation(x, other);
            y = func::binary_exponentiation(y, other);
            return *this;
        }

        Vec2<ValueType> operator-() const noexcept {
            return Vec2<ValueType>(-x, -y);
        }

        Vec2<ValueType> operator+(const Vec2<ValueType>& other) const noexcept {
            return Vec2<ValueType>(x + other.x, y + other.y);
        }

        Vec2<ValueType> operator-(const Vec2<ValueType>& other) const noexcept {
            return Vec2<ValueType>(x - other.x, y - other.y);
        }

        Vec2<ValueType> operator*(ValueType other) const noexcept {
            return Vec2<ValueType>(x * other, y * other);
        }

        ValueType operator*(const Vec2<ValueType>& other) const noexcept {
            return x * other.x + y * other.y;
        }

        ValueType operator^(const Vec2<ValueType>& other) const noexcept {
            return x * other.y - y * other.x;
        }

        Vec2<ValueType> operator^(ValueType other) const {
            if (x < ValueType(0.0) || y < ValueType(0.0)) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator^, raising a negative number to a power.\n\n");
            }

            return Vec2<ValueType>(std::pow(x, other), std::pow(y, other));
        }

        Vec2<ValueType> operator^(uint32_t other) const noexcept {
            return Vec2<ValueType>(func::binary_exponentiation(x, other), func::binary_exponentiation(y, other));
        }

        Vec2<ValueType> operator/(ValueType other) const {
            if (func::equality(other, ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/, division by zero.\n\n");
            }

            return Vec2<ValueType>(x / other, y / other);
        }

        ValueType length_sqr() const noexcept {
            return *this * *this;
        }

        ValueType length() const {
            return ValueType(std::sqrtl(*this * *this));
        }

        ValueType get_value(std::function<ValueType(ValueType, ValueType)> func) const {
            return func(x, y);
        }

        ValueType get_value(const ValueType& func(const ValueType&, const ValueType&)) const {
            return get_value([&](const ValueType& left, const ValueType& right) -> const ValueType& { return func(left, right); });
        }

        ValueType get_value(ValueType func(ValueType, ValueType)) const {
            return get_value([&](const ValueType& left, const ValueType& right) -> const ValueType& { return func(left, right); });
        }

        Vec2<ValueType> normalize() const {
            ValueType vect_length = length();
            if (func::equality(vect_length, ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "normalize, null vector normalization.\n\n");
            }

            return *this / vect_length;
        }

        Vec2<ValueType> reflect_vect(const Vec2<ValueType>& n) const {
            try {
                const Vec2<ValueType>& norm = n.normalize();
                return norm * (norm * *this) * 2 - *this;
            }
            catch (func::AlgDomainError) {
                throw func::AlgDomainError(__FILE__, __LINE__, "reflect_vect, the normal vector has zero length.\n\n");
            }
        }

        bool in_angle(const Vec2<ValueType>& v1, const Vec2<ValueType>& v2) const noexcept {
            int32_t sign1 = func::sgn(v1 ^ *this);
            int32_t sign2 = func::sgn(v2 ^ *this);
            int32_t sign3 = func::sgn(v1 ^ v2);
            return (sign1 == 0 || sign1 == sign3) && !(sign2 == 0 || sign2 == -sign3);
        }

        bool in_triangle(const Vec2<ValueType>& v1, const Vec2<ValueType>& v2, const Vec2<ValueType>& v3) const noexcept {
            return (*this - v1).in_angle(v2 - v1, v3 - v1) && (*this - v2).in_angle(v1 - v2, v3 - v2);
        }

        Vec2<ValueType> multiply(const Vec2<ValueType>& other) const noexcept {
            return Vec2<ValueType>(x * other.x, y * other.y);
        }

        Vec2<ValueType> divide(const Vec2<ValueType>& other) const {
            if (func::equality(other.x, ValueType(0.0)) || func::equality(other.y, ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "divide, division by zero.\n\n");
            }

            return Vec2<ValueType>(x / other.x, y / other.y);
        }

        void apply_func(std::function<ValueType(ValueType)> func) {
            x = func(x);
            y = func(y);
        }

        void apply_func(const ValueType& func(const ValueType&)) {
            apply_func([&](const ValueType& value) -> const ValueType& { return func(value); });
        }

        void apply_func(ValueType func(ValueType)) {
            apply_func([&](const ValueType& value) -> const ValueType& { return func(value); });
        }

        static ValueType cos_angle(const Vec2<ValueType>& v1, const Vec2<ValueType>& v2) {
            ValueType length_prod = v1.length() * v2.length();
            if (func::equality(length_prod, ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "cos_angle, one of the vectors has zero length.\n\n");
            }

            return (v1 * v2) / length_prod;
        }

        static ValueType sin_angle(const Vec2<ValueType>& v1, const Vec2<ValueType>& v2) {
            ValueType length_prod = v1.length() * v2.length();
            if (func::equality(length_prod, ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "sin_angle, one of the vectors has zero length.\n\n");
            }

            return (v1 ^ v2) / length_prod;
        }

        static Vec2<ValueType> zip_map(const Vec2<ValueType>& v1, const Vec2<ValueType>& v2, std::function<ValueType(ValueType, ValueType)> zip_func) {
            return Vec2<ValueType>(zip_func(v1.x, v2.x), zip_func(v1.y, v2.y));
        }
    };

    template <typename T>
    struct std::hash<alg::geom::Vec2<T>> {
        size_t operator()(const alg::geom::Vec2<T>& vector) const noexcept {
            size_t result = 0;
            alg::func::hash_combine(result, vector.x);
            alg::func::hash_combine(result, vector.y);
            return result;
        }
    };

    template <typename T>
    std::istream& operator>>(std::istream& fin, Vec2<T>& vector) noexcept {
        fin >> vector.x >> vector.y;
        return fin;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& fout, const Vec2<T>& vector) {
        std::string x_str = func::to_string(vector.x);
        std::string y_str = func::to_string(vector.y);

        if (std::count(x_str.begin(), x_str.end(), '\n') == 0 && std::count(y_str.begin(), y_str.end(), '\n') == 0) {
            fout << '(' << vector.x << ", " << vector.y << ')';
        } else {
            fout << func::make_table_decorated({ { x_str, y_str } }, false, "", "", char(179), std::string(1, char(218)) + " " + std::string(1, char(191)), std::string(1, char(192)) + " " + std::string(1, char(217)));
        }

        return fout;
    }

    template <typename T>
    Vec2<T> operator*(T value, const Vec2<T>& vector) noexcept {
        return Vec2<T>(vector.x * value, vector.y * value);
    }


    // ^^^ ------------Vec2----------- ^^^
    // -----------------------------------
    // vvv ------------Vec3----------- vvv


    template <typename ValueType = long double>  // ValueType - standard numeric type
    class Vec3 {
    public:
        ValueType x = ValueType(0.0);
        ValueType y = ValueType(0.0);
        ValueType z = ValueType(0.0);

        Vec3() noexcept {
        }

        explicit Vec3(ValueType value) noexcept {
            x = value;
            y = value;
            z = value;
        }

        Vec3(ValueType x, ValueType y, ValueType z) noexcept {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        Vec3(const Vec2<ValueType>& xy, ValueType z) noexcept {
            x = xy.x;
            y = xy.y;
            this->z = z;
        }

        Vec3(ValueType x, const Vec2<ValueType>& yz) noexcept {
            this->x = x;
            y = yz.x;
            z = yz.y;
        }

        template <typename T>  // Casts required: ValueType(T)
        Vec3(const std::initializer_list<T>& init) {
            size_t comp_id = 0;
            for (const T& element : init) {
                (*this)[comp_id] = ValueType(element);

                if (++comp_id == 3) {
                    break;
                }
            }
        }

        template <typename T>  // Casts required: ValueType(T)
        explicit Vec3(const std::vector<T>& init) {
            for (size_t comp_id = 0; comp_id < std::min(static_cast<size_t>(3), init.size()); ++comp_id) {
                (*this)[comp_id] = ValueType(init[comp_id]);
            }
        }

        template <typename T>  // Constructors required: T(T), T(ValueType)
        explicit operator std::vector<T>() const {
            return { T(x), T(y), T(z) };
        }

        explicit operator std::string() const {
            return func::to_string(*this);
        }

        ValueType& operator[](size_t index) {
            if (index == 0) {
                return x;
            }
            if (index == 1) {
                return y;
            }
            if (index == 2) {
                return z;
            }
            throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
        }

        const ValueType& operator[](size_t index) const {
            if (index == 0) {
                return x;
            }
            if (index == 1) {
                return y;
            }
            if (index == 2) {
                return z;
            }
            throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
        }

        bool operator==(const Vec3<ValueType>& other) const noexcept {
            return func::equality(x, other.x) && func::equality(y, other.y) && func::equality(z, other.z);
        }

        bool operator!=(const Vec3<ValueType>& other) const noexcept {
            return !func::equality(x, other.x) || !func::equality(y, other.y) || !func::equality(z, other.z);
        }

        Vec3<ValueType>& operator+=(const Vec3<ValueType>& other)& noexcept {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vec3<ValueType>& operator-=(const Vec3<ValueType>& other)& noexcept {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vec3<ValueType>& operator*=(ValueType other)& noexcept {
            x *= other;
            y *= other;
            z *= other;
            return *this;
        }

        Vec3<ValueType>& operator/=(ValueType other)& {
            if (func::equality(other, ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/=, division by zero.\n\n");
            }

            x /= other;
            y /= other;
            z /= other;
            return *this;
        }

        Vec3<ValueType>& operator^=(ValueType other)& {
            if (x < ValueType(0) || y < ValueType(0) || z < ValueType(0)) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator^=, raising a negative number to a power.\n\n");
            }

            x = std::pow(x, other);
            y = std::pow(y, other);
            z = std::pow(z, other);
            return *this;
        }

        Vec3<ValueType>& operator^=(uint32_t other)& noexcept {
            x = func::binary_exponentiation(x, other);
            y = func::binary_exponentiation(y, other);
            z = func::binary_exponentiation(z, other);
            return *this;
        }

        Vec3<ValueType> operator-() const noexcept {
            return Vec3<ValueType>(-x, -y, -z);
        }

        Vec3<ValueType> operator+(const Vec3<ValueType>& other) const noexcept {
            return Vec3<ValueType>(x + other.x, y + other.y, z + other.z);
        }

        Vec3<ValueType> operator-(const Vec3<ValueType>& other) const noexcept {
            return Vec3<ValueType>(x - other.x, y - other.y, z - other.z);
        }

        Vec3<ValueType> operator*(ValueType other) const noexcept {
            return Vec3<ValueType>(x * other, y * other, z * other);
        }

        ValueType operator*(const Vec3<ValueType>& other) const noexcept {
            return x * other.x + y * other.y + z * other.z;
        }

        Vec3<ValueType> operator/(ValueType other) const {
            if (func::equality(other, ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/, division by zero.\n\n");
            }

            return Vec3<ValueType>(x / other, y / other, z / other);
        }

        Vec3<ValueType> operator^(const Vec3<ValueType>& other) const noexcept {
            return Vec3<ValueType>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
        }

        Vec3<ValueType> operator^(ValueType other) const {
            if (x < ValueType(0) || y < ValueType(0) || z < ValueType(0)) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator^, raising a negative number to a power.\n\n");
            }

            return Vec3<ValueType>(std::pow(x, other), std::pow(y, other), std::pow(z, other));
        }

        Vec3<ValueType> operator^(uint32_t other) const noexcept {
            return Vec3<ValueType>(func::binary_exponentiation(x, other), func::binary_exponentiation(y, other), func::binary_exponentiation(z, other));
        }

        ValueType length_sqr() const noexcept {
            return *this * *this;
        }

        ValueType length() const {
            return ValueType(std::sqrtl(*this * *this));
        }

        ValueType get_value(std::function<ValueType(ValueType, ValueType)> func) const {
            return func(func(x, y), z);
        }

        ValueType get_value(const ValueType& func(const ValueType&, const ValueType&)) const {
            return get_value([&](const ValueType& left, const ValueType& right) -> const ValueType& { return func(left, right); });
        }

        ValueType get_value(ValueType func(ValueType, ValueType)) const {
            return get_value([&](const ValueType& left, const ValueType& right) -> const ValueType& { return func(left, right); });
        }

        Vec3<ValueType> normalize() const {
            ValueType vect_length = length();
            if (func::equality(vect_length, ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "normalize, null vector normalization.\n\n");
            }

            return *this / vect_length;
        }

        Vec3<ValueType> horizont() const noexcept {
            Vec3<ValueType> horizont_vect(ValueType(1), ValueType(0), ValueType(0));
            if (!func::equality(Vec3(z, ValueType(0), -x).length(), ValueType(0.0))) {
                horizont_vect = Vec3(z, ValueType(0), -x).normalize();
            }

            return horizont_vect;
        }

        Vec3<ValueType> reflect_vect(const Vec3<ValueType>& n) const {
            if (func::equality(n.length(), ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "reflect_vect, the normal vector has zero length.\n\n");
            }

            Vec3<ValueType> norm = n.normalize();
            return norm * (norm * *this) * ValueType(2.0) - *this;
        }

        Vec3<ValueType> symmetry(const Vec3<ValueType>& center) const noexcept {
            return (center - *this) * ValueType(2.0) + *this;
        }

        bool in_two_side_angle(const Vec3<ValueType>& v1, const Vec3<ValueType>& v2) const noexcept {
            try {
                return func::equality(cos_angle(v1 ^ *this, v2 ^ *this), ValueType(-1.0));
            }
            catch (func::AlgDomainError) {
                return false;
            }
        }

        bool in_angle(const Vec3<ValueType>& v1, const Vec3<ValueType>& v2) const noexcept {
            Vec3<ValueType> prod1 = v1 ^ *this;
            Vec3<ValueType> prod2 = v2 ^ *this;
            Vec3<ValueType> prod3 = v1 ^ v2;
            if (func::equality(prod1.length() * prod3.length(), ValueType(0.0))) {
                return false;
            }

            try {
                if (func::equality(prod1.length() * prod2.length(), ValueType(0.0))) {
                    return func::equality(cos_angle(*this, v1), ValueType(1.0)) || func::equality(cos_angle(*this, v2), ValueType(1.0));
                }
                return func::equality(cos_angle(prod1, prod2), ValueType(-1.0)) && func::equality(cos_angle(prod1, prod3), ValueType(1.0));
            }
            catch (func::AlgDomainError) {
                return false;
            }
        }

        bool in_triangle(const Vec3<ValueType>& v1, const Vec3<ValueType>& v2, const Vec3<ValueType>& v3) const noexcept {
            return (*this - v1).in_angle(v2 - v1, v3 - v1) && (*this - v2).in_angle(v1 - v2, v3 - v2);
        }

        Vec3<ValueType> multiply(const Vec3<ValueType>& other) const noexcept {
            return Vec3<ValueType>(x * other.x, y * other.y, z * other.z);
        }

        Vec3<ValueType> divide(const Vec3<ValueType>& other) const {
            if (func::equality(other.x, ValueType(0.0)) || func::equality(other.y, ValueType(0.0)) || func::equality(other.z, ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "divide, division by zero.\n\n");
            }

            return Vec3<ValueType>(x / other.x, y / other.y, z / other.z);
        }

        void apply_func(std::function<ValueType(ValueType)> func) {
            x = func(x);
            y = func(y);
            z = func(z);
        }

        void apply_func(const ValueType& func(const ValueType&)) {
            apply_func([&](const ValueType& value) -> const ValueType& { return func(value); });
        }

        void apply_func(ValueType func(ValueType)) {
            apply_func([&](const ValueType& value) -> const ValueType& { return func(value); });
        }

        static ValueType cos_angle(const Vec3<ValueType>& v1, const Vec3<ValueType>& v2) {
            ValueType length_prod = v1.length() * v2.length();
            if (func::equality(length_prod, ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "cos_angle, one of the vectors has zero length.\n\n");
            }

            return (v1 * v2) / length_prod;
        }

        static ValueType sin_angle(const Vec3<ValueType>& v1, const Vec3<ValueType>& v2) {
            ValueType length_prod = v1.length() * v2.length();
            if (func::equality(length_prod, ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "sin_angle, one of the vectors has zero length.\n\n");
            }

            return (v1 ^ v2).length() / length_prod;
        }

        static Vec3<ValueType> zip_map(const Vec3<ValueType>& v1, const Vec3<ValueType>& v2, std::function<ValueType(ValueType, ValueType)> zip_func) {
            return Vec3<ValueType>(zip_func(v1.x, v2.x), zip_func(v1.y, v2.y), zip_func(v1.z, v2.z));
        }
    };

    template <typename T>
    struct std::hash<alg::geom::Vec3<T>> {
        size_t operator()(const alg::geom::Vec3<T>& vector) const noexcept {
            size_t result = 0;
            alg::func::hash_combine(result, vector.x);
            alg::func::hash_combine(result, vector.y);
            alg::func::hash_combine(result, vector.z);
            return result;
        }
    };

    template <typename T>
    std::istream& operator>>(std::istream& fin, Vec3<T>& vector) noexcept {
        fin >> vector.x >> vector.y >> vector.z;
        return fin;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& fout, const Vec3<T>& vector) {
        std::string x_str = func::to_string(vector.x);
        std::string y_str = func::to_string(vector.y);
        std::string z_str = func::to_string(vector.z);

        if (std::count(x_str.begin(), x_str.end(), '\n') == 0 && std::count(y_str.begin(), y_str.end(), '\n') == 0 && std::count(z_str.begin(), z_str.end(), '\n') == 0) {
            fout << '(' << x_str << ", " << y_str << ", " << z_str << ')';
        } else {
            fout << func::make_table_decorated({ { x_str, y_str, z_str } }, false, "", "", char(179), std::string(1, char(218)) + " " + std::string(1, char(191)), std::string(1, char(192)) + " " + std::string(1, char(217)));
        }
        return fout;
    }

    template <typename T>
    Vec3<T> operator*(T value, const Vec3<T>& vector) noexcept {
        return Vec3<T>(vector.x * value, vector.y * value, vector.z * value);
    }


    // ^^^ ------------Vec3----------- ^^^
    // -----------------------------------
}   // Vec2, Vec3 | Version: 1.0

using namespace alg::geom;
