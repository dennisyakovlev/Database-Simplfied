// Author: Dennis Yakovlev

// File containing members relating to probability calculations.

#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iterator>
#include <BigInt.h>
#include <limits>
#include <prob_utils.h>
#include <prob_vars.h>
#include <vector>

start_probability

/**
 * @brief Number of paths through a point.
 * 
 * <p> Assume the given point is the final point in the grid
       meaning that there is no where to move after the point.
       So it is not necessary to multiply by the number of paths
       after the point since there are none.
 * </p>
 * 
 */
BigUnsigned path_num_end(coord_ty end) {

    return smart_factorial(end.first + end.second) / (smart_factorial(end.first) * smart_factorial(end.second));

}

BigUnsigned path_num_rel(coord_ty point, coord_ty end) {
    // Usage: get number of paths through a point
    // Explanation: assume the given point is anywhere on the grid. This means
    //              the total number of paths is the number before and after
    //              the point multiplied.
    // <point> coords of point
    // <end> coords of ending point

    return path_num_end(point) * path_num_end(relative(end, point));

}

/**
 * @brief Probability that path goes through a coordinate relative to end coordinate.
 * 
 * <p> Assume path_point <= path_end </p>
 * 
 * @param path_point number of paths through required coordinate
 * @param path_end number of paths through end coordinate
 * 
 */
double chance_path(const BigUnsigned& path_point, const BigUnsigned& path_end) {

    assert(_std numeric_limits<double>::digits10 >= precision10_digits && "no loss precision of double must be >= to precision10_digits");

    auto res = (path_end * BigUnsigned("1" + _std string(precision10_digits - 1, '0'))) / path_point;

    double as_dbl = BigUnsigned_10_dbl(res);

    if (as_dbl == -1) {
        return -1;
    }

    return static_cast<double>(precision10_value) * (1.0 / as_dbl); // assert needed here

}

/**
 * @brief Probability that path goes through a coordinate relative to end coordinate.
 * 
 * <p> Assume point.first <= end.first && point.second <= end.second </p>
 * 
 * @param point required coordinate coordinate 
 * @param end end coordinate
 */
double chance_path(coord_ty point, coord_ty end) {

    return chance_path(path_num_end(point) * path_num_end(relative(end, point)), path_num_end(end));

}

// need to fix when getting vertical line

/**
 * @brief Calculate the probability that an edge will be visited by a path for a coordinate 
 *        for all edges of that coordinate.
 * 
 * @param end 
 * @param _ tag to reference wanted function 
 * @return auto Container contains numbers in range of [0, 100] of percent chance for
 *         that edge being used by a path. The first element in the container is 100 for simplicity. 
 *         Should be ignored as it is not actual value. Order in container is bottom row of horizontal
 *         edges, then bottom row of vertical edges, continuing pattern going upwards in the grid.
 */
auto edge_prob(coord_ty end, DOUBLE _) {

    using prob_vec = _prob container_ty<double>;

    prob_vec::size_type size = static_cast<prob_vec::size_type>(1 + (end.first * (end.second + 1)) + ((end.first + 1) * end.second));
    prob_vec res(size, 100); // container for all percentages

    double remaining_moves = end.first + end.second; // dangerous from size_t to double, might not be enough space in double

    auto res_iter = res.begin();
    *res_iter = 100;
    ++res_iter;

    if (end.first == 0 || end.second == 0) {
        return res;
    }

    for (size_t i = 0; i != end.first; ++i, ++res_iter) { // first row of horizontal edges
        *res_iter = *(res_iter - 1) * ((end.first - i) / (remaining_moves - i));
    }

    for (size_t i = 0; i != end.first + 1; ++i, ++res_iter) { // first row of vertical edges
        *res_iter = *(res_iter - end.first - 1) * (end.second / (remaining_moves - i));
    }

    for (size_t i = 0; i != end.second - 1; ++i) {

        --remaining_moves;

        // horizontal
        *res_iter = *(res_iter - end.first - 1) * (end.first / remaining_moves);
        ++res_iter;
        for (size_t j = 0; j != end.first - 1; ++j, ++res_iter) {
            *res_iter = (*(res_iter - 1) + *(res_iter - end.first - 1)) * ((end.first - j - 1) / (remaining_moves - j - 1));
        }

        // vertical
        *res_iter = *(res_iter - (2 * end.first) - 1) * ((end.second - i - 1) / remaining_moves);
        ++res_iter;
        for (size_t j = 0; j != end.first; ++j, ++res_iter) {
            *res_iter = (*(res_iter - end.first - 1) + *(res_iter - (2 * end.first) - 1)) * ((end.second - i - 1) / (remaining_moves - j -1));
        }

    }

    --remaining_moves;
    *res_iter = *(res_iter - end.first - 1) * (end.first / remaining_moves);
    ++res_iter;
    for (size_t j = 0; j != end.first - 1; ++j, ++res_iter) { // last row of horizontal edges
        *res_iter = (*(res_iter - 1) + *(res_iter - end.first - 1)) * ((end.first - j - 1) / (remaining_moves - j - 1));
    }

    return res;

}

