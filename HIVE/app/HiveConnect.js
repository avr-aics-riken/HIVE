/*jslint devel:true*/
/*global WebSocket*/

(function (window) {
	'use strict';
	
	function HiveConnect() {
		var url = location.host === '' ? "ws://localhost:8080/" : "ws://" + location.host;
		this.url = url;
		this.messageCount = 0;
		this.callbacks = {};
		this.reconnectTimeout = 2000;
		this.reconnect();
	}
	
	// public:
	HiveConnect.prototype.masterResult = function (result) {
		var id = 0; // TODO
		this.ws.send(JSON.stringify({jsonrpc: "2.0", result: result, id: id, to: 'master'}));
	};

	HiveConnect.prototype.rendererResult = function (result) {
		var id = 0; // TODO
		this.ws.send(JSON.stringify({jsonrpc: "2.0", result: result, id: id, to: 'renderer'}));
	};

	HiveConnect.prototype.masterMethod = function (method, param, callback) {
		this.messageCount = this.messageCount + 1;
		if (callback) {
			this.callbacks[this.messageCount] = callback;
		}
		this.ws.send(JSON.stringify({jsonrpc: "2.0", method: method, param: param, id: this.messageCount, to: 'master'}));
	};

	HiveConnect.prototype.rendererMethod = function (method, param, callback) {
		this.messageCount = this.messageCount + 1;
		if (callback) {
			this.callbacks[this.messageCount] = callback;
		}
		this.ws.send(JSON.stringify({jsonrpc: "2.0", method: method, param: param, id: this.messageCount, to: 'renderer'}));
	};

	//-------------------
	
	HiveConnect.prototype.reconnect = function () {
		var ws  = new WebSocket(this.url);
		this.ws = ws;
		console.log('[wsConnect] Connecting:', this.url);

		ws.addEventListener('open', (function (self) {
			return function (event) {
				console.log('[NETWORK] Connected.');
				self.masterMethod('register', {mode: 'client'}); // Initial response
			};
		}(this)));

		ws.addEventListener('message', (function (self) {
			return function (event) {
				//console.log('[RECV]', event.data);
				var data;
				try {
					data = JSON.parse(event.data);
				} catch (e) {
					console.error('[Error] Recv Invalid JSON / ', e);
					console.error(event.data);
				}
				if (data.error) {
					console.error('[Error] Response => ', data.error);
				} else if (data.result) {
					console.log('[DEBUG] Result => ', data.result);
					if (!data.id) {
						console.error('[Error] Not found message ID');
						console.error(event.data);
						return;
					}
					if (self.callbacks[data.id]) {
						self.callbacks[data.id](data.result, data.id);
					}
				}
			};
		}(this)));

		ws.addEventListener('error', function (event) {
			console.log('[NETWORK] Error occured.', event);
		});

		ws.addEventListener('close', (function (self) {
			return function (event) {
				console.log('[NETWORK] Connection Closed.');
				setTimeout(function () {
					self.reconnect();
				}, self.reconnectTimeout);
			};
		}(this)));
	};
	
	window.HiveConnect = HiveConnect;
}(window));
