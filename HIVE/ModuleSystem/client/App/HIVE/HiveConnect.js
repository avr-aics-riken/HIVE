/*jslint devel:true*/
/*global WebSocket, Blob, metabinary*/

(function (window) {
	'use strict';

	function HiveConnect(opt) {
		var wsUrl, ipcAddress, openglMode;

		if (opt) {
			if (opt.url) {
				wsUrl = opt.url;
			}
			//if (opt.ipc) {
			//	ipcAddress = opt.ipc;
			//}
			if (opt.opengl) {
				openglMode = opt.opengl;
			}
		}
		if (!wsUrl) {
			wsUrl = location.host === '' ? "ws://localhost:8080/" : "ws://" + location.host;
		}
		this.url = wsUrl;
		//this.ipcAddress = ipcAddress;
		this.openGLMode = openglMode;
		this.messageCount = 0;
		this.callbacks = {};
		this.methodFuncs = {};
		this.myid = -1;
		this.rendererId = -1;
		this.reconnectTimeout = 2000;
		this.reconnect();

		/*if (ipcAddress) {
			// Electron only
			var nano = require('nanomsg');
			var sc = nano.socket('pair');
			var buffercopy = require('buffercopy');
			var meta = require('../lib/metabinary'); // path from index.html
			var ret = sc.bind(ipcAddress);
			console.log('IPC bind = ', ret, ipcAddress);

			sc.on('data', function (data) {
				if (!meta.loadMetaBinary(data, function(meta, data) {
					var w, h,
						param = meta.param;
					if (param.type === 'jpg') {
						// resultElement is img.
						resultElement.src = URL.createObjectURL(new Blob([data], {type: "image/jpeg"}));
						if (hiveCore.websocketConn) {
							w = hiveCore.screenSize[0];
							h = hiveCore.screenSize[1];
							sendImageToSIP(hiveCore.websocketConn, hiveCore.chowderConnectionID, data, w, h);
						}
					} else if (param.type === 'raw'){
						// resultElement is canvas.
						//resultElement.setAttribute('width', param.width),
						//resultElement.setAttribute('height', param.height);
						var context = resultElement.getContext('2d');
						var imageData = context.createImageData(param.width, param.height);
						buffercopy.buffercopy(data, imageData.data);
						context.putImageData(imageData, 0, 0);
					}

				})) {
					console.error('Not metabin foramt');
				};
			});
		}*/

	}

	HiveConnect.prototype.setRendererId = function (id) {
		this.rendererId = parseInt(id);
	}

	// public:
	HiveConnect.prototype.masterResult = function (result) {
		var id = 0; // TODO
		this.ws.send(JSON.stringify({jsonrpc: "2.0", result: result, id: id, to: 'master', from: this.myid}));
	};

	HiveConnect.prototype.rendererResult = function (result) {
		var id = 0; // TODO
		this.ws.send(JSON.stringify({jsonrpc: "2.0", result: result, id: id, to: this.rendererId, from: this.myid}));
	};

	HiveConnect.prototype.masterMethod = function (method, param, callback) {
		this.messageCount = this.messageCount + 1;
		if (callback) {
			this.callbacks[this.messageCount] = callback;
		}
		this.ws.send(JSON.stringify({jsonrpc: "2.0", method: method, param: param, id: this.messageCount, to: 'master', from: this.myid}));
	};

	HiveConnect.prototype.rendererMethod = function (method, param, callback) {
		this.messageCount = this.messageCount + 1;
		if (callback) {
			this.callbacks[this.messageCount] = callback;
		}
		this.ws.send(JSON.stringify({jsonrpc: "2.0", method: method, param: param, id: this.messageCount, to: this.rendererId, from: this.myid}));
	};

	HiveConnect.prototype.method = function (methodname, func) {
		this.methodFuncs[methodname] = func;
	};

	//-------------------

	HiveConnect.prototype.reconnect = function () {
		var ws  = new WebSocket(this.url);
		this.ws = ws;
		this.myid = -1;
		console.log('[wsConnect] Connecting:', this.url);

		ws.addEventListener('open', (function (self) {
			return function (event) {
				console.log('[NETWORK] Connected.');
				self.masterMethod('register', {mode: 'client', opengl:self.openGLMode, ipc:self.ipcAddress}, function (err, ret) {
					console.log('Reigstered', ret);
					self.myid = ret.id;
				}); // Initial response
				if (self.methodFuncs.open) {
					self.methodFuncs.open();
				}
			};
		}(this)));

		ws.addEventListener('message', (function (self) {
			return function (event) {
				var data, par;
				//console.log('[RECV]', event.data);
				if (event.data instanceof Blob) {
					//console.log('[DEBUG] binary data recved.');
					window.metabinary.loadMetaBinary(event.data, function (meta, binData) {
						//console.log('[DEBUG] Meta=', meta);
						if (self.methodFuncs[meta.method]) {
							//console.log('[DEBUG] Call Method=>', meta.method);
							self.methodFuncs[meta.method](meta.param, binData);
						}
					});
				} else {
					try {
						data = JSON.parse(event.data);
					} catch (e) {
						console.error('[Error] Recv Invalid JSON / ', e);
						console.error(event.data);
					}
					if (data.error) {
						//console.error('[Error] Response => ', data.error);
						self.callbacks[data.id](data.error, null);
						delete self.callbacks[data.id];
					} else if (data.method) {
						if (self.methodFuncs[data.method]) {
							//console.log('[DEBUG] Call Method=>', data.method);
                            
                            try {
                                par = JSON.parse(data.param);
                            } catch (e) {
                                par = data.param; // direct
                            }
							self.methodFuncs[data.method](par, null);
						}
					} else if (data.result) {
						//console.log('[DEBUG] Result => ', data.result);
						if (!data.id) {
							console.error('[Error] Not found message ID');
							console.error(event.data);
							return;
						}
						if (self.callbacks[data.id]) {
							self.callbacks[data.id](null, JSON.parse(data.result), data.id);
							delete self.callbacks[data.id];
						}
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
