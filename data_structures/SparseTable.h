namespace alg::data_struct {
    // -----------------------------------
    // vvv --------SparseTable-------- vvv


    // VT constructors required: VT(VT); operators required: =(VT, VT)
    // PR operators required: (VT, VT) -> bool
    template <typename VT = int64_t, typename PR = std::less<VT>>
    class SparseTable {
        inline static std::vector<uint32_t> log2_ = { 0 };

        const PR& predicate_;
        std::vector<std::vector<VT>> table_;

        void build(const std::vector<VT>& init) {
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
                    const VT& left = table_[degree - 1][i];
                    const VT& right = table_[degree - 1][i + static_cast<size_t>(1 << (degree - 1))];
                    table_[degree].push_back(predicate_(left, right) ? left : right);
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
        using ConstIterator = std::vector<VT>::const_iterator;

        SparseTable() : predicate_(PR()) {
            build({});
        }

        explicit SparseTable(const PR& predicate) : predicate_(predicate) {
            build({});
        }

        explicit SparseTable(size_t size, const VT& value = VT(), const PR& predicate = PR()) : predicate_(predicate) {
            build(std::vector<VT>(size, value));
        }

        SparseTable(const std::initializer_list<VT>& init, const PR& predicate = PR()) : predicate_(predicate) {
            build(init);
        }

        explicit SparseTable(const std::vector<VT>& init, const PR& predicate = PR()) : predicate_(predicate) {
            build(init);
        }

        SparseTable<VT, PR>& operator=(const SparseTable<VT, PR>& other)& {
            table_ = other.table_;
            return *this;
        }

        SparseTable<VT, PR>& operator=(const std::vector<VT>& value)& {
            clear();
            build(value);
            return *this;
        }

        bool operator==(const SparseTable<VT, PR>& other) const {
            return table_[0] == other.table_[0];
        }

        bool operator!=(const SparseTable<VT, PR>& other) const {
            return table_[0] != other.table_[0];
        }

        const VT& operator[](size_t index) const {
            if (index >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return table_[0][index];
        }

        const VT& top_element(size_t left, size_t right) const {
            if (left > right) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "top_element, invalid range.\n\n");
            }

            if (right >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "top_element, invalid range size.\n\n");
            }

            uint32_t degree = log2_[right - left];
            const VT& left_val = table_[degree][left];
            const VT& right_val = table_[degree][right + 1 - static_cast<size_t>(1 << degree)];
            return predicate_(left_val, right_val) ? left_val : right_val;
        }

        size_t size() const noexcept {
            return table_[0].size();
        }

        bool empty() const noexcept {
            return table_[0].empty();
        }

        const VT& front() const {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "front, called from empty table.\n\n");
            }

            return table_[0].front();
        }

        const VT& back() const {
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

        void swap(SparseTable<VT, PR>& other) noexcept {
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

        void push_back(const VT& value) {
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

                const VT& left = table_[degree - 1][table_[degree].size()];
                const VT& right = table_[degree - 1][right_id];
                table_[degree].push_back(predicate_(left, right) ? left : right);
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