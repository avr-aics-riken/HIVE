/*jslint devel:true, node: true, nomen: true */
/*global require, Error, process*/

var	HRENDER = '../hrender',
	HRENDER_ARG = ['hrender_server.lua'],
	HRENDER_THUMBNAIL_ARG = ['hrender_thumbnail.lua'],
	HTTP_ROOT_DIR = './root/',
	port = process.argv.length > 2 ? process.argv[2] : 8080,
	http = require('http'),
	metabin = require('./lib/metabinary'),
	path = require('path'),
	fs = require('fs'),
	spawn = require('child_process').spawn,
	seserver = http.createServer(function (req, res) {
		'use strict';
		console.log('REQ>', req.url);
		var file, fname;
		if (req.url === '/') {
			file = fs.readFileSync(HTTP_ROOT_DIR + 'index.html');
			res.end(file);
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
	id_counter = 0,
	renderNode = null, // one renderer
	clientNode = null; // one client

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
function requestFileList(dir, msg_id) {
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

function requestShaderList(msg_id) {
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

function saveScene(filepath, data) {
	'use strict';
	var jsonstr;
	try {
		jsonstr = JSON.stringify(data, function (key, val) { return val; }, "    ");
		fs.writeFileSync(filepath, jsonstr);
		console.log("saved:" + filepath);
	} catch (e) {
		console.error('[Error] Failed to save: ' + filepath);
	}
}

function loadScene(filepath, msg_id) {
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
	}
}

ws.on('request', function (request) {
	"use strict";
	var connection = request.accept(null, request.origin),
		retlua;

	console.log('[CONNECTION] Connection accepted : id = ' + id_counter + ' / ' + (new Date()));
	// save connection with id
	connection.id = id_counter;
	ws_connections.id_counter = connection;
	id_counter = id_counter + 1;
	
	/*
		Master process methods
	*/
	function masterMethod(method, param, msg_id) {
		console.log('[DEBUG] masterMethod:', method, param, msg_id);
		if (method === 'register') { // Register Renderer or Client
			if (param.mode === 'renderer') {
				console.log('[CONNECTION] Connected Renderer id = ' + connection.id);
				connection.type = 'renderer';
				renderNode = connection;
			} else if (param.mode === 'client') {
				console.log('[CONNECTION] Connected client id = ' + connection.id);
				connection.type = 'client';
				clientNode = connection;
			}
		} else if (method === 'requestFileList') {
			requestFileList(param.path, msg_id);
		} else if (method === 'requestShaderList') {
			requestShaderList(msg_id);
		} else if (method === 'saveScene') {
			saveScene(param.path, param.data);
		} else if (method === 'loadScene') {
			loadScene(param.path, msg_id);
		} else {
			console.error('Error: Unknow method');
		}
	}
	
	/*
		Process utf8 text message
	*/
	function eventTextMessage(ret, utf8Data) {
		var result, param;
		if (ret.error) {
			if (ret.to === 'master') {
				console.error('[Error]:', ret.err);
			} else if (clientNode && ret.to === 'client') { // for client
				clientNode.send(utf8Data);
			} else if (renderNode && ret.to === 'renderer') { // for renderer
				clientNode.send(utf8Data);
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
			} else if (clientNode && ret.to === 'client') { // for client
				clientNode.send(JSON.stringify(ret));
			} else if (renderNode && ret.to === 'renderer') { // for renderer
				clientNode.send(JSON.stringify(ret));
			}
		} else if (ret.method) {

			// syntax sugar
			if (typeof ret.param === 'object') {
				param = ret.param;
			} else {
				param = JSON.parse(ret.result);
			}

			if (ret.to === 'master') {
				masterMethod(ret.method, param, ret.id);
			} else if (ret.to === 'client') { // for client
				if (clientNode) {
					clientNode.send(JSON.stringify(ret));
				} else {
					sendErrorMessage(renderNode, 'Not connected ClientNode', ret.id);
				}
			} else if (ret.to === 'renderer') { // for renderer
				if (renderNode) {
					renderNode.send(JSON.stringify(ret));
				} else {
					sendErrorMessage(clientNode, 'Not connected RenderNode', ret.id);
				}
			}
		}
	}
	
	/*
		Process binary message
	*/
	function eventBinaryMessage(meta, binMessage) {
		if (meta.result) {
			if (meta.to === 'master') {
				console.error('[Result]:', meta);
			} else if (clientNode && meta.to === 'client') { // for client
				clientNode.send(binMessage);
			} else if (renderNode && meta.to === 'renderer') { // for renderer
				clientNode.send(binMessage);
			}
		} else if (meta.method) {
			if (meta.to === 'master') {
				masterMethod(meta.method, meta);
			} else if (clientNode && meta.to === 'client') { // for client
				clientNode.send(binMessage);
			} else if (renderNode && meta.to === 'renderer') { // for renderer
				renderNode.send(binMessage);
			}
		}
	}
	
	connection.on('message', function (message) {
		var ret;
		//console.log('[DEBUG] message=', message);
		if (message.type === 'utf8') {
			console.log('[DEBUG] RET=' + message.utf8Data);
			try {
				ret = JSON.parse(message.utf8Data);
				eventTextMessage(ret, message.utf8Data);
			} catch (e) {
				console.error('[ERROR] Invalid JSON data:-->\n' + message.utf8Data + '\n', e);
			}
		} else if (message.type === 'binary') {
			metabin.loadMetaBinary(message.binaryData, (function (binData) {
				return function (meta, content) {
					//console.log('[DEBUG] MetaBin:', meta);
					eventBinaryMessage(meta, binData);
				};
			}(message.binaryData)));
		}
	});
	
	connection.on('close', function () {
		delete ws_connections[connection.id];
		console.log('[CONNECTION] Connection closed : type = [' + connection.type + "] id = " + connection.id);
		if (connection.type === 'renderer') {
			renderNode = null;
		} else if (connection.type === 'client') {
			clientNode = null;
		}
	});
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
					json = fs.readFileSync(jsonpath);
					if (!fs.existsSync(outpath)) {
						callback(index, fullpath, outpath, json);
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
captureThumbnail();

function startupHRenderServer() {
	'use strict';
	try {
		var process = spawn(HRENDER, HRENDER_ARG);
		process.stdout.on('data', function (data) {
			console.log('stdout: ' + data);
		});
		process.stderr.on('data', function (data) {
			console.error('stderr: ' + data);
		});
		process.on('exit', function (code) {
			console.error('-------------------------\nhrender is terminated.\n-------------------------');
			console.log('exit code: ' + code);
			startupHRenderServer(); // reboot
		});
		process.on('error', function (err) {
			console.log('process error', err);
		});
	} catch (e) {
		console.log('process error', e);
	}
}
startupHRenderServer();
