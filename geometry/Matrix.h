namespace alg::geom {
    // -----------------------------------
    // vvv ---------MatrixLine-------- vvv


    template <typename VT = long double>  // VT - standard numeric type
    class MatrixLine {
        template <typename T>
        friend class Matrix;

        std::vector<VT> line_;

        template <typename T>  // Casts required: VT(T)
        MatrixLine(const std::initializer_list<T>& init) {
            line_.reserve(init.size());
            for (const T& element : init) {
                line_.emplace_back(element);
            }
        }

        template <typename T>  // Casts required: VT(T)
        explicit MatrixLine(const std::vector<T>& init) {
            line_.reserve(init.size());
            for (const T& element : init) {
                line_.emplace_back(element);
            }
        }

        explicit MatrixLine(size_t count_columns, VT value = VT(0.0)) {
            line_.resize(count_columns, value);
        }

    public:
        template <typename T>  // Constructors required: T(T), T(VT)
        explicit operator std::vector<T>() const {
            std::vector<T> result;
            result.reserve(line_.size());
            for (VT element : line_) {
                result.emplace_back(element);
            }
            return result;
        }

        VT& operator[](size_t index) {
            if (line_.size() <= index) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return line_[index];
        }

        const VT& operator[](size_t index) const {
            if (line_.size() <= index) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return line_[index];
        }

        MatrixLine<VT>& operator=(const MatrixLine<VT>& other)& {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator=, invalid line sizes.\n\n");
            }

            line_ = other.line_;
            return *this;
        }

        bool operator==(const MatrixLine<VT>& other) const noexcept {
            if (line_.size() != other.line_.size()) {
                return false;
            }

            for (size_t i = 0; i < line_.size(); ++i) {
                if (!func::equality(line_[i], other[i])) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const MatrixLine<VT>& other) const noexcept {
            return !(*this == other);
        }

        MatrixLine<VT>& operator+=(const MatrixLine<VT>& other)& {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator+=, invalid line sizes.\n\n");
            }

            for (size_t i = 0; i < line_.size(); ++i) {
                line_[i] += other[i];
            }
            return *this;
        }

        MatrixLine<VT>& operator-=(const MatrixLine<VT>& other)& {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator-=, invalid line sizes.\n\n");
            }

            for (size_t i = 0; i < line_.size(); ++i) {
                line_[i] -= other[i];
            }
            return *this;
        }

        MatrixLine<VT>& operator*=(VT other)& noexcept {
            for (VT& element : line_) {
                element *= other;
            }
            return *this;
        }

        MatrixLine<VT>& operator/=(VT other)& {
            if (func::equality(other, VT(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/=, division by zero.\n\n");
            }

            for (VT& element : line_) {
                element /= other;
            }
            return *this;
        }

        MatrixLine<VT> operator-() const {
            return *this * VT(-1.0);
        }

        MatrixLine<VT> operator+(const MatrixLine<VT>& other) const {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator+, invalid line sizes.\n\n");
            }

            MatrixLine<VT> result = *this;
            for (size_t i = 0; i < other.size(); ++i) {
                result[i] += other[i];
            }

            return result;
        }

        MatrixLine<VT> operator-(const MatrixLine<VT>& other) const {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator-, invalid line sizes.\n\n");
            }

            MatrixLine<VT> result = *this;
            for (size_t i = 0; i < other.size(); ++i) {
                result[i] -= other[i];
            }

            return result;
        }

        MatrixLine<VT> operator*(VT other) const {
            MatrixLine<VT> result = *this;
            return result *= other;
        }

        MatrixLine<VT> operator/(VT other) const {
            if (func::equality(other, VT(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/, division by zero.\n\n");
            }

            MatrixLine<VT> result = *this;
            for (VT& element : result.line_) {
                element /= other;
            }
            return result;
        }

        VT operator*(const MatrixLine<VT>& other) const {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator*, invalid line sizes.\n\n");
            }

            VT result = 0;
            for (size_t i = 0; i < line_.size(); ++i) {
                result += line_[i] * other[i];
            }
            return result;
        }

        VT get_value(std::function<VT(VT, VT)> func) const {
            if (line_.empty()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_value, called from empty line.\n\n");
            }

            VT result = line_[0];
            for (size_t i = 1; i < line_.size(); ++i) {
                result = func(result, line_[i]);
            }
            return result;
        }

        VT get_value(const VT& func(const VT&, const VT&)) const {
            return get_value([&](const VT& left, const VT& right) -> const VT& { return func(left, right); });
        }

        VT get_value(VT func(VT, VT)) const {
            return get_value([&](const VT& left, const VT& right) -> const VT& { return func(left, right); });
        }

        VT& front() {
            if (line_.empty()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "front, called from empty line.\n\n");
            }

            return line_.front();
        }

        VT& back() {
            if (line_.empty()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "back, called from empty line.\n\n");
            }

            return line_.back();
        }

        const VT& back() const {
            if (line_.empty()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "back, called from empty line.\n\n");
            }

            return line_.back();
        }

        size_t size() const noexcept {
            return line_.size();
        }

        bool empty() const noexcept {
            return line_.size();
        }

        MatrixLine<VT> multiply(const MatrixLine<VT>& other) const {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "multiply, invalid line sizes.\n\n");
            }

            MatrixLine<VT> result = *this;
            for (size_t i = 0; i < other.size(); ++i) {
                result[i] *= other[i];
            }

            return result;
        }

        MatrixLine<VT> divide(const MatrixLine<VT>& other) const {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "divide, invalid line sizes.\n\n");
            }

            MatrixLine<VT> result = *this;
            for (size_t i = 0; i < other.size(); ++i) {
                if (func::equality(other[i], VT(0.0))) {
                    throw func::AlgDomainError(__FILE__, __LINE__, "divide, division by zero.\n\n");
                }

                result[i] /= other[i];
            }

            return result;
        }

        void apply_func(std::function<VT(VT)> func) {
            for (size_t i = 0; i < line_.size(); ++i) {
                line_[i] = func(line_[i]);
            }
        }

        void apply_func(const VT& func(const VT&)) {
            apply_func([&](const VT& value) -> const VT& { return func(value); });
        }

        void apply_func(VT func(VT)) {
            apply_func([&](const VT& value) -> const VT& { return func(value); });
        }

        void swap(MatrixLine<VT>& other) {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "swap, invalid line sizes.\n\n");
            }

            line_.swap(other.line_);
        }
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& fout, const MatrixLine<T>& matrix_line) {
        std::vector<std::vector<std::string>> description(1, std::vector<std::string>(matrix_line.size()));
        for (size_t i = 0; i < matrix_line.size(); ++i) {
            description[0][i] = func::to_string(matrix_line[i]);
        }

        fout << func::make_table_decorated(description, false, "", "", char(179), std::string(1, char(218)) + " " + std::string(1, char(191)), std::string(1, char(192)) + " " + std::string(1, char(217)));
        return fout;
    }

    template <typename T>
    MatrixLine<T> operator*(T value, const MatrixLine<T>& matrix) {
        return matrix * value;
    }


    // ^^^ ---------MatrixLine-------- ^^^
    // -----------------------------------
    // vvv -----------Matrix---------- vvv


    template <typename VT = long double>  // VT - standard numeric type
    class Matrix {
        std::vector<MatrixLine<VT>> matrix_;

    public:
        template <typename T>  // Casts required: VT(T)
        explicit Matrix(const T& value) {
            matrix_.push_back(MatrixLine<VT>(1, VT(value)));
        }

        template <typename T>  // Casts required: VT(T)
        Matrix(const std::initializer_list<T>& init) {
            matrix_.reserve(init.size());
            for (const T& value : init) {
                matrix_.push_back(MatrixLine<VT>(1, VT(value)));
            }
        }

        template <typename T>  // Casts required: VT(T)
        explicit Matrix(const std::vector<T>& init) {
            matrix_.reserve(init.size());
            for (const T& value : init) {
                matrix_.push_back(MatrixLine<VT>(1, VT(value)));
            }
        }

        template <typename T>  // Casts required: VT(T)
        Matrix(const std::initializer_list<std::initializer_list<T>>& init) {
            size_t columns = 0;
            if (init.size() > 0) {
                columns = init.begin()->size();
            }

            matrix_.reserve(init.size());
            for (const auto& line : init) {
                if (columns != line.size()) {
                    throw func::AlgInvalidArgument(__FILE__, __LINE__, "Matrix, not all rows are the same size.\n\n");
                }

                matrix_.push_back(MatrixLine<VT>(line));
            }
        }

        template <typename T>  // Casts required: double(T)
        explicit Matrix(const std::vector<std::vector<T>>& init) {
            size_t columns = 0;
            if (!init.empty()) {
                columns = init[0].size();
            }

            matrix_.reserve(init.size());
            for (const auto& line : init) {
                if (columns != line.size()) {
                    throw func::AlgInvalidArgument(__FILE__, __LINE__, "Matrix, not all rows are the same size.\n\n");
                }

                matrix_.push_back(MatrixLine<VT>(line));
            }
        }

        Matrix(const Vec3<VT>& vector_x, const Vec3<VT>& vector_y, const Vec3<VT>& vector_z) {
            *this = Matrix<VT>({
                { vector_x.x, vector_y.x, vector_z.x, VT(0.0) },
                { vector_x.y, vector_y.y, vector_z.y, VT(0.0) },
                { vector_x.z, vector_y.z, vector_z.z, VT(0.0) },
                {    VT(0.0),    VT(0.0),    VT(0.0), VT(1.0) },
            });
        }

        Matrix(size_t count_lines, size_t count_columns, VT value, const Matrix<VT>& init) {
            matrix_.resize(count_lines, MatrixLine<VT>(count_columns, value));
            for (size_t i = 0; i < std::min(count_lines, init.count_strings()); ++i) {
                for (size_t j = 0; j < std::min(count_columns, init.count_columns()); ++j) {
                    matrix_[i][j] = init[i][j];
                }
            }
        }

        Matrix(size_t count_lines, size_t count_columns, VT value = VT(0.0)) {
            matrix_.resize(count_lines, MatrixLine<VT>(count_columns, value));
        }

        Matrix(size_t count_lines, const MatrixLine<VT>& value) {
            matrix_.resize(count_lines, value);
        }

        template <typename T>  // Constructors required: T(VT)
        explicit operator std::vector<T>() const {
            std::vector<T> result;
            result.reserve(matrix_.size() * count_columns());
            for (size_t j = 0; j < count_columns(); ++j) {
                for (size_t i = 0; i < matrix_.size(); ++i) {
                    result.emplace_back(matrix_[i][j]);
                }
            }
            return result;
        }

        MatrixLine<VT>& operator[](size_t index) {
            if (matrix_.size() <= index) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return matrix_[index];
        }

        const MatrixLine<VT>& operator[](size_t index) const {
            if (matrix_.size() <= index) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return matrix_[index];
        }

        bool operator==(const Matrix<VT>& other) const noexcept {
            if (matrix_.size() != other.count_strings()) {
                return false;
            }

            for (size_t i = 0; i < matrix_.size(); ++i) {
                if (matrix_[i] != other[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const Matrix<VT>& other) const noexcept {
            return !(*this == other);
        }

        Matrix<VT>& operator+=(const Matrix<VT>& other)& {
            if (matrix_.size() != other.count_strings() || count_columns() != other.count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator+=, invalid matrix sizes.\n\n");
            }

            for (size_t i = 0; i < matrix_.size(); i++) {
                matrix_[i] += other[i];
            }
            return *this;
        }

        Matrix<VT>& operator-=(const Matrix<VT>& other)& {
            if (matrix_.size() != other.count_strings() || count_columns() != other.count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator-=, invalid matrix sizes.\n\n");
            }

            for (size_t i = 0; i < matrix_.size(); i++) {
                matrix_[i] -= other[i];
            }
            return *this;
        }

        Matrix<VT>& operator*=(VT other)& noexcept {
            for (size_t i = 0; i < matrix_.size(); i++) {
                matrix_[i] *= other;
            }
            return *this;
        }

        Matrix<VT>& operator*=(const Matrix<VT>& other)& {
            if (count_columns() != other.count_strings()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator*=, invalid matrix sizes.\n\n");
            }

            const Matrix<VT>& transposed = other.transpose();
            Matrix<VT> result(matrix_.size(), other.count_columns(), VT(0.0));
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < other.count_columns(); ++j) {
                    result[i][j] = matrix_[i] * transposed[j];
                }
            }
            return *this = result;
        }

        Matrix<VT>& operator/=(VT other)& {
            if (func::equality(other, VT(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/=, division by zero.\n\n");
            }

            for (size_t i = 0; i < matrix_.size(); i++) {
                matrix_[i] /= other;
            }
            return *this;
        }

        Matrix<VT>& operator/=(const Matrix<VT>& other)& {
            try {
                *this *= other.inverse();
            }
            catch (func::AlgDomainError) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/=, division by a nonsingular matrix.\n\n");
            }
            return *this;
        }

        Matrix<VT>& operator^=(uint32_t other)& {
            if (matrix_.size() != count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator^=, invalid matrix size.\n\n");
            }

            return *this = func::binary_exponentiation(*this, other, one_matrix(matrix_.size()));
        }

        Matrix<VT>& operator|=(const Matrix<VT>& other)& {
            if (matrix_.size() != other.count_strings()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator|=, invalid matrix sizes.\n\n");
            }

            for (size_t i = 0; i < matrix_.size(); ++i) {
                matrix_[i].line_.reserve(count_columns() + other.count_columns());
                for (size_t j = 0; j < other.count_columns(); ++j) {
                    matrix_[i].line_.push_back(other[i][j]);
                }
            }
            return *this;
        }

        Matrix<VT> operator-() const {
            return *this * VT(-1.0);
        }

        Matrix<VT> operator+(const Matrix<VT>& other) const {
            if (matrix_.size() != other.count_strings() || count_columns() != other.count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator+, invalid matrix sizes.\n\n");
            }

            Matrix<VT> result = *this;
            for (size_t i = 0; i < matrix_.size(); i++) {
                result[i] += other[i];
            }
            return result;
        }

        Matrix<VT> operator-(const Matrix<VT>& other) const {
            if (matrix_.size() != other.count_strings() || count_columns() != other.count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator-, invalid matrix sizes.\n\n");
            }

            Matrix<VT> result = *this;
            for (size_t i = 0; i < matrix_.size(); i++) {
                result[i] -= other[i];
            }
            return result;
        }

        Matrix<VT> operator*(VT other) const {
            Matrix<VT> result = *this;
            for (size_t i = 0; i < matrix_.size(); i++) {
                result[i] *= other;
            }
            return result;
        }

        Matrix<VT> operator*(const Matrix<VT>& other) const {
            if (count_columns() != other.count_strings()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator*, invalid matrix sizes.\n\n");
            }

            const Matrix<VT>& transposed = other.transpose();
            Matrix<VT> result(matrix_.size(), other.count_columns(), VT(0.0));
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < other.count_columns(); ++j) {
                    result[i][j] = matrix_[i] * transposed[j];
                }
            }
            return result;
        }

        Vec3<VT> operator*(const Vec3<VT>& other) const {
            if (matrix_.size() != 4 || matrix_[0].size() != 4) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator*, invalid matrix size.\n\n");
            }

            Vec3<VT> result;
            for (size_t i = 0; i < 3; ++i) {
                for (size_t j = 0; j < 3; ++j) {
                    result[i] += matrix_[i][j] * other[j];
                }
                result[i] += matrix_[i][3];
            }
            return result;
        }

        Matrix<VT> operator/(VT other) const {
            if (func::equality(other, VT(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/, division by zero.\n\n");
            }

            Matrix<VT> result = *this;
            for (size_t i = 0; i < matrix_.size(); i++) {
                result[i] /= other;
            }
            return result;
        }

        Matrix<VT> operator/(const Matrix<VT>& other) const {
            try {
                return *this * other.inverse();
            }
            catch (func::AlgDomainError) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/, division by a nonsingular matrix.\n\n");
            }
        }

        Matrix<VT> operator^(uint32_t other) const {
            if (matrix_.size() != count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator^, invalid matrix size.\n\n");
            }

            return func::binary_exponentiation(*this, other, one_matrix(matrix_.size()));
        }

        Matrix<VT> operator|(const Matrix<VT>& other) const {
            if (matrix_.size() != other.count_strings()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator|, invalid matrix sizes.\n\n");
            }

            Matrix<VT> result(matrix_.size(), count_columns() + other.count_columns(), VT(0), *this);
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < other.count_columns(); ++j) {
                    result[i][count_columns() + j] = other[i][j];
                }
            }
            return result;
        }

        Matrix<VT> multiply(const Matrix<VT>& other) const {
            if (count_strings() != other.count_strings() || count_columns() != other.count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "multiply, invalid matrix sizes.\n\n");
            }

            Matrix<VT> result = *this;
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < matrix_[i].size(); ++j) {
                    result[i][j] *= other[i][j];
                }
            }
            return result;
        }

        Matrix<VT> divide(const Matrix<VT>& other) const {
            if (count_strings() != other.count_strings() || count_columns() != other.count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "divide, invalid matrix sizes.\n\n");
            }

            Matrix<VT> result = *this;
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < matrix_[i].size(); ++j) {
                    if (func::equality(other[i][j], VT(0.0))) {
                        throw func::AlgDomainError(__FILE__, __LINE__, "divide, division by zero.\n\n");
                    }

                    result[i][j] /= other[i][j];
                }
            }

            return result;
        }

        VT get_value(std::function<VT(VT, VT)> func) const {
            if (matrix_.empty()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_value, called from empty matrix.\n\n");
            }

            VT result = matrix_[0][0];
            bool initialized = false;
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < matrix_[i].size(); ++j) {
                    if (!initialized) {
                        initialized = true;
                        continue;
                    }

                    result = func(result, matrix_[i][j]);
                }
            }
            return result;
        }

        VT get_value(const VT& func(const VT&, const VT&)) const {
            return get_value([&](const VT& left, const VT& right) -> const VT& { return func(left, right); });
        }

        VT get_value(VT func(VT, VT)) const {
            return get_value([&](const VT& left, const VT& right) -> const VT& { return func(left, right); });
        }

        // axis = 0: horizontal; axis = 1: vertical
        Matrix<VT> get_value(std::function<VT(VT, VT)> func, bool axis) const {
            if (matrix_.empty()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_value, called from empty matrix.\n\n");
            }

            Matrix<VT> current_matrix = *this;
            if (axis) {
                current_matrix = current_matrix.transpose();
            }

            Matrix<VT> result(current_matrix.count_strings(), 1);
            for (size_t i = 0; i < current_matrix.count_strings(); ++i) {
                result[i][0] = current_matrix[i][0];
                for (size_t j = 1; j < current_matrix[i].size(); ++j) {
                    result[i][0] = func(result[i][0], current_matrix[i][j]);
                }
            }

            if (axis) {
                return result.transpose();
            }
            return result;
        }

        // axis = 0: horizontal; axis = 1: vertical
        Matrix<VT> get_value(const VT& func(const VT&, const VT&), bool axis) const {
            return get_value([&](const VT& left, const VT& right) -> const VT& { return func(left, right); }, axis);
        }

        // axis = 0: horizontal; axis = 1: vertical
        Matrix<VT> get_value(VT func(VT, VT), bool axis) const {
            return get_value([&](const VT& left, const VT& right) -> const VT& { return func(left, right); }, axis);
        }

        size_t count_strings() const noexcept {
            return matrix_.size();
        }

        size_t count_columns() const noexcept {
            if (matrix_.empty()) {
                return 0;
            }
            return matrix_[0].size();
        }

        bool empty() const noexcept {
            return matrix_.empty();
        }

        void apply_func(std::function<VT(VT)> func) {
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < matrix_[i].size(); ++j) {
                    matrix_[i][j] = func(matrix_[i][j]);
                }
            }
        }

        void apply_func(const VT& func(const VT&)) {
            apply_func([&](const VT& value) -> const VT& { return func(value); });
        }

        void apply_func(VT func(VT)) {
            apply_func([&](const VT& value) -> const VT& { return func(value); });
        }

        void swap(Matrix<VT>& other) noexcept {
            matrix_.swap(other.matrix_);
        }

        Matrix<VT> transpose() const {
            Matrix<VT> result(count_columns(), matrix_.size());
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < matrix_[i].size(); ++j) {
                    result[j][i] = matrix_[i][j];
                }
            }
            return result;
        }

        Matrix<VT> flip_horizontally() const {
            Matrix<VT> result(matrix_.size(), count_columns());
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < matrix_[i].size(); ++j) {
                    result[matrix_.size() - 1 - i][j] = matrix_[i][j];
                }
            }
            return result;
        }

        Matrix<VT> flip_vertically() const {
            Matrix<VT> result(matrix_.size(), count_columns());
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < matrix_[i].size(); ++j) {
                    result[i][matrix_[i].size() - 1 - j] = matrix_[i][j];
                }
            }
            return result;
        }

        Matrix<VT> rotate_clockwise() const {
            return flip_horizontally().transpose();
        }

        Matrix<VT> rotate_counterclockwise() const {
            return flip_vertically().transpose();
        }

        Matrix<VT> submatrix(size_t line, size_t column, size_t height, size_t width) const {
            if (line + height > matrix_.size() || column + width > count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "submatrix, invalid submatrix size.\n\n");
            }

            Matrix<VT> result(height, width);
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    result[i][j] = matrix_[line + i][column + j];
                }
            }
            return result;
        }

        Matrix<VT> improved_step_view() const {
            Matrix<VT> result = *this;
            for (size_t j = 0, i0 = 0; j < count_columns() && i0 < matrix_.size(); ++j) {
                size_t k = matrix_.size();
                for (size_t i = i0; i < matrix_.size(); ++i) {
                    if (!func::equality(result[i][j], VT(0.0))) {
                        k = i;
                        break;
                    }
                }

                if (k == matrix_.size()) {
                    continue;
                }

                result[i0].swap(result[k]);
                result[i0] *= 1.0 / result[i0][j];
                for (size_t i = 0; i < matrix_.size(); ++i) {
                    if (i == i0) {
                        continue;
                    }

                    result[i] -= result[i0] * result[i][j];
                }
                ++i0;
            }
            return result;
        }

        VT determinant() const {
            if (matrix_.size() != count_columns()) {
                throw func::AlgDomainError(__FILE__, __LINE__, "determinant, not a square matrix.\n\n");
            }

            VT result(1.0);
            Matrix<VT> cur_matrix = *this;
            for (size_t j = 0; j < count_columns(); ++j) {
                size_t k = matrix_.size();
                for (size_t i = j; i < matrix_.size(); ++i) {
                    if (!func::equality(cur_matrix[i][j], VT(0.0))) {
                        k = i;
                        break;
                    }
                }

                if (k == matrix_.size()) {
                    return VT(0.0);
                }

                if (k != j) {
                    result *= VT(-1.0);
                    cur_matrix[j].swap(cur_matrix[k]);
                }

                result *= cur_matrix[j][j];
                for (size_t i = k + 1; i < matrix_.size(); ++i) {
                    cur_matrix[i] -= cur_matrix[j] * (cur_matrix[i][j] / cur_matrix[j][j]);
                }
            }
            return result;
        }

        Matrix<VT> inverse() const {
            if (matrix_.size() != count_columns()) {
                throw func::AlgDomainError(__FILE__, __LINE__, "inverse, not a square matrix.\n\n");
            }

            Matrix<VT> result = (*this | one_matrix(matrix_.size())).improved_step_view();
            if (result.submatrix(0, 0, matrix_.size(), matrix_.size()) != one_matrix(matrix_.size())) {
                throw func::AlgDomainError(__FILE__, __LINE__, "inverse, the matrix is not invertible.\n\n");
            }
            return result.submatrix(0, matrix_.size(), matrix_.size(), matrix_.size());
        }

        Matrix<VT> solve_equation(const Matrix<VT>& value) const {
            if (value.count_columns() != 1 || matrix_.size() != value.count_strings()) {
                throw func::AlgDomainError(__FILE__, __LINE__, "solve_equation, invalid matrix size.\n\n");
            }

            Matrix<VT> isv_matrix = (*this | Matrix<VT>(value)).improved_step_view();
            Matrix<VT> result(count_columns(), 1, VT(0.0));
            for (size_t i = 0, j = 0; i < matrix_.size(); ++i) {
                for (; j <= count_columns() && !func::equality(isv_matrix[i][j], VT(1.0)); ++j) {}

                if (j == count_columns()) {
                    return Matrix(0, 0);
                }
                if (j > count_columns()) {
                    break;
                }

                result[j][0] = isv_matrix[i].back();
            }
            return result;
        }

        static Matrix<VT> zip_map(const Matrix<VT>& matrix1, const Matrix<VT>& matrix2, std::function<VT(VT, VT)> zip_func) {
            Matrix<VT> result(std::min(matrix1.count_strings(), matrix2.count_strings()), std::min(matrix1.count_columns(), matrix2.count_columns()));
            for (size_t i = 0; i < result.count_strings(); ++i) {
                for (size_t j = 0; j < result.count_columns(); ++j) {
                    result[i][j] = zip_func(matrix1[i][j], matrix2[i][j]);
                }
            }
            return result;
        }

        static Matrix<VT> one_matrix(size_t size) {
            Matrix<VT> result(size, size, VT(0.0));
            for (size_t i = 0; i < size; ++i) {
                result[i][i] = 1;
            }
            return result;
        }

        static Matrix<VT> scale_matrix(VT scale_x, VT scale_y, VT scale_z) {
            return Matrix<VT>({
                { scale_x, VT(0.0), VT(0.0), VT(0.0) },
                { VT(0.0), scale_y, VT(0.0), VT(0.0) },
                { VT(0.0), VT(0.0), scale_z, VT(0.0) },
                { VT(0.0), VT(0.0), VT(0.0), VT(1.0) },
            });
        }

        static Matrix<VT> scale_matrix(const Vec3<VT>& scale) {
            return Matrix<VT>({
                { scale.x, VT(0.0), VT(0.0), VT(0.0) },
                { VT(0.0), scale.y, VT(0.0), VT(0.0) },
                { VT(0.0), VT(0.0), scale.z, VT(0.0) },
                { VT(0.0), VT(0.0), VT(0.0), VT(1.0) },
            });
        }

        static Matrix<VT> scale_matrix(VT scale) {
            return Matrix<VT>({
                {   scale, VT(0.0), VT(0.0), VT(0.0) },
                { VT(0.0),   scale, VT(0.0), VT(0.0) },
                { VT(0.0), VT(0.0),   scale, VT(0.0) },
                { VT(0.0), VT(0.0), VT(0.0), VT(1.0) },
            });
        }

        static Matrix<VT> translation_matrix(VT translation_x, VT translation_y, VT translation_z) {
            return Matrix<VT>({
                { VT(1.0), VT(0.0), VT(0.0), translation_x },
                { VT(0.0), VT(1.0), VT(0.0), translation_y },
                { VT(0.0), VT(0.0), VT(1.0), translation_z },
                { VT(0.0), VT(0.0), VT(0.0),       VT(1.0) },
            });
        }

        static Matrix<VT> translation_matrix(const Vec3<VT>& translation) {
            return Matrix<VT>({
                { VT(1.0), VT(0.0), VT(0.0), translation.x },
                { VT(0.0), VT(1.0), VT(0.0), translation.y },
                { VT(0.0), VT(0.0), VT(1.0), translation.z },
                { VT(0.0), VT(0.0), VT(0.0),       VT(1.0) },
            });
        }

        static Matrix<VT> rotation_matrix(const Vec3<VT>& axis, VT angle) {
            if (func::equality(axis.length(), VT(0.0))) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "rotation_matrix, the axis vector has zero length.\n\n");
            }

            const Vec3<VT>& norm_axis = axis.normalize();
            VT x = norm_axis.x;
            VT y = norm_axis.y;
            VT z = norm_axis.z;
            VT c = cosl(angle);
            VT s = sinl(angle);

            return Matrix<VT>({
                {     c + x * x * (1 - c), x * y * (1 - c) - z * s, x * z * (1 - c) + y * s, VT(0.0) },
                { y * x * (1 - c) + z * s,     c + y * y * (1 - c), y * z * (1 - c) - x * s, VT(0.0) },
                { z * x * (1 - c) - y * s, z * y * (1 - c) + x * s,     c + z * z * (1 - c), VT(0.0) },
                {                 VT(0.0),                 VT(0.0),                 VT(0.0), VT(1.0) },
            });
        }

        static Matrix<VT> normal_transform(const Matrix<VT>& transform) {
            if (transform.count_strings() != 4 || transform.count_columns() != 4) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "normal_transform, invalid matrix size.\n\n");
            }

            try {
                return transform.inverse().transpose();
            }
            catch (func::AlgDomainError) {
                throw func::AlgDomainError(__FILE__, __LINE__, "normal_transform, the matrix is not invertible.\n\n");
            }
        }
    };

    template <typename T>
    std::istream& operator>>(std::istream& fin, Matrix<T>& matrix) noexcept {
        for (size_t i = 0; i < matrix.count_strings(); ++i) {
            for (size_t j = 0; j < matrix.count_columns(); ++j) {
                fin >> matrix[i][j];
            }
        }
        return fin;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& fout, const Matrix<T>& matrix) {
        std::vector<std::vector<std::string>> description(matrix.count_strings(), std::vector<std::string>(matrix.count_columns()));
        for (size_t i = 0; i < matrix.count_strings(); ++i) {
            for (size_t j = 0; j < matrix.count_columns(); ++j) {
                description[i][j] = func::to_string(matrix[i][j]);
            }
        }

        fout << func::make_table_decorated(description, false, "", "", char(179), std::string(1, char(218)) + " " + std::string(1, char(191)), std::string(1, char(192)) + " " + std::string(1, char(217)));
        return fout;
    }

    template <typename T>
    Matrix<T> operator*(T value, const Matrix<T>& matrix) {
        return matrix * value;
    }


    // ^^^ -----------Matrix---------- ^^^
    // -----------------------------------
}   // MatrixLine, Matrix | Version: 1.0

using namespace alg::geom;
