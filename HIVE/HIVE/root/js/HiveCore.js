/*jslint devel:true*/
/*global Blob, URL*/
/*global HiveConnect*/  // HiveConnect.js
/*global HiveCommand*/  // HiveCommand.js
/*global vec3, vec4, mat4, subtract, add, scale, rotate, cross, translate, dot, normalize, transpose, mat3, lerp */ // MV.js

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
				var activecam = null,
					i;
				if (!core.sceneInfo.objectlist) {
					console.erorr('Not find objectlist');
					return;
				}
				if (!core.sceneInfo.objecttimeline) {
					console.erorr('Not find objecttimeline');
					return;
				}
				if (Array.isArray(core.sceneInfo.objecttimeline)) { // if empty array, it is empty object.
					core.sceneInfo.objecttimeline = {};
				}

				activecam = core.findObject(core.activeCamera);
				if (!activecam) { // fall back
					activecam = core.findObject('view');
					core.activeCamera = 'view';
				}
				if (!activecam) {
					console.erorr('Not find active camera');
				} else {
					core.viewCamera = activecam.info;
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
						cmd = HiveCommand.renderCamera(w, h, hiveCore.activeCamera, true);
					runScript(hiveCore.conn, cmd);
				}
			}
		});
	}
		
	function lerpInfo(preInfo, postInfo, time) {
		var result = null,
			i,
			s,
			prop,
			val,
			lerpProps = [
				"position",
				"target",
				"up",
				"fov",
				"clearcolor",
				"scale",
				"rotate",
				"translate"
			];
		if (!preInfo && !postInfo) {
			return result;
		}
		if (!preInfo) {
			result = postInfo.info;
		} else if (!postInfo) {
			result = preInfo.info;
		} else {
			s = (time - preInfo.time) / (postInfo.time - preInfo.time);
			result = JSON.parse(JSON.stringify(preInfo.info));
			for (i = 0; i < lerpProps.length; i = i + 1) {
				prop = lerpProps[i];
				if (preInfo.info.hasOwnProperty(prop)) {
					val = lerp(preInfo.info[prop], postInfo.info[prop], s);
					result[prop] = val;
				}
			}
		}
		return result;
	}
	
	function craeteLoadModelCommand(filepath, modelName, shader) {
		var name = modelName,
			cmd,
			shaderpath = './shader/polygon.frag';
		if (shader) {
			shaderpath = shader;
		}
		console.log("create load model coomand:" + filepath);
		if (filepath.substr(filepath.length - 4) === ".obj") {
			cmd = HiveCommand.loadOBJ(name, filepath, shaderpath, false);
		} else if (filepath.substr(filepath.length - 4) === ".stl") {
			cmd = HiveCommand.loadSTL(name, filepath, shaderpath, false);
		} else if (filepath.substr(filepath.length - 4) === ".pdb") {
			cmd = HiveCommand.loadPDB(name, filepath, shaderpath, false);
		} else if (filepath.substr(filepath.length - 4) === ".sph") {
			cmd = HiveCommand.loadSPH(name, filepath, shaderpath, false);
		} else {
			console.error('Not supported file type:', filepath);
		}
		return cmd;
	}

	//----------------------------------------------------------------------------------------------
	// Contructor
	//
	var HiveCore = function (resultElement, width, height, infoCallback) {
		this.conn = new HiveConnect();
		this.resize(width, height);
		this.modelCount = 0;
		this.sceneInfo = {};
		this.updateSceneCallback = infoCallback;
		this.activeCamera = 'view';
		this.viewCamera = {position: vec3(0, 0, 300), target: vec3(0, 0, 0), up: vec3(0, 1, 0), fov: 60}; // default
		this.sceneInfo.objecttimeline = {};
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
		cmd += HiveCommand.renderCamera(parseInt(this.screenSize[0] / 32, 10), parseInt(this.screenSize[1] / 32, 10), this.activeCamera, true);
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
			cmd = HiveCommand.loadOBJ(name, filepath, shaderpath, true);
		this.modelCount = this.modelCount + 1;
		runScript(this.conn, cmd);
	};
	HiveCore.prototype.loadSTL = function (filepath, shaderpath) {
		var name = genModelname(filepath) + this.modelCount,
			cmd = HiveCommand.loadSTL(name, filepath, shaderpath, true);
		this.modelCount = this.modelCount + 1;
		runScript(this.conn, cmd);
	};
	HiveCore.prototype.loadPDB = function (filepath, shaderpath) {
		var name = genModelname(filepath) + this.modelCount,
			cmd = HiveCommand.loadPDB(name, filepath, shaderpath, true);
		this.modelCount = this.modelCount + 1;
		runScript(this.conn, cmd);
	};
	HiveCore.prototype.loadSPH = function (filepath, shaderpath) {
		var name = genModelname(filepath) + this.modelCount,
			cmd = HiveCommand.loadSPH(name, filepath, shaderpath, true);
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
	
	HiveCore.prototype.saveScene = function (filepath) {
		var cmd = '',
			jsonstr;
		if (this.sceneInfo) {
			this.conn.masterMethod('saveScene', {path : filepath, data : this.sceneInfo}, function (err, res, id) {
				if (err) {
					console.log(err);
				}
			});
		}
	};
	
	HiveCore.prototype.createSceneCommand = function (sceneInfo) {
		var sceneInfo = this.sceneInfo,
			cmd = HiveCommand.newScene(),
			i,
			r,
			g,
			b,
			a,
			width,
			height,
			obj,
			hasProp = function (target, prop) {
				return target.hasOwnProperty(prop);
			},
			modelCount = 0;
		
		if (!sceneInfo) {
			return;
		}
		if (!sceneInfo.objectlist) {
			return;
		}
		
		for (i = 0; i < sceneInfo.objectlist.length; i = i + 1) {
			obj = sceneInfo.objectlist[i];
			if (hasProp(obj, 'type') && hasProp(obj, 'info') && hasProp(obj, 'name')) {
				if (obj.type === 'CAMERA') {
					cmd = cmd + HiveCommand.createCamera(obj.name) + "\n";
					modelCount = modelCount + 1;
					if (hasProp(obj.info, 'position') && hasProp(obj.info, 'target') && hasProp(obj.info, 'up') && hasProp(obj.info, 'fov')) {
						cmd = cmd + HiveCommand.cameraLookat(obj.name, obj.info.position, obj.info.target, obj.info.up, obj.info.fov) + "\n";
					}
					if (hasProp(obj.info, 'clearcolor')) {
						r = Math.min(Math.max(obj.info.clearcolor[0], 0.0), 1.0);
						g = Math.min(Math.max(obj.info.clearcolor[1], 0.0), 1.0);
						b = Math.min(Math.max(obj.info.clearcolor[2], 0.0), 1.0);
						a = Math.min(Math.max(obj.info.clearcolor[3], 0.0), 1.0);
						cmd = cmd + HiveCommand.cameraClearColor(obj.name, r, g, b, a) + "\n";
					}
					if (hasProp(obj.info, 'screensize')) {
						width = obj.info.screensize[0];
						height = obj.info.screensize[1];
						cmd = cmd + HiveCommand.cameraScreenSize(obj.name, width, height) + "\n";
					}
					if (hasProp(obj.info, 'outputfile')) {
						cmd = cmd + HiveCommand.cameraFilename(obj.name, obj.info.outputfile) + "\n";
					}
				}
			}
		}
		for (i = 0; i < sceneInfo.objectlist.length; i = i + 1) {
			obj = sceneInfo.objectlist[i];
			if (hasProp(obj, 'type') && hasProp(obj, 'info') && hasProp(obj, 'name')) {
				if (obj.type === 'POLYGON') {
					if (hasProp(obj.info, 'filename')) {
						cmd = cmd + craeteLoadModelCommand(obj.info.filename, obj.name, obj.info.shader) + "\n";
						modelCount = modelCount + 1;
						if (hasProp(obj.info, 'translate') && hasProp(obj.info, 'rotate') && hasProp(obj.info, 'scale')) {
							cmd = cmd + HiveCommand.setModelTranslation(obj.name, obj.info.translate, obj.info.rotate, obj.info.scale) + "\n";
						}
						if (hasProp(obj.info, 'shader')) {
							cmd = cmd + HiveCommand.setModelShader(obj.name, obj.info.shader) + "\n";
						}
						cmd = cmd + this.createModelUniformCommand(obj.name, obj.info);
					}
				}
			}
		}
		if (hasProp(sceneInfo, 'objecttimeline')) {
			cmd = cmd + HiveCommand.storeObjectTimeline(sceneInfo.objecttimeline) + "\n";
		}
		return { command : cmd, modelcount : modelCount };
	};
	
	HiveCore.prototype.exportScene = function (filepath) {
		var cmd = '',
			jsonstr;
		if (this.sceneInfo) {
			cmd = this.createSceneCommand(this.sceneInfo);
			if (cmd && cmd.command) {
				console.log("prototype.exportScene");
				this.conn.masterMethod('exportScene', {path : filepath, data : cmd.command}, function (err, res, id) {
					if (err) {
						console.log(err);
					}
				});
			}
		}
	};
	
	HiveCore.prototype.reloadScene = function (callback) {
		var cmd = this.createSceneCommand(this.sceneInfo);
		if (cmd && cmd.command) {
			this.modelCount = cmd.modelcount;
			
			runScript(this.conn, cmd.command, function () {
				callback();
			});
		}
	};
	
	HiveCore.prototype.loadScene = function (filepath, callback) {
		this.conn.masterMethod('loadScene', {path: filepath}, (function (core) {
			return function (err, res, id) {
				if (err) {
					console.log(err);
					return;
				}
				core.sceneInfo = res;
				core.reloadScene(function () {
					core.updateSceneInformation();
					if (callback) {
						callback();
					}
				});
			};
		}(this)));
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
	
	HiveCore.prototype.storeObjectTimeline = function () {
		runScript(this.conn, HiveCommand.storeObjectTimeline(this.sceneInfo.objecttimeline));
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
	
	HiveCore.prototype.setModelTranRotScale = function (objname, trans, rot, scale, redraw) {
		var obj = this.findObject(objname),
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}
		if (obj.type === "CAMERA") {
			return;
		}
		obj.info.translate[0] = trans[0]; // store
		obj.info.translate[1] = trans[1]; // store
		obj.info.translate[2] = trans[2]; // store
		obj.info.rotate[0] = rot[0]; // store
		obj.info.rotate[1] = rot[1]; // store
		obj.info.rotate[2] = rot[2]; // store
		obj.info.scale[0] = scale[0]; // store
		obj.info.scale[1] = scale[1]; // store
		obj.info.scale[2] = scale[2]; // store
		console.log('setModelTranRotScale', trans, rot, scale);

		if (redraw) {
			redrawfunc = (function (core) {
				return function (err, data) {
					return core.render();
				};
			}(this));
		}
		runScript(this.conn, HiveCommand.setModelTranslation(objname, obj.info.translate, obj.info.rotate, obj.info.scale), redrawfunc);
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

	HiveCore.prototype.setModelUniforms = function (objname, uniforms, redraw) {
		var i,
			obj = this.findObject(objname),
			name,
			src = '',
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}

		for (i = 0; i < uniforms.length; i = i + 1) {
			//console.log(uniforms[i].name, uniforms[i].uniform, uniforms[i].val);
			name = uniforms[i].name;
			if (uniforms[i].uniform === 'vec4') {
				obj.info.vec4[name] = [uniforms[i].val[0], uniforms[i].val[1], uniforms[i].val[2], uniforms[i].val[3]];
				src += HiveCommand.setModelUniformVec4(objname, name, uniforms[i].val);
			} else if (uniforms[i].uniform === 'vec3') {
				obj.info.vec3[name] = [uniforms[i].val[0], uniforms[i].val[1], uniforms[i].val[2] ];
				src += HiveCommand.setModelUniformVec3(objname, name, uniforms[i].val);
			} else if (uniforms[i].uniform === 'vec2') {
				obj.info.vec2[name] = [uniforms[i].val[0], uniforms[i].val[1] ];
				src += HiveCommand.setModelUniformVec2(objname, name, uniforms[i].val);
			} else if (uniforms[i].uniform === 'float') {
				obj.info.float[name] = uniforms[i].val;
				src += HiveCommand.setModelUniformFloat(objname, name, uniforms[i].val);
			}
		}
		if (redraw) {
			redrawfunc = (function (core) {
				return function (err, data) {
					return core.render();
				};
			}(this));
		}
		runScript(this.conn, src, redrawfunc);
	};
	
	HiveCore.prototype.createModelUniformCommand = function (objname, objinfo) {
		var name,
			uniforms,
			src = '';
		
		uniforms = objinfo.vec4;
		for (name in uniforms) {
			if (uniforms.hasOwnProperty(name)) {
				src += HiveCommand.setModelUniformVec4(objname, name, uniforms[name]);
			}
		}
		uniforms = objinfo.vec3;
		for (name in uniforms) {
			if (uniforms.hasOwnProperty(name)) {
				src += HiveCommand.setModelUniformVec3(objname, name, uniforms[name]);
			}
		}
		uniforms = objinfo.vec2;
		for (name in uniforms) {
			if (uniforms.hasOwnProperty(name)) {
				src += HiveCommand.setModelUniformVec2(objname, name, uniforms[name]);
			}
		}
		uniforms = objinfo.float;
		for (name in uniforms) {
			if (uniforms.hasOwnProperty(i)) {
				name = i;
				src += HiveCommand.setModelUniformFloat(objname, name, uniforms[name]);
			}
		}
		return src;
	};

	HiveCore.prototype.setModelUniformsFromInfo = function (objname, objinfo, redraw) {
		var i,
			obj = this.findObject(objname),
			name,
			src = '',
			redrawfunc = null,
			uniforms;
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}

		uniforms = objinfo.vec4;
		for (i in uniforms) {
			if (uniforms.hasOwnProperty(i)) {
				name = i;
				obj.info.vec4[name] = [uniforms[i][0], uniforms[i][1], uniforms[i][2], uniforms[i][3]];
				console.log(objname, name, uniforms[i]);
				src += HiveCommand.setModelUniformVec4(objname, name, uniforms[i]);
			}
		}
		uniforms = objinfo.vec3;
		for (i in uniforms) {
			if (uniforms.hasOwnProperty(i)) {
				name = i;
				obj.info.vec3[name] = [uniforms[i][0], uniforms[i][1], uniforms[i][2]];
				src += HiveCommand.setModelUniformVec3(objname, name, uniforms[i]);
			}
		}
		uniforms = objinfo.vec2;
		for (i in uniforms) {
			if (uniforms.hasOwnProperty(i)) {
				name = i;
				obj.info.vec2[name] = [uniforms[i][0], uniforms[i][1]];
				src += HiveCommand.setModelUniformVec2(objname, name, uniforms[i]);
			}
		}
		uniforms = objinfo.float;
		for (i in uniforms) {
			if (uniforms.hasOwnProperty(i)) {
				name = i;
				obj.info.float[name] = uniforms[i];
				src += HiveCommand.setModelUniformFloat(objname, name, uniforms[i]);
			}
		}

		if (redraw) {
			redrawfunc = (function (core) {
				return function (err, data) {
					return core.render();
				};
			}(this));
		}
		runScript(this.conn, src, redrawfunc);
	};
	
	
	
	HiveCore.prototype.setModelVec4 = function (objname, vname, x, y, z, w, redraw) {
		var i,
			obj = this.findObject(objname),
			src = '',
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}
		obj.info.vec4[vname][0] = x;
		obj.info.vec4[vname][1] = y;
		obj.info.vec4[vname][2] = z;
		obj.info.vec4[vname][3] = w;
		src = HiveCommand.setModelUniformVec4(objname, vname, obj.info.vec4[vname]);
		if (redraw) {
			redrawfunc = (function (core) {
				return function (err, data) {
					return core.render();
				};
			}(this));
		}
		runScript(this.conn, src, redrawfunc);
	};
	
	HiveCore.prototype.setModelVec3 = function (objname, vname, x, y, z, redraw) {
		var i,
			obj = this.findObject(objname),
			src = '',
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}
		obj.info.vec3[vname][0] = x;
		obj.info.vec3[vname][1] = y;
		obj.info.vec3[vname][2] = z;
		src = HiveCommand.setModelUniformVec3(objname, vname, obj.info.vec3[vname]);
		if (redraw) {
			redrawfunc = (function (core) {
				return function (err, data) {
					return core.render();
				};
			}(this));
		}
		runScript(this.conn, src, redrawfunc);
	};
	
	HiveCore.prototype.setModelVec2 = function (objname, vname, x, y, redraw) {
		var i,
			obj = this.findObject(objname),
			src = '',
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}
		obj.info.vec2[vname][0] = x;
		obj.info.vec2[vname][1] = y;
		src = HiveCommand.setModelUniformVec2(objname, vname, obj.info.vec2[vname]);
		if (redraw) {
			redrawfunc = (function (core) {
				return function (err, data) {
					return core.render();
				};
			}(this));
		}
		runScript(this.conn, src, redrawfunc);
	};
	
	HiveCore.prototype.setModelFloat = function (objname, vname, val, redraw) {
		var i,
			obj = this.findObject(objname),
			src = '',
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}
		obj.info.float[vname] = val;
		src = HiveCommand.setModelUniformFloat(objname, vname, obj.info.float[vname]);
		if (redraw) {
			redrawfunc = (function (core) {
				return function (err, data) {
					return core.render();
				};
			}(this));
		}
		runScript(this.conn, src, redrawfunc);
	};
	
	//----------------------------------------------------------------------------------------------
	// Camera operation
	//
	HiveCore.prototype.setCameraParameters = function (objname, pos, tar, up, fov, redraw) {
		var obj = this.findObject(objname),
			redrawfunc = null,
			src = '';
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}
		if (obj.type !== 'CAMERA') {
			return;
		}
		console.log('setCameraParameters', pos, tar, up, fov);
		obj.info.position[0] = pos[0];
		obj.info.position[1] = pos[1];
		obj.info.position[2] = pos[2];
		obj.info.target[0] = tar[0];
		obj.info.target[1] = tar[1];
		obj.info.target[2] = tar[2];
		obj.info.up[0] = up[0];
		obj.info.up[1] = up[1];
		obj.info.up[2] = up[2];
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
	HiveCore.prototype.setCameraScreenSize = function (cameraname, width, height) {
		var obj = this.findObject(cameraname),
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', cameraname);
			return;
		}
		obj.info.screensize[0] = width;
		obj.info.screensize[1] = height;
		runScript(this.conn, HiveCommand.cameraScreenSize(cameraname, width, height));
	};
	HiveCore.prototype.setOutputFilename = function (cameraname, filename) {
		var obj = this.findObject(cameraname),
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', cameraname);
			return;
		}
		obj.info.outputfile = filename;
		runScript(this.conn, HiveCommand.cameraFilename(cameraname, obj.info.filename));
	};
	HiveCore.prototype.setClearColor = function (cameraname, red, green, blue, alpha, redraw) {
		var obj = this.findObject(cameraname),
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', cameraname);
			return;
		}
		red = Math.min(1.0, red);
		green = Math.min(1.0, green);
		blue  = Math.min(1.0, blue);
		alpha = Math.min(1.0, green);
		obj.info.clearcolor[0] = red;
		obj.info.clearcolor[1] = green;
		obj.info.clearcolor[2] = blue;
		obj.info.clearcolor[3] = alpha;
		
		if (redraw) {
			redrawfunc = (function (core) {
				return function (err, data) {
					return core.render();
				};
			}(this));
		}
		runScript(this.conn, HiveCommand.cameraClearColor(cameraname, red, green, blue, alpha), redrawfunc);
	};

	HiveCore.prototype.setActiveCamera = function (cameraname) {
		var obj = this.findObject(cameraname),
			redrawfunc = null;
		if (!obj) {
			console.error('[Error] Not found object:', cameraname);
			return;
		}
		//console.log('ACTIVE CAMERA = ', cameraname);
		this.activeCamera = cameraname;
		this.viewCamera   = obj.info;
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
	// Timeline animation
	//
	
	HiveCore.prototype.addKey = function (objname, tm) {
		var obj = this.findObject(objname),
			cinfo,
			tinfo;
		if (!obj) {
			console.error('[Error] Not found object:', objname);
			return;
		}
		console.log(obj);
		
		// copy propteies
		cinfo = JSON.parse(JSON.stringify(obj.info));
		tinfo = this.sceneInfo.objecttimeline[objname];
		if (tinfo === undefined) {
			tinfo = [];
			this.sceneInfo.objecttimeline[objname] = tinfo;
		}
		tinfo.push({info: cinfo, time: tm});
		tinfo.sort(function (a, b) { return a.time > b.time; });
		console.log(tinfo);
		
		// server store
		this.storeObjectTimeline();
	};
	
	HiveCore.prototype.updateTime = function (tm) {
		if (!this.sceneInfo) { return; }
		if (!this.sceneInfo.objectlist) { return; }
		var n = this.sceneInfo.objectlist.length,
			i,
			j,
			m,
			objname,
			objtype,
			tinfo,
			infoptr,
			preInfo,
			postInfo;
		for (i = 0; i < n; i = i + 1) {
			objname = this.sceneInfo.objectlist[i].name;
			objtype = this.sceneInfo.objectlist[i].type;
			tinfo = this.sceneInfo.objecttimeline[objname];
			infoptr = null;
			preInfo = null;
			postInfo = null;
			if (tinfo !== undefined) {
				m = tinfo.length;
				for (j = 0; j < m; j = j + 1) {
					if (tinfo[j].time < tm) {
						preInfo = tinfo[j];
					} else {
						postInfo = tinfo[j];
						break;
					}
				}
				//console.log("pre", this.sceneInfo.objectlist[i]);
				//console.log("post", postInfo);
				infoptr = lerpInfo(preInfo, postInfo, tm);
				if (infoptr) {
					this.setModelUniformsFromInfo(objname, infoptr);
					this.setModelTranRotScale(objname, infoptr.translate, infoptr.rotate, infoptr.scale);
					if (objtype === "CAMERA") {
						this.setClearColor(objname, infoptr.clearcolor[0], infoptr.clearcolor[1], infoptr.clearcolor[2]);
					}
					this.setCameraParameters(objname, infoptr.position, infoptr.target, infoptr.up, infoptr.fov);
				}
			}
		}
		this.render();
	};
	
	HiveCore.prototype.getTimeline = function () {
		return this.sceneInfo.objecttimeline;
	};
	
	window.HiveCore = HiveCore;
}(window));