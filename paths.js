// Author: Dennis Yakovlev

// file for paths inside of server_grid folder

var path = require('path');

const routers_dir = 'routes'; // contains all routes
const database_dir = 'db_access'; // contains files for database access
const calc_dir = 'no_db_access'; // contains files which do not require database but need c++

module.exports.routers_dir = `${path.sep}${routers_dir}`;
module.exports.database_dir = `${path.sep}${database_dir}`;
module.exports.calc_dir = `${path.sep}${calc_dir}`;

module.exports.calc_path = path.join(routers_dir, calc_dir); // path to calculation dir
module.exports.database_path = path.join(routers_dir, database_dir); // path to database dir
module.exports.releaseEntry_path = path.join('cpp', 'build', 'Release', 'entry'); // path to access cpp functions