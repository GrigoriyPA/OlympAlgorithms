namespace alg::func {
    // -----------------------------------
    // vvv ---------Functions--------- vvv


    const long double EPS = 1e-7;
    const long double FI = (sqrtl(5.0) - 1.0) / 2.0;
    const long double PI = acosl(-1.0);


    class AlgDomainError : public std::domain_error {
    public:
        AlgDomainError(const char* filename, uint32_t line, std::string message) noexcept :
            domain_error("Domain error.\nFilename: " + std::string(filename) + "\nLine: " + std::to_string(line) + "\nDescription: " + message) {
        }
    };

    class AlgInvalidArgument : public std::invalid_argument {
    public:
        AlgInvalidArgument(const char* filename, uint32_t line, std::string message) noexcept :
            invalid_argument("Invalid argument error.\nFilename: " + std::string(filename) + "\nLine: " + std::to_string(line) + "\nDescription: " + message) {
        }
    };

    class AlgOutOfRange : public std::out_of_range {
    public:
        AlgOutOfRange(const char* filename, uint32_t line, std::string message) noexcept :
            out_of_range("Out of range error.\nFilename: " + std::string(filename) + "\nLine: " + std::to_string(line) + "\nDescription: " + message) {
        }
    };

    class AlgRuntimeError : public std::runtime_error {
    public:
        AlgRuntimeError(const char* filename, uint32_t line, std::string message) noexcept :
            runtime_error("Runtime error.\nFilename: " + std::string(filename) + "\nLine: " + std::to_string(line) + "\nDescription: " + message) {
        }
    };


    template <typename T>  // Operators required: <(T, T), ==(T, T)
    int32_t sgn(const T& x, const T& zero = T(0)) {
        if (x < zero) {
            return -1;
        }
        if (x == zero) {
            return 0;
        }
        return 1;
    }

    template <typename T1, typename T2>  // T1, T2 - standard numeric types
    bool equality(T1 left, T2 right, long double eps = EPS) noexcept {
        return std::abs(left - right) <= eps;
    }

    template <typename T1, typename T2>  // T1, T2 - standard numeric types
    bool less_equality(T1 left, T2 right, long double eps = EPS) noexcept {
        return left < right || equality(left, right, eps);
    }

    template <typename T>  // Operators required: <<(std::stringstream, T)
    std::string to_string(const T& value, std::streamsize precision = 6) {
        std::stringstream str_stream;
        str_stream.precision(precision);

        str_stream << value;
        return str_stream.str();
    }

    std::vector<std::string> split(const std::string& str, std::function<bool(char)> pred) {
        std::vector<std::string> split_str(1);
        for (char character : str) {
            bool skip_character = pred(character);
            if (split_str.back().size() > 0 && skip_character) {
                split_str.push_back("");
            } else if (!skip_character) {
                split_str.back().push_back(character);
            }
        }
        if (split_str.back().empty()) {
            split_str.pop_back();
        }
        return split_str;
    }

    std::vector<std::string> split(const std::string& str, char split_character) {
        return split(str, [=](char character) { return character == split_character; });
    }

    std::string make_table(const std::vector<std::vector<std::string>>& description) {
        size_t max_string_length = 0;
        for (const std::vector<std::string>& str : description) {
            max_string_length = std::max(max_string_length, str.size());
        }

        std::vector<size_t> max_cell_height(description.size(), 0);
        std::vector<size_t> max_cell_width(max_string_length, 0);
        std::vector<std::vector<std::vector<std::string>>> splited_description(description.size(), std::vector<std::vector<std::string>>(max_string_length));
        for (size_t i = 0; i < description.size(); ++i) {
            for (size_t j = 0; j < description[i].size(); ++j) {
                splited_description[i][j] = split(description[i][j], '\n');

                max_cell_height[i] = std::max(max_cell_height[i], splited_description[i][j].size());
                for (const std::string& str : splited_description[i][j]) {
                    max_cell_width[j] = std::max(max_cell_width[j], str.size());
                }
            }
        }

        std::string table;
        for (size_t i = 0; i < description.size(); ++i) {
            for (size_t str_id = 0; str_id < max_cell_height[i]; ++str_id) {
                for (size_t j = 0; j < max_string_length; ++j) {
                    size_t cur_length = 0;
                    if (str_id < splited_description[i][j].size()) {
                        cur_length = splited_description[i][j][str_id].size();
                        table += splited_description[i][j][str_id];
                    }

                    for (; cur_length < max_cell_width[j]; ++cur_length) {
                        table.push_back(' ');
                    }
                    table.push_back(' ');
                }
                table.push_back('\n');
            }
        }
        return table;
    }

    template <typename T>  // Operators required: <<(std::stringstream, T)
    std::string make_table(const std::vector<std::vector<T>>& description, std::streamsize precision = 6) {
        std::vector<std::vector<std::string>> description_strings(description.size());
        for (size_t i = 0; i < description.size(); ++i) {
            description_strings[i].resize(description[i].size());
            for (size_t j = 0; j < description[i].size(); ++j) {
                description_strings[i][j] = to_string(description[i][j], precision);
            }
        }
        return make_table(description_strings);
    }

    template <typename It>  // Operators required: !=(It, It), ++(It), *(It)
    std::string make_string(It container_begin, It container_end, std::streamsize precision = 6) {
        std::vector<std::vector<std::string>> table_description(1);
        for (; container_begin != container_end; ++container_begin) {
            table_description[0].push_back(to_string(*container_begin, precision));
        }
        return make_table(table_description);
    }
    
    // Up and down string format: "lmr" (l - left symbol, m - middle symbols, r - right symbol)
    std::string make_table_decorated(const std::vector<std::vector<std::string>>& description, bool named_columns = false, const std::string& vertical_sep = std::string(1, char(179)), const std::string& horizontal_sep = std::string(1, char(196)), char vertical_border = char(179), const std::string& up_border = std::string(1, char(218)) + std::string(1, char(196)) + std::string(1, char(191)), const std::string& down_border = std::string(1, char(192)) + std::string(1, char(196)) + std::string(1, char(217))) {
        size_t max_string_length = 0;
        for (const std::vector<std::string>& str : description) {
            max_string_length = std::max(max_string_length, str.size());
        }

        std::vector<size_t> max_cell_height(description.size(), 0);
        std::vector<size_t> max_cell_width(max_string_length, 0);
        std::vector<std::vector<std::vector<std::string>>> splited_description(description.size(), std::vector<std::vector<std::string>>(max_string_length));
        for (size_t i = 0; i < description.size(); ++i) {
            for (size_t j = 0; j < description[i].size(); ++j) {
                splited_description[i][j] = split(description[i][j], '\n');

                max_cell_height[i] = std::max(max_cell_height[i], splited_description[i][j].size());
                for (const std::string& str : splited_description[i][j]) {
                    max_cell_width[j] = std::max(max_cell_width[j], str.size());
                }
            }
        }

        std::string table;
        if (!up_border.empty()) {
            if (up_border.size() != 3) {
                throw AlgInvalidArgument(__FILE__, __LINE__, "make_table_decorated, invalid up border description");
            }

            table += up_border[0];
            table += up_border[1];
            for (size_t i = 0; i < max_string_length; ++i) {
                for (size_t j = 0; j < max_cell_width[i]; ++j) {
                    table += up_border[1];
                }

                if (up_border[1] == char(196) && vertical_sep == std::string(1, char(179)) && i + 1 < max_string_length) {
                    table += up_border[1];
                    table += char(194);
                    table += up_border[1];
                } 
                else {
                    for (size_t j = 0; j < 2 + vertical_sep.size() && i + 1 < max_string_length; ++j) {
                        table += up_border[1];
                    }
                }
            }
            table += up_border[1];
            table += up_border[2];
            table += '\n';
        }

        for (size_t i = 0; i < description.size(); ++i) {
            for (size_t str_id = 0; str_id < max_cell_height[i]; ++str_id) {
                table += std::string(1, vertical_border) + " ";
                for (size_t j = 0; j < max_string_length; ++j) {
                    size_t cur_length = 0;
                    if (str_id < splited_description[i][j].size()) {
                        cur_length = splited_description[i][j][str_id].size();
                        table += splited_description[i][j][str_id];
                    }

                    for (; cur_length < max_cell_width[j]; ++cur_length) {
                        table += ' ';
                    }
                    if (j + 1 < max_string_length) {
                        table += " " + vertical_sep + " ";
                    }
                }
                table += " " + std::string(1, vertical_border);
                table += '\n';
            }

            if (i == 0 && named_columns && i + 1 < description.size()) {
                if (up_border.empty()) {
                    throw AlgInvalidArgument(__FILE__, __LINE__, "make_table_decorated, invalid up border description");
                }

                if (vertical_border == char(179) && up_border[1] == char(196)) {
                    table += char(195);
                } 
                else {
                    table += vertical_border;
                }

                table += up_border[1];
                for (size_t j = 0; j < max_string_length; ++j) {
                    for (size_t k = 0; k < max_cell_width[j]; ++k) {
                        table += up_border[1];
                    }

                    if (up_border[1] == char(196) && vertical_sep == std::string(1, char(179)) && j + 1 < max_string_length) {
                        table += up_border[1];
                        table += char(197);
                        table += up_border[1];
                    } 
                    else {
                        for (size_t k = 0; k < 2 + vertical_sep.size() && j + 1 < max_string_length; ++k) {
                            table += up_border[1];
                        }
                    }
                }
                table += up_border[1];

                if (vertical_border == char(179) && up_border[1] == char(196)) {
                    table += char(180);
                } 
                else {
                    table += vertical_border;
                }

                table += '\n';
            } 
            else if (!horizontal_sep.empty() && i + 1 < description.size()) {
                if (horizontal_sep.size() != 1) {
                    throw AlgInvalidArgument(__FILE__, __LINE__, "make_table_decorated, invalid horizontal sep description");
                }

                if (vertical_border == char(179) && horizontal_sep == std::string(1, char(196))) {
                    table += char(195);
                }
                else {
                    table += vertical_border;
                }

                table += horizontal_sep;
                for (size_t j = 0; j < max_string_length; ++j) {
                    for (size_t k = 0; k < max_cell_width[j]; ++k) {
                        table += horizontal_sep;
                    }

                    if (j + 1 < max_string_length) {
                        if (horizontal_sep == std::string(1, char(196)) && vertical_sep == std::string(1, char(179))) {
                            table += horizontal_sep;
                            table += char(197);
                            table += horizontal_sep;
                        } 
                        else {
                            table += horizontal_sep + vertical_sep + horizontal_sep;
                        }
                    }
                }
                table += horizontal_sep;

                if (vertical_border == char(179) && horizontal_sep == std::string(1, char(196))) {
                    table += char(180);
                } 
                else {
                    table += vertical_border;
                }

                table += '\n';
            }
        }

        if (!down_border.empty()) {
            if (down_border.size() != 3) {
                throw AlgInvalidArgument(__FILE__, __LINE__, "make_table_decorated, invalid down border description");
            }

            table += down_border[0];
            table += down_border[1];
            for (size_t i = 0; i < max_string_length; ++i) {
                for (size_t j = 0; j < max_cell_width[i]; ++j) {
                    table += down_border[1];
                }

                if (down_border[1] == char(196) && vertical_sep == std::string(1, char(179)) && i + 1 < max_string_length) {
                    table += down_border[1];
                    table += char(193);
                    table += down_border[1];
                }
                else {
                    for (size_t j = 0; j < 2 + vertical_sep.size() && i + 1 < max_string_length; ++j) {
                        table += down_border[1];
                    }
                }
            }
            table += down_border[1];
            table += down_border[2];
            table += '\n';
        }

        return table;
    }

    template <typename T>  // Operators required: <<(std::stringstream, T); Up and down string format: "lmr" (l - left symbol, m - middle symbols, r - right symbol)
    std::string make_table_decorated(const std::vector<std::vector<T>>& description, std::streamsize precision = 6, bool named_columns = false, const std::string& vertical_sep = std::string(1, char(179)), const std::string& horizontal_sep = std::string(1, char(196)), char vertical_border = char(179), const std::string& up_border = std::string(1, char(218)) + std::string(1, char(196)) + std::string(1, char(191)), const std::string& down_border = std::string(1, char(192)) + std::string(1, char(196)) + std::string(1, char(217))) {
        std::vector<std::vector<std::string>> description_strings(description.size());
        for (size_t i = 0; i < description.size(); ++i) {
            description_strings[i].resize(description[i].size());
            for (size_t j = 0; j < description[i].size(); ++j) {
                description_strings[i][j] = to_string(description[i][j], precision);
            }
        }
        return make_table_decorated(description_strings, named_columns, vertical_sep, horizontal_sep, vertical_border, up_border, down_border);
    }

    template <typename T>  // Constructors required: T(T); Operators required: *(T, T), =(T, T)
    T binary_exponentiation(const T& base, uint32_t degree, const T& one = T(1)) {
        T result(one);
        for (size_t i = 8 * sizeof(uint32_t); i > 0; --i) {
            result = result * result;

            if (degree & (static_cast<uint32_t>(1) << (i - 1))) {
                result = result * base;
            }
        }
        return result;
    }

    template <typename T, class HT = std::hash<T>>  // Structures required: std::hash<T>
    void hash_combine(size_t& seed, const T& value, const HT& hasher = HT()) {
        seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template <typename It>  // Operators required: !=(It, It), ++(It), *(It); Structures required: std::iterator_traits<It>
    typename std::iterator_traits<It>::value_type get_value(It container_begin, It container_end, std::function<typename std::iterator_traits<It>::value_type(typename std::iterator_traits<It>::value_type, typename std::iterator_traits<It>::value_type)> func) {
        if (!(container_begin != container_end)) {
            throw func::AlgOutOfRange(__FILE__, __LINE__, "get_value, called from empty container.\n\n");
        }

        typename std::iterator_traits<It>::value_type result = *(container_begin++);
        for (; container_begin != container_end; ++container_begin) {
            result = func(result, *container_begin);
        }
        return result;
    }

    template <typename It>  // Operators required: !=(It, It), ++(It), *(It); Structures required: std::iterator_traits<It>
    typename std::iterator_traits<It>::value_type get_value(It container_begin, It container_end, const typename std::iterator_traits<It>::value_type& func(const typename std::iterator_traits<It>::value_type&, const typename std::iterator_traits<It>::value_type&)) {
        return get_value(container_begin, container_end, [&](const typename std::iterator_traits<It>::value_type& left, const typename std::iterator_traits<It>::value_type& right) -> const typename std::iterator_traits<It>::value_type& { return func(left, right); });
    }

    template <typename It>  // Operators required: !=(It, It), ++(It), *(It); Structures required: std::iterator_traits<It>
    typename std::iterator_traits<It>::value_type get_value(It container_begin, It container_end, typename std::iterator_traits<It>::value_type func(typename std::iterator_traits<It>::value_type, typename std::iterator_traits<It>::value_type)) {
        return get_value(container_begin, container_end, [&](const typename std::iterator_traits<It>::value_type& left, const typename std::iterator_traits<It>::value_type& right) -> const typename std::iterator_traits<It>::value_type& { return func(left, right); });
    }

    template <typename It>  // Operators required: !=(It, It), ++(It), *(It); Structures required: std::iterator_traits<It>
    void apply_func(It container_begin, It container_end, std::function<typename std::iterator_traits<It>::value_type(typename std::iterator_traits<It>::value_type)> func) {
        for (; container_begin != container_end; ++container_begin) {
            *container_begin = func(*container_begin);
        }
    }

    template <typename It>  // Operators required: !=(It, It), ++(It), *(It); Structures required: std::iterator_traits<It>
    void apply_func(It container_begin, It container_end, const typename std::iterator_traits<It>::value_type& func(const typename std::iterator_traits<It>::value_type&)) {
        apply_func(container_begin, container_end, [&](const typename std::iterator_traits<It>::value_type& value) -> const typename std::iterator_traits<It>::value_type& { return func(value); });
    }

    template <typename It>  // Operators required: !=(It, It), ++(It), *(It); Structures required: std::iterator_traits<It>
    void apply_func(It container_begin, It container_end, typename std::iterator_traits<It>::value_type func(typename std::iterator_traits<It>::value_type)) {
        apply_func(container_begin, container_end, [&](const typename std::iterator_traits<It>::value_type& value) -> const typename std::iterator_traits<It>::value_type& { return func(value); });
    }


    // ^^^ ---------Functions--------- ^^^
    // -----------------------------------
    // vvv -----------Random---------- vvv


    class Random {
        template <bool HAS_ZIP_MAP, typename T>
        class RandIntStruct {};

        template <bool HAS_ZIP_MAP, typename T>
        class RandFloatStruct {};

        template <typename T>  // Methods required: T::zip_map
        class RandIntStruct<true, T> {
            Random* random = nullptr;

        public:
            explicit RandIntStruct(Random* rnadom) {
                this->random = rnadom;
            }

            T operator()(const T& left, const T& right) const {
                return T::zip_map(left, right, [&](auto left, auto right) { return random->rand_int(left, right); });
            }
        };

        template <typename T>  // Methods required: T::zip_map
        class RandFloatStruct<true, T> {
            Random* random = nullptr;

        public:
            explicit RandFloatStruct(Random* rnadom) {
                this->random = rnadom;
            }

            T operator()(const T& left, const T& right) const {
                return T::zip_map(left, right, [&](auto left, auto right) { return random->rand_float(left, right); });
            }
        };

        template <typename T>
        class RandIntStruct<false, T> {
            Random* random = nullptr;

        public:
            explicit RandIntStruct(Random* rnadom) {
                this->random = rnadom;
            }

            T operator()(const T& left, const T& right) const {
                return random->get_rand_int(left, right);
            }
        };

        template <typename T>
        class RandFloatStruct<false, T> {
            Random* random = nullptr;

        public:
            explicit RandFloatStruct(Random* rnadom) {
                this->random = rnadom;
            }

            T operator()(const T& left, const T& right) const {
                return random->get_rand_float(left, right);
            }
        };

        std::mt19937_64 generator;

        long double rand() {
            return static_cast<long double>(generator()) / static_cast<long double>(generator.max());
        }

        int64_t get_rand_int(int64_t left, int64_t right) {
            if (right < left) {
                throw AlgInvalidArgument(__FILE__, __LINE__, "get_rand_int, invalid range.\n\n");
            }

            std::uniform_int_distribution<int64_t> int_generator(left, right);
            return int_generator(generator);
        }

        long double get_rand_float(long double left, long double right) {
            if (right < left) {
                throw AlgInvalidArgument(__FILE__, __LINE__, "get_rand_float, invalid range.\n\n");
            }

            return (right - left) * rand() + left;
        }

        template <typename T, typename... Args, typename U = decltype(T::zip_map(std::declval<Args>()...))>
        static constexpr std::true_type has_zip_map_impl(int, Args &&...args);

        template <typename T, typename... Args>
        static constexpr std::false_type has_zip_map_impl(float, Args &&...args);

        template <typename T, typename... Args>
        static constexpr bool has_zip_map(Args &&...args) {
            return decltype(has_zip_map_impl<T>(0, std::forward<Args>(args)...))::value;
        }

    public:
        explicit Random(uint64_t seed) {
            generator.seed(seed);
        }

        Random& set_seed(uint64_t seed) {
            generator.seed(seed);
            return *this;
        }

        template <typename T>
        T rand_int(T left, T right) {
            return RandIntStruct<has_zip_map<T>(left, right, [&](auto left, auto right) { return left; }), T>(this)(left, right);
        }

        template <typename It>  // Operators required: !=(It, It), ++(It), *(It)
        void rand_int_container(It left_begin, It left_end, It right_begin, It right_end, It result_begin) {
            for (; left_begin != left_end && right_begin != right_end; ++left_begin, ++right_begin, ++result_begin) {
                *result_begin = rand_int(*left_begin, *right_begin);
            }
        }

        template <typename It>  // Operators required: !=(It, It), ++(It), *(It); Structures required: std::iterator_traits<It>
        void rand_int_container(const typename std::iterator_traits<It>::value_type& left, const typename std::iterator_traits<It>::value_type& right, It result_begin, It result_end) {
            for (; result_begin != result_end; ++result_begin) {
                *result_begin = rand_int(left, right);
            }
        }

        template <typename T>
        T rand_float(T left, T right) {
            return RandFloatStruct<has_zip_map<T>(left, right, [&](auto left, auto right) { return left; }), T>(this)(left, right);
        }

        template <typename It>  // Operators required: !=(It, It), ++(It), *(It)
        void rand_float_container(It left_begin, It left_end, It right_begin, It right_end, It result_begin) {
            for (; left_begin != left_end && right_begin != right_end; ++left_begin, ++right_begin, ++result_begin) {
                *result_begin = rand_float(*left_begin, *right_begin);
            }
        }

        template <typename It>  // Operators required: !=(It, It), ++(It), *(It); Structures required: std::iterator_traits<It>
        void rand_float_container(const typename std::iterator_traits<It>::value_type& left, const typename std::iterator_traits<It>::value_type& right, It result_begin, It result_end) {
            for (; result_begin != result_end; ++result_begin) {
                *result_begin = rand_float(left, right);
            }
        }

        template <typename It>
        void shuffle(It begin, It end) {
            std::shuffle(begin, end, generator);
        }

        std::vector<size_t> permutation(size_t size) {
            std::vector<size_t> result(size);
            for (size_t i = 0; i < size; ++i) {
                result[i] = i + 1;
            }

            shuffle(result.begin(), result.end());
            return result;
        }
    };


    // ^^^ -----------Random---------- ^^^
    // -----------------------------------
}   // Functions, Random

using namespace alg::func;