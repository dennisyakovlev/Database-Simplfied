// Author: Dennis Yakovlev

// variables

module.exports.QUERY_KEY_COORD = 'coord'; // query string key for coordinate
module.exports.QUERY_KEY_COORD_END = 'coord_end'; // query string key for ending coordinate relative to another

module.exports.INVALID_COORD_CODE = '1'; // error code for invalid coordinate
module.exports.INVALID_RELATIVE_COORD_CODE = '2'; // error code for invalid relative coordinate

module.exports.GRID_DIGITS_MAX = 2; // maximum number of grid digits
                                    // Note: same as <max_grid_digits> in prob_vars.h

module.exports.INVALID_INPUT = null; // return from function when input is invalid

// validate functions

module.exports.valid_coord = function _valid_coord(str) {
    // <str> string to see if valid coords
    //       valid in format num,num
    // <return> true if valid false otherwise

    if (typeof(str) != 'string') {

        return false;

    }

    const regex = `^[0-9]{1,${module.exports.GRID_DIGITS_MAX}},[0-9]{1,${module.exports.GRID_DIGITS_MAX}}$`;
    return str.match(regex) != null;

}

module.exports.valid_coords = function _valid_coords(str, num) {
    // <str> string to see if valid coords and the number of coords
    //       matches num
    // <number of valid coords>
    // <return> true if valid false otherwise

    const arr = str.match(/[^,]+(\,[^,]+)?/g);

    
    if (arr == null) {

        return false;

    }

    if (arr.length != num) {
        
        return false;

    }

    return arr.reduce((accum, curr) => {
        accum = accum && module.exports.valid_coord(curr);
        return accum
    }, true);

}

module.exports.valid_coord_relative = function _valid_coord_relative(coord_1_str, coord_2_str) {
    // <coord_1> string to see if it is a valid coordinate relative to <coord_2>.
    //           Means <coords> must be closer to 0,0 than <coord_2>
    // <coord_2> Coordinate further away from 0,0 than <coord_1>
    // <return> true if valid false otherwise

    if (!module.exports.valid_coord(coord_1_str) || !module.exports.valid_coord(coord_2_str)) {

        return false;

    }

    const coord_1_obj = module.exports.get_coord_unchecked(coord_1_str);
    const coord_2_obj = module.exports.get_coord_unchecked(coord_2_str);

    return (coord_1_obj.x <= coord_2_obj.x) && (coord_1_obj.y <= coord_2_obj.y);

}

module.exports.get_coord_unchecked = function _get_coord_unchecked(str) {
    // <str> string to get coord out of
    // Note: does not check to see that str is valid coord

    return {
        x: parseInt(str.split(',')[0]),
        y: parseInt(str.split(',')[1])
    };

}

module.exports.get_coord = function _get_coord(str) {
    // <str> string to get coord out of
    // Note: checks to see that str is valid coord
    // <return> null if not valid coord
    
    return module.exports.valid_coord(str) ? module.exports.get_coord_unchecked(str) : module.exports.INVALID_INPUT;

}

module.exports.get_coords_unchecked = function _get_coords_unchecked(str) {
    // <str> string to get coords out of
    // Note: does not check to see that str is set of valid coords
    // <return> return object with keys as order of the coords in <str>
    //          and values as the associated coords


    const arr = str.match(/[^,]+(\,[^,]+)?/g);

    let obj = {};

    for (let i = 0; i != arr.length; ++i) {

        obj[i] = module.exports.get_coord_unchecked(arr[i]);

    }

    return obj;

}

module.exports.get_coords = function _get_coords(str, num) {
    // <str> string to get coords out of
    // Note: checks to see that str is set of valid coords
    // <return> return object with keys as order of the coords in <str>
    //          and values as the associated coords
    //          return null if not valid

    return module.exports.valid_coords(str, num) ? module.exports.get_coord_unchecked(str) : module.exports.INVALID_INPUT;

}

module.exports.get_coord_relative_unchecked = function _get_coord_relative_unchecked(coord_1_str, coord_2_str) {

    return {
        1: module.exports.get_coord(coord_1_str),
        2: module.exports.get_coord(coord_2_str),
    };

}

module.exports.get_coord_relative = function _get_coord_relative(coord_1_str, coord_2_str) {

    return module.exports.valid_coord_relative(coord_1_str, coord_2_str) ? module.exports.get_coord_relative_unchecked(coord_1_str, coord_2_str) : module.exports.INVALID_INPUT;

}

// errors

module.exports.INVALID_COORD = function _INVALID_COORD(coord_str) {
    
    const error = new Error("Invalid coordinate string of \"" + coord_str + "\"");
  
    error.code = module.exports.INVALID_COORD_CODE;

    return error;

}
module.exports.INVALID_COORD.prototype = Object.create(Error.prototype);

module.exports.INVALID_RELATIVE_COORDS = function _INVALID_RELATIVE_COORDS(coord_1_str, coord_2_str) {
    
    const error = new Error("Invalid relative coordinate string of \"" + coord_1_str + "\" to \"" + coord_2_str + "\"");
  
    error.code = module.exports.INVALID_RELATIVE_COORD_CODE;

    return error;

}
module.exports.INVALID_RELATIVE_COORDS.prototype = Object.create(Error.prototype);