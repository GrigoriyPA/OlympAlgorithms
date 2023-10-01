namespace alg::geom {
    // -----------------------------------
    // vvv ------------Ray------------ vvv


    template <typename ValueType = long double>  // ValueType - standard numeric type
    class Ray {
        Line<ValueType> line_;
        Vec2<ValueType> start_;
        Vec2<ValueType> direction_;

    public:
        Ray() noexcept {
        }

        Ray(const Vec2<ValueType>& start, const Vec2<ValueType>& direction) noexcept {
            line_ = Line<ValueType>(start, start + direction);
            start_ = start;
            direction_ = direction;
        }

        bool operator==(const Ray<ValueType>& other) const noexcept {
            return line_ == other.line_ && start_ == other.start_;
        }

        bool operator!=(const Ray<ValueType>& other) const noexcept {
            return *this != other;
        }

        const Line<ValueType>& get_line() const noexcept {
            return line_;
        }

        const Vec2<ValueType>& get_start() const noexcept {
            return start_;
        }

        const Vec2<ValueType>& get_direction() const noexcept {
            return direction_;
        }

        bool contains(const Vec2<ValueType>& point) const noexcept {
            if (direction_ == Vec2<ValueType>()) {
                return start_ == point;
            }
            return line_.contains(point) && func::less_equality(ValueType(0.0), (point - start_) * direction_);
        }

        bool is_intersect(const Line<ValueType>& line) const noexcept {
            return is_intersect(Ray<ValueType>(line.get_point(), line.get_direction()))
                || is_intersect(Ray<ValueType>(line.get_point(), -line.get_direction()));
        }

        bool is_intersect(const Ray<ValueType>& ray) const noexcept {
            if (contains(ray.start_) || ray.contains(start_)) {
                return true;
            }

            Vec2<ValueType> ortoganal = direction_.rotate90();
            ValueType delta = ortoganal * (start_ - ray.start_);
            if (func::equality(ValueType(0.0), delta)) {
                return false;
            }

            if (func::sgn(delta) != func::sgn(ortoganal * ray.direction_)) {
                return false;
            }

            return func::sgn(delta) == func::sgn(ray.direction_ ^ (start_ - ray.start_));
        }

        ValueType distance(const Vec2<ValueType>& point) const {
            if (func::less_equality(ValueType(0.0), (point - start_) * direction_)) {
                return line_.distance(point);
            }
            return (point - start_).length();
        }

        ValueType distance(const Line<ValueType>& line) const {
            return is_intersect(line) ? ValueType(0.0) : line.distance(start_);
        }

        ValueType distance(const Ray<ValueType>& ray) const {
            if (is_intersect(ray)) {
                return ValueType(0.0);
            }
            return std::min(
                distance(ray.start_),
                ray.distance(start_)
            );
        }
    };

    template <typename T>
    std::istream& operator>>(std::istream& fin, Ray<T>& ray) noexcept {
        Vec2<T> start;
        fin >> start;

        Vec2<T> direction;
        fin >> direction;

        ray = Ray<T>(start, direction);

        return fin;
    }


    // ^^^ ------------Ray------------ ^^^
    // -----------------------------------
}   // Line | Version: 0.0

using namespace alg::geom;