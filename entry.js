// Author: Dennis Yakovlev

// File for grid project

const express = require('express');
const app = express();
const router = express.Router({
    'strict': true
});
const path = require('path');

const main_paths = require('/root/production/main_paths.js');
const client_paths = require(path.join(main_paths.start_path, main_paths.client_paths));
const custom_paths = require(path.join(main_paths.start_path, main_paths.paths));
const route_paths = require(path.join(main_paths.start_path, main_paths.route_paths));
const project_paths = require(path.join(main_paths.start_path, route_paths.projects_path, 'server_grid', 'paths')); 

const projectName = 'server_grid';

// get all serve files
const serveFiles = custom_paths.get_all_directories(
    path.join(main_paths.start_path, client_paths.projects_path, projectName),
    {
        [custom_paths.keepDirs]: false,
        [custom_paths.keepFiles]: ['.css', '.js']
    }
);

// set all serve file names
for (const serveFile of serveFiles) {

    router.get(`/${serveFile[custom_paths.name]}`, (req, res) => {
        res.sendFile(serveFile[custom_paths.full_path]);
    });

}

// serve index.html
router.get('/', (req, res) => {
    res.sendFile(path.join(main_paths.start_path, client_paths.projects_path, projectName, 'index.html'));
});

const infoRouter = require(
    path.join(main_paths.start_path, route_paths.projects_path, projectName, project_paths.database_path, 'info')
); // router to get information file
router.use('/info', infoRouter);

const chanceRouter = require(
    path.join(main_paths.start_path, route_paths.projects_path, projectName, project_paths.calc_path, 'chance')
); // router to get chance file
router.use('/chance', chanceRouter);

module.exports = router;