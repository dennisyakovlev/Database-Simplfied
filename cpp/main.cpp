// Author: Dennis Yakovlev

#include <iostream>
#include <BigInt.h>
#include <prob_probability.h>
#include <prob_createInfo.h>
#include <prob_file.h>
#include <algorithm>
#include <prob_vars.h>

#include <prob_file.h>

using namespace std;
using namespace pathprob;

void create_files() {

    auto hashed = hash_all(); // hash all values

    copy(hashed.cbegin(), hashed.cend(), ostream_iterator<pathprob::size_t>(cout, ", ")); // print hashed values

    cout << "\n\nHashed Size: " << hashed.size() << "\n\n";  // print number of elements in hashed container

    auto result_vec = write_info("info.bin", hashed); // write out information to file which stores info

    copy(result_vec.cbegin(), result_vec.cend(), ostream_iterator<IndexInfo>(cout, "\n")); // print out written information

    cout << endl << endl; // spacing

    write_map("map.bin", result_vec); // write out information to map file

}

void read_coord(pathprob::size_t x, pathprob::size_t y) {

    auto res_pair = read_map("map.bin", "info.bin", coord_ty(x,y), DOUBLE{});
    cout << setprecision(precision10_digits) << res_pair.first << endl;
    copy(++res_pair.second.cbegin(), res_pair.second.cend(), ostream_iterator<int_least64_t>(cout, ", ")); // print edges prob
    cout << endl;

}

int main() {

    create_files();

    // read_coord(30,20);

    // read_map("map.bin", "info.bin", coord_ty(0,0), DOUBLE{});

}

