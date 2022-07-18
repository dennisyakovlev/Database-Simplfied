// Author: Dennis Yakovlev

// File containing members to be used by other members in pathprob namespace.

#pragma once
#include <array>
#include <BigInt.h>
#include <climits>
#include <cstdint>
#include <vector>

// Try to limit use of macros replacing simple functions and variables
#define namespace_pathprob pathprob

#define start_probability namespace namespace_pathprob {
#define end_probability }

#define _std std::
#define _prob namespace_pathprob::

start_probability

    // functions --------------------------------
    /**
     * @brief constexpr power function. 
     * 
     */
    constexpr _std int_least64_t pow(_std int_least64_t base, _std int_least64_t exp) {
        return exp == 1 ? 1 : base * pow(base, exp - 1);
    }

    // using declerations -----------------------
    /**
     * @brief size type for pathprob namespace. Must be signed interger type.
     * 
     */
    using size_t = long long;
    /**
     * @brief unsigned char type.
     * 
     */
    using uchar_t = unsigned char;
    /**
     * @brief Container type for pathprob namespace.
     * 
     * <p> Must be ordered container. </p>
     * 
     */
    template<typename T> using container_ty = _std vector<T>;
    
    // variables --------------------------------
    /**
     * @brief Maximum X, Y coordinate as described in coord_y.
     * 
     * <p> Use some form of 99... since hash table in map file is populated in a 
     *     percentage of number of grid points. Where highest percentage
     *     population is when highest value for that number of digits. 
     * </p>
     * 
     */
    static constexpr size_t max_grid_sz = 99;
    /**
     * @brief Number of digits in max_grid_sz.
     * 
     */
    static constexpr size_t max_grid_digits = 2;
    static constexpr char new_line = '\n';
    /**
     * @brief Number of base10 digit precision.
     * 
     */
    static constexpr size_t precision10_digits = _std numeric_limits<int_least64_t>::digits10 / 2;
    /**
     * @brief base10 value corresponding 10 ^ (precision10_digits - 1).
     * 
     */
    static constexpr _std int_least64_t precision10_value = _prob pow(10, precision10_digits);

    // using declerations -----------------------
    using size_vec = _std vector<size_t>;
    /**
     * @brief factorial array
     * 
     */
    using fac_arr = _std array<BigUnsigned, (2 * max_grid_sz) + 1>;
    /**
     * @brief Coordinate pair for point. First is move right and second is move up.
     *        As a result coordinates start at 0,0.
     * 
     */
    using coord_ty = _std pair<size_t, size_t>;

    // variables --------------------------------
    /**
     * @brief Factorial array.
     * 
     */
    fac_arr factorials;
    /**
     * @brief true if \p factorials has been populated.
     * 
     */
    bool factorials_populated = false;

    // size variables ---------------------------
    static constexpr size_t size_sz = sizeof(size_t{}); // size of size type
    static constexpr size_t char_sz = sizeof(uchar_t{}); // size of char type
    static constexpr size_t double_sz = sizeof(double{}); // size of double
    static constexpr size_t size_max = INT32_MAX; // max size of size tpye
    static constexpr size_t uchar_max = UCHAR_MAX; // max size of char tpye

    // structs ----------------------------------
    /**
     * @brief double tag
     * 
     */
    struct DOUBLE {};
    /**
     * @brief int_least64_t tag
     * 
     */
    struct INT_LEAST64 {};

end_probability