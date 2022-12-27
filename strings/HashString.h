namespace alg::str {
    // -----------------------------------
    // vvv ---------SubString--------- vvv


    template <uint64_t BASE, uint64_t MODUL, uint8_t MINIMAL_SYMBOL>
    class HashString;

    template <uint64_t BASE, uint64_t MODUL, uint8_t MINIMAL_SYMBOL>
    class SubString {
        friend class HashString<BASE, MODUL, MINIMAL_SYMBOL>;

        size_t left_id_;
        size_t right_id_;
        const HashString<BASE, MODUL, MINIMAL_SYMBOL>* str_;

        SubString(size_t left_id, size_t right_id, const HashString<BASE, MODUL, MINIMAL_SYMBOL>* str) {
            if (left_id > right_id) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "SubString, invalid range.\n\n");
            }

            if (right_id >= str->size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "SubString, invalid range size.\n\n");
            }

            left_id_ = left_id;
            right_id_ = right_id;
            str_ = str;
        }

    public:
        char operator[](size_t index) const {
            if (index >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return (*str_)[left_id_ + index];
        }

        bool operator==(const SubString& other) const noexcept {
            return get_hash() == other.get_hash();
        }

        bool operator!=(const SubString& other) const noexcept {
            return get_hash() != other.get_hash();
        }

        std::strong_ordering operator<=>(const SubString& other) const noexcept {
            if (*this == other) {
                return std::strong_ordering::equal;
            }

            size_t equal_prefix_size = *this & other;
            if (equal_prefix_size == std::min(size(), other.size())) {
                return size() <=> other.size();
            }
            return (*this)[equal_prefix_size] <=> other[equal_prefix_size];
        }

        size_t operator&(const SubString& other) const noexcept {
            if ((*this)[0] != other[0]) {
                return 0;
            }

            size_t left = 0;
            size_t right = std::min(size(), other.size());
            while (right - left > 1) {
                size_t middle = (left + right) / 2;

                if (get_hash(0, middle) == other.get_hash(0, middle)) {
                    left = middle;
                }
                else {
                    right = middle;
                }
            }
            return right;
        }

        uint64_t get_hash(size_t left_id, size_t right_id) const {
            if (left_id > right_id) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "get_hash, invalid range.\n\n");
            }

            if (right_id >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_hash, invalid range size.\n\n");
            }

            return str_->get_hash(left_id_ + left_id, left_id_ + right_id);
        }

        uint64_t get_hash() const noexcept {
            return str_->get_hash(left_id_, right_id_);
        }

        std::string get_string() const noexcept {
            std::string str;
            for (size_t i = 0; i < size(); ++i) {
                str.push_back((*this)[i]);
            }
            return str;
        }

        size_t size() const noexcept {
            return right_id_ - left_id_ + 1;
        }
    };

    template <uint64_t BASE, uint64_t MODUL, uint8_t MINIMAL_SYMBOL>
    std::ostream& operator<<(std::ostream& fout, const SubString<BASE, MODUL, MINIMAL_SYMBOL>& sub_str) noexcept {
        fout << sub_str.get_string();
        return fout;
    }


    // ^^^ ---------SubString--------- ^^^
    // -----------------------------------
    // vvv ---------HashString-------- vvv


    // Default values for strings of English characters
    template <uint64_t BASE = 31, uint64_t MODUL = 1000000007, uint8_t MINIMAL_SYMBOL = 'a'>
    class HashString {
        inline static std::vector<uint64_t> power_ = std::vector<uint64_t>(1, 1);

        std::vector<uint64_t> hash_;

        void update_hash(const std::string& str) {
            hash_.clear();
            hash_.resize(str.size() + 1, 0);
            for (size_t i = 1; i <= str.size(); ++i) {
                if (str[i - 1] < MINIMAL_SYMBOL) {
                    throw func::AlgInvalidArgument(__FILE__, __LINE__, "update_hash, invalid minimal symbol value.\n\n");
                }

                hash_[i] = (hash_[i - 1] * BASE + static_cast<uint64_t>(str[i - 1] - MINIMAL_SYMBOL) + 1) % MODUL;
            }

            update_power(str.size() + 1);
        }

        static void update_power(size_t new_size) noexcept {
            power_.reserve(new_size);
            while (power_.size() < new_size) {
                power_.push_back((power_.back() * BASE) % MODUL);
            }
        }

    public:
        class ConstIterator {
            friend class HashString<BASE, MODUL, MINIMAL_SYMBOL>;

            int64_t id_;
            const HashString* str_;

            ConstIterator(int64_t id, const HashString* str) noexcept {
                id_ = id;
                str_ = str;
            }

        public:
            bool operator==(const ConstIterator& other) const {
                return id_ == other.id_ && str_ == other.str_;
            }

            bool operator!=(const ConstIterator& other) const {
                return !(*this == other);
            }

            std::strong_ordering operator<=>(const ConstIterator& other) const {
                return id_ <=> other.id_;
            }

            ConstIterator& operator+=(int64_t shift) noexcept {
                id_ += shift;
                return *this;
            }

            ConstIterator& operator-=(int64_t shift) noexcept {
                id_ -= shift;
                return *this;
            }

            char operator*() const {
                return (*str_)[id_];
            }

            ConstIterator& operator++() noexcept {
                id_++;
                return *this;
            }

            ConstIterator operator++(int) noexcept {
                ConstIterator result = *this;
                id_++;
                return result;
            }

            ConstIterator& operator--() noexcept {
                id_--;
                return *this;
            }

            ConstIterator operator--(int) noexcept {
                ConstIterator result = *this;
                id_--;
                return result;
            }

            ConstIterator operator+(int64_t shift) const noexcept {
                return ConstIterator(id_ + shift, str_);
            }

            ConstIterator operator-(int64_t shift) const noexcept {
                return ConstIterator(id_ - shift, str_);
            }

            int64_t operator-(ConstIterator other) const noexcept {
                return id_ - other.id_;
            }
        };

        HashString() noexcept {
            hash_.push_back(0);
        }

        explicit HashString(size_t size, char symbol = MINIMAL_SYMBOL) noexcept {
            if (symbol < MINIMAL_SYMBOL) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "HashString, invalid minimal symbol value.\n\n");
            }

            update_hash(std::string(size, symbol));
        }

        explicit HashString(const std::string& str) {
            update_hash(str);
        }

        HashString& operator=(const std::string& other)& {
            update_hash(other);
            return *this;
        }

        char operator[](size_t index) const {
            if (index >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            uint64_t hash = get_hash(index, index) + MODUL - 1;
            return MINIMAL_SYMBOL + static_cast<char>(hash < MODUL ? hash : hash - MODUL);
        }

        SubString<BASE, MODUL, MINIMAL_SYMBOL> operator()(size_t left_id, size_t right_id) const {
            if (left_id > right_id) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator(), invalid range.\n\n");
            }

            if (right_id >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator(), invalid range size.\n\n");
            }

            return SubString<BASE, MODUL, MINIMAL_SYMBOL>(left_id, right_id, this);
        }

        bool operator==(const HashString& other) const noexcept {
            return get_hash() == other.get_hash();
        }

        bool operator!=(const HashString& other) const noexcept {
            return get_hash() != other.get_hash();
        }

        std::strong_ordering operator<=>(const HashString& other) const noexcept {
            if (empty() && other.empty()) {
                return std::strong_ordering::equal;
            }
            if (empty()) {
                return std::strong_ordering::less;
            }
            if (other.empty()) {
                return std::strong_ordering::greater;
            }
            return (*this)(0, size() - 1) <=> other(0, other.size() - 1);
        }

        size_t operator&(const HashString& other) const noexcept {
            if (empty() || other.empty()) {
                return 0;
            }
            return *this & other;
        }

        uint64_t get_hash(size_t left_id, size_t right_id) const {
            if (left_id > right_id) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "get_hash, invalid range.\n\n");
            }

            if (right_id >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_hash, invalid range size.\n\n");
            }

            uint64_t hash = hash_[right_id + 1] + MODUL - (hash_[left_id] * power_[right_id - left_id + 1]) % MODUL;
            return hash < MODUL ? hash : hash - MODUL;
        }

        uint64_t get_hash() const noexcept {
            return hash_.back() % MODUL;
        }

        std::string get_string() const noexcept {
            std::string str;
            for (size_t i = 0; i < size(); ++i) {
                str.push_back((*this)[i]);
            }
            return str;
        }

        size_t size() const noexcept {
            return hash_.size() - 1;
        }

        bool empty() const noexcept {
            return hash_.size() > 1;
        }

        ConstIterator begin() const noexcept {
            return ConstIterator(0, this);
        }

        ConstIterator end() const noexcept {
            return ConstIterator(size(), this);
        }

        void swap(HashString& other) noexcept {
            std::swap(hash_, other.hash_);
        }

        void reserve(size_t size) noexcept {
            hash_.reserve(size + 1);
            power_.reserve(size + 1);
        }

        void push_back(char symbol) {
            if (symbol < MINIMAL_SYMBOL) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "push_back, invalid minimal symbol value.\n\n");
            }

            hash_.push_back((hash_.back() * BASE + static_cast<uint64_t>(symbol - MINIMAL_SYMBOL) + 1) % MODUL);
            update_power(hash_.size());
        }

        void pop_back() {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "pop_back, attempt to pop from empty string.\n\n");
            }

            hash_.pop_back();
        }

        void clear() noexcept {
            hash_.clear();
            hash_.push_back(0);
        }
    };

    template <uint64_t BASE, uint64_t MODUL, uint8_t MINIMAL_SYMBOL>
    std::istream& operator>>(std::istream& fin, HashString<BASE, MODUL, MINIMAL_SYMBOL>& hash_str) noexcept {
        std::string str;
        fin >> str;
        hash_str = str;
        return fin;
    }

    template <uint64_t BASE, uint64_t MODUL, uint8_t MINIMAL_SYMBOL>
    std::ostream& operator<<(std::ostream& fout, const HashString<BASE, MODUL, MINIMAL_SYMBOL>& hash_str) noexcept {
        fout << hash_str.get_string();
        return fout;
    }


    // ^^^ ---------HashString-------- ^^^
    // -----------------------------------
}   // HashString

using namespace alg::str;