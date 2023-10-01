namespace alg::geom {
    // -----------------------------------
    // vvv ----------PolyAngle-------- vvv


    template <typename ValueType = long double>  // ValueType - standard numeric type
    class PolyAngle {
        std::vector<Vec2<ValueType>> points_;

    public:
        using Iterator = std::vector<Vec2<ValueType>>::iterator;
        using ConstIterator = std::vector<Vec2<ValueType>>::const_iterator;

        PolyAngle() noexcept {
        }

        bool operator==(const PolyAngle<ValueType>& other) const noexcept {
            return points_ == other.points_;
        }

        bool operator!=(const PolyAngle<ValueType>& other) const noexcept {
            return *this != other;
        }

        Vec2<ValueType>& operator[](size_t index) noexcept {
            return points_[index % size()];
        }

        const Vec2<ValueType>& operator[](size_t index) const noexcept {
            return points_[index % size()];
        }

        const std::vector<Vec2<ValueType>>& get_points() const noexcept {
            return points_;
        }

        ValueType get_area() const noexcept {
            ValueType area = 0.0;
            for (size_t i = 0; i < size(); ++i) {
                area += (*this)[i] ^ (*this)[i + 1];
            }
            return abs(area);
        }

        ValueType get_perimeter() const {
            ValueType perimeter = 0.0;
            for (size_t i = 0; i < size(); ++i) {
                perimeter += ((*this)[i] - (*this)[i + 1]).length();
            }
            return perimeter;
        }

        PolyAngle<ValueType> get_convex_hull() {
            if (empty()) {
                return PolyAngle<ValueType>();
            }

            Vec2<ValueType> start = points_[0];
            for (const Vec2<ValueType>& point : points_) {
                if (!func::less_equality(start.y, point.y)
                    || func::equality(start.y, point.y) && !func::less_equality(start.x, point.x)) {
                    start = point;
                }
            }
            sort(points_.begin(), points_.end(), [&](const Vec2<ValueType>& left, const Vec2<ValueType>& right) -> bool {
                return angle_order_comparator<ValueType>(left - start, right - start);
                });

            PolyAngle<ValueType> convex_hull;
            convex_hull.insert_point(points_[0]);
            for (size_t i = 1; i < size(); ++i) {
                while (convex_hull.size() > 1
                    && func::less_equality(ValueType(0.0), ((points_[i] - convex_hull[convex_hull.size() - 1]) ^ (convex_hull[convex_hull.size() - 1] - convex_hull[convex_hull.size() - 2])))) {
                    convex_hull.erase_point();
                }
                convex_hull.insert_point(points_[i]);
            }

            return convex_hull;
        }

        bool contains(const Vec2<ValueType>& point) const noexcept {
            Ray<ValueType> check_ray(point, Vec2<ValueType>(1.0, 0.0));
            size_t number_intersections = 0;
            for (size_t i = 0; i < size(); ++i) {
                const Vec2<ValueType>& left = (*this)[i];
                const Vec2<ValueType>& right = (*this)[i + 1];
                Cut<ValueType> cut(left, right);

                if (cut.contains(point)) {
                    return true;
                }

                if (!cut.is_intersect(check_ray)
                    || func::equality(left.y, right.y)
                    || func::equality(point.y, std::min(left.y, right.y))) {
                    continue;
                }

                ++number_intersections;
            }
            return number_intersections % 2;
        }

        size_t size() const noexcept {
            return points_.size();
        }

        bool empty() const noexcept {
            return points_.empty();
        }

        Iterator begin() noexcept {
            return points_.begin();
        }

        Iterator end() noexcept {
            return points_.end();
        }

        ConstIterator begin() const noexcept {
            return points_.begin();
        }

        ConstIterator end() const noexcept {
            return points_.end();
        }

        void reserve(size_t size) {
            points_.reserve(size);
        }

        void insert_point(const Vec2<ValueType>& point) {
            points_.push_back(point);
        }

        void erase_point() {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "erase_point, try to erase from empty points list.\n\n");
            }

            points_.pop_back();
        }
    };


    // ^^^ ----------PolyAngle-------- ^^^
    // -----------------------------------
}   // PolyAngle | Version: 0.0

using namespace alg::geom;