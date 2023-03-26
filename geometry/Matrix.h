namespace alg::geom {
    // -----------------------------------
    // vvv ---------MatrixLine-------- vvv


    template <typename ValueType = long double>  // ValueType - standard numeric type
    class MatrixLine {
        template <typename T>
        friend class Matrix;

        std::vector<ValueType> line_;

        template <typename T>  // Casts required: ValueType(T)
        MatrixLine(const std::initializer_list<T>& init) {
            line_.reserve(init.size());
            for (const T& element : init) {
                line_.emplace_back(element);
            }
        }

        template <typename T>  // Casts required: ValueType(T)
        explicit MatrixLine(const std::vector<T>& init) {
            line_.reserve(init.size());
            for (const T& element : init) {
                line_.emplace_back(element);
            }
        }

        explicit MatrixLine(size_t count_columns, ValueType value = ValueType(0.0)) {
            line_.resize(count_columns, value);
        }

    public:
        template <typename T>  // Constructors required: T(T), T(ValueType)
        explicit operator std::vector<T>() const {
            std::vector<T> result;
            result.reserve(line_.size());
            for (ValueType element : line_) {
                result.emplace_back(element);
            }
            return result;
        }

        ValueType& operator[](size_t index) {
            if (line_.size() <= index) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return line_[index];
        }

        const ValueType& operator[](size_t index) const {
            if (line_.size() <= index) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return line_[index];
        }

        MatrixLine<ValueType>& operator=(const MatrixLine<ValueType>& other)& {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator=, invalid line sizes.\n\n");
            }

            line_ = other.line_;
            return *this;
        }

        bool operator==(const MatrixLine<ValueType>& other) const noexcept {
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

        bool operator!=(const MatrixLine<ValueType>& other) const noexcept {
            return !(*this == other);
        }

        MatrixLine<ValueType>& operator+=(const MatrixLine<ValueType>& other)& {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator+=, invalid line sizes.\n\n");
            }

            for (size_t i = 0; i < line_.size(); ++i) {
                line_[i] += other[i];
            }
            return *this;
        }

        MatrixLine<ValueType>& operator-=(const MatrixLine<ValueType>& other)& {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator-=, invalid line sizes.\n\n");
            }

            for (size_t i = 0; i < line_.size(); ++i) {
                line_[i] -= other[i];
            }
            return *this;
        }

        MatrixLine<ValueType>& operator*=(ValueType other)& noexcept {
            for (ValueType& element : line_) {
                element *= other;
            }
            return *this;
        }

        MatrixLine<ValueType>& operator/=(ValueType other)& {
            if (func::equality(other, ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/=, division by zero.\n\n");
            }

            for (ValueType& element : line_) {
                element /= other;
            }
            return *this;
        }

        MatrixLine<ValueType> operator-() const {
            return *this * ValueType(-1.0);
        }

        MatrixLine<ValueType> operator+(const MatrixLine<ValueType>& other) const {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator+, invalid line sizes.\n\n");
            }

            MatrixLine<ValueType> result = *this;
            for (size_t i = 0; i < other.size(); ++i) {
                result[i] += other[i];
            }

            return result;
        }

        MatrixLine<ValueType> operator-(const MatrixLine<ValueType>& other) const {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator-, invalid line sizes.\n\n");
            }

            MatrixLine<ValueType> result = *this;
            for (size_t i = 0; i < other.size(); ++i) {
                result[i] -= other[i];
            }

            return result;
        }

        MatrixLine<ValueType> operator*(ValueType other) const {
            MatrixLine<ValueType> result = *this;
            return result *= other;
        }

        MatrixLine<ValueType> operator/(ValueType other) const {
            if (func::equality(other, ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/, division by zero.\n\n");
            }

            MatrixLine<ValueType> result = *this;
            for (ValueType& element : result.line_) {
                element /= other;
            }
            return result;
        }

        ValueType operator*(const MatrixLine<ValueType>& other) const {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator*, invalid line sizes.\n\n");
            }

            ValueType result = 0;
            for (size_t i = 0; i < line_.size(); ++i) {
                result += line_[i] * other[i];
            }
            return result;
        }

        ValueType get_value(std::function<ValueType(ValueType, ValueType)> func) const {
            if (line_.empty()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_value, called from empty line.\n\n");
            }

            ValueType result = line_[0];
            for (size_t i = 1; i < line_.size(); ++i) {
                result = func(result, line_[i]);
            }
            return result;
        }

        ValueType get_value(const ValueType& func(const ValueType&, const ValueType&)) const {
            return get_value([&](const ValueType& left, const ValueType& right) -> const ValueType& { return func(left, right); });
        }

        ValueType get_value(ValueType func(ValueType, ValueType)) const {
            return get_value([&](const ValueType& left, const ValueType& right) -> const ValueType& { return func(left, right); });
        }

        ValueType& front() {
            if (line_.empty()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "front, called from empty line.\n\n");
            }

            return line_.front();
        }

        ValueType& back() {
            if (line_.empty()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "back, called from empty line.\n\n");
            }

            return line_.back();
        }

        const ValueType& back() const {
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

        MatrixLine<ValueType> multiply(const MatrixLine<ValueType>& other) const {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "multiply, invalid line sizes.\n\n");
            }

            MatrixLine<ValueType> result = *this;
            for (size_t i = 0; i < other.size(); ++i) {
                result[i] *= other[i];
            }

            return result;
        }

        MatrixLine<ValueType> divide(const MatrixLine<ValueType>& other) const {
            if (line_.size() != other.size()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "divide, invalid line sizes.\n\n");
            }

            MatrixLine<ValueType> result = *this;
            for (size_t i = 0; i < other.size(); ++i) {
                if (func::equality(other[i], ValueType(0.0))) {
                    throw func::AlgDomainError(__FILE__, __LINE__, "divide, division by zero.\n\n");
                }

                result[i] /= other[i];
            }

            return result;
        }

        void apply_func(std::function<ValueType(ValueType)> func) {
            for (size_t i = 0; i < line_.size(); ++i) {
                line_[i] = func(line_[i]);
            }
        }

        void apply_func(const ValueType& func(const ValueType&)) {
            apply_func([&](const ValueType& value) -> const ValueType& { return func(value); });
        }

        void apply_func(ValueType func(ValueType)) {
            apply_func([&](const ValueType& value) -> const ValueType& { return func(value); });
        }

        void swap(MatrixLine<ValueType>& other) {
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


    template <typename ValueType = long double>  // ValueType - standard numeric type
    class Matrix {
        std::vector<MatrixLine<ValueType>> matrix_;

    public:
        template <typename T>  // Casts required: ValueType(T)
        explicit Matrix(const T& value) {
            matrix_.push_back(MatrixLine<ValueType>(1, ValueType(value)));
        }

        template <typename T>  // Casts required: ValueType(T)
        Matrix(const std::initializer_list<T>& init) {
            matrix_.reserve(init.size());
            for (const T& value : init) {
                matrix_.push_back(MatrixLine<ValueType>(1, ValueType(value)));
            }
        }

        template <typename T>  // Casts required: ValueType(T)
        explicit Matrix(const std::vector<T>& init) {
            matrix_.reserve(init.size());
            for (const T& value : init) {
                matrix_.push_back(MatrixLine<ValueType>(1, ValueType(value)));
            }
        }

        template <typename T>  // Casts required: ValueType(T)
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

                matrix_.push_back(MatrixLine<ValueType>(line));
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

                matrix_.push_back(MatrixLine<ValueType>(line));
            }
        }

        Matrix(const Vec3<ValueType>& vector_x, const Vec3<ValueType>& vector_y, const Vec3<ValueType>& vector_z) {
            *this = Matrix<ValueType>({
                { vector_x.x, vector_y.x, vector_z.x, ValueType(0.0) },
                { vector_x.y, vector_y.y, vector_z.y, ValueType(0.0) },
                { vector_x.z, vector_y.z, vector_z.z, ValueType(0.0) },
                {    ValueType(0.0),    ValueType(0.0),    ValueType(0.0), ValueType(1.0) },
            });
        }

        Matrix(size_t count_lines, size_t count_columns, ValueType value, const Matrix<ValueType>& init) {
            matrix_.resize(count_lines, MatrixLine<ValueType>(count_columns, value));
            for (size_t i = 0; i < std::min(count_lines, init.count_strings()); ++i) {
                for (size_t j = 0; j < std::min(count_columns, init.count_columns()); ++j) {
                    matrix_[i][j] = init[i][j];
                }
            }
        }

        Matrix(size_t count_lines, size_t count_columns, ValueType value = ValueType(0.0)) {
            matrix_.resize(count_lines, MatrixLine<ValueType>(count_columns, value));
        }

        Matrix(size_t count_lines, const MatrixLine<ValueType>& value) {
            matrix_.resize(count_lines, value);
        }

        template <typename T>  // Constructors required: T(ValueType)
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

        MatrixLine<ValueType>& operator[](size_t index) {
            if (matrix_.size() <= index) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return matrix_[index];
        }

        const MatrixLine<ValueType>& operator[](size_t index) const {
            if (matrix_.size() <= index) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return matrix_[index];
        }

        bool operator==(const Matrix<ValueType>& other) const noexcept {
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

        bool operator!=(const Matrix<ValueType>& other) const noexcept {
            return !(*this == other);
        }

        Matrix<ValueType>& operator+=(const Matrix<ValueType>& other)& {
            if (matrix_.size() != other.count_strings() || count_columns() != other.count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator+=, invalid matrix sizes.\n\n");
            }

            for (size_t i = 0; i < matrix_.size(); i++) {
                matrix_[i] += other[i];
            }
            return *this;
        }

        Matrix<ValueType>& operator-=(const Matrix<ValueType>& other)& {
            if (matrix_.size() != other.count_strings() || count_columns() != other.count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator-=, invalid matrix sizes.\n\n");
            }

            for (size_t i = 0; i < matrix_.size(); i++) {
                matrix_[i] -= other[i];
            }
            return *this;
        }

        Matrix<ValueType>& operator*=(ValueType other)& noexcept {
            for (size_t i = 0; i < matrix_.size(); i++) {
                matrix_[i] *= other;
            }
            return *this;
        }

        Matrix<ValueType>& operator*=(const Matrix<ValueType>& other)& {
            if (count_columns() != other.count_strings()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator*=, invalid matrix sizes.\n\n");
            }

            const Matrix<ValueType>& transposed = other.transpose();
            Matrix<ValueType> result(matrix_.size(), other.count_columns(), ValueType(0.0));
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < other.count_columns(); ++j) {
                    result[i][j] = matrix_[i] * transposed[j];
                }
            }
            return *this = result;
        }

        Matrix<ValueType>& operator/=(ValueType other)& {
            if (func::equality(other, ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/=, division by zero.\n\n");
            }

            for (size_t i = 0; i < matrix_.size(); i++) {
                matrix_[i] /= other;
            }
            return *this;
        }

        Matrix<ValueType>& operator/=(const Matrix<ValueType>& other)& {
            try {
                *this *= other.inverse();
            }
            catch (func::AlgDomainError) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/=, division by a nonsingular matrix.\n\n");
            }
            return *this;
        }

        Matrix<ValueType>& operator^=(uint32_t other)& {
            if (matrix_.size() != count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator^=, invalid matrix size.\n\n");
            }

            return *this = func::binary_exponentiation(*this, other, one_matrix(matrix_.size()));
        }

        Matrix<ValueType>& operator|=(const Matrix<ValueType>& other)& {
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

        Matrix<ValueType> operator-() const {
            return *this * ValueType(-1.0);
        }

        Matrix<ValueType> operator+(const Matrix<ValueType>& other) const {
            if (matrix_.size() != other.count_strings() || count_columns() != other.count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator+, invalid matrix sizes.\n\n");
            }

            Matrix<ValueType> result = *this;
            for (size_t i = 0; i < matrix_.size(); i++) {
                result[i] += other[i];
            }
            return result;
        }

        Matrix<ValueType> operator-(const Matrix<ValueType>& other) const {
            if (matrix_.size() != other.count_strings() || count_columns() != other.count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator-, invalid matrix sizes.\n\n");
            }

            Matrix<ValueType> result = *this;
            for (size_t i = 0; i < matrix_.size(); i++) {
                result[i] -= other[i];
            }
            return result;
        }

        Matrix<ValueType> operator*(ValueType other) const {
            Matrix<ValueType> result = *this;
            for (size_t i = 0; i < matrix_.size(); i++) {
                result[i] *= other;
            }
            return result;
        }

        Matrix<ValueType> operator*(const Matrix<ValueType>& other) const {
            if (count_columns() != other.count_strings()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator*, invalid matrix sizes.\n\n");
            }

            const Matrix<ValueType>& transposed = other.transpose();
            Matrix<ValueType> result(matrix_.size(), other.count_columns(), ValueType(0.0));
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < other.count_columns(); ++j) {
                    result[i][j] = matrix_[i] * transposed[j];
                }
            }
            return result;
        }

        Vec3<ValueType> operator*(const Vec3<ValueType>& other) const {
            if (matrix_.size() != 4 || matrix_[0].size() != 4) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator*, invalid matrix size.\n\n");
            }

            Vec3<ValueType> result;
            for (size_t i = 0; i < 3; ++i) {
                for (size_t j = 0; j < 3; ++j) {
                    result[i] += matrix_[i][j] * other[j];
                }
                result[i] += matrix_[i][3];
            }
            return result;
        }

        Matrix<ValueType> operator/(ValueType other) const {
            if (func::equality(other, ValueType(0.0))) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/, division by zero.\n\n");
            }

            Matrix<ValueType> result = *this;
            for (size_t i = 0; i < matrix_.size(); i++) {
                result[i] /= other;
            }
            return result;
        }

        Matrix<ValueType> operator/(const Matrix<ValueType>& other) const {
            try {
                return *this * other.inverse();
            }
            catch (func::AlgDomainError) {
                throw func::AlgDomainError(__FILE__, __LINE__, "operator/, division by a nonsingular matrix.\n\n");
            }
        }

        Matrix<ValueType> operator^(uint32_t other) const {
            if (matrix_.size() != count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator^, invalid matrix size.\n\n");
            }

            return func::binary_exponentiation(*this, other, one_matrix(matrix_.size()));
        }

        Matrix<ValueType> operator|(const Matrix<ValueType>& other) const {
            if (matrix_.size() != other.count_strings()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator|, invalid matrix sizes.\n\n");
            }

            Matrix<ValueType> result(matrix_.size(), count_columns() + other.count_columns(), ValueType(0), *this);
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < other.count_columns(); ++j) {
                    result[i][count_columns() + j] = other[i][j];
                }
            }
            return result;
        }

        Matrix<ValueType> multiply(const Matrix<ValueType>& other) const {
            if (count_strings() != other.count_strings() || count_columns() != other.count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "multiply, invalid matrix sizes.\n\n");
            }

            Matrix<ValueType> result = *this;
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < matrix_[i].size(); ++j) {
                    result[i][j] *= other[i][j];
                }
            }
            return result;
        }

        Matrix<ValueType> divide(const Matrix<ValueType>& other) const {
            if (count_strings() != other.count_strings() || count_columns() != other.count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "divide, invalid matrix sizes.\n\n");
            }

            Matrix<ValueType> result = *this;
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < matrix_[i].size(); ++j) {
                    if (func::equality(other[i][j], ValueType(0.0))) {
                        throw func::AlgDomainError(__FILE__, __LINE__, "divide, division by zero.\n\n");
                    }

                    result[i][j] /= other[i][j];
                }
            }

            return result;
        }

        ValueType get_value(std::function<ValueType(ValueType, ValueType)> func) const {
            if (matrix_.empty()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_value, called from empty matrix.\n\n");
            }

            ValueType result = matrix_[0][0];
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

        ValueType get_value(const ValueType& func(const ValueType&, const ValueType&)) const {
            return get_value([&](const ValueType& left, const ValueType& right) -> const ValueType& { return func(left, right); });
        }

        ValueType get_value(ValueType func(ValueType, ValueType)) const {
            return get_value([&](const ValueType& left, const ValueType& right) -> const ValueType& { return func(left, right); });
        }

        // axis = 0: horizontal; axis = 1: vertical
        Matrix<ValueType> get_value(std::function<ValueType(ValueType, ValueType)> func, bool axis) const {
            if (matrix_.empty()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_value, called from empty matrix.\n\n");
            }

            Matrix<ValueType> current_matrix = *this;
            if (axis) {
                current_matrix = current_matrix.transpose();
            }

            Matrix<ValueType> result(current_matrix.count_strings(), 1);
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
        Matrix<ValueType> get_value(const ValueType& func(const ValueType&, const ValueType&), bool axis) const {
            return get_value([&](const ValueType& left, const ValueType& right) -> const ValueType& { return func(left, right); }, axis);
        }

        // axis = 0: horizontal; axis = 1: vertical
        Matrix<ValueType> get_value(ValueType func(ValueType, ValueType), bool axis) const {
            return get_value([&](const ValueType& left, const ValueType& right) -> const ValueType& { return func(left, right); }, axis);
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

        void apply_func(std::function<ValueType(ValueType)> func) {
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < matrix_[i].size(); ++j) {
                    matrix_[i][j] = func(matrix_[i][j]);
                }
            }
        }

        void apply_func(const ValueType& func(const ValueType&)) {
            apply_func([&](const ValueType& value) -> const ValueType& { return func(value); });
        }

        void apply_func(ValueType func(ValueType)) {
            apply_func([&](const ValueType& value) -> const ValueType& { return func(value); });
        }

        void swap(Matrix<ValueType>& other) noexcept {
            matrix_.swap(other.matrix_);
        }

        Matrix<ValueType> transpose() const {
            Matrix<ValueType> result(count_columns(), matrix_.size());
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < matrix_[i].size(); ++j) {
                    result[j][i] = matrix_[i][j];
                }
            }
            return result;
        }

        Matrix<ValueType> flip_horizontally() const {
            Matrix<ValueType> result(matrix_.size(), count_columns());
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < matrix_[i].size(); ++j) {
                    result[matrix_.size() - 1 - i][j] = matrix_[i][j];
                }
            }
            return result;
        }

        Matrix<ValueType> flip_vertically() const {
            Matrix<ValueType> result(matrix_.size(), count_columns());
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < matrix_[i].size(); ++j) {
                    result[i][matrix_[i].size() - 1 - j] = matrix_[i][j];
                }
            }
            return result;
        }

        Matrix<ValueType> rotate_clockwise() const {
            return flip_horizontally().transpose();
        }

        Matrix<ValueType> rotate_counterclockwise() const {
            return flip_vertically().transpose();
        }

        Matrix<ValueType> submatrix(size_t line, size_t column, size_t height, size_t width) const {
            if (line + height > matrix_.size() || column + width > count_columns()) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "submatrix, invalid submatrix size.\n\n");
            }

            Matrix<ValueType> result(height, width);
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    result[i][j] = matrix_[line + i][column + j];
                }
            }
            return result;
        }

        Matrix<ValueType> improved_step_view() const {
            Matrix<ValueType> result = *this;
            for (size_t j = 0, i0 = 0; j < count_columns() && i0 < matrix_.size(); ++j) {
                size_t k = matrix_.size();
                for (size_t i = i0; i < matrix_.size(); ++i) {
                    if (!func::equality(result[i][j], ValueType(0.0))) {
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

        ValueType determinant() const {
            if (matrix_.size() != count_columns()) {
                throw func::AlgDomainError(__FILE__, __LINE__, "determinant, not a square matrix.\n\n");
            }

            ValueType result(1.0);
            Matrix<ValueType> cur_matrix = *this;
            for (size_t j = 0; j < count_columns(); ++j) {
                size_t k = matrix_.size();
                for (size_t i = j; i < matrix_.size(); ++i) {
                    if (!func::equality(cur_matrix[i][j], ValueType(0.0))) {
                        k = i;
                        break;
                    }
                }

                if (k == matrix_.size()) {
                    return ValueType(0.0);
                }

                if (k != j) {
                    result *= ValueType(-1.0);
                    cur_matrix[j].swap(cur_matrix[k]);
                }

                result *= cur_matrix[j][j];
                for (size_t i = k + 1; i < matrix_.size(); ++i) {
                    cur_matrix[i] -= cur_matrix[j] * (cur_matrix[i][j] / cur_matrix[j][j]);
                }
            }
            return result;
        }

        Matrix<ValueType> inverse() const {
            if (matrix_.size() != count_columns()) {
                throw func::AlgDomainError(__FILE__, __LINE__, "inverse, not a square matrix.\n\n");
            }

            Matrix<ValueType> result = (*this | one_matrix(matrix_.size())).improved_step_view();
            if (result.submatrix(0, 0, matrix_.size(), matrix_.size()) != one_matrix(matrix_.size())) {
                throw func::AlgDomainError(__FILE__, __LINE__, "inverse, the matrix is not invertible.\n\n");
            }
            return result.submatrix(0, matrix_.size(), matrix_.size(), matrix_.size());
        }

        Matrix<ValueType> solve_equation(const Matrix<ValueType>& value) const {
            if (value.count_columns() != 1 || matrix_.size() != value.count_strings()) {
                throw func::AlgDomainError(__FILE__, __LINE__, "solve_equation, invalid matrix size.\n\n");
            }

            Matrix<ValueType> isv_matrix = (*this | Matrix<ValueType>(value)).improved_step_view();
            Matrix<ValueType> result(count_columns(), 1, ValueType(0.0));
            for (size_t i = 0, j = 0; i < matrix_.size(); ++i) {
                for (; j <= count_columns() && !func::equality(isv_matrix[i][j], ValueType(1.0)); ++j) {}

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

        static Matrix<ValueType> zip_map(const Matrix<ValueType>& matrix1, const Matrix<ValueType>& matrix2, std::function<ValueType(ValueType, ValueType)> zip_func) {
            Matrix<ValueType> result(std::min(matrix1.count_strings(), matrix2.count_strings()), std::min(matrix1.count_columns(), matrix2.count_columns()));
            for (size_t i = 0; i < result.count_strings(); ++i) {
                for (size_t j = 0; j < result.count_columns(); ++j) {
                    result[i][j] = zip_func(matrix1[i][j], matrix2[i][j]);
                }
            }
            return result;
        }

        static Matrix<ValueType> one_matrix(size_t size) {
            Matrix<ValueType> result(size, size, ValueType(0.0));
            for (size_t i = 0; i < size; ++i) {
                result[i][i] = 1;
            }
            return result;
        }

        static Matrix<ValueType> scale_matrix(ValueType scale_x, ValueType scale_y, ValueType scale_z) {
            return Matrix<ValueType>({
                { scale_x, ValueType(0.0), ValueType(0.0), ValueType(0.0) },
                { ValueType(0.0), scale_y, ValueType(0.0), ValueType(0.0) },
                { ValueType(0.0), ValueType(0.0), scale_z, ValueType(0.0) },
                { ValueType(0.0), ValueType(0.0), ValueType(0.0), ValueType(1.0) },
            });
        }

        static Matrix<ValueType> scale_matrix(const Vec3<ValueType>& scale) {
            return Matrix<ValueType>({
                { scale.x, ValueType(0.0), ValueType(0.0), ValueType(0.0) },
                { ValueType(0.0), scale.y, ValueType(0.0), ValueType(0.0) },
                { ValueType(0.0), ValueType(0.0), scale.z, ValueType(0.0) },
                { ValueType(0.0), ValueType(0.0), ValueType(0.0), ValueType(1.0) },
            });
        }

        static Matrix<ValueType> scale_matrix(ValueType scale) {
            return Matrix<ValueType>({
                {   scale, ValueType(0.0), ValueType(0.0), ValueType(0.0) },
                { ValueType(0.0),   scale, ValueType(0.0), ValueType(0.0) },
                { ValueType(0.0), ValueType(0.0),   scale, ValueType(0.0) },
                { ValueType(0.0), ValueType(0.0), ValueType(0.0), ValueType(1.0) },
            });
        }

        static Matrix<ValueType> translation_matrix(ValueType translation_x, ValueType translation_y, ValueType translation_z) {
            return Matrix<ValueType>({
                { ValueType(1.0), ValueType(0.0), ValueType(0.0), translation_x },
                { ValueType(0.0), ValueType(1.0), ValueType(0.0), translation_y },
                { ValueType(0.0), ValueType(0.0), ValueType(1.0), translation_z },
                { ValueType(0.0), ValueType(0.0), ValueType(0.0),       ValueType(1.0) },
            });
        }

        static Matrix<ValueType> translation_matrix(const Vec3<ValueType>& translation) {
            return Matrix<ValueType>({
                { ValueType(1.0), ValueType(0.0), ValueType(0.0), translation.x },
                { ValueType(0.0), ValueType(1.0), ValueType(0.0), translation.y },
                { ValueType(0.0), ValueType(0.0), ValueType(1.0), translation.z },
                { ValueType(0.0), ValueType(0.0), ValueType(0.0),       ValueType(1.0) },
            });
        }

        static Matrix<ValueType> rotation_matrix(const Vec3<ValueType>& axis, ValueType angle) {
            if (func::equality(axis.length(), ValueType(0.0))) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "rotation_matrix, the axis vector has zero length.\n\n");
            }

            const Vec3<ValueType>& norm_axis = axis.normalize();
            ValueType x = norm_axis.x;
            ValueType y = norm_axis.y;
            ValueType z = norm_axis.z;
            ValueType c = cosl(angle);
            ValueType s = sinl(angle);

            return Matrix<ValueType>({
                {     c + x * x * (1 - c), x * y * (1 - c) - z * s, x * z * (1 - c) + y * s, ValueType(0.0) },
                { y * x * (1 - c) + z * s,     c + y * y * (1 - c), y * z * (1 - c) - x * s, ValueType(0.0) },
                { z * x * (1 - c) - y * s, z * y * (1 - c) + x * s,     c + z * z * (1 - c), ValueType(0.0) },
                {          ValueType(0.0),          ValueType(0.0),          ValueType(0.0), ValueType(1.0) },
            });
        }

        static Matrix<ValueType> normal_transform(const Matrix<ValueType>& transform) {
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
