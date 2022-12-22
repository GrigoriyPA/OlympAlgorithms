namespace alg::geom {
    // -----------------------------------
    // vvv ------------Vec2----------- vvv


    template <typename VT = long double>  // VT - numeric type
    class Vec2 {
    public:
        VT x = VT(0.0);
        VT y = VT(0.0);

        Vec2() noexcept {
        }

        explicit Vec2(VT value) noexcept {
            x = value;
            y = value;
        }

        Vec2(VT x, VT y) noexcept {
            this->x = x;
            this->y = y;
        }

        template <typename T>  // Casts required: VT(T)
        Vec2(const std::initializer_list<T>& init) {
            size_t comp_id = 0;
            for (const T& element : init) {
                (*this)[comp_id] = VT(element);

                if (++comp_id == 2) {
                    break;
                }
            }
        }

        template <typename T>  // Casts required: VT(T)
        explicit Vec2(const std::vector<T>& init) {
            for (size_t comp_id = 0; comp_id < std::min(static_cast<size_t>(2), init.size()); ++comp_id) {
                (*this)[comp_id] = VT(init[comp_id]);
            }
        }

        template <typename T>  // Constructors required: T(T), T(VT)
        explicit operator std::vector<T>() const {
            return { T(x), T(y) };
        }

        explicit operator std::string() const noexcept {
            return func::to_string(x) + " " + func::to_string(y);
        }

        VT& operator[](size_t index) {
            if (index == 0) {
                return x;
            }
            if (index == 1) {
                return y;
            }
            throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
        }

        const VT& operator[](size_t index) const {
            if (index == 0) {
                return x;
            }
            if (index == 1) {
                return y;
            }
            throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
        }

        bool operator==(const Vec2& other) const noexcept {
            return func::equality(x, other.x) && func::equality(y, other.y);
        }

        bool operator!=(const Vec2& other) const noexcept {
            return !(*this == other);
        }

        Vec2& operator+=(const Vec2& other)& noexcept {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vec2& operator-=(const Vec2& other)& noexcept {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vec2& operator*=(VT other)& noexcept {
            x *= other;
            y *= other;
            return *this;
        }

        Vec2& operator/=(VT other)& {
            if (func::equality(other, VT(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/=, division by zero.\n\n");
            }

            x /= other;
            y /= other;
            return *this;
        }

        Vec2& operator^=(VT other)& {
            if (x < 0 || y < 0) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator^=, raising a negative number to a power.\n\n");
            }

            x = pow(x, other);
            y = pow(y, other);
            return *this;
        }

        Vec2& operator^=(uint32_t other)& noexcept {
            x = func::binary_exponentiation(x, other);
            y = func::binary_exponentiation(y, other);
            return *this;
        }

        Vec2 operator-() const noexcept {
            return Vec2(-x, -y);
        }

        Vec2 operator+(const Vec2& other) const noexcept {
            return Vec2(x + other.x, y + other.y);
        }

        Vec2 operator-(const Vec2& other) const noexcept {
            return Vec2(x - other.x, y - other.y);
        }

        Vec2 operator*(VT other) const noexcept {
            return Vec2(x * other, y * other);
        }

        VT operator*(const Vec2& other) const noexcept {
            return x * other.x + y * other.y;
        }

        VT operator^(const Vec2& other) const noexcept {
            return x * other.y - y * other.x;
        }

        Vec2 operator^(VT other) const {
            if (x < 0 || y < 0) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator^, raising a negative number to a power.\n\n");
            }

            return Vec2(pow(x, other), pow(y, other));
        }

        Vec2 operator^(uint32_t other) const noexcept {
            return Vec2(func::binary_exponentiation(x, other), func::binary_exponentiation(y, other));
        }

        Vec2 operator/(VT other) const {
            if (func::equality(other, VT(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/, division by zero.\n\n");
            }

            return Vec2(x / other, y / other);
        }

        VT length_sqr() const noexcept {
            return *this * *this;
        }

        long double length() const noexcept {
            return sqrtl(*this * *this);
        }

        VT get_value(std::function<VT(VT, VT)> func) const {
            return func(x, y);
        }

        VT get_value(const VT& func(const VT&, const VT&)) const {
            return get_value([&](const VT& left, const VT& right) -> const VT& { return func(left, right); });
        }

        VT get_value(VT func(VT, VT)) const {
            return get_value([&](const VT& left, const VT& right) -> const VT& { return func(left, right); });
        }

        Vec2 normalize() const {
            long double vect_length = length();
            if (func::equality(vect_length, static_cast<long double>(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "normalize, null vector normalization.\n\n");
            }

            return *this / vect_length;
        }

        Vec2 reflect_vect(const Vec2& n) const {
            if (func::equality(n.length(), static_cast<long double>(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "reflect_vect, the normal vector has zero length.\n\n");
            }

            Vec2 norm = n.normalize();
            return norm * (norm * *this) * 2 - *this;
        }

        bool in_angle(const Vec2& v1, const Vec2& v2) const noexcept {
            int32_t sign1 = func::sgn(v1 ^ *this);
            int32_t sign2 = func::sgn(v2 ^ *this);
            int32_t sign3 = func::sgn(v1 ^ v2);
            return (sign1 == 0 || sign1 == sign3) && !(sign2 == 0 || sign2 == -sign3);
        }

        bool in_triangle(const Vec2& v1, const Vec2& v2, const Vec2& v3) const noexcept {
            return (*this - v1).in_angle(v2 - v1, v3 - v1) && (*this - v2).in_angle(v1 - v2, v3 - v2);
        }

        Vec2 multiply(const Vec2& other) {
            return Vec2(x * other.x, y * other.y);
        }

        Vec2 divide(const Vec2& other) {
            if (func::equality(other.x, VT(0.0)) || func::equality(other.y, VT(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "divide, division by zero.\n\n");
            }

            return Vec2(x / other.x, y / other.y);
        }

        Vec2& apply_func(std::function<VT(VT)> func) {
            x = func(x);
            y = func(y);
            return *this;
        }

        Vec2& apply_func(const VT& func(const VT&)) {
            apply_func([&](const VT& value) -> const VT& { return func(value); });
            return *this;
        }

        Vec2& apply_func(VT func(VT)) {
            apply_func([&](const VT& value) -> const VT& { return func(value); });
            return *this;
        }

        static long double cos_angle(const Vec2& v1, const Vec2& v2) {
            long double length_prod = v1.length() * v2.length();
            if (func::equality(length_prod, static_cast<long double>(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "cos_angle, one of the vectors has zero length.\n\n");
            }

            return static_cast<long double>(v1 * v2) / length_prod;
        }

        static long double sin_angle(const Vec2& v1, const Vec2& v2) {
            long double length_prod = v1.length() * v2.length();
            if (func::equality(length_prod, static_cast<long double>(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "sin_angle, one of the vectors has zero length.\n\n");
            }

            return static_cast<long double>(v1 ^ v2) / length_prod;
        }

        static Vec2 zip_map(const Vec2& v1, const Vec2& v2, std::function<VT(VT, VT)> zip_func) {
            return Vec2(zip_func(v1.x, v2.x), zip_func(v1.y, v2.y));
        }

        template <typename T>  // Casts required: VT(T)
        static std::vector<Vec2> move_in(size_t size, T* data) {
            std::vector<Vec2> result(size);
            for (size_t i = 0; i < size; ++i) {
                result[i] = Vec2(VT(data[2 * i]), VT(data[2 * i + 1]));
            }

            delete[] data;
            return result;
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
    std::ostream& operator<<(std::ostream& fout, const Vec2<T>& vector) noexcept {
        std::string x_str = func::to_string(vector.x);
        std::string y_str = func::to_string(vector.y);

        if (std::count(x_str.begin(), x_str.end(), '\n') == 0 && std::count(y_str.begin(), y_str.end(), '\n') == 0) {
            fout << '(' << vector.x << ", " << vector.y << ')';
        } else {
            fout << func::make_table_decorated({ { x_str, y_str } }, false, "", "", char(179), std::string(1, char(218)) + " " + std::string(1, char(191)), std::string(1, char(192)) + " " + std::string(1, char(217)));
        }

        return fout;
    }

    template <typename T1, typename T2>
    Vec2<T2> operator*(T1 value, const Vec2<T2>& vector) noexcept {
        return Vec2(vector.x * value, vector.y * value);
    }


    // ^^^ ------------Vec2----------- ^^^
    // -----------------------------------
    // vvv ------------Vec3----------- vvv


    template <typename VT = long double>  // VT - numeric type
    class Vec3 {
    public:
        VT x = VT(0.0);
        VT y = VT(0.0);
        VT z = VT(0.0);

        Vec3() noexcept {
        }

        template <typename T>  // Casts required: VT(T)
        explicit Vec3(T value) noexcept {
            x = VT(value);
            y = VT(value);
            z = VT(value);
        }

        Vec3(VT x, VT y, VT z) noexcept {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        Vec3(const Vec2<VT>& xy, VT z) noexcept {
            x = xy.x;
            y = xy.y;
            this->z = z;
        }

        Vec3(VT x, const Vec2<VT>& yz) noexcept {
            this->x = x;
            y = yz.x;
            z = yz.y;
        }

        template <typename T>  // Casts required: VT(T)
        Vec3(const std::initializer_list<T>& init) {
            size_t comp_id = 0;
            for (const T& element : init) {
                (*this)[comp_id] = VT(element);

                if (++comp_id == 3) {
                    break;
                }
            }
        }

        template <typename T>  // Casts required: VT(T)
        explicit Vec3(const std::vector<T>& init) {
            for (size_t comp_id = 0; comp_id < std::min(static_cast<size_t>(3), init.size()); ++comp_id) {
                (*this)[comp_id] = VT(init[comp_id]);
            }
        }

        template <typename T>  // Constructors required: T(T), T(VT)
        explicit operator std::vector<T>() const {
            return { T(x), T(y), T(z) };
        }

        explicit operator std::string() const noexcept {
            return func::to_string(x) + " " + func::to_string(y) + " " + func::to_string(z);
        }

        VT& operator[](size_t index) {
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

        const VT& operator[](size_t index) const {
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

        bool operator==(const Vec3& other) const noexcept {
            return func::equality(x, other.x) && func::equality(y, other.y) && func::equality(z, other.z);
        }

        bool operator!=(const Vec3& other) const noexcept {
            return !(*this == other);
        }

        Vec3& operator+=(const Vec3& other) & noexcept {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vec3& operator-=(const Vec3& other) & noexcept {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vec3& operator*=(VT other) & noexcept {
            x *= other;
            y *= other;
            z *= other;
            return *this;
        }

        Vec3& operator/=(VT other)& {
            if (func::equality(other, VT(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/=, division by zero.\n\n");
            }

            x /= other;
            y /= other;
            z /= other;
            return *this;
        }

        Vec3& operator^=(VT other)& {
            if (x < 0 || y < 0 || z < 0) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator^=, raising a negative number to a power.\n\n");
            }

            x = pow(x, other);
            y = pow(y, other);
            z = pow(z, other);
            return *this;
        }

        Vec3& operator^=(uint32_t other) & noexcept {
            x = func::binary_exponentiation(x, other);
            y = func::binary_exponentiation(y, other);
            z = func::binary_exponentiation(z, other);
            return *this;
        }

        Vec3 operator-() const noexcept {
            return Vec3(-x, -y, -z);
        }

        Vec3 operator+(const Vec3& other) const noexcept {
            return Vec3(x + other.x, y + other.y, z + other.z);
        }

        Vec3 operator-(const Vec3& other) const noexcept {
            return Vec3(x - other.x, y - other.y, z - other.z);
        }

        Vec3 operator*(VT other) const noexcept {
            return Vec3(x * other, y * other, z * other);
        }

        VT operator*(const Vec3& other) const noexcept {
            return x * other.x + y * other.y + z * other.z;
        }

        Vec3 operator/(VT other) const {
            if (func::equality(other, VT(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/, division by zero.\n\n");
            }

            return Vec3(x / other, y / other, z / other);
        }

        Vec3 operator^(const Vec3& other) const noexcept {
            return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
        }

        Vec3 operator^(VT other) const {
            if (x < 0 || y < 0 || z < 0) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator^, raising a negative number to a power.\n\n");
            }

            return Vec3(pow(x, other), pow(y, other), pow(z, other));
        }

        Vec3 operator^(uint32_t other) const noexcept {
            return Vec3(func::binary_exponentiation(x, other), func::binary_exponentiation(y, other), func::binary_exponentiation(z, other));
        }

        VT length_sqr() const noexcept {
            return *this * *this;
        }

        long double length() const noexcept {
            return sqrtl(*this * *this);
        }

        VT get_value(std::function<VT(VT, VT)> func) const {
            return func(func(x, y), z);
        }

        VT get_value(const VT& func(const VT&, const VT&)) const {
            return get_value([&](const VT& left, const VT& right) -> const VT& { return func(left, right); });
        }

        VT get_value(VT func(VT, VT)) const {
            return get_value([&](const VT& left, const VT& right) -> const VT& { return func(left, right); });
        }

        Vec3 normalize() const {
            long double vect_length = length();
            if (func::equality(vect_length, static_cast<long double>(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "normalize, null vector normalization.\n\n");
            }

            return *this / vect_length;
        }

        Vec3 horizont() const noexcept {
            Vec3 horizont_vect(VT(1), VT(0), VT(0));
            if (!func::equality(Vec3(z, VT(0), -x).length(), static_cast<long double>(0.0))) {
                horizont_vect = Vec3(z, VT(0), -x).normalize();
            }

            return horizont_vect;
        }

        Vec3 reflect_vect(const Vec3& n) const {
            if (func::equality(n.length(), static_cast<long double>(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "reflect_vect, the normal vector has zero length.\n\n");
            }

            Vec3 norm = n.normalize();
            return norm * (norm * *this) * VT(2.0) - *this;
        }

        Vec3 symmetry(const Vec3& center) const noexcept {
            return (center - *this) * VT(2.0) + *this;
        }

        bool in_two_side_angle(const Vec3& v1, const Vec3& v2) const noexcept {
            try {
                return func::equality(cos_angle(v1 ^ *this, v2 ^ *this), static_cast<long double>(-1.0));
            }
            catch (func::AlgDomainError) {
                return false;
            }
        }

        bool in_angle(const Vec3& v1, const Vec3& v2) const noexcept {
            Vec3 prod1 = v1 ^ *this, prod2 = v2 ^ *this, prod3 = v1 ^ v2;
            if (func::equality(prod1.length() * prod3.length(), static_cast<long double>(0.0))) {
                return false;
            }

            try {
                if (func::equality(prod1.length() * prod2.length(), static_cast<long double>(0.0))) {
                    return func::equality(cos_angle(*this, v1), static_cast<long double>(1.0)) || func::equality(cos_angle(*this, v2), static_cast<long double>(1.0));
                }
                return func::equality(cos_angle(prod1, prod2), static_cast<long double>(-1.0)) && func::equality(cos_angle(prod1, prod3), static_cast<long double>(1.0));
            }
            catch (func::AlgDomainError) {
                return false;
            }
        }

        bool in_triangle(const Vec3& v1, const Vec3& v2, const Vec3& v3) const noexcept {
            return (*this - v1).in_angle(v2 - v1, v3 - v1) && (*this - v2).in_angle(v1 - v2, v3 - v2);
        }

        Vec3 multiply(const Vec3& other) {
            return Vec3(x * other.x, y * other.y, z * other.z);
        }

        Vec3 divide(const Vec3& other) {
            if (func::equality(other.x, VT(0.0)) || func::equality(other.y, VT(0.0)) || func::equality(other.z, VT(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "divide, division by zero.\n\n");
            }

            return Vec3(x / other.x, y / other.y, z / other.z);
        }

        Vec3& apply_func(std::function<VT(VT)> func) {
            x = func(x);
            y = func(y);
            z = func(z);
            return *this;
        }

        Vec3& apply_func(const VT& func(const VT&)) {
            apply_func([&](const VT& value) -> const VT& { return func(value); });
            return *this;
        }

        Vec3& apply_func(VT func(VT)) {
            apply_func([&](const VT& value) -> const VT& { return func(value); });
            return *this;
        }

        static long double cos_angle(const Vec3& v1, const Vec3& v2) {
            long double length_prod = v1.length() * v2.length();
            if (func::equality(length_prod, static_cast<long double>(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "cos_angle, one of the vectors has zero length.\n\n");
            }

            return static_cast<long double>(v1 * v2) / length_prod;
        }

        static long double sin_angle(const Vec3& v1, const Vec3& v2) {
            long double length_prod = v1.length() * v2.length();
            if (func::equality(length_prod, static_cast<long double>(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "sin_angle, one of the vectors has zero length.\n\n");
            }

            return (v1 ^ v2).length() / length_prod;
        }

        static Vec3 zip_map(const Vec3& v1, const Vec3& v2, std::function<VT(VT, VT)> zip_func) {
            return Vec3(zip_func(v1.x, v2.x), zip_func(v1.y, v2.y), zip_func(v1.z, v2.z));
        }

        template <typename T>  // Casts required: VT(T)
        static std::vector<Vec3> move_in(size_t size, T* data) {
            std::vector<Vec3> result(size);
            for (size_t i = 0; i < size; ++i) {
                result[i] = Vec3(VT(data[3 * i]), VT(data[3 * i + 1]), VT(data[3 * i + 2]));
            }

            delete[] data;
            return result;
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
    std::ostream& operator<<(std::ostream& fout, const Vec3<T>& vector) noexcept {
        std::string x_str = func::to_string(vector.x);
        std::string y_str = func::to_string(vector.y);
        std::string z_str = func::to_string(vector.z);

        if (std::count(x_str.begin(), x_str.end(), '\n') == 0 && std::count(y_str.begin(), y_str.end(), '\n') == 0 && std::count(z_str.begin(), z_str.end(), '\n') == 0) {
            fout << '(' << vector.x << ", " << vector.y << ", " << vector.z << ')';
        } else {
            fout << func::make_table_decorated({ { x_str, y_str, z_str } }, false, "", "", char(179), std::string(1, char(218)) + " " + std::string(1, char(191)), std::string(1, char(192)) + " " + std::string(1, char(217)));
        }
        return fout;
    }

    template <typename T1, typename T2>
    Vec3<T2> operator*(T1 value, const Vec3<T2>& vector) noexcept {
        return Vec3(vector.x * value, vector.y * value, vector.z * value);
    }


    // ^^^ ------------Vec3----------- ^^^
    // -----------------------------------
}   // Vec2, Vec3

using namespace alg::geom;
