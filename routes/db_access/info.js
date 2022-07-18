// Author: Dennis Yakovlev

// file for getting database info of a point
// the edges and number of paths which go through it

const express = require('express');
const router = express.Router();
const path = require('path');

const main_paths = require('/root/production/main_paths.js');
const route_paths = require(path.join(main_paths.start_path, main_paths.route_paths));
const thisProjectPath = path.join(main_paths.start_path, route_paths.projects_path, 'server_grid'); // path to this project folder
const project_paths = require(path.join(thisProjectPath, 'paths')); 
const cpp = require(path.join(thisProjectPath, project_paths.releaseEntry_path));
const utils = require(path.join(thisProjectPath, project_paths.routers_dir, 'routing_utils'));

router.get('/', (req, res) => {

    const coord_str = req.query[utils.QUERY_KEY_COORD];
    const coord_obj = utils.get_coord(coord_str);
    if (coord_obj == utils.INVALID_INPUT) {
        res.status(400);
        res.send('invalid coordinates');
    } else {
        res.send(cpp.request_info(coord_obj));
    }

});

module.exports = router;