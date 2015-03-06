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
	renderNode = null,
	clientNode = null;
	

ws.on('request', function (request) {
	"use strict";
	var connection = request.accept(null, request.origin),
		retlua;

	console.log((new Date()) + " ServerImager Connection accepted : " + id_counter);
	// save connection with id
	connection.id = id_counter;
	ws_connections.id_counter = connection;
	id_counter = id_counter + 1;
	
	
	retlua = "{mode = ClientType()}";
	connection.send('print("Connected with HIVE master")\n return JSON.encode(' + retlua + ')'); // TODO: renderer open event
	//connection.send('return JSON.encode({aaa=tostring(JSON)})');
	
	connection.on('message', (function (conn) { return function (message) {
		var ret, data;
		//console.log('message:', message);
		if (message.type === 'utf8') {
			//console.log("RET:" + message.utf8Data);
			try {
				ret = JSON.parse(message.utf8Data);
				if (ret.error) {
					console.error('[Error]:', ret.error);
					if (clientNode) {
						clientNode.send(message.utf8Data);
					}
				} else if (ret.result) {
					console.error('[Result]:', ret.result);
					if (typeof ret.result === 'object') {
						data = ret.result;
					} else {
						data = JSON.parse(ret.result);
					}
					
					// echo
					if (clientNode && data.from !== 'client') {
						clientNode.send(JSON.stringify(data));
					}
					
					// Renderer or Client
					if (data.mode === 'renderer') {
						console.log('Connected Renderer');
						renderNode = conn;
					} else if (data.mode === 'client') {
						console.log('Connected client');
						clientNode = conn;
					}
					
					if (data.script) {
						console.log('Script request:', data.script);
						if (renderNode) {
							console.log('Send script');
							renderNode.send(data.script);
						}
					}
				}
			} catch (e) {
				console.error('Invalid JSON data:-->\n' + message.utf8Data + '\n', e);
			}
		}
	}; }(connection)));
	
	connection.on('close', function () {
		delete ws_connections[connection.id];
		console.log('connection closed :' + connection.id);
	});
	
});