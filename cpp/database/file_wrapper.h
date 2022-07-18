// Author: Dennis Yakovlev

// File containing members relating to IO wrapper of binary information.
// Requires std17 =<. 
// Can be used from multiple threads without lock as long as types used in IO
//     are <= bit size of the bus width.

#pragma once
#include <algorithm>
#include <fstream>
#include <ios>
#include <iterator>
#include <ostream>
#include <stddef.h>
#include <tuple>
#include <type_traits>
#include <utility>

#include <thread>

/**
 * @brief Writable is something that can be written to binary file. 
 * 
 */
struct Writable {

    Writable(std::streamsize sz, std::streamsize sz_tot) : size(sz), size_total(sz_tot) {}

    /**
     * @brief Size of element. Represents
     *        <p> size of type for a native type.
     *            <br> OR size of element type for container.
     *        </p>
     * 
     */
    std::streamsize size;

    /**
     * @brief Total size of stored variable. Represents
     *        <p> same as size for native type
     *            <br> OR number of elements in container times \p size
     *        </p>
     * 
     */
    std::streamsize size_total;

};

/**
 * @brief Native types to be written to file.
 * 
 * @tparam T Must meet requirements of std::is_arithmetic_v.
 * 
 * <p> Note: The variables are not stored as const since when reading in they need to be
 *     written into. However, when writing, the original values will not be
 *     modified.
 * </p>
 */
template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
struct NativeWritable : public Writable {

    using ty = T; // type stored

    NativeWritable(T* native) : var(native), Writable::Writable(sizeof(T), sizeof(T)) {}

    /**
     * @brief Pointer to original value.
     * 
     */
    T* var;

};

/**
 * @brief Container of native types to be written to file.
 * 
 * @tparam Cont Cont::value_type must meet same requirements as \p NativeWritable
 * 
 * <p> Assume Cont is container type validated with \p _is_container_v </p>
 * <p> Note: The variables are not stored as const since when reading in they need to be
 *     written into. However, when writing, the original values will not be
 *     modified.
 * </p>
 * 
 */
template<typename Cont, typename = std::enable_if_t<std::is_arithmetic_v<typename Cont::value_type>>>
struct ContainerWritable : public Writable {

    using ty = Cont; // type stored

    ContainerWritable(Cont* cont) : Writable::Writable(static_cast<std::streamsize>(sizeof(typename Cont::value_type)), 
                                                       static_cast<std::streamsize>(sizeof(typename Cont::value_type) * cont->size())), 
                                                       var(cont) {}

    /**
     * @brief Pointer to original container.
     * 
     */
    Cont* var; 

};

// these should check for the member (does) AND that it returns the correct type (doesnt)

/**
 * @brief Whether type has <b> size </b> member function.
 * 
 */
template<typename T, typename = void>
struct _has_size_member : std::false_type {};

/**
 * @brief Whether type has <b> size </b> member function.
 * 
 */
template<typename T>
struct _has_size_member<T, std::void_t<std::is_member_function_pointer<decltype(&T::size)>>> : std::true_type {};

/**
 * @brief Whether type has <b> size </b> member function value.
 * 
 */
template<typename T> constexpr bool _has_size_member_v = _has_size_member<T>::value;

/**
 * @brief Whether type has <b> cbegin </b> member function.
 * 
 */
template<typename T, typename = void>
struct _has_cbegin_member : std::false_type {};

/**
 * @brief Whether type has <b> cbegin </b> member function.
 * 
 */
template<typename T>
struct _has_cbegin_member<T, std::void_t<std::is_member_function_pointer<decltype(&T::cbegin)>>> : std::true_type {}; 

/**
 * @brief Whether type has <b> cbegin </b> member function value.
 * 
 */
template<typename T> constexpr bool _has_cbegin_member_v = _has_cbegin_member<T>::value;

/**
 * @brief Whether type has <b> cend </b> member function.
 */
template<typename T, typename = void>
struct _has_cend_member : std::false_type {};

/**
 * @brief Whether type has <b> cend </b> member function.
 */
template<typename T>
struct _has_cend_member<T, std::void_t<std::is_member_function_pointer<decltype(&T::cend)>>> : std::true_type {};

/**
 * @brief Whether type has <b> cend </b> member function value.
 */
