// Author: Dennis Yakovlev

const express = require('express');
const router = express.Router();
const path = require('path');

const main_paths = require('/root/production/main_paths.js');
const route_paths = require(path.join(main_paths.start_path, main_paths.route_paths));
const thisProjectPath = path.join(main_paths.start_path, route_paths.projects_path, 'server_grid'); // path to this project folder
const project_paths = require(path.join(thisProjectPath, 'paths')); 
const cpp = require(path.join(thisProjectPath, project_paths.releaseEntry_path));
const utils = require(path.join(thisProjectPath, project_paths.routers_dir, 'routing_utils'));

function toCpp(first, second) {
    return {
        x1: first.x,
        y1: first.y,
        x2: second.x,
        y2: second.y
    };
}

router.get('/', (req, res) => {

    const coord_first_str = req.query[utils.QUERY_KEY_COORD];
    const coord_second_str = req.query[utils.QUERY_KEY_COORD_END];
    
    if (!utils.valid_coord_relative(coord_first_str, coord_second_str)) {
        res.status(400);
        res.send('invalid coordinates');
    } else {
        const coord_first = utils.get_coord(coord_first_str);
        const coord_end = utils.get_coord(coord_second_str);
        res.send(cpp.calculate_chance(toCpp(coord_first, coord_end)));
    }

});

module.exports = router;