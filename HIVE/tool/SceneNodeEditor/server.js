/*jslint devel: true, browser: true*/
/*global require, process*/
//
//  SceneNodeEditor
//
var http = require('http'),
    fs = require('fs'),
    port = 8080;

//-----------------------------------------------------

function makeNodeList(callback) {
	"use strict";
	var nodeDir = './nodes';
	fs.readdir(nodeDir, function (err, files) {
		var infofile,
			nodeDirPath,
			fileCounter,
			customFuncLua,
			nodelist = [],
			i;
		if (err) {
			return;
		}

		fileCounter = 0;
		function finishLoad() {
			fileCounter = fileCounter - 1;
			if (fileCounter === 0) {
				callback(null, nodelist);
			}
		}
		function loadFunc(nodeDirPath) {
			return function (err, data) {
				try {
					var json = JSON.parse(data);
					if (json.customfuncfile !== undefined) {
						customFuncLua = fs.readFileSync(nodeDirPath + "/" + json.customfuncfile, 'utf8');
						json.customfunc = customFuncLua;
					}
					nodelist.push(json);
				} catch (e) {
					console.log('[Error] Failed Load:' + nodeDirPath + "/info.json");
				}
				finishLoad();
			};
		}
		for (i in files) {
			if (files.hasOwnProperty(i)) {
				nodeDirPath = nodeDir + "/" + files[i];
				infofile = nodeDirPath + "/info.json";
				console.log(infofile);
				fileCounter = fileCounter + 1;
				fs.readFile(infofile, 'utf8', loadFunc(nodeDirPath));
			}
		}
	});
}

//------------------------------------------------------

var server = http.createServer(function (req, res) {
    'use strict';
    console.log('REQ>', req.url);
    var file, fname;
    if (req.url === '/') {
        file = fs.readFileSync('./root/index.html');
        res.end(file);
	} else if (req.url === '/nodelist.json') {
		makeNodeList((function (res) {
			return function (err, nodelist) {
				file = JSON.stringify(nodelist);
				res.end(file);
			};
		}(res)));
    } else {
        try {
            file = fs.readFileSync('./root' + req.url.length);
            res.end(file);
        } catch (e) {
            res.writeHead(404, {'Content-Type': 'text/plain'});
            res.end('not found\n');
        }
    }
});
if (process.argv.length > 2) {
    port = process.argv[2];
}
server.listen(port);
console.log('start server "http://localhost:' + port + '/"');