template<typename T> constexpr bool _has_cend_member_v = _has_cend_member<T>::value;                

/**
 * @brief Whether type is valid container value.
 * 
 * <p> Note: doesnt actually check ALL requirments </p>
 * 
 */
template<typename T> constexpr bool _is_container_v = _has_size_member_v<T> && 
                                                      _has_cbegin_member_v<T> && 
                                                      _has_cend_member_v<T>;

/**
 * @brief Whether type is \p NativeWritable.
 * 
 */
template<typename T, typename = void>
struct _is_nativewritable : std::false_type {};

/**
 * @brief Whether type is \p NativeWritable.
 * 
 */
template<typename T>
struct _is_nativewritable<T, std::void_t<std::enable_if_t<std::is_same_v<T, NativeWritable<typename T::ty>>>>> : std::true_type {};

/**
 * @brief Whether type is \p NativeWritable value.
 * 
 */
template<typename T> constexpr bool _is_nw_v = _is_nativewritable<T>::value;

/**
 * @brief Whether type is \p ContainerWritable.
 * 
 */
template<typename T, typename = void>
struct _is_containerwritable : std::false_type {};

/**
 * @brief Whether type is \p ContainerWritable.
 * 
 */
template<typename T>
struct _is_containerwritable<T, std::void_t<std::enable_if_t<std::is_same_v<T, ContainerWritable<typename T::ty>>>>> : std::true_type {};

/**
 * @brief Whether type is \p ContainerWritable value.
 * 
 */
template<typename T> constexpr bool _is_cw_v = _is_containerwritable<T>::value;

/**
 * @brief Whether all of U and Args are a base of T.
 * 
 * @tparam T 
 * @tparam U 
 * @tparam Args 
 */
template<typename T, typename U, typename... Args>
struct _all_base {
    template<typename = std::enable_if_t<std::is_base_of_v<T, U>>>
    static void check() {
        _all_base<T, Args...>::check();
    }
};

/**
 * @brief base case
 * 
 * @tparam T 
 * @tparam U 
 */
template<typename T, typename U>
struct _all_base<T, U> {
    template<typename = std::enable_if_t<std::is_base_of_v<T, U>>>
    static void check() {}
};

/**
 * @brief Get correct writable type. 
 * 
 */
template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<std::remove_pointer_t<T>>>>
static NativeWritable<std::remove_pointer_t<T>> _get_writable(T var) {

    return NativeWritable(var);
    
}

/**
 * @brief Get correct writable type. 
 * 
 */
template<typename T, typename = std::enable_if_t<_is_container_v<std::remove_pointer_t<T>>>>
static ContainerWritable<std::remove_pointer_t<T>> _get_writable(T var) {

    return ContainerWritable(var);

}

/**
 * @brief Base case.
 * 
 */
template<typename T>
auto get_args_tuple(T arg) {

    return std::tuple(_get_writable(arg));

}

/**
 * @brief Get tuple of writables corresponding to args.
 * 
 */
template<typename T, typename... Args>
auto get_args_tuple(T arg, Args... args) {

    return std::tuple_cat(std::tuple(_get_writable(arg)), get_args_tuple(args...));

}

/**
 * @brief A "block" is some combination of information to be written/read to/from a file.
 *        <Br> A valid block consists of only fundamental types.
          <br> write) writes to file
 *        <br> read) reads from file
 * 
 * @tparam T tuple of information for be written/read to/from file which comprises the block
 * @tparam N number of tuple arguements
 * 
 */
template<typename T, std::size_t N, typename = void>
struct _block {};

template<typename T, std::size_t N>
struct _block<T, N, std::enable_if_t<_is_nw_v<std::tuple_element_t<N - 1, T>>>> {

    static void write(std::ofstream* outf, T* tupe) {

        _block<T, N - 1>::write(outf, tupe);

        auto arg = std::get<N - 1>(*tupe);
        outf->write(reinterpret_cast<char*>(&*arg.var), arg.size);

    }

    static void read(std::ifstream* inf, T* tupe) {

        _block<T, N - 1>::read(inf, tupe);

        auto arg = std::get<N - 1>(*tupe);
        inf->read(reinterpret_cast<char*>(&*arg.var), arg.size);

    }

};

