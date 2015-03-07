/*jslint devel:true*/
/*global WebSocket*/

(function (window) {
	'use strict';
	
	function HiveConnect() {
		var url = location.host === '' ? "ws://localhost:8080/" : "ws://" + location.host;
		this.url = url;
		this.reconnectTimeout = 2000;
		this.reconnect();
	}
	
	// public:
	HiveConnect.prototype.sendMsg = function (data) {
		this.ws.send(JSON.stringify({jsonrpc: "2.0", result: data, from: 'client'}));
	};
	
	HiveConnect.prototype.reconnect = function () {
		var ws  = new WebSocket(this.url);
		this.ws = ws;
		console.log('[wsConnect] Connecting:', this.url);

		ws.addEventListener('open', (function (self) {
			return function (event) {
				console.log('[NETWORK] Connected.');
				self.sendMsg({mode: 'client'}); // Initial response
			};
		}(this)));

		ws.addEventListener('message', function (event) {
			console.log('[RECV]', event.data);
		});

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
