// Author: Dennis Yakovlev

// File containing members related to standard output.
// For debugging purposes.

#pragma once
#include <file_wrapper.h>
#include <ostream>
#include <stddef.h>
#include <tuple>

/**
 * @brief \p NativeWritable output operator.
 * 
 */
template<typename T>
std::ostream& operator<< (std::ostream& out, const NativeWritable<T>& writable) {

    out << "Byte Size: " << writable.size_total << " | Value: " << *writable.var << " | Type: " << typeid(T).name();
    return out;

}

/**
 * @brief \p ContainerWritable output operator.
 * 
 */
template<typename T>
std::ostream& operator<< (std::ostream& out, const ContainerWritable<T>& writable) {

    out << "Byte Size: " << writable.size_total << " | Value:  ";
    std::copy(writable.var->cbegin(), writable.var->cend(), std::ostream_iterator<typename T::value_type>(out, ", "));
    out << "| Type: " << typeid(T).name();
    return out;

}

/**
 * @brief \p print_tuple helper structs.
 * 
 * @tparam Tuple tuple type
 * @tparam N number of tuple elements
 */
template<typename Tuple, std::size_t N>
struct _print_tuple {

    static std::ostream& print(std::ostream& out, Tuple* tupe) {

        _print_tuple<Tuple, N - 1>::print(out, tupe);
        out << std::get<N - 1>(*tupe) << "\n";
        return out;

    }

};

/**
 * @brief Base case.
 * 
 */
template<typename Tuple>
struct _print_tuple<Tuple, 1> {

    static std::ostream& print(std::ostream& out, Tuple* tupe) {

        out << std::get<0>(*tupe) << "\n";
        return out;

    }

};

/**
 * @brief Print contents of tuple.
 * 
 * <p> Elements of tupe must have << operators defined. </p>
 */
template<typename Tuple>
std::ostream& print_tuple(std::ostream& out, Tuple* tupe) {

    return _print_tuple<Tuple, std::tuple_size_v<Tuple>>::print(out, tupe);

}