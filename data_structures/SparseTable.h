namespace alg::data_struct {
    // -----------------------------------
    // vvv --------SparseTable-------- vvv


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

        ValueType top_element(size_t left, size_t right) const {
            if (left > right) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "top_element, invalid range.\n\n");
            }

            if (right >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "top_element, invalid range size.\n\n");
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


    // ^^^ --------SparseTable-------- ^^^
    // -----------------------------------
}   // SparseTable | Version: 1.0

using namespace alg::data_struct;