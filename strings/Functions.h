namespace alg::str {
    // -----------------------------------
    // vvv ---------Functions--------- vvv


    std::vector<size_t> prefix_function(const std::string& str) {
        std::vector<size_t> pref_func(str.size(), 0);
        for (size_t i = 1; i < str.size(); ++i) {
            size_t last_pref_function = pref_func[i - 1];
            while (last_pref_function > 0 && str[i] != str[last_pref_function]) {
                last_pref_function = pref_func[last_pref_function - 1];
            }

            pref_func[i] = last_pref_function + static_cast<size_t>(str[i] == str[last_pref_function]);
        }

        return pref_func;
    }


    std::vector<size_t> z_function(const std::string& str) {
        std::vector<size_t> z_func(str.size(), 0);

        size_t z_block_left = 0;
        size_t z_block_right = 0;
        for (size_t i = 1; i < str.size(); ++i) {
            if (i <= z_block_right) {
                z_func[i] = std::min(z_block_right - i, z_func[i - z_block_left]);
            }
            while (i + z_func[i] < str.size() && str[z_func[i]] == str[i + z_func[i]]) {
                ++z_func[i];
            }

            if (i + z_func[i] > z_block_right) {
                z_block_left = i;
                z_block_right = i + z_func[i];
            }
        }

        return z_func;
    }


    // ^^^ ---------Functions--------- ^^^
    // -----------------------------------
}   // Functions | Version: 0.0

using namespace alg::str;