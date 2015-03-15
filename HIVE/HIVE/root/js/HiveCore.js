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

		hiveCore.conn.method('updateInfo', (function (core, infoCallback) {
			return function (param) {
				core.sceneInfo = param;
				// find viewCamera
				var view = null,
					i;
				if (!core.sceneInfo.objectlist) {
					console.erorr('Not find objectlist');
					return;
				}
				view = core.findObject('view').info;
				if (!view) {
					console.erorr('Not find view camera');
				} else {
					core.viewCamera = view;
				}
				infoCallback(param);
			};
		}(hiveCore, infoCallback)));
		
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
						cmd = HiveCommand.renderCamera(w, h, hiveCore.activeCamera);
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
		this.viewCamera = {position: vec3(0, 0, 300), target: vec3(0, 0, 0), up: vec3(0, 1, 0), fov: 60};
		this.sceneInfo = {};
		this.updateSceneCallback = infoCallback;
		this.activeCamera = 'view';
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
		var cmd = '';
		cmd += HiveCommand.cameraLookat(this.activeCamera, this.viewCamera.position, this.viewCamera.target, this.viewCamera.up, this.viewCamera.fov);
		cmd += HiveCommand.renderCamera(parseInt(this.screenSize[0] / 32, 10), parseInt(this.screenSize[1] / 32, 10), this.activeCamera);
		runScript(this.conn, cmd);
	};
	
	//----------------------------------------------------------------------------------------------
	// Loader
	//
	function genModelname(fpath) {
		var fileTypes = fpath.split("/"),
			len = fileTypes.length;
		if (len === 0) { return fpath; }
		return fileTypes[len - 1].replace('.', '_');
	}
	HiveCore.prototype.loadOBJ = function (filepath, shaderpath) {
		var name = genModelname(filepath) + this.modelCount,
			cmd = HiveCommand.loadOBJ(name, filepath, shaderpath);
		this.modelCount = this.modelCount + 1;
		runScript(this.conn, cmd);
	};
	HiveCore.prototype.loadSTL = function (filepath, shaderpath) {
		var name = genModelname(filepath) + this.modelCount,
			cmd = HiveCommand.loadSTL(name, filepath, shaderpath);
		this.modelCount = this.modelCount + 1;
		runScript(this.conn, cmd);
	};
	HiveCore.prototype.loadPDB = function (filepath, shaderpath) {
		var name = genModelname(filepath) + this.modelCount,
			cmd = HiveCommand.loadPDB(name, filepath, shaderpath);
		this.modelCount = this.modelCount + 1;
		runScript(this.conn, cmd);
	};
	HiveCore.prototype.loadSPH = function (filepath, shaderpath) {
		var name = genModelname(filepath) + this.modelCount,
			cmd = HiveCommand.loadSPH(name, filepath, shaderpath);
		this.modelCount = this.modelCount + 1;
		runScript(this.conn, cmd);
	};

	//----------------------------------------------------------------------------------------------
	// Information
	//
	HiveCore.prototype.updateSceneInformation = function () {
		var cmd = HiveCommand.updateSceneInformation();
		runScript(this.conn, cmd);
	};
	
	HiveCore.prototype.findObject = function (objname) {
		var obj = null,
			i;
		if (this.sceneInfo && this.sceneInfo.objectlist) {
			for (i = 0; i < this.sceneInfo.objectlist.length; i = i + 1) {
				if (this.sceneInfo.objectlist[i].name === objname) {
					obj = this.sceneInfo.objectlist[i];
					break;
				}
			}
		}
		return obj;
	};
	
	HiveCore.prototype.getSceneData = function () {
		return this.sceneInfo;
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
		runScript(this.conn, HiveCommand.deleteObject(name));
	};
	
	//----------------------------------------------------------------------------------------------
	// Model operation
	//
	HiveCore.prototype.setModelShader = function (objname, shaderpath) {
		var obj = this.findObject(objname);
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}
		obj.info.shader = shaderpath;
		runScript(this.conn, HiveCommand.setModelShader(objname, shaderpath));
	};
	HiveCore.prototype.setModelTranslate = function (objname, trans, redraw) {
		var obj = this.findObject(objname),
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}
		obj.info.translate[0] = trans[0]; // store
		obj.info.translate[1] = trans[1]; // store
		obj.info.translate[2] = trans[2]; // store
		if (redraw) {
			redrawfunc = (function (core) {
				return function (err, data) {
					return core.render();
				};
			}(this));
		}
		runScript(this.conn, HiveCommand.setModelTranslation(objname, obj.info.translate, obj.info.rotate, obj.info.scale), redrawfunc);
	};
	HiveCore.prototype.setModelRotate = function (objname, rot, redraw) {
		var obj = this.findObject(objname),
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}
		obj.info.rotate[0] = rot[0]; // store
		obj.info.rotate[1] = rot[1]; // store
		obj.info.rotate[2] = rot[2]; // store
		if (redraw) {
			redrawfunc = (function (core) {
				return function (err, data) {
					return core.render();
				};
			}(this));
		}
		runScript(this.conn, HiveCommand.setModelTranslation(objname, obj.info.translate, obj.info.rotate, obj.info.scale), redrawfunc);
	};
	HiveCore.prototype.setModelScale = function (objname, scale, redraw) {
		var obj = this.findObject(objname),
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}
		obj.info.scale[0] = scale[0]; // store
		obj.info.scale[1] = scale[1]; // store
		obj.info.scale[2] = scale[2]; // store
		if (redraw) {
			redrawfunc = (function (core) {
				return function (err, data) {
					return core.render();
				};
			}(this));
		}
		runScript(this.conn, HiveCommand.setModelTranslation(objname, obj.info.translate, obj.info.rotate, obj.info.scale), redrawfunc);
	};

	//----------------------------------------------------------------------------------------------
	// Camera operation
	//
	HiveCore.prototype.setCameraPosition = function (objname, pos, redraw) {
		var obj = this.findObject(objname),
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}
		obj.info.position[0] = pos[0];
		obj.info.position[1] = pos[1];
		obj.info.position[2] = pos[2];
		if (redraw) {
			redrawfunc = (function (core) {
				return function (err, data) {
					return core.render();
				};
			}(this));
		}
		runScript(this.conn, HiveCommand.cameraLookat(objname, obj.info.position, obj.info.target, obj.info.up, obj.info.fov), redrawfunc);
	};
	HiveCore.prototype.setCameraTarget = function (objname, tar, redraw) {
		var obj = this.findObject(objname),
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}
		obj.info.target[0] = tar[0];
		obj.info.target[1] = tar[1];
		obj.info.target[2] = tar[2];
		if (redraw) {
			redrawfunc = (function (core) {
				return function (err, data) {
					return core.render();
				};
			}(this));
		}
		runScript(this.conn, HiveCommand.cameraLookat(objname, obj.info.position, obj.info.target, obj.info.up, obj.info.fov), redrawfunc);
	};
	HiveCore.prototype.setCameraUp = function (objname, up, redraw) {
		var obj = this.findObject(objname),
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}
		obj.info.up[0] = up[0];
		obj.info.up[1] = up[1];
		obj.info.up[2] = up[2];
		if (redraw) {
			redrawfunc = (function (core) {
				return function (err, data) {
					return core.render();
				};
			}(this));
		}
		runScript(this.conn, HiveCommand.cameraLookat(objname, obj.info.position, obj.info.target, obj.info.up, obj.info.fov), redrawfunc);
	};
	HiveCore.prototype.setCameraFov = function (objname, fov, redraw) {
		var obj = this.findObject(objname),
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}
		obj.info.fov = fov;
		if (redraw) {
			redrawfunc = (function (core) {
				return function (err, data) {
					return core.render();
				};
			}(this));
		}
		runScript(this.conn, HiveCommand.cameraLookat(objname, obj.info.position, obj.info.target, obj.info.up, obj.info.fov), redrawfunc);
	};
	
	HiveCore.prototype.getActiveCamera = function () {
		return this.activeCamera;
	};

	//----------------------------------------------------------------------------------------------
	// View operation
	//
	HiveCore.prototype.Rotate = function (rotx, roty) {
		var eyedir = subtract(this.viewCamera.position, this.viewCamera.target),
			v = vec4(eyedir[0], eyedir[1], eyedir[2], 0.0),
			rx = rotate(roty, vec3(0, 1, 0)),
			ry = rotate(rotx, vec3(1, 0, 0));
		v = vec4(dot(ry[0], v), dot(ry[1], v), dot(ry[2], v), 0.0);
		v = vec3(dot(rx[0], v), dot(rx[1], v), dot(rx[2], v));
		this.viewCamera.position = add(this.viewCamera.target, v);
		this.render();
	};
	HiveCore.prototype.Zoom = function (zoom) {
		var v = subtract(this.viewCamera.position, this.viewCamera.target),
			r = 1.0 - (zoom / 1000.0);
		v = scale(r, v);
		console.log(v);
		this.viewCamera.position = add(this.viewCamera.target, v);
		this.render();
	};
	HiveCore.prototype.Translate = function (tx, ty) {
		/*
			TODO: FIX ME
		*/
		var mv = vec3(-tx, ty, 0.0),
			az = normalize(subtract(this.viewCamera.position, this.viewCamera.target)),
			ax = normalize(cross(vec3(0, 1, 0), az)),
			ay = normalize(cross(az, ax)),
			mx = transpose(mat3(ax, ay, az)),
			mm = vec3(vec3(dot(mv, mx[0]), dot(mv, mx[1]), dot(mv, mx[2])));
		//this.viewCamera.position = add(this.viewCamera.position, mm);
		//this.viewCamera.target = add(this.viewCamera.target, mm);
		this.viewCamera.position = add(add(this.viewCamera.position, scale(-tx, ax)), scale(ty, ay));
		this.viewCamera.target   = add(add(this.viewCamera.target,   scale(-tx, ax)), scale(ty, ay));
		
		this.render();
	};
	
	//----------------------------------------------------------------------------------------------
	// Filelist
	//
	HiveCore.prototype.getFileList = function (dir, callback) {
		this.conn.masterMethod('requestFileList', {path: dir}, callback);
	};
	HiveCore.prototype.getShaderList = function (callback) {
		this.conn.masterMethod('requestShaderList', {}, callback);
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