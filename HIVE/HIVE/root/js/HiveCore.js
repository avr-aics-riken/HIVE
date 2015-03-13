/*jslint devel:true*/
/*global Blob, URL*/
/*global HiveConnect*/  // HiveConnect.js
/*global HiveCommand*/ // HiveCommand.js

(function (window) {
	'use strict';
	
	//----------------------------------------------------------------------------------------------
	// internal functions
	//
	function defaultCallback(src) {
		return function (err, res) {
			if (err) {
				console.error('runscript error:', err, {script: src});
			} else {
				console.log('runscript result:', res, {script: src});
			}
		};
	}
	function runScript(conn, src, callback) {
		conn.rendererMethod('runscript', {script: src}, callback || defaultCallback(src));
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
					cmd += sceneCommands.render('view');
					runScript(cmd);
				}
			}*/
		});
	}
	
	//----------------------------------------------------------------------------------------------
	// Contructor
	//
	var HiveCore = function (resultElement, width, height) {
		this.conn = new HiveConnect();
		this.resize(width, height);
		this.modelCount = 0;
		registerMethods(this, resultElement);
	};

	//----------------------------------------------------------------------------------------------
	// Basic
	//
	HiveCore.prototype.resize = function (width, height) {
		this.screenSize = [Math.floor(width), Math.floor(height)];
	};
	
	HiveCore.prototype.render = function () {
		// TODO: Temp camera
		var cmd = '';//sceneCommands.createCamera('camera');
		//cmd += sceneCommands.cameraScreenSize('camera', this.screenSize[0], this.screenSize[1]);
		cmd += HiveCommand.cameraPos('view', [0, 0, 300]);
		cmd += HiveCommand.renderCamera(this.screenSize[0], this.screenSize[1], 'view');
		runScript(this.conn, cmd);
	};
	
	
	//----------------------------------------------------------------------------------------------
	// Loader
	//
	HiveCore.prototype.loadOBJ = function (filepath, shaderpath) {
		var cmd = HiveCommand.loadOBJ('model' + this.modelCount, filepath, shaderpath);
		runScript(this.conn, cmd);
	};
	HiveCore.prototype.loadSTL = function (filepath, shaderpath) {
		var cmd = HiveCommand.loadSTL('model' + this.modelCount, filepath, shaderpath);
		runScript(this.conn, cmd);
	};
	HiveCore.prototype.loadPDB = function (filepath, shaderpath) {
		var cmd = HiveCommand.loadPDB('model' + this.modelCount, filepath, shaderpath);
		runScript(this.conn, cmd);
	};
	HiveCore.prototype.loadSPH = function (filepath, shaderpath) {
		var cmd = HiveCommand.loadSPH('model' + this.modelCount, filepath, shaderpath);
		runScript(this.conn, cmd);
	};

	//----------------------------------------------------------------------------------------------
	// Information
	//
	HiveCore.prototype.getSceneInformation = function () {
		var cmd = HiveCommand.getObjectList();
		runScript(this.conn, cmd, function (err, res) {
			console.log(err, res);
		});
	};
	//----------------------------------------------------------------------------------------------
	// Scene operation
	//
	HiveCore.prototype.newScene = function () {
		var cmd = HiveCommand.newScene();
		console.log('newscene');
		runScript(this.conn, cmd);
		this.render();
	};

	//----------------------------------------------------------------------------------------------
	
	HiveCore.prototype.updateTime = function (tm) {
		//console.log('Time = ', tm);
	};
	window.HiveCore = HiveCore;
}(window));