template<typename T, std::size_t N>
struct _block<T, N, std::enable_if_t<_is_cw_v<std::tuple_element_t<N - 1, T>>>> {

    static void write(std::ofstream* outf, T* tupe) {

        _block<T, N - 1>::write(outf, tupe);

        const auto arg = std::get<N - 1>(*tupe);
        for (auto iter = arg.var->cbegin(); iter != arg.var->cend(); ++iter) {
            outf->write(reinterpret_cast<const char*>(&*iter), arg.size);
        }

    }

    static void read(std::ifstream* inf, T* tupe) {

        _block<T, N - 1>::read(inf, tupe);

        auto arg = std::get<N - 1>(*tupe);
        for (auto iter = arg.var->begin(); iter != arg.var->end(); ++iter) {
            inf->read(reinterpret_cast<char*>(&*iter), arg.size);
        }

    }

};

/**
 * @brief base case
 * 
 */
template<typename T>
struct _block<T, 1, std::enable_if_t<_is_nw_v<std::tuple_element_t<0, T>>>> {

    static void write(std::ofstream* outf, T* tupe) {

        auto arg = std::get<0>(*tupe);
        outf->write(reinterpret_cast<char*>(&*arg.var), arg.size);

    }

    static void read(std::ifstream* inf, T* tupe) {

        auto arg = std::get<0>(*tupe);
        inf->read(reinterpret_cast<char*>(&*arg.var), arg.size);

    }

};

/**
 * @brief base case
 * 
 */
template<typename T>
struct _block<T, 1, std::enable_if_t<_is_cw_v<std::tuple_element_t<0, T>>>> {

    static void write(std::ofstream* outf, T* tupe) {

        const auto arg = std::get<0>(*tupe);
        for (auto iter = arg.var->cbegin(); iter != arg.var->cend(); ++iter) {
            outf->write(reinterpret_cast<const char*>(&*iter), arg.size);
        }

    }

    static void read(std::ifstream* inf, T* tupe) {

        auto arg = std::get<0>(*tupe);
        for (auto iter = arg.var->begin(); iter != arg.var->end(); ++iter) {
            inf->read(reinterpret_cast<char*>(&*iter), arg.size);
        }

    }

};

/**
 * @brief Function will only works if args is made purely of types which meet \p NativeWriteable requirements
 * and containers meeting \p _is_container_v requirements whose elements meet \p NativeWriteable requirements.
 * 
 * @param outf output file
 * @param args pointers to information to write from
 */
template<typename... Args>
auto write_block(std::ofstream* outf, Args... args) {

    auto info_tuple = get_args_tuple(args...);

    using tuple_ty = decltype(info_tuple);

    _block<tuple_ty, std::tuple_size_v<tuple_ty>>::write(outf, &info_tuple);

    return info_tuple;

}

/**
 * @brief Function will only works if args is made purely of types which meet \p NativeWriteable requirements
 * and containers meeting \p _is_container_v requirements whose elements meet \p NativeWriteable requirements.
 * 
 * @param inf input file
 * @param args pointers to information to read into
 */
template<typename... Args>
auto read_block(std::ifstream* inf, Args... args) {

    auto info_tuple = get_args_tuple(args...);

    using tuple_ty = decltype(info_tuple);

    _block<tuple_ty, std::tuple_size_v<tuple_ty>>::read(inf, &info_tuple);

    return info_tuple;

}

/**
 * @brief \p block_size_total helper struct.
 * 
 */
template<typename T, std::size_t N>
struct _block_size {

    static std::streamsize size(T* tupe) {

        return std::get<N - 1>(*tupe).size_total + _block_size<T, N - 1>::size(tupe);

    }

};

/**
 * @brief Base case.
 * 
 */
template<typename T>
struct _block_size<T, 1> {

    static std::streamsize size(T* tupe) {

        return std::get<0>(*tupe).size_total;

    }

};

/**
 * @brief Get total byte size of block.
 * 
 * @tparam T tuple pointer which contains only subtypes of \p Writable 
 */
template<typename T, typename = _all_base<Writable, T>>
std::streamsize block_size_total(T* tupe) {

    return _block_size<T, std::tuple_size_v<T>>::size(tupe);

}