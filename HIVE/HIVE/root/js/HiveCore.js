/*jslint devel:true*/
/*global HiveConnect, Blob, URL, sceneCommands*/

(function (window) {
	'use strict';
	function renderScript(conn, src) {
		conn.rendererMethod('runscript', {script: src}, function (err, res) {
			if (err) {
				console.error('runscript error:', err, {script: src});
			} else {
				console.log('runscript result:', res, {script: src});
			}
		});
	}
	function registerMethods(hiveCore, resultElement) {
		if (!hiveCore.conn) { return; }
		
		hiveCore.conn.method('open', function (res) {
			hiveCore.render();
		});

		hiveCore.conn.method('renderedImage', function (param, data) {
			if (param.type === 'jpg') {
				resultElement.src = URL.createObjectURL(new Blob([data], {type: "image/jpeg"}));
			}
			
			// Refine render
			/*if (!param.canceled) {
				if (param.width < camera_maxsize[0] || param.height < camera_maxsize[1]) {
					//console.log('REFINE RENDER!', param.width * 2, param.height * 2);
					cmd = sceneCommands.cameraScreenSize('camera', param.width * 2, param.height * 2);
					cmd += sceneCommands.render();
					renderScript(cmd);
				}
			}*/
		});
	}
	
	var HiveCore = function (resultElement, width, height) {
		this.conn = new HiveConnect();
		this.resize(width, height);
		registerMethods(this, resultElement);
	};

	//----------
	HiveCore.prototype.resize = function (width, height) {
		this.screenSize = [Math.floor(width), Math.floor(height)];
	};
	
	HiveCore.prototype.render = function () {
		// TODO: Temp camera
		var cmd = sceneCommands.createCamera('camera');
		cmd += sceneCommands.cameraScreenSize('camera', this.screenSize[0], this.screenSize[1]);
		cmd += sceneCommands.cameraPos('camera', [0, 0, 300]);
		cmd += sceneCommands.render();
		renderScript(this.conn, cmd);
		//console.log('RENDER!!', this.screenSize[0], this.screenSize[1]);
	};
	HiveCore.prototype.loadObj = function (filepath, shaderpath) {
		if (filepath || filepath === '') {
			console.error('[Error] loadObj: Invalid filepath or shaderpath');
		} else {
			renderScript(this.conn, sceneCommands.loadObj('model', filepath, shaderpath));
		}
	};
	HiveCore.prototype.updateTime = function (tm) {
		//console.log('Time = ', tm);
	};
	window.HiveCore = HiveCore;
}(window));