/**
 * @brief Calculate the probability that an edge will be visited by a path for a coordinate 
 *        for all edges of that coordinate. Probability not in [0,1] range, adjusted for
 *        system int size. 
 * 
 * <p> No reliance on double. Should be preferred function to use for database creation. </p>
 * 
 * @param end
 * @param _ tag to reference wanted function
 * @return auto Container contains numbers in range of [0, precision10_value] of percent chance for
 *         that edge being used by a path. The first element in the container is precision10_value for simplicity. 
 *         Should be ignored as it is not actual value. Order in container is bottom row of horizontal
 *         edges, then bottom row of vertical edges, continuing pattern going upwards in the grid.
 * 
 */
auto edge_prob(coord_ty end, INT_LEAST64 _) {

    using prob_vec = _prob container_ty<int_least64_t>;

    prob_vec::size_type size = static_cast<prob_vec::size_type>(1 + (end.first * (end.second + 1)) + ((end.first + 1) * end.second));
    prob_vec res(size, precision10_value); // container for all percentages

    int_least64_t remaining_moves = end.first + end.second;

    auto res_iter = res.begin();
    *res_iter = precision10_value;
    ++res_iter;

    if (end.first == 0 || end.second == 0) {
        return res;
    }

    for (size_t i = 0; i != end.first; ++i, ++res_iter) { // first row of horizontal edges        
        *res_iter = (*(res_iter - 1) * ((precision10_value * (end.first - i)) / (remaining_moves - i))) / precision10_value;
    }

    for (size_t i = 0; i != end.first + 1; ++i, ++res_iter) { // first row of vertical edges
        *res_iter = (*(res_iter - end.first - 1) * ((precision10_value * end.second) / (remaining_moves - i))) / precision10_value;
    }

    for (size_t i = 0; i != end.second - 1; ++i) {

        --remaining_moves;

        // horizontal
        *res_iter = (*(res_iter - end.first - 1) * ((precision10_value * end.first) / remaining_moves)) / precision10_value;
        ++res_iter;
        for (size_t j = 0; j != end.first - 1; ++j, ++res_iter) {
            *res_iter = ((*(res_iter - 1) + *(res_iter - end.first - 1)) * ((precision10_value * (end.first - j - 1)) / (remaining_moves - j - 1))) / precision10_value;
        }

        // vertical
        *res_iter = (*(res_iter - (2 * end.first) - 1) * ((precision10_value * (end.second - i - 1)) / remaining_moves)) / precision10_value;
        ++res_iter;
        for (size_t j = 0; j != end.first; ++j, ++res_iter) {
            *res_iter = ((*(res_iter - end.first - 1) + *(res_iter - (2 * end.first) - 1)) * ((precision10_value * (end.second - i - 1)) / (remaining_moves - j -1))) / precision10_value;
        }

    }

    --remaining_moves;
    *res_iter = (*(res_iter - end.first - 1) * ((precision10_value * end.first) / remaining_moves)) / precision10_value;
    ++res_iter;
    for (size_t j = 0; j != end.first - 1; ++j, ++res_iter) { // last row of horizontal edges
        *res_iter = ((*(res_iter - 1) + *(res_iter - end.first - 1)) * ((precision10_value * (end.first - j - 1)) / (remaining_moves - j - 1))) / precision10_value;
    }

    return res;

}

/**
 * @brief depreciated
 * 
 */
_std vector<int_least64_t> _convert_int64(_std vector<double>::const_iterator start, _std vector<double>::const_iterator end) {

    _std vector<int_least64_t> return_vec(end - start);
    auto iter_ret = return_vec.begin();
    for (; start != end; ++start, ++iter_ret) {
        // take double and make into 1 in X chance
        // Ex: 2 would be 1 in 2 = 50%
        *iter_ret = static_cast<int_least64_t>(std::llround(1 / (*start / 100)));
    }

    return return_vec;

}

/**
 * @brief depreciated
 * 
 */
_std vector<double> _convert_double(_std vector<int_least64_t>::const_iterator start, _std vector<int_least64_t>::const_iterator end) {

    _std vector<double> return_vec(end - start);
    auto iter_ret = return_vec.begin();
    for (; start != end; ++start, ++iter_ret) {
        *iter_ret = static_cast<double>(100 * (double(1) / *start));
    }

    return return_vec;

}

/**
 * @brief Convert vector of probabilities of int_least64_t into doubles of range [0,1].
 * 
 */
auto _int64_to_double(_prob container_ty<int_least64_t>::const_iterator start, _prob container_ty<int_least64_t>::const_iterator end) {

    _prob container_ty<double> mapped{};
    _std transform(start, end, _std back_inserter(mapped), 
        [=](int_least64_t n) -> double {
            return static_cast<double>(n) / precision10_value;
        }
    );

    return mapped;

}

end_probability