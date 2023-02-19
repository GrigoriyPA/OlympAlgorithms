namespace alg::data_struct {
    // -----------------------------------
    // vvv -------SparseTable1D------- vvv


    // ValueType constructors required: ValueType(ValueType); operators required: =(ValueType, ValueType)
    // Operation operators required: (ValueType, ValueType) -> ValueType
    // Expected: Operation -- associativity, commutativity, idempotence
    template <typename ValueType = int64_t, typename Operation = std::bit_or<ValueType>>
    class SparseTable {
        inline static std::vector<uint32_t> log2_ = { 0 };

        const Operation& operation_;
        std::vector<std::vector<ValueType>> table_;

        void build(const std::vector<ValueType>& init) {
            if (init.empty()) {
                table_.resize(1);
                return;
            }

            update_log2(init.size());
            table_.resize(log2_[init.size() - 1] + 1);

            table_[0] = init;
            for (uint32_t degree = 1; degree < table_.size(); ++degree) {
                table_[degree].reserve(init.size() + 1 - static_cast<size_t>(1 << degree));
                for (size_t i = 0; i + static_cast<size_t>(1 << degree) <= init.size(); ++i) {
                    const ValueType& left = table_[degree - 1][i];
                    const ValueType& right = table_[degree - 1][i + static_cast<size_t>(1 << (degree - 1))];
                    table_[degree].push_back(operation_(left, right));
                }
            }
        }

        static void update_log2(size_t new_size) {
            log2_.reserve(new_size);
            while (log2_.size() < new_size) {
                log2_.push_back(log2_.back());
                log2_.back() += static_cast<size_t>(1 << (log2_.back() + 1)) <= log2_.size();
            }
        }

    public:
        using ConstIterator = std::vector<ValueType>::const_iterator;

        SparseTable() : operation_(Operation()) {
            build({});
        }

        explicit SparseTable(const Operation& operation) : operation_(operation) {
            build({});
        }

        explicit SparseTable(size_t size, const ValueType& value = ValueType(), const Operation& operation = Operation()) : operation_(operation) {
            build(std::vector<ValueType>(size, value));
        }

        SparseTable(const std::initializer_list<ValueType>& init, const Operation& operation = Operation()) : operation_(operation) {
            build(init);
        }

        explicit SparseTable(const std::vector<ValueType>& init, const Operation& operation = Operation()) : operation_(operation) {
            build(init);
        }

        SparseTable<ValueType, Operation>& operator=(const SparseTable<ValueType, Operation>& other)& {
            table_ = other.table_;
            return *this;
        }

        SparseTable<ValueType, Operation>& operator=(const std::vector<ValueType>& value)& {
            clear();
            build(value);
            return *this;
        }

        bool operator==(const SparseTable<ValueType, Operation>& other) const {
            return table_[0] == other.table_[0];
        }

        bool operator!=(const SparseTable<ValueType, Operation>& other) const {
            return table_[0] != other.table_[0];
        }

        const ValueType& operator[](size_t index) const {
            if (index >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return table_[0][index];
        }

        ValueType get_result(size_t left, size_t right) const {
            if (left > right) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "get_result, invalid range.\n\n");
            }

            if (right >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_result, invalid range size.\n\n");
            }

            uint32_t degree = log2_[right - left];
            const ValueType& left_val = table_[degree][left];
            const ValueType& right_val = table_[degree][right + 1 - static_cast<size_t>(1 << degree)];
            return operation_(left_val, right_val);
        }

        size_t size() const noexcept {
            return table_[0].size();
        }

        bool empty() const noexcept {
            return table_[0].empty();
        }

        const ValueType& front() const {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "front, called from empty table.\n\n");
            }

            return table_[0].front();
        }

        const ValueType& back() const {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "back, called from empty table.\n\n");
            }

            return table_[0].back();
        }

        ConstIterator begin() const noexcept {
            return table_[0].begin();
        }

        ConstIterator end() const noexcept {
            return table_[0].end();
        }

        void swap(SparseTable<ValueType, Operation>& other) noexcept {
            table_.swap(other.table_);
        }

        void reserve(size_t size) {
            if (size == 0) {
                return;
            }

            update_log2(size);
            if (table_.size() < log2_[size - 1] + 1) {
                table_.resize(log2_[size - 1] + 1);
            }

            for (size_t degree = 0; degree < table_.size(); ++degree) {
                table_[degree].reserve(size + 1 - static_cast<size_t>(1 << degree));
            }
        }

        void push_back(const ValueType& value) {
            update_log2(size() + 1);
            if (table_.size() < log2_[size()] + 1) {
                table_.resize(log2_[size()] + 1);
            }

            table_[0].push_back(value);
            for (uint32_t degree = 1; degree < table_.size(); ++degree) {
                size_t right_id = table_[degree].size() + static_cast<size_t>(1 << (degree - 1));
                if (right_id >= table_[degree - 1].size()) {
                    break;
                }

                const ValueType& left = table_[degree - 1][table_[degree].size()];
                const ValueType& right = table_[degree - 1][right_id];
                table_[degree].push_back(operation_(left, right));
            }
        }

        void pop_back() {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "pop_back, attempt to pop from empty table.\n\n");
            }

            for (uint32_t degree = 0; degree < table_.size() && !table_[degree].empty(); ++degree) {
                table_[degree].pop_back();
            }
        }

        void clear() {
            table_.clear();
            table_.resize(1);
        }
    };


    // ^^^ -------SparseTable1D------- ^^^
    // -----------------------------------
    // vvv -------SparseTable2D------- vvv


    // ValueType constructors required: ValueType(ValueType); operators required: =(ValueType, ValueType)
    // Operation operators required: (ValueType, ValueType) -> ValueType
    // Expected: Operation -- associativity, commutativity, idempotence
    template <typename ValueType = int64_t, typename Operation = std::bit_or<ValueType>>
    class SparseTable2D {
        inline static std::vector<uint32_t> log2_ = { 0 };

        const Operation& operation_;
        std::vector<std::vector<std::vector<std::vector<ValueType>>>> table_;

        void build(const std::vector<std::vector<ValueType>>& init) {
            size_t init_width = init[0].size();
            for (const auto& element : init) {
                if (element.size() != init_width) {
                    throw func::AlgInvalidArgument(__FILE__, __LINE__, "build, init matrix is not rectangle.\n\n");
                }
            }

            if (init.empty() || init_width == 0) {
                table_.resize(1, std::vector<std::vector<std::vector<ValueType>>>(1));
                return;
            }

            update_log2(std::max(init.size(), init_width));
            table_.resize(log2_[init.size() - 1] + 1, std::vector<std::vector<std::vector<ValueType>>>(log2_[init_width - 1] + 1));

            table_[0][0] = init;
            for (uint32_t degree_x = 0; degree_x < table_.size(); ++degree_x) {
                for (uint32_t degree_y = 0; degree_y < table_[degree_x].size(); ++degree_y) {
                    if (degree_x == 0 && degree_y == 0) {
                        continue;
                    }

                    table_[degree_x][degree_y].resize(init.size() + 1 - static_cast<size_t>(1 << degree_x));
                    for (size_t i = 0; i + static_cast<size_t>(1 << degree_x) <= init.size(); ++i) {
                        table_[degree_x][degree_y][i].reserve(init[i].size() + 1 - static_cast<size_t>(1 << degree_y));
                        for (size_t j = 0; j + static_cast<size_t>(1 << degree_y) <= init[i].size(); ++j) {
                            if (degree_x == 0) {
                                const ValueType& left = table_[degree_x][degree_y - 1][i][j];
                                const ValueType& right = table_[degree_x][degree_y - 1][i][j + static_cast<size_t>(1 << (degree_y - 1))];
                                table_[degree_x][degree_y][i].push_back(operation_(left, right));
                            }
                            else {
                                const ValueType& left = table_[degree_x - 1][degree_y][i][j];
                                const ValueType& right = table_[degree_x - 1][degree_y][i + static_cast<size_t>(1 << (degree_x - 1))][j];
                                table_[degree_x][degree_y][i].push_back(operation_(left, right));
                            }
                        }
                    }
                }
            }
        }

        static void update_log2(size_t new_size) {
            log2_.reserve(new_size);
            while (log2_.size() < new_size) {
                log2_.push_back(log2_.back());
                log2_.back() += static_cast<size_t>(1 << (log2_.back() + 1)) <= log2_.size();
            }
        }

    public:
        SparseTable2D() : operation_(Operation()) {
            build({});
        }

        explicit SparseTable2D(const Operation& operation) : operation_(operation) {
            build({});
        }

        explicit SparseTable2D(size_t height, size_t width, const ValueType& value = ValueType(), const Operation& operation = Operation()) : operation_(operation) {
            build(std::vector<std::vector<ValueType>>(height, std::vector<ValueType>(width, value)));
        }

        SparseTable2D(const std::initializer_list<std::initializer_list<ValueType>>& init, const Operation& operation = Operation()) : operation_(operation) {
            std::vector<std::vector<ValueType>> vector_init(init.size());
            for (const auto& element : init) {
                vector_init.emplace_back(element);
            }
            build(vector_init);
        }

        explicit SparseTable2D(const std::vector<std::vector<ValueType>>& init, const Operation& operation = Operation()) : operation_(operation) {
            build(init);
        }

        SparseTable2D<ValueType, Operation>& operator=(const SparseTable2D<ValueType, Operation>& other)& {
            table_ = other.table_;
            return *this;
        }

        SparseTable2D<ValueType, Operation>& operator=(const std::vector<std::vector<ValueType>>& value)& {
            clear();
            build(value);
            return *this;
        }

        bool operator==(const SparseTable2D<ValueType, Operation>& other) const {
            return table_[0][0] == other.table_[0][0];
        }

        bool operator!=(const SparseTable2D<ValueType, Operation>& other) const {
            return table_[0][0] != other.table_[0][0];
        }

        const std::vector<ValueType>& operator[](size_t index) const {
            if (index >= height()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return table_[0][0][index];
        }

        ValueType get_result(size_t up, size_t left, size_t down, size_t right) const {
            if (left > right || up > down) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "get_result, invalid range.\n\n");
            }

            if (down >= height() || right >= width()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_result, invalid range size.\n\n");
            }

            uint32_t degree_x = log2_[down - up];
            uint32_t degree_y = log2_[right - left];
            const ValueType& up_left_val = table_[degree_x][degree_y][up][left];
            const ValueType& up_right_val = table_[degree_x][degree_y][up][right + 1 - static_cast<size_t>(1 << degree_y)];
            const ValueType& down_left_val = table_[degree_x][degree_y][down + 1 - static_cast<size_t>(1 << degree_x)][left];
            const ValueType& down_right_val = table_[degree_x][degree_y][down + 1 - static_cast<size_t>(1 << degree_x)][right + 1 - static_cast<size_t>(1 << degree_y)];
            return operation_(operation_(up_left_val, up_right_val), operation_(down_left_val, down_right_val));
        }

        size_t height() const noexcept {
            return table_[0][0].size();
        }

        size_t width() const noexcept {
            if (table_[0][0].empty()) {
                return 0;
            }
            return table_[0][0][0].size();
        }

        bool empty() const noexcept {
            return table_[0][0].empty();
        }

        void swap(SparseTable2D<ValueType, Operation>& other) noexcept {
            table_.swap(other.table_);
        }

        void clear() {
            table_.clear();
            table_.resize(1, std::vector<std::vector<std::vector<ValueType>>>(1));
        }
    };


    // ^^^ -------SparseTable2D------- ^^^
    // -----------------------------------
    // vvv ----DisjointSparseTable---- vvv


    // ValueType constructors required: ValueType(ValueType); operators required: =(ValueType, ValueType)
    // Operation operators required: (ValueType, ValueType) -> ValueType
    // Expected: Operation -- associativity, commutativity
    template <typename ValueType = int64_t, typename Operation = std::plus<ValueType>>
    class DisjointSparseTable {
        inline static std::vector<uint32_t> log2_ = { 0 };

        const Operation& operation_;
        std::vector<std::vector<ValueType>> table_;

        void build_table(size_t left, size_t right, size_t level, const std::vector<ValueType>& init) {
            size_t middle = (left + right) / 2;

            for (size_t i = middle + 2; i < std::min(right + 1, init.size()); ++i) {
                table_[level][i] = operation_(table_[level][i - 1], init[i]);
            }

            if (middle > 0) {
                for (size_t i = std::min(middle - 1, init.size() - 2); i >= left; --i) {
                    table_[level][i] = operation_(table_[level][i + 1], init[i]);

                    if (i == left) {
                        break;
                    }
                }
            }

            if (left != right) {
                build_table(left, middle, level - 1, init);
                build_table(middle + 1, right, level - 1, init);
            }
        }

        void build(const std::vector<ValueType>& init) {
            if (init.empty()) {
                table_.resize(1);
                return;
            }

            update_log2(2 * init.size() - 1);
            uint32_t size = log2_[2 * init.size() - 2];
            table_.resize(size + 1, init);

            if (init.size() > 1) {
                build_table(0, (1ll << size) - 1, size, init);
            }
        }

        static void update_log2(size_t new_size) {
            log2_.reserve(new_size);
            while (log2_.size() < new_size) {
                log2_.push_back(log2_.back());
                log2_.back() += static_cast<size_t>(1 << (log2_.back() + 1)) <= log2_.size();
            }
        }

    public:
        using ConstIterator = std::vector<ValueType>::const_iterator;

        DisjointSparseTable() : operation_(Operation()) {
            build({});
        }

        explicit DisjointSparseTable(const Operation& operation) : operation_(operation) {
            build({});
        }

        explicit DisjointSparseTable(size_t size, const ValueType& value = ValueType(), const Operation& operation = Operation()) : operation_(operation) {
            build(std::vector<ValueType>(size, value));
        }

        DisjointSparseTable(const std::initializer_list<ValueType>& init, const Operation& operation = Operation()) : operation_(operation) {
            build(init);
        }

        explicit DisjointSparseTable(const std::vector<ValueType>& init, const Operation& operation = Operation()) : operation_(operation) {
            build(init);
        }

        DisjointSparseTable<ValueType, Operation>& operator=(const DisjointSparseTable<ValueType, Operation>& other)& {
            table_ = other.table_;
            return *this;
        }

        DisjointSparseTable<ValueType, Operation>& operator=(const std::vector<ValueType>& value)& {
            clear();
            build(value);
            return *this;
        }

        bool operator==(const DisjointSparseTable<ValueType, Operation>& other) const {
            return table_[0] == other.table_[0];
        }

        bool operator!=(const DisjointSparseTable<ValueType, Operation>& other) const {
            return table_[0] != other.table_[0];
        }

        const ValueType& operator[](size_t index) const {
            if (index >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return table_[0][index];
        }

        ValueType get_result(size_t left, size_t right) const {
            if (left > right) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "get_result, invalid range.\n\n");
            }

            if (right >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_result, invalid range size.\n\n");
            }

            if (left == right) {
                return table_[0][left];
            }

            uint32_t level = log2_[(left ^ right) - 1] + 1;
            ValueType result = table_[level][left];

            if (right & (1ll << (level - 1))) {
                return operation_(result, table_[level][right]);
            }
            return result;
        }

        size_t size() const noexcept {
            return table_[0].size();
        }

        bool empty() const noexcept {
            return table_[0].empty();
        }

        const ValueType& front() const {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "front, called from empty table.\n\n");
            }

            return table_[0].front();
        }

        const ValueType& back() const {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "back, called from empty table.\n\n");
            }

            return table_[0].back();
        }

        ConstIterator begin() const noexcept {
            return table_[0].begin();
        }

        ConstIterator end() const noexcept {
            return table_[0].end();
        }

        void swap(DisjointSparseTable<ValueType, Operation>& other) noexcept {
            table_.swap(other.table_);
        }

        void clear() {
            table_.clear();
            table_.resize(1);
        }
    };


    // ^^^ ----DisjointSparseTable---- ^^^
    // -----------------------------------
}   // SparseTable1D, SparseTable2D, DisjointSparseTable | Version: 1.0

using namespace alg::data_struct;