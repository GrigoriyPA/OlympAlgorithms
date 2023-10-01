namespace alg::geom {
    // -----------------------------------
    // vvv ------------Line----------- vvv


    template <typename ValueType = long double>  // ValueType - standard numeric type
    class Line {
    public:
        Vec2<ValueType> orthogonal = Vec2<ValueType>(0.0, 1.0);
        ValueType dist = 0.0;

        Line() noexcept {
        }

        Line(const Vec2<ValueType>& point1, const Vec2<ValueType>& point2) noexcept {
            orthogonal = (point2 - point1).rotate90();
            dist = orthogonal * point1;
        }

        Line(ValueType a, ValueType b, ValueType c) noexcept {
            orthogonal = Vec2<ValueType>(a, b);
            dist = -c;
        }

        bool operator==(const Line<ValueType>& other) const noexcept {
            return orthogonal.normalize() == other.orthogonal.normalize()
                && func::equality(dist / orthogonal.length(), other.dist / other.orthogonal.length());
        }

        bool operator!=(const Line<ValueType>& other) const noexcept {
            return *this != other;
        }

        Vec2<ValueType> get_point() const noexcept {
            return orthogonal * dist / orthogonal.length_sqr();
        }

        Vec2<ValueType> get_direction() const noexcept {
            return orthogonal.rotate90();
        }

        bool contains(const Vec2<ValueType>& point) const noexcept {
            return func::equality(point * orthogonal, dist);
        }

        bool is_intersect(const Line<ValueType>& line) const noexcept {
            if (contains(line.get_point())) {
                return true;
            }

            ValueType delta = orthogonal ^ line.orthogonal;
            return !func::equality(ValueType(0.0), delta);
        }

        ValueType distance(const Vec2<ValueType>& point) const {
            return abs(point * orthogonal - dist) / orthogonal.length();
        }

        ValueType distance(const Line<ValueType>& line) const {
            return is_intersect(line) ? ValueType(0.0) : distance(line.get_point());
        }

        template <typename T>
        Vec2<T> intersect(const Line<ValueType>& line) const {
            return Vec2<T>(
                T(dist * line.orthogonal.y - line.dist * orthogonal.y) / T(orthogonal ^ line.orthogonal),
                T(orthogonal.x * line.dist - line.orthogonal.x * dist) / T(orthogonal ^ line.orthogonal)
            );
        }

        Vec2<ValueType> intersect(const Line<ValueType>& line) const {
            return Vec2<ValueType>(
                (dist * line.orthogonal.y - line.dist * orthogonal.y) / (orthogonal ^ line.orthogonal),
                (orthogonal.x * line.dist - line.orthogonal.x * dist) / (orthogonal ^ line.orthogonal)
            );
        }

        Vec2<ValueType> project(const Vec2<ValueType>& point) const {
            Vec2<ValueType> normal = orthogonal.normalize();
            return point - normal * (normal * (point - get_point()));
        }
    };

    template <typename T>
    std::istream& operator>>(std::istream& fin, Line<T>& line) noexcept {
        Vec2<T> point1;
        fin >> point1;

        Vec2<T> point2;
        fin >> point2;

        line = Line<T>(point1, point2);

        return fin;
    }


    // ^^^ ------------Line----------- ^^^
    // -----------------------------------
}   // Line | Version: 0.0

using namespace alg::geom;