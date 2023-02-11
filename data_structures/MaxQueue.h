namespace alg::data_struct {
    // -----------------------------------
    // vvv ----------MaxQueue--------- vvv


    template <typename ValueType = int64_t, typename Predicate = std::less<ValueType>>  // Constructors required: ValueType(), ValueType(ValueType); Operators required: <(ValueType, ValueType)
    class MaxQueue {
        struct QueueElement {
            ValueType value = ValueType();
            size_t id = 0;
        };

        size_t left_id_ = 0;
        size_t right_id_ = 0;

        std::deque<QueueElement> max_queue_;

        template <typename T>  // Casts required: ValueType(T)
        void build_queue(const std::vector<T>& value) noexcept {
            for (const T& element : value) {
                push(element);
            }
        }

    public:
        MaxQueue() noexcept {
        }

        MaxQueue(size_t size, const ValueType& init) noexcept {
            build_queue(std::vector<ValueType>(size, init));
        }

        template <typename T>  // Casts required: ValueType(T)
        MaxQueue(const std::vector<T>& init) noexcept {
            build_queue(init);
        }

        template <typename T>  // Casts required: ValueType(T)
        MaxQueue(const std::initializer_list<T>& init) noexcept {
            build_queue(std::vector<T>(init));
        }

        size_t get_left_id() const noexcept {
            return left_id_;
        }

        size_t get_right_id() const noexcept {
            return right_id_;
        }

        ValueType max_element() const {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "max_element, attempt to find maximal element in empty queue.\n\n");
            }

            return max_queue_.front().value;
        }

        size_t max_element_id() const {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "max_element_id, attempt to find maximal element in empty queue.\n\n");
            }

            return max_queue_.front().id;
        }

        size_t size() const noexcept {
            return right_id_ - left_id_;
        }

        bool empty() const noexcept {
            return max_queue_.empty();
        }

        void swap(MaxQueue& other) noexcept {
            std::swap(left_id_, other.left_id_);
            std::swap(right_id_, other.right_id_);
            std::swap(max_queue_, other.max_queue_);
        }

        void reserve(size_t size) noexcept {
            max_queue_.reserve(size);
        }

        size_t push(const ValueType& value) noexcept {
            while (!max_queue_.empty() && Predicate()(max_queue_.back().value, value)) {
                max_queue_.pop_back();
            }

            max_queue_.push_back(QueueElement{ .value = value, .id = right_id_ });
            return right_id_++;
        }

        // Delete all elements before id
        void pop(size_t id) {
            if (id > right_id_) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "pop, invalid index.\n\n");
            }

            for (; !max_queue_.empty() && max_queue_.front().id < id; max_queue_.pop_front()) {}
            left_id_ = id;
        }

        void pop() {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "pop, attempt to pop from empty queue.\n\n");
            }

            if (max_queue_.front().id == left_id_) {
                max_queue_.pop_front();
            }
            ++left_id_;
        }

        void clear() noexcept {
            left_id_ = 0;
            right_id_ = 0;
            max_queue_.clear();
        }

        template <typename T, typename PRED>
        friend std::ostream& operator<<(std::ostream& fout, const MaxQueue<T, PRED>& vector) noexcept;
    };

    template <typename T, typename Predicate>
    std::istream& operator>>(std::istream& fin, MaxQueue<T, Predicate>& max_queue) noexcept {
        size_t size = max_queue.size();
        max_queue.clear();
        for (size_t i = 0; i < size; ++i) {
            T value;
            fin >> value;

            max_queue.push(value);
        }
        return fin;
    }

    template <typename T, typename Predicate>
    std::ostream& operator<<(std::ostream& fout, const MaxQueue<T, Predicate>& max_queue) noexcept {
        fout << "Left id: " << max_queue.left_id_ << "\n";
        fout << "Right id: " << max_queue.right_id_ << "\n";
        fout << "Queue state:\n";

        std::vector<std::vector<std::string>> description(max_queue.max_queue_.size() + 1);
        description[0] = { "id", "value" };
        size_t i = 0;
        for (const auto& [value, id] : max_queue.max_queue_) {
            description[++i] = { func::to_string(id), func::to_string(value) };
        }

        fout << func::make_table_decorated(description);
        return fout;
    }


    // ^^^ ----------MaxQueue--------- ^^^
    // -----------------------------------
}   // MaxQueue | Version: 0.0

using namespace alg::data_struct;