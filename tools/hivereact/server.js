/*jslint devel:true, node: true, nomen: true */
/*global require, Error, process*/

var	HRENDER = __dirname + '/../../build/bin/hrender',
//var	HRENDER = __dirname + '/../hrender',
	HRENDER_ARG = [__dirname +'/lua/hrender_server.lua'],
	HRENDER_THUMBNAIL_ARG = [__dirname + '/lua/hrender_thumbnail.lua'],
	HTTP_ROOT_DIR = __dirname + '/client/',
	metabin = require(__dirname + '/lib/metabinary'),
	port = process.argv.length > 2 ? process.argv[2] : 8080,
	http = require('http'),
	path = require('path'),
    babel = require('babel-core'),
	fs = require('fs'),
	spawn = require('child_process').spawn,
	seserver = http.createServer(function (req, res) {
		'use strict';
		console.log('REQ>', req.url);
		var file, fname, jsondata;
		if (req.url === '/') {
			file = fs.readFileSync(HTTP_ROOT_DIR + 'index.html');
			res.end(file);
        } else if (req.url === '/nodelist.json') { // temp
            makeNodeList((function (res) {
                return function (err, nodelist) {
                    jsondata = {error: err, data:nodelist};
                    file = JSON.stringify(jsondata);
                    res.end(file);
                };
            }(res)));
		} else {
			try {
				fname = req.url.substr(1, req.url.length); // remove '/'
				file = fs.readFileSync(HTTP_ROOT_DIR + fname);
				res.end(file);
			} catch (e) {
				res.writeHead(404, {'Content-Type': 'text/plain'});
				res.end('not found\n');
			}
		}
	}).listen(port),
	websocket = require('websocket'),
	ws = new websocket.server({httpServer : seserver,
							   maxReceivedFrameSize : 0x1000000, // more receive buffer!! default 65536B
							   autoAcceptConnections : false}),
	ws_connections = {},
	id_counter = 0;
    
//-----------------------------------------------------

function makeNodeList(callback) {
	"use strict";
	var nodeDir = './nodedata';
	fs.readdir(nodeDir, function (err, files) {
		var infofile,
			nodeDirPath,
			fileCounter,
            readError,
			customFuncLua,
            uiFunc,
			nodelist = [],
			i;
		if (err) {
			return;
		}

		fileCounter = 0;
        readError = '';
		function finishLoad() {
			fileCounter = fileCounter - 1;
			if (fileCounter === 0) {
				callback((readError.length === 0 ? null : readError), nodelist);
			}
		}
		function loadFunc(nodeDirPath) {
			return function (err, data) {
				try {
					var json = JSON.parse(data);
					if (json.customfuncfile !== undefined) {
						customFuncLua = fs.readFileSync(nodeDirPath + "/" + json.customfuncfile, 'utf8');
						json.customfunc = customFuncLua;
                        uiFunc = babel.transformFileSync(nodeDirPath + "/" + json.uifile, {ignore:'react'}).code;
						json.uiFunc = uiFunc;                        
					}
					nodelist.push(json);
				} catch (e) {
                    var errmsg = '[Error] Failed Load:' + nodeDirPath + "/info.json";
					console.error(errmsg, e);
                    readError += errmsg + '\n' + e.toString() + '\n';
				}
				finishLoad();
			};
		}
		for (i in files) {
			if (files.hasOwnProperty(i)) {
				if (files[i].substr(0, 1) !== '.') {
					nodeDirPath = nodeDir + "/" + files[i];
					infofile = nodeDirPath + "/info.json";
					//console.log(infofile);
					fileCounter = fileCounter + 1;
					fs.readFile(infofile, 'utf8', loadFunc(nodeDirPath));
				}
			}
		}
	});
}

//---------------

function sendErrorMessage(ws_node, msgstr, id) {
	'use strict';
	if (!ws_node) {
		return;
	}

	ws_node.send(JSON.stringify({
		JSONRPC: "2.0",
		error: msgstr,
		id: id
	}));
}


function getFile(dir, list) {
	'use strict';
	var files, i, name;
	try {
		files = fs.readdirSync(dir);
		if (!files) { return; }
		if (dir.substr(dir.length - 1) !== "/") { dir += "/"; }
		for (i in files) {
			if (files.hasOwnProperty(i)) {
				name = dir + files[i];
				if (fs.statSync(name).isDirectory()) {
					list.push({"name": files[i], "type": "dir", "path": name});
				} else if (files[i].substring(0, 1) !== '.') {
					list.push({"name": files[i], "type": "file", "path": name});
				}
			}
		}
	} catch (e) {
		list = [];
		console.error("not found dir:" + dir);
	}
}
function requestFileList(clientNode, dir, msg_id) {
	'use strict';
	var files = [];
	console.log('[DEBUG] requestFileList:', dir);
	getFile(dir, files);
	clientNode.send(JSON.stringify({
		JSONRPC: "2.0",
		result: JSON.stringify(files),
		id: msg_id
	}));
}

