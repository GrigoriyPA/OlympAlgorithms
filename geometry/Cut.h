namespace alg::geom {
    // -----------------------------------
    // vvv ------------Cut------------ vvv


    template <typename ValueType = long double>  // ValueType - standard numeric type
    class Cut {
        Line<ValueType> line_;
        Vec2<ValueType> start_;
        Vec2<ValueType> end_;

    public:
        Cut() noexcept {
        }

        Cut(const Vec2<ValueType>& start, const Vec2<ValueType>& end) noexcept {
            line_ = Line<ValueType>(start, end);
            start_ = start;
            end_ = end;
        }

        bool operator==(const Cut<ValueType>& other) const noexcept {
            return start_ == other.start_ && end_ == other.end_;
        }

        bool operator!=(const Cut<ValueType>& other) const noexcept {
            return *this != other;
        }

        const Line<ValueType>& get_line() const noexcept {
            return line_;
        }

        const Vec2<ValueType>& get_start() const noexcept {
            return start_;
        }

        const Vec2<ValueType>& get_end() const noexcept {
            return end_;
        }

        bool contains(const Vec2<ValueType>& point) const noexcept {
            if (start_ == end_) {
                return start_ == point;
            }

            Vec2<ValueType> direction = end_ - start_;
            return line_.contains(point)
                && func::less_equality(ValueType(0.0), (point - start_) * direction)
                && func::less_equality(ValueType(0.0), (end_ - point) * direction);
        }

        bool is_intersect(const Line<ValueType>& line) const noexcept {
            return is_intersect(Ray<ValueType>(line.get_point(), line.get_direction()))
                || is_intersect(Ray<ValueType>(line.get_point(), -line.get_direction()));
        }

        bool is_intersect(const Ray<ValueType>& ray) const noexcept {
            return Ray<ValueType>(start_, end_ - start_).is_intersect(ray)
                && Ray<ValueType>(end_, start_ - end_).is_intersect(ray);
        }

        bool is_intersect(const Cut<ValueType>& cut) const noexcept {
            return cut.is_intersect(Ray<ValueType>(start_, end_ - start_))
                && cut.is_intersect(Ray<ValueType>(end_, start_ - end_));
        }

        ValueType distance(const Vec2<ValueType>& point) const {
            Vec2<ValueType> direction = end_ - start_;
            if (func::less_equality((point - start_) * direction, ValueType(0.0))) {
                return (point - start_).length();
            }
            if (func::less_equality((end_ - point) * direction, ValueType(0.0))) {
                return (point - end_).length();
            }
            return line_.distance(point);
        }

        ValueType distance(const Line<ValueType>& line) const {
            if (is_intersect(line)) {
                return ValueType(0.0);
            }
            return std::min(
                line.distance(start_),
                line.distance(end_)
            );
        }

        ValueType distance(const Ray<ValueType>& ray) const {
            if (is_intersect(ray)) {
                return ValueType(0.0);
            }
            return std::min(std::min(
                ray.distance(start_),
                ray.distance(end_)),
                distance(ray.get_start())
            );
        }

        ValueType distance(const Cut<ValueType>& cut) const {
            if (is_intersect(cut)) {
                return ValueType(0.0);
            }
            return std::min(
                std::min(distance(cut.start_), distance(cut.end_)),
                std::min(cut.distance(start_), cut.distance(end_))
            );
        }

        void order_points() noexcept {
            if (start_.x > end_.x || start_.x == end_.x && start_.y > end_.y) {
                std::swap(start_, end_);
            }
        }
    };

    template <typename T>
    std::istream& operator>>(std::istream& fin, Cut<T>& cut) noexcept {
        Vec2<T> start;
        fin >> start;

        Vec2<T> end;
        fin >> end;

        cut = Cut<T>(start, end);

        return fin;
    }


    // ^^^ ------------Cut------------ ^^^
    // -----------------------------------
}   // Line | Version: 0.0

using namespace alg::geom;