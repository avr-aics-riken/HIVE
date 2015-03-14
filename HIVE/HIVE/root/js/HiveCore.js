/*jslint devel:true*/
/*global Blob, URL*/
/*global HiveConnect*/  // HiveConnect.js
/*global HiveCommand*/  // HiveCommand.js
/*global vec3, vec4, mat4, subtract, add, scale, rotate, cross, translate, dot, normalize, transpose, mat3 */ // MV.js

(function (window) {
	'use strict';
	
	//----------------------------------------------------------------------------------------------
	// internal functions
	//
	function defaultCallback(src) {
		return function (err, res) {
			if (err) {
				console.error('runscript error:', err, {script: src});
				console.trace();
			} else {
				console.log('runscript result:', res, {script: src});
			}
		};
	}
	function runScript(conn, src, callback) {
		conn.rendererMethod('runscript', {script: src}, callback || defaultCallback(src));
	}
	function registerMethods(hiveCore, resultElement, infoCallback) {
		if (!hiveCore.conn) { return; }
		
		hiveCore.conn.method('open', function (res) {
			hiveCore.updateSceneInformation();
		});

		hiveCore.conn.method('updateInfo', infoCallback);/*function (param, data) {
			console.log('AAAAAAAA:updateInfo', param, data);
		});*/
		
		hiveCore.conn.method('renderedImage', function (param, data) {
			if (param.type === 'jpg') {
				resultElement.src = URL.createObjectURL(new Blob([data], {type: "image/jpeg"}));
			}
			
			// Refine render
			if (!param.canceled) {
				if (param.width <= hiveCore.screenSize[0] && param.height <= hiveCore.screenSize[1]) {
					//console.log('REFINE RENDER!', param.width * 2, param.height * 2);
					var w = Math.min(param.width * 2, hiveCore.screenSize[0] * 2),
						h = Math.min(param.height * 2, hiveCore.screenSize[1] * 2),
						cmd = HiveCommand.renderCamera(w, h, 'view');
					runScript(hiveCore.conn, cmd);
				}
			}
		});
	}
	
	//----------------------------------------------------------------------------------------------
	// Contructor
	//
	var HiveCore = function (resultElement, width, height, infoCallback) {
		this.conn = new HiveConnect();
		this.resize(width, height);
		this.modelCount = 0;
		this.viewCamera = {pos: vec3(0, 0, 300), tar: vec3(0, 0, 0), up: vec3(0, 1, 0), fov: 60};
		this.objectList = [];
		this.updateSceneCallback = infoCallback;
		registerMethods(this, resultElement, infoCallback);
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
		//cmd += HiveCommand.cameraPos('view', [0, 0, 300]);
		cmd += HiveCommand.cameraLookat('view', this.viewCamera.pos, this.viewCamera.tar, this.viewCamera.up, this.viewCamera.fov);
		cmd += HiveCommand.renderCamera(parseInt(this.screenSize[0] / 8, 10), parseInt(this.screenSize[1] / 8, 10), 'view');
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
	HiveCore.prototype.updateSceneInformation = function () {
		var cmd = HiveCommand.updateSceneInformation();
		runScript(this.conn, cmd);
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
	
	HiveCore.prototype.addCamera = function (name) {
		var cmd = '';
		if (name) {
			cmd = HiveCommand.createCamera(name);
		} else {
			cmd = HiveCommand.createCamera('Camera-' + this.modelCount);
		}
		this.modelCount = this.modelCount + 1;
		runScript(this.conn, cmd);
	};
	
	HiveCore.prototype.deleteObject = function (name) {
		// delete
		//TODO:
		runScript(this.conn, HiveCommand.deleteObject(name));
	};
	
	//----------------------------------------------------------------------------------------------
	// View operation
	//
	HiveCore.prototype.Rotate = function (rotx, roty) {
		var eyedir = subtract(this.viewCamera.pos, this.viewCamera.tar),
			v = vec4(eyedir[0], eyedir[1], eyedir[2], 0.0),
			rx = rotate(roty, vec3(0, 1, 0)),
			ry = rotate(rotx, vec3(1, 0, 0));
		v = vec4(dot(ry[0], v), dot(ry[1], v), dot(ry[2], v), 0.0);
		v = vec3(dot(rx[0], v), dot(rx[1], v), dot(rx[2], v));
		this.viewCamera.pos = add(this.viewCamera.tar, v);
		this.render();
	};
	HiveCore.prototype.Zoom = function (zoom) {
		var v = subtract(this.viewCamera.pos, this.viewCamera.tar),
			r = 1.0 - (zoom / 1000.0);
		v = scale(r, v);
		console.log(v);
		this.viewCamera.pos = add(this.viewCamera.tar, v);
		this.render();
	};
	HiveCore.prototype.Translate = function (tx, ty) {
		/*
			TODO: FIX ME
		*/
		var mv = vec3(-tx, ty, 0.0),
			az = normalize(subtract(this.viewCamera.pos, this.viewCamera.tar)),
			ax = normalize(cross(vec3(0, 1, 0), az)),
			ay = normalize(cross(az, ax)),
			mx = transpose(mat3(ax, ay, az)),
			mm = vec3(vec3(dot(mv, mx[0]), dot(mv, mx[1]), dot(mv, mx[2])));
		this.viewCamera.pos = add(this.viewCamera.pos, mm);
		this.viewCamera.tar = add(this.viewCamera.tar, mm);
		this.render();
	};
	
	//----------------------------------------------------------------------------------------------
	// Callback
	//
	
	//----------------------------------------------------------------------------------------------
	
	HiveCore.prototype.updateTime = function (tm) {
		//console.log('Time = ', tm);
	};
	window.HiveCore = HiveCore;
}(window));