function requestShaderList(clientNode, msg_id) {
	'use strict';
	var files = [],
		shaderlist = [],
		i,
		infofile;
	console.log('[DEBUG] requestShaderList');
	getFile('./shader', files);
	for (i = 0; i < files.length; i = i + 1) {
		if (files[i].type === 'file' && files[i].name.substr(files[i].name.length - 5) === '.frag') {
			infofile = files[i].path.replace('.frag', '.json');
			try {
				files[i].info = JSON.parse(fs.readFileSync(infofile));
				shaderlist.push(files[i]);
			} catch (e) {
				console.error('[Error] Failed to read: ' + infofile);
			}
		}
	}
	clientNode.send(JSON.stringify({
		JSONRPC: "2.0",
		result: JSON.stringify(shaderlist),
		id: msg_id
	}));
}

function saveScene(clientNode, filepath, data, msg_id) {
	'use strict';
	var jsonstr;
	try {
		jsonstr = JSON.stringify(data, function (key, val) { return val; }, "    ");
		fs.writeFileSync(filepath, jsonstr);
		console.log("saved:" + filepath);
	} catch (e) {
		console.error('[Error] Failed to save: ' + filepath);
		sendErrorMessage(clientNode, '[Error] Failed to save: ' + filepath, msg_id);
		return;
	}
	clientNode.send(JSON.stringify({
		JSONRPC: "2.0",
		result: "{}",
		id: msg_id
	}));
}

function copyResourceFile(filepath) {
	'use strict';
	var basedir = path.resolve(__dirname, '.'),
		hiveCommandFile = path.join(basedir, 'HiveCommand.lua'),
		dstpath;
	try {
		if (fs.existsSync(hiveCommandFile)) {
			dstpath = path.join(path.dirname(filepath), 'HiveCommand.lua');
			fs.createReadStream(hiveCommandFile).pipe(fs.createWriteStream(dstpath));
		}
	} catch (e) {
		console.error('[Error] Failed to copy resource file: ' + filepath);
	}
}

function exportScene(clientNode, filepath, data, msg_id) {
	'use strict';
	try {
		fs.writeFileSync(filepath, data);
		copyResourceFile(filepath);
		console.log("saved:" + filepath);
	} catch (e) {
		console.error('[Error] Failed to export: ' + filepath);
		sendErrorMessage(clientNode, '[Error] Failed to export: ' + filepath, msg_id);
		return;
	}
	clientNode.send(JSON.stringify({
		JSONRPC: "2.0",
		result: "{}",
		id: msg_id
	}));
}

function loadScene(clientNode, filepath, msg_id) {
	'use strict';
	var json;
	try {
		json = JSON.parse(fs.readFileSync(filepath));
		console.log("loaded:" + filepath);
		clientNode.send(JSON.stringify({
			JSONRPC: "2.0",
			result: JSON.stringify(json),
			id: msg_id
		}));
	} catch (e) {
		console.error('[Error] Failed to load: ' + filepath);
		sendErrorMessage(clientNode, '[Error] Failed to load: ' + filepath, msg_id);
		return;
	}
	clientNode.send(JSON.stringify({
		JSONRPC: "2.0",
		result: "{}",
		id: msg_id
	}));
}

function copyShaderFile(clientNode, filepath, srcFileNames, msg_id) {
	'use strict';
	var basedir = path.resolve(__dirname, '.'),
		fileNames,
		srcpath,
		dstpath,
		i;
	try {
		fileNames = srcFileNames;
		for (i = 0; i < fileNames.length; i = i + 1) {
			srcpath = path.join(basedir, fileNames[i]);
			dstpath = path.join(path.dirname(filepath), path.basename(fileNames[i]));
			if (fs.existsSync(srcpath)) {
				if (!fs.existsSync(dstpath)) {
					fs.createReadStream(srcpath).pipe(fs.createWriteStream(dstpath));
				}
			}
		}
	} catch (e) {
		console.error('[Error] Failed to copy shader: ' + srcFileNames);
		sendErrorMessage(clientNode, '[Error] Failed to copy shader: ' + srcFileNames, msg_id);
		return;
	}
	clientNode.send(JSON.stringify({
		JSONRPC: "2.0",
		result: "{}",
		id: msg_id
	}));
}

