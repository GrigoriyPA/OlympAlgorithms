namespace alg::geom {
    // -----------------------------------
    // vvv -----------Circle---------- vvv


    template <typename ValueType = long double>  // ValueType - standard numeric type
    class Circle {
        ValueType radius_ = 0.0;

    public:
        Vec2<ValueType> center;

        Circle() noexcept {
        }

        Circle(const Vec2<ValueType>& center, ValueType radius) {
            if (radius < ValueType(0.0)) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "Circle, invalid radius.\n\n");
            }

            this->center = center;
            radius_ = radius;
        }

        bool operator==(const Circle<ValueType>& other) const noexcept {
            return center == other.center && func::equality(radius_, other.radius_);
        }

        bool operator!=(const Circle<ValueType>& other) const noexcept {
            return *this != other;
        }

        ValueType get_radius() const noexcept {
            return radius_;
        }

        bool contains(const Vec2<ValueType>& point) const noexcept {
            return func::equality((center - point).length(), radius_);
        }

        bool in_circle(const Vec2<ValueType>& point) const noexcept {
            return func::less_equality((center - point).length(), radius_);
        }

        bool is_intersect(const Line<ValueType>& line) const noexcept {
            return func::less_equality(line.distance(center), radius_);
        }

        bool is_intersect(const Circle<ValueType>& circle) const noexcept {
            return func::less_equality((center - circle.center).length(), radius_ + circle.radius_)
                && func::less_equality(std::max(radius_, circle.radius_), (center - circle.center).length() + std::min(radius_, circle.radius_));
        }

        std::pair<Vec2<ValueType>, Vec2<ValueType>> intersect(const Line<ValueType>& line) {
            Vec2<ValueType> normal = line.orthogonal.normalize();
            Vec2<ValueType> middle = center - normal * (normal * (center - line.get_point()));

            ValueType dist = line.distance(center);
            ValueType len = sqrtl(radius_ * radius_ - dist * dist);

            Vec2<ValueType> direction = line.get_direction().normalize();

            return std::make_pair(
                middle - direction * len,
                middle + direction * len
            );
        }

        std::pair<Vec2<ValueType>, Vec2<ValueType>> intersect(const Circle<ValueType>& circle) {
            Line<ValueType> line(
                ValueType(2.0) * (circle.center.x - center.x),
                ValueType(2.0) * (circle.center.y - center.y),
                center.length_sqr() - circle.center.length_sqr() - radius_ * radius_ + circle.radius_ * circle.radius_
            );

            return intersect(line);
        }

        std::pair<Vec2<ValueType>, Vec2<ValueType>> tangents(const Vec2<ValueType>& point) {
            return intersect(Circle<ValueType>(
                (center + point) / ValueType(2.0),
                (center - point).length() / ValueType(2.0)
            ));
        }
    };

    template <typename T>
    std::istream& operator>>(std::istream& fin, Circle<T>& circle) {
        Vec2<T> center;
        fin >> center;

        T radius;
        fin >> radius;

        circle = Circle<T>(center, radius);

        return fin;
    }


    // ^^^ -----------Circle---------- ^^^
    // -----------------------------------
}   // Circle | Version: 0.0

using namespace alg::geom;