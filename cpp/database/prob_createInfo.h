// Author: Dennis Yakovlev

// File containing members related to hash information.

#pragma once
#include <algorithm>
#include <prob_vars.h>
#include <string>

start_probability

/**
 * @brief Hash the coords into a unique number. 
 * 
 * @param coords 
 * @return size_t 
 */
size_t hash(coord_ty coords) {

    const auto first = _std string(max_grid_digits - _std to_string(coords.first).size(), '0') + _std to_string(coords.first);
    const auto second = _std string(max_grid_digits - _std to_string(coords.second).size(), '0') + _std to_string(coords.second);

    return _std stoll(first + second);

}

/**
 * @brief Unhash number into coords.
 * 
 */
auto unhash(size_t num) {

    _std string num_str = _std to_string(num);

    _std string first;
    _std string second;
    if (max_grid_digits > num_str.size()) {
        first = "0";
        second = "0";
    } else {
        first = num_str.substr(0, num_str.size() - max_grid_digits);
        second = num_str.substr(num_str.size() - max_grid_digits, max_grid_digits);
    }

    if (first.size() == 0) {
        first = "0";
    }

    if (second.size() == 0) {
        second = "0";
    }

    return coord_ty(_std stoll(first), _std stoll(second));

}

/**
 * @brief Get hashed values of all possible coords.
 * 
 */
size_vec hash_all() {

    size_vec vec(static_cast<size_vec::size_type>(_std round(_std pow(max_grid_sz + 1, 2))));
    auto iter_vec = vec.begin();
    for (size_t i = 0; i != max_grid_sz + 1; ++i) {
        for (size_t j = 0; j != max_grid_sz + 1; ++j) {
            *iter_vec = hash(coord_ty(i, j));
            ++iter_vec;
        }
    }

    return vec;

}
    
end_probability