ws.on('request', function (request) {
	"use strict";
	var connection = request.accept(null, request.origin),
		retlua;

	console.log('[CONNECTION] Connection accepted : id = ' + id_counter + ' / ' + (new Date()));
	// save connection with id
	connection.id = id_counter;
	ws_connections[id_counter] = {id:id_counter , conn:connection};
	id_counter = id_counter + 1;

	/*
		Master process methods
	*/
	function masterMethod(method, param, msg_id, from) {
		//console.log('[DEBUG] masterMethod:', method, param, msg_id);
		var clientNode, json, fr_conn, wsc, args;
		if (from != undefined) {
			wsc = ws_connections[parseInt(from)];
			if (wsc) {
				fr_conn = wsc.conn;
			}
		}
		if (method === 'register') { // Register Renderer or Client
			if (param.mode === 'renderer') {
				console.log('[CONNECTION] Connected Renderer id = ' + connection.id);
				connection.type = 'renderer';
				wsc = ws_connections[parseInt(param.targetid)];
				if (!wsc) {
					console.error('Unexpected to connect node. Zombie process?');
					return;
				}
				clientNode = wsc.conn;
				json = {id: connection.id};
				clientNode.send(JSON.stringify({
					JSONRPC: "2.0",
					method: 'registerRender',
					param:JSON.stringify(json),
					id: msg_id
				}));

			} else if (param.mode === 'client') {
				console.log('[CONNECTION] Connected client id = ' + connection.id);
				connection.type = 'client';
				clientNode = connection;
				json = {
					ret:'registered',
					id: connection.id
				};
				clientNode.send(JSON.stringify({
					JSONRPC: "2.0",
					result: JSON.stringify(json),
					id: msg_id
				}));
				args = ['--client:' + connection.id, 'ws://localhost:' + port];
				if (param.opengl == true) {
					args.push('--opengl');
				}
				if (param.ipc && param.ipc.slice(0,6) === 'ipc://') {
					args.push(param.ipc);
				}
				var a;
				for (a in args) {
					console.log('ARG=[' + a + '] = ' + args[a]);
				}
				clientNode.renderproc = startupHRenderServer(args);
			}
		} else if (method === 'requestFileList') {
			requestFileList(fr_conn, param.path, msg_id);
		} else if (method === 'requestShaderList') {
			requestShaderList(fr_conn, msg_id);
		} else if (method === 'saveScene') {
			saveScene(fr_conn, param.path, param.data, msg_id);
		} else if (method === 'exportScene') {
			exportScene(fr_conn, param.path, param.data, msg_id);
		} else if (method === 'loadScene') {
			loadScene(fr_conn, param.path, msg_id);
		} else if (method === 'copyShaderFile') {
			copyShaderFile(fr_conn, param.path, param.data, msg_id);
		} else {
			console.error('Error: Unknow method');
		}
	}

	/*
		Process utf8 text message
	*/
	function eventTextMessage(ret, utf8Data) {
		//console.log('[Log]eventTextMessage:', ret);
		if (ret.to) {
			var node = null;
			if (ret.to !== 'master' && ws_connections[ret.to]) {
				node = ws_connections[ret.to].conn;
			}
			eventTextMessageNode(node, ret, utf8Data);
		} else {
			console.error('Error: Not found "to" id !!');
		}
	}
	function eventTextMessageNode(node, ret, utf8Data) {
		var result, param;
		if (ret.error) {
			if (ret.to === 'master') {
				console.error('[Error]:', ret.err);
			} else {
				node.send(utf8Data);
			}
		} else if (ret.result) {
			// syntax sugar
			if (typeof ret.result === 'object') {
				result = ret.result;
			} else {
				result = JSON.parse(ret.result);
			}

			if (ret.to === 'master') {
				console.error('[Result]:', result);
			} else {
				node.send(JSON.stringify(ret));
			}
		} else if (ret.method) {

			// syntax sugar
			if (typeof ret.param === 'object') {
				param = ret.param;
			} else {
				param = JSON.parse(ret.param);
			}

			if (ret.to === 'master') {
				masterMethod(ret.method, param, ret.id, ret.from);
			} else {
				if (node) {
					node.send(JSON.stringify(ret));
				} else {
					sendErrorMessage(node, 'Not connected node', ret.id);
				}
			}
		}
	}

	/*
		Process binary message
	*/
	function eventBinaryMessage(meta, binMessage) {
		if (meta.to) {
			var node = null;
			if (meta.to !== 'master') {
				node =  ws_connections[meta.to].conn;
			}
			eventBinaryMessageNode(node, meta, binMessage);
		} else {
			console.error('Error: Not found "to" id !!');
		}
	}
	function eventBinaryMessageNode(node, meta, binMessage) {
		if (meta.result) {
			if (meta.to === 'master') {
				console.error('[Result]:', meta);
			} else {
				node.send(binMessage);
			}
		} else if (meta.method) {
			if (meta.to === 'master') {
				masterMethod(meta.method, meta.param, meta.id, meta.from);
			} else {
				node.send(binMessage);
			}
		}
	}

	connection.on('message', function (message) {
		var ret;
		//console.log('[DEBUG] message=', message);
		if (message.type === 'utf8') {
			//console.log('[DEBUG] RET=' + message.utf8Data);
			try {
				ret = JSON.parse(message.utf8Data);
			} catch (e) {
				console.error('[ERROR] Invalid JSON data:-->\n' + message.utf8Data + '\n', e);
				return;
			}
			eventTextMessage(ret, message.utf8Data);
		} else if (message.type === 'binary') {
			metabin.loadMetaBinary(message.binaryData, (function (binData) {
				return function (meta, content) {
					//console.log('[DEBUG] MetaBin:', meta);
					eventBinaryMessage(meta, binData);
				};
			}(message.binaryData)));
		}
	});

	connection.on('close', function (connection) {
		return function () {
			console.log('[CONNECTION] Connection closed : type = [' + connection.type + "] id = " + connection.id);
			if (connection.type === 'client') {
				console.log('Dissconnect client', connection.renderproc);
				if (connection.renderproc){
					connection.renderproc.kill();
				}
			}
			delete ws_connections[connection.id];

			/*if (connection.type === 'renderer') {
				renderNode = null;
			} else if (connection.type === 'client') {
				clientNode = null;
			}*/
		};
	}(connection));
});

function captureThumbnail() {
	'use strict';
	var shaderdir = path.resolve(__dirname, './shader'),
		files,
		thumbnailCount = 0,
		captureCount = 0,
		i,
		iterateFrags = function (index, callback) {
			var outdir = path.resolve(__dirname, './root/shader'),
				fullpath,
				filename,
				outpath,
				jsonpath,
				json;

			if (files.hasOwnProperty(index)) {
				fullpath = path.join(shaderdir, files[index]);
				if (path.extname(fullpath) === ".frag") {
					captureCount = captureCount + 1;
					console.log("[CaptureThumbnail]  " + captureCount + "/" + thumbnailCount);
					filename = path.basename(fullpath, path.extname(fullpath));
					outpath = path.join(outdir, path.basename(fullpath, path.extname(fullpath)) + '.jpg');
					jsonpath = path.join(path.dirname(fullpath), filename + '.json');

					//json = fs.readFileSync(jsonpath);
					if (!fs.existsSync(outpath) && fs.existsSync(jsonpath)) {
						callback(index, fullpath, outpath, jsonpath);
					} else {
						iterateFrags(index + 1, callback);
					}
				} else {
					iterateFrags(index + 1, callback);
				}
			}
		},
		executeFunc = function (index, fragpath, outpath, json) {
			var process = spawn(HRENDER, [HRENDER_THUMBNAIL_ARG, fragpath, outpath, json]);

			console.log("fragpath:" + fragpath);
			console.log("outpath:" + outpath);

			process.stdout.on('data', function (data) {
				console.log('stdout: ' + data);
			});
			process.stderr.on('data', function (data) {
				console.error('stderr: ' + data);
			});
			process.on('exit', function (code) {
				console.log('exit code: ' + code);
				iterateFrags(index + 1, executeFunc);
			});
			process.on('error', function (err) {
				console.log('process error', err);
			});
		};

	try {
		files = fs.readdirSync(shaderdir);
		// search valid frags for progress
		for (i in files) {
			if (files.hasOwnProperty(i)) {
				if (path.extname(path.join(shaderdir, files[i])) === ".frag") {
					thumbnailCount = thumbnailCount + 1;
				}
			}
		}
		console.log("captureThumbnail");
		iterateFrags(0, executeFunc);
	} catch (e) {
		console.log('process error', e);
	}
}
//captureThumbnail();


var spawnProcesses = [];

function startupHRenderServer(optarray) {
	'use strict';
	var process = null,
		i,
		arg = JSON.parse(JSON.stringify(HRENDER_ARG)); // copy

	if (optarray) {
		for (i = 0; i < optarray.length; i = i + 1) {
			arg.push(optarray[i]);
		}
	}

	try {
		process = spawn(HRENDER, arg);
		process.stdout.on('data', function (data) {
			console.log('stdout: ' + data);
		});
		process.stderr.on('data', function (data) {
			console.error('stderr: ' + data);
		});
		process.on('exit', function (code) {
			console.error('-------------------------\nhrender is terminated.\n-------------------------');
			console.log('exit code: ', code);
		});
		process.on('error', function (err) {
			console.log('process error', err);
		});
	} catch (e) {
		console.log('process error', e);
	}

	spawnProcesses.push(process);
	return process;
}
function stopHRenderServer() {
	var p;
	for (p in spawnProcesses) {
		spawnProcesses[p].kill();
	}
	spawnProcesses = [];
}

module.exports.stopHRenderServer = stopHRenderServer;
