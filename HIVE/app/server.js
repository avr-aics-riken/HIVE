/*jslint devel:true*/
/*global require*/

var http = require('http'),
	seserver = http.createServer(function (req, res) {
		'use strict';
		console.log('REQ>', req.url);
		res.end("websocket sender");
	}).listen(8080),
	websocket = require('websocket'),
	ws = new websocket.server({httpServer : seserver,
							   maxReceivedFrameSize : 0x1000000, // more receive buffer!! default 65536B
							   autoAcceptConnections : false}),
	ws_connections = {},
	id_counter = 0,
	renderNode = null, // one renderer
	clientNode = null; // one client
	

ws.on('request', function (request) {
	"use strict";
	var connection = request.accept(null, request.origin),
		retlua;

	console.log((new Date()) + "[CONNECTION] Connection accepted : " + id_counter);
	// save connection with id
	connection.id = id_counter;
	ws_connections.id_counter = connection;
	id_counter = id_counter + 1;
	
	connection.on('message', function (message) {
		var ret, data;
		//console.log('message:', message);
		if (message.type === 'utf8') {
			//console.log("RET:" + message.utf8Data);
			try {
				ret = JSON.parse(message.utf8Data);
				if (ret.error) {
					console.error('[Error]:', ret.error);
				} else if (ret.result) {
					console.error('[Result]:', ret.result);
					
					// syntax sugar
					if (typeof ret.result === 'object') {
						data = ret.result;
					} else {
						data = JSON.parse(ret.result);
					}
										
					// Register Renderer or Client
					if (data.mode === 'renderer') {
						console.log('[CONNECTION] Connected Renderer id = ' + connection.id);
						connection.type = 'renderer';
						renderNode = connection;
					} else if (data.mode === 'client') {
						console.log('[CONNECTION] Connected client id = ' + connection.id);
						connection.type = 'client';
						clientNode = connection;
					}
					
					// response routing
					if (clientNode && ret.from !== 'client') { // for client
						clientNode.send(JSON.stringify(data));
					}
					if (renderNode && ret.from !== 'renderer') { // for renderer
						if (data.script) {
							console.log('[SCRIPT] Script request:', data.script);
							if (renderNode) {
								console.log('Send script');
								renderNode.send(data.script);
							}
						}
					}
				}
			} catch (e) {
				console.error('[ERROR] Invalid JSON data:-->\n' + message.utf8Data + '\n', e);
			}
		}
	});
	
	connection.on('close', function () {
		delete ws_connections[connection.id];
		console.log('[CONNECTION] Connection closed : type = [' + connection.type + "] id = " + connection.id);
	});
});