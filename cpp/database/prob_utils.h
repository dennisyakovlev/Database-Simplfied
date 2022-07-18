// Author: Dennis Yakovlev

// File containing miscellaneous members.

#pragma once
#include <BigInt.h>
#include <iterator>
#include <prob_vars.h>
#include <string>
#include <type_traits>
#include <utility>

start_probability

template<typename T, typename U>
T _adv(T& iter, U n) {

    _std advance(iter, n);
    return iter;

}

template<typename T, typename U>
T _adv(const T& iter, U n) {

    auto iter_new(iter);
    _std advance(iter_new, n);
    return iter_new;

}

/**
 * @brief Equivalent to std::advance just with return value. Allows advancing of
 *        iterator without needing temporary variable to assign.
 * 
 * @param iter see std::advance
 * @param n see std::advance
 * @return auto advanced iterator
 */
template<typename T, typename U>
auto advance(T&& iter, U n) {

    return _adv(_std forward<T>(iter), n);

}

/**
 * @brief Convert result of std::disatnce to \p size_t
 * 
 * <p> Use function since easier to debug than macro
       and performance is not that vital. </p>
 * 
 * @param first see std::distance
 * @param last see std::distance
 */
template<typename Iter>
inline size_t dist_to_sizety(Iter first, Iter last) {

    return static_cast<size_t>(_std distance(first, last));

}

// should not allow negative num
/**
 * @brief Mathematical factorial of a number.
 * 
 * <p> Assume num >= 0 </p>
 * 
 */
BigUnsigned factorial(size_t num) {
    
    BigUnsigned res("1");
    num += 1;

    while (--num > 0) { // compare to 0 to avoid new variable
        res = res * BigUnsigned(_std to_string(num));
    }

    return res;

}

/**
 * @brief Instead of calculating factorial, lookup from table if num is
 *        in table. Otherwise calculate normally.
 * 
 */
BigUnsigned smart_factorial(size_t num) {

    if (factorials_populated && num < factorials.size()) {
        return *(advance(factorials.cbegin(), num));
    }

    return factorial(num);

}

/**
 * @brief Populate container with factorial in range [0, container size).
 * 
 * <p> The distance between the iterator and starting iterator is the value
 *     of the factorial of that iterator position.
 *     <br> Ex: cont_ptr->cbeing() + 3 will have factorial value for 3.
 * </p>
 * <p> Do not check that T is valid pointer to valid container.
 * </p>
 * <p> Container must have a value_type of BigUnsigned.
 * </p>
 * 
 * @param cont_ptr pointer to container to populate
 */
template<typename T, typename = _std enable_if_t<_std is_same_v<typename _std remove_pointer_t<T>::value_type, BigUnsigned>>>
void populate_factorial(T cont_ptr) {

    for (auto curr = cont_ptr->begin(); curr != cont_ptr->end(); ++curr) {
        *curr = factorial(dist_to_sizety(cont_ptr->begin(), curr));
    }

    factorials_populated = true;

}

/**
 * @brief Get coordinate first relative to coordinate second.
 * 
 */
inline coord_ty relative(coord_ty second, coord_ty first) {

    return coord_ty(second.first - first.first, second.second - first.second);

}

end_probability