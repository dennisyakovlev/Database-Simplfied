// Author: Dennis Yakovlev

// File containing members relating to IO of databse.

#pragma once
#include <BigInt.h>
#include <cassert>
#include <cstdint>
#include <file_wrapper.h>
#include <iostream>
#include <prob_createInfo.h>
#include <prob_probability.h>
#include <prob_vars.h>
#include <string>
#include <tuple>
#include <utility>

start_probability

/**
 * @brief Assertion struct to ensure functions work properly.
 * 
 * <p> For debugging purposes. </p>
 * 
 */
struct _check_sizes {

    _check_sizes() {

        assert(sizeof(_std streamsize) <= sizeof(int_least64_t) && "for function to work std streamsize type size must be less than or equal to int_least64_t size");                                                                       
        assert(sizeof(BigUnsigned::cont_ull::size_type) <= sizeof(int_least64_t) && "for function to work BigUnsigned container type size must be less than or equal to int_least64_t size");
        assert(sizeof(_prob container_ty<double>::size_type) <= sizeof(int_least64_t) && "for function to work container size_type type size must be less than or equal to int_least64_t size");
                                                                                    
    }
};

/**
 * @brief Struct to wrap info related to position of information in files.
 * 
 */
struct IndexInfo {
    IndexInfo() : hashed_coord(0), start(0), size_paths(0), size_edges(0) {}
    IndexInfo(int_least64_t a, int_least64_t b, int_least64_t c, int_least64_t d) : hashed_coord(a), start(b), size_paths(c), size_edges(d) {}
    size_t hashed_coord; // hashed coordinate value
    int_least64_t start; // starting byte (inclusive) of information in file
    int_least64_t size_paths; // size in elements of number of paths stored
    int_least64_t size_edges; // size in bytes of number of edge probalilities stored
};

_std ostream& operator<< (_std ostream& out, const IndexInfo& info) {
    out << "Unhashed Coord: " << unhash(info.hashed_coord).first << "," << unhash(info.hashed_coord).second << " | " <<
           "Hashed Coord: " << info.hashed_coord << " | " << 
           "Start: " << info.start << " | " <<
           "Digit Paths: " << info.size_paths << " | " <<
           "Digit edges: " << info.size_edges;
    return out;
}

/**
 * @brief Rebind a primary template with one template paramter to a different parameter.
 * 
 * @tparam T type to substitute into primary
 * @tparam U primary template with old types to be replaced
 */
template<typename T, typename U>
struct Rebind;

template<typename ty_new, template<typename> typename primary, typename... ty_old>
struct Rebind<ty_new, primary<ty_old...>> {
    using ty = primary<ty_new>;
};

using _bignum_val = _std integral_constant<_std size_t, sizeof(BigUnsigned::cont_ull::value_type)>;
using _in64_sz = _std integral_constant<_std size_t, sizeof(int_least64_t)>;

/**
 * @brief Write to file digits of a \p BigUnsigned
 * 
 * <p> Have two cases of sizeof(BigUnsigned.digits::value_type) == sizeof(int_least64_t) then can just write normally (true).
 *     Otherwise must cast digits to int_least64_t (false).
 * </p>
 * 
 * <p> write) write to file
 *     <br> read) read from file
 * </p>
 * 
 */
template<typename = void>
struct _to_int64 {};

template<>
struct _to_int64<_std integral_constant<bool, true>> {

    static auto write(_std ofstream* outf, BigUnsigned* num) {

        return write_block(outf, &(num->digits));

    }

    static void read(_std ifstream* inf, BigUnsigned* num) {

        read_block(inf, &(num->digits));

    }

};

template<>
struct _to_int64<_std integral_constant<bool, false>> {

    using cont_ty = typename Rebind<int_least64_t, typename BigUnsigned::cont_ull>::ty;
    
    static auto write(_std ofstream* outf, BigUnsigned* num) {

        cont_ty new_digits(num->digits.size());
        auto iter_new_digits = new_digits.begin();
        for (auto iter_old_digits = num->digits.cbegin(); iter_old_digits != num->digits.cend(); ++iter_old_digits, ++iter_new_digits) {
            *iter_new_digits = static_cast<int_least64_t>(*iter_old_digits);
        }

        return write_block(outf, &new_digits);

    }

    static void read(_std ifstream* inf, BigUnsigned* num) {

        cont_ty new_digits(num->digits.size());

        auto res = read_block(inf, &new_digits);

        auto iter_num = num->digits.begin();
        for (auto iter_digit = new_digits.cbegin(); iter_digit != new_digits.cend(); ++iter_digit, ++iter_num) {
            *iter_num = static_cast<BigUnsigned::cont_ull::value_type>(*iter_digit);
        }

    }

};

/**
 * @brief Initiate writing to file process.
 */
auto _write_int64(_std ofstream* outf, BigUnsigned* num) {

    return _to_int64<_std integral_constant<bool, _std is_same_v<_bignum_val, _in64_sz>>>::write(outf, num);

}

/**
 * @brief Write information to info file corresponding to numbers in \p hashed
 * 
 * <p> Always overrides old files. Files created are not cross platform. Must be
 *     created for every machine/ compiler each time.
 * </p>
 * 
 * @param name name of info file to write to
 * @param hashed container of hashed values in sorted order according to \p write_map
 * @return _prob container_ty<IndexInfo> Info's corresponding to \p hashed in same order  
 * 
 */
template<typename T>
_prob container_ty<IndexInfo> write_info(T name, const size_vec& hashed) {

    _std ofstream outf{name, _std ios::binary};

    if (!outf) {
        _std cerr << "cannot open writing file" << _std endl;
    }

    _std streamsize bytes_written = 0; // total number of bytes written
    _prob container_ty<IndexInfo> index_vec(hashed.size());  // container containing info for locating numbers
    auto iter_index_vec = index_vec.begin();
    for (auto iter_hashed = hashed.cbegin(); iter_hashed != hashed.cend(); ++iter_hashed, ++iter_index_vec) {

            const auto unhashed = unhash(*iter_hashed); // unhashed coords

            _std cout << unhashed.first << "," << unhashed.second << " | ";

            BigUnsigned num_paths = path_num_end(unhashed); // number of paths
            auto res_tuple_path = _write_int64(&outf, &num_paths); // number of paths
            auto res_edge = edge_prob(unhashed, INT_LEAST64{}); // highlight lines
                                                                // Note: this writes out the starting probability

            auto res_tuple_edge = write_block(&outf, &res_edge);

            *iter_index_vec = IndexInfo(*iter_hashed, static_cast<int_least64_t>(bytes_written), 
                                                      static_cast<int_least64_t>(num_paths.digits.size()), 
                                                      static_cast<int_least64_t>(res_edge.size()));

            auto total_tuple = _std tuple_cat(res_tuple_path, res_tuple_edge);
            bytes_written += block_size_total(&total_tuple);

    }

    outf.close();
    if (outf.fail()) {
        _std cerr << "cannot close writing file" << _std endl;
    }

    return index_vec;

}

/**
 * @brief Write information to map file corresponding to positions given from \p write_info
 * 
 * <p> Always overrides old files. Files created are not cross platform. Must be
 *     created for every machine/ compiler each time.
 * </p>
 * 
 * @param name name of map file to write to
 * @param index_vec must be sorted least to greatest (ascending from beginning) in terms of IndexInfo.hashed_coord and
 *                  be returned information from \p write_info
 * 
 */
template<typename T>
void write_map(T name, const _prob container_ty<IndexInfo>& index_vec) {

    _std ofstream outf(name, _std ios::binary);

    if (!outf) {
        _std cerr << "cannot open reading file" << _std endl;
    }

    int_least64_t start_0(0);
    int_least64_t size_paths_0(0);
    int_least64_t size_edges_0(0);

    size_t max_hash = index_vec.crbegin()->hashed_coord;
    auto index_vec_iter = index_vec.cbegin();
    for (size_t i = 0; i != max_hash; ++i) {
        if (i == index_vec_iter->hashed_coord) {
            // write out info
            auto info = *index_vec_iter;
            write_block(&outf, &info.start, &info.size_paths, &info.size_edges);
            ++index_vec_iter;
        } else {
            write_block(&outf, &start_0, &size_paths_0, &size_edges_0);
        }
    }

    outf.close();
    if (outf.fail()) {
        _std cerr << "cannot close reading file" << _std endl;
    }

}

/**
 * @brief Initiate reading from file process.
 */
auto _read_int64(_std ifstream* inf, BigUnsigned* num) {

    return _to_int64<_std integral_constant<bool, _std is_same_v<_bignum_val, _in64_sz>>>::read(inf, num);

}

/**
 * @brief Take in coordinate and return information related associated with the coordinate
 *  
 * @param name_map map file name
 * @param name_info info file name
 * @param coord coordinate to get information for
 * @param _ tag to reference wanted function
 * @return _std pair<BigUnsigned, _prob container_ty<int_least64_t>> return pair of 
 *         <p> first) Number of paths to point
 *             <br> second) Chance that path uses edge. Returned in precision defined by \p precision10_value
 *         </p>
 * 
 */
template<typename T, typename U>
_std pair<BigUnsigned, _prob container_ty<int_least64_t>> read_map(T name_map, U name_info, coord_ty coord, INT_LEAST64 _) {

    auto hashed_coord = _prob hash(coord); // hashed coord

    _std ifstream inf_map(name_map, _std ios::binary); // map file

    if (!inf_map) {
        _std cerr << "cannot open map file" << _std endl;
    }

    _std streamsize block_size = sizeof(int_least64_t) * 3; // block size of file

    inf_map.seekg(block_size * hashed_coord); // seek to information in map file
                                              // seekg is relative to start
    IndexInfo info; // initiate info

    read_block(&inf_map, &info.start, &info.size_paths, &info.size_edges); // read in info

    inf_map.close(); 

    if (inf_map.fail()) {
        _std cerr << "cannot close map file" << _std endl;
    }
    
    _std ifstream inf_info(name_info, _std ios::binary); // info file

    if (!inf_info) {
        _std cerr << "cannot open info file" << _std endl;
    }

    inf_info.seekg(info.start); // seek to position of data

    BigUnsigned num_paths(info.size_paths); // number of paths
    _read_int64(&inf_info, &num_paths);

    _prob container_ty<int_least64_t> edges_prob_int64(info.size_edges); // edge probabilities
    read_block(&inf_info, &edges_prob_int64); // read in info

    inf_info.close();

    if (inf_info.fail()) {
        _std cerr << "cannot close info file" << _std endl;
    }

    return _std pair(num_paths, edges_prob_int64);
    
}

/**
 * @brief Take in coordinate and return information related associated with the coordinate
 *  
 * @param name_map map file name
 * @param name_info info file name
 * @param coord coordinate to get information for
 * @param _ tag to reference wanted function
 * @return _std pair<BigUnsigned, _prob container_ty<int_least64_t>> return pair of 
 *         <p> first) Number of paths to point
 *             <br> second) Chance that path uses edge. Returned in range [0,1].
 *         </p>
 * 
 */
template<typename T, typename U>
_std pair<BigUnsigned, _prob container_ty<double>> read_map(T name_map, U name_info, coord_ty coord, DOUBLE _) {

    auto hashed_coord = _prob hash(coord); // hashed coord

    _std ifstream inf_map(name_map, _std ios::binary); // map file

    if (!inf_map) {
        _std cerr << "cannot open map file" << _std endl;
    }

    _std streamsize block_size = sizeof(int_least64_t) * 3; // block size of file

    inf_map.seekg(block_size * hashed_coord); // seek to information in map file
                                              // seekg is relative to start
    IndexInfo info; // initiate info

    read_block(&inf_map, &info.start, &info.size_paths, &info.size_edges); // read in info

    inf_map.close(); 

    if (inf_map.fail()) {
        _std cerr << "cannot close map file" << _std endl;
    }
    
    _std ifstream inf_info(name_info, _std ios::binary); // info file

    if (!inf_info) {
        _std cerr << "cannot open info file" << _std endl;
    }

    inf_info.seekg(info.start); // seek to position of data

    BigUnsigned num_paths(info.size_paths); // number of paths
    _read_int64(&inf_info, &num_paths);

    _prob container_ty<int_least64_t> edges_prob_int64(info.size_edges); // edge probabilities
    read_block(&inf_info, &edges_prob_int64); // read in info

    inf_info.close();

    if (inf_info.fail()) {
        _std cerr << "cannot close info file" << _std endl;
    }

    return _std pair(num_paths, _int64_to_double(edges_prob_int64.cbegin(), edges_prob_int64.cend()));
    
}

end_probability