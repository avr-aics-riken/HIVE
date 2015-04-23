/*jslint devel:true*/
/*global HiveCore*/                              // HiveCore
/*global kvtoolsUI_init, kvtoolsUI_update, kUI*/ // kvtoolsUI
/*global $, $toggle*/                            // qatrix
/*global FileDialog*/                            // Filedlg

(function (window) {
	'use strict';
	function showFiledialog(show) {
		
	}
	function setPropertyMode(type) {
		if (type === 'CAMERA') {
			$('object-transformshader').style.display = 'none';
			$('camera-information').style.display = '';
			$('uniformproperty').style.display = 'none';
		} else if (type === 'NONE') {
			$('object-transformshader').style.display = 'none';
			$('camera-information').style.display = 'none';
			$('uniformproperty').style.display = 'none';
		} else {
			$('object-transformshader').style.display = '';
			$('camera-information').style.display = 'none';
			$('uniformproperty').style.display = '';
		}
	}
	function clearTextChangeEvent() {
		$('objname').onchange     = function () {};
		$('translate_x').onchange = function () {};
		$('translate_y').onchange = function () {};
		$('translate_z').onchange = function () {};
		$('rotate_x').onchange    = function () {};
		$('rotate_y').onchange    = function () {};
		$('rotate_z').onchange    = function () {};
		$('scale_x').onchange     = function () {};
		$('scale_y').onchange     = function () {};
		$('scale_z').onchange     = function () {};
		$('camera_name').onchange = function () {};
		$('position_x').onchange  = function () {};
		$('position_y').onchange  = function () {};
		$('position_z').onchange  = function () {};
		$('target_x').onchange    = function () {};
		$('target_y').onchange    = function () {};
		$('target_z').onchange    = function () {};
		$('up_x').onchange        = function () {};
		$('up_y').onchange        = function () {};
		$('up_z').onchange        = function () {};
		$('camera_fov').onchange  = function () {};
		$('camera_screen_width').onchange  = function () {};
		$('camera_screen_height').onchange  = function () {};
		$('camera_output_filename').onchange  = function () {};
	}
	function setCameraProperty(name, objprop, core) {
		clearTextChangeEvent();
		$('camera_name').value = name;
		$('position_x').value = objprop.position[0];
		$('position_y').value = objprop.position[1];
		$('position_z').value = objprop.position[2];
		$('target_x').value = objprop.target[0];
		$('target_y').value = objprop.target[1];
		$('target_z').value = objprop.target[2];
		$('up_x').value = objprop.up[0];
		$('up_y').value = objprop.up[1];
		$('up_z').value = objprop.up[2];
		$('camera_fov').value = objprop.fov;
		$('camera_screen_width').value = objprop.screensize[0];
		$('camera_screen_height').value = objprop.screensize[1];
		$('camera_output_filename').value = objprop.outputfile;
		kUI('camera_clearcolor').setColor(objprop.clearcolor[0],
										  objprop.clearcolor[1],
										  objprop.clearcolor[2],
										  objprop.clearcolor[3]);
		
		function valueChangePosition(name, pos, i, core) {
			return function (ev) {
				pos[i] = parseFloat(ev.target.value);
				core.setCameraPosition(name, pos, true);
			};
		}
		function valueChangeTarget(name, tar, i, core) {
			return function (ev) {
				tar[i] = parseFloat(ev.target.value);
				core.setCameraTarget(name, tar, true);
			};
		}
		function valueChangeUp(name, up, i, core) {
			return function (ev) {
				up[i] = parseFloat(ev.target.value);
				core.setCameraUp(name, up, true);
			};
		}
		function valueChangeFov(name, core) {
			return function (ev) {
				var fov = parseFloat(ev.target.value);
				core.setCameraFov(name, fov, true);
			};
		}
		function valueChangeScreenSize(name, v, i, core) {
			return function (ev) {
				var s = parseFloat(ev.target.value);
				v[i] = s;
				core.setCameraScreenSize(name, v[0], v[1]);
			};
		}
		function valueChangeOutputFiel(name, core) {
			return function (ev) {
				var fname = ev.target.value;
				core.setOutputFilename(name, fname);
			};
		}

		$('position_x').onchange = valueChangePosition(name, objprop.position, 0, core);
		$('position_y').onchange = valueChangePosition(name, objprop.position, 1, core);
		$('position_z').onchange = valueChangePosition(name, objprop.position, 2, core);
		$('target_x').onchange   = valueChangeTarget(name, objprop.target, 0, core);
		$('target_y').onchange   = valueChangeTarget(name, objprop.target, 1, core);
		$('target_z').onchange   = valueChangeTarget(name, objprop.target, 2, core);
		$('up_x').onchange       = valueChangeUp(name, objprop.up, 0, core);
		$('up_y').onchange       = valueChangeUp(name, objprop.up, 1, core);
		$('up_z').onchange       = valueChangeUp(name, objprop.up, 2, core);
		$('camera_fov').onchange = valueChangeFov(name, core);
		$('camera_screen_width').onchange = valueChangeScreenSize(name, objprop.screensize, 0, core);
		$('camera_screen_height').onchange = valueChangeScreenSize(name, objprop.screensize, 1, core);
		$('camera_output_filename').onchange = valueChangeOutputFiel(name, core);
		kUI('camera_clearcolor').ChangeColorCallback((function (core, name) {
			return function (r, g, b, a) {
				//core.setModelVec4(objname, paramname, r, g, b, a, true);
				core.setClearColor(name, r, g, b, a, true);
			};
		}(core, name)));
	}
	
	function splitfilename(fpath) {
		var fileTypes = fpath.split("/"),
			len = fileTypes.length;
		if (len === 0) { return fpath; }
		return fileTypes[len - 1];
	}
	function updateShaderParameters(core, objname, uniforms) {
		// Uniforms
		var unif = uniforms,
			paramname,
			d,
			i,
			pp = document.getElementById('uniformproperty'),
			objinfo = core.findObject(objname).info,
			colpick;
		
		function changeObjVec4(core, objname, paramname) {
			return function (r, g, b, a) {
				core.setModelVec4(objname, paramname, r, g, b, a, true);
			};
		}
		function changeModelFloat(core, objname, paramname) {
			return function (v) {
				core.setModelFloat(objname, paramname, v, true);
			};
		}

		pp.innerHTML = ''; // clear
		// Add UIs
		for (i in unif) {
			if (unif.hasOwnProperty(i)) {
				if (unif[i].ui === 'colorpicker') {
					paramname = unif[i].name;
					d = document.createElement('div');
					d.classList.add('ppTable');
					d.innerHTML = '<div class="ppRow">' +
								'<div class="ppCell ppTri"><div class="KCaption">' + unif[i].label + ':</div></div>' +
								'<div class="ppCell"><div class="KColorPicker" id="' + paramname + '"></div></div>' +
								'</div>';
					pp.appendChild(d);
					kvtoolsUI_update(d);
					colpick = kUI(paramname);
					if (objinfo.vec4.hasOwnProperty(paramname)) {
						colpick.setColor(objinfo.vec4[paramname][0],
										objinfo.vec4[paramname][1],
										objinfo.vec4[paramname][2],
										objinfo.vec4[paramname][3]);
						colpick.ChangeColorCallback(changeObjVec4(core, objname, paramname));
					}
					
				} else if (unif[i].ui === 'slider') {
					paramname = unif[i].name;
					d = document.createElement('div');
					d.classList.add('ppTable');
					d.innerHTML = '<div class="ppRow"><div class="ppCell ppTri">' +
						'<div class="KCaption">' + unif[i].label + ':</div></div>' +
						'<div class="ppCell"><div class="KSlider" id="' + paramname + '"></div>' +
						'</div></div>';
					pp.appendChild(d);
					kvtoolsUI_update(d);
					kUI(paramname).ChangeCallback(changeModelFloat(core, objname, paramname));

				} else if (unif[i].ui === 'vec3') {
					paramname = unif[i].name;
					d = document.createElement('div');
					d.classList.add('ppTable');
					d.innerHTML = '<div class="ppRow">' +
						'<div class="ppCell ppTri"><div class="KCaption">' + unif[i].label + ':</div></div>' +
						'<div class="ppCell"><input class="KInput ppFull" id="' + paramname + '_x" type="number"></div>' +
						'<div class="ppCell"><input class="KInput ppFull" id="' + paramname + '_y" type="number"></div>' +
						'<div class="ppCell"><input class="KInput ppFull" id="' + paramname + '_z" type="number"></div>' +
						'</div>';
					pp.appendChild(d);
					kvtoolsUI_update(d);
					
				} else if (unif[i].ui === 'transferfunction') {
					paramname = unif[i].name;
					d = document.createElement('div');
					d.classList.add('ppTable');
					d.innerHTML = '<div class="ppRow"><div class="ppCell ppTri"><div class="KCaption">' + unif[i].label + ':</div></div></div>';
					pp.appendChild(d);

					d = document.createElement('div');
					d.classList.add('ppTable');
					d.innerHTML = '<div class="ppRow">' +
						'<div class="ppCell" style="width:8px"></div>' +
						'<div class="ppCell">' +
						'<div class="KTransferFunction" id="' + paramname + '"></div>' +
						'</div></div>';
					pp.appendChild(d);
					kvtoolsUI_update(d);
					
				} else {
					console.log('Error: Unkown UI type -> ' + unif[i].ui + ' - ' + unif[i].name);
				}
			}
		}
		//kvtoolsUI_update(pp);
	}
	function changeShader(shaderpath, info, objectname, core) {
		return function (ev) {
			console.log('[DEBUG] CHANGE SHADER', objectname, shaderpath);
			kUI('shader_name').Select(splitfilename(shaderpath));
			
			updateShaderParameters(core, objectname, info.uniforms);
			core.setModelUniforms(objectname, info.uniforms);
			core.setModelShader(objectname, shaderpath);
			core.render();
		};
	}
	function setObjectProperty(name, objprop, core) {
		clearTextChangeEvent();
		$('objname').value     = name;
		$('translate_x').value = objprop.translate[0];
		$('translate_y').value = objprop.translate[1];
		$('translate_z').value = objprop.translate[2];
		$('rotate_x').value    = objprop.rotate[0];
		$('rotate_y').value    = objprop.rotate[1];
		$('rotate_z').value    = objprop.rotate[2];
		$('scale_x').value     = objprop.scale[0];
		$('scale_y').value     = objprop.scale[1];
		$('scale_z').value     = objprop.scale[2];
		function valueChangeTranslate(name, trans, i, core) {
			return function (ev) {
				trans[i] = parseFloat(ev.target.value);
				core.setModelTranslate(name, trans, true);
			};
		}
		function valueChangeRotate(name, rot, i, core) {
			return function (ev) {
				rot[i] = parseFloat(ev.target.value);
				core.setModelRotate(name, rot, true);
			};
		}
		function valueChangeScale(name, scale, i, core) {
			return function (ev) {
				scale[i] = parseFloat(ev.target.value);
				core.setModelScale(name, scale, true);
			};
		}

		$('translate_x').onchange = valueChangeTranslate(name, objprop.translate, 0, core);
		$('translate_y').onchange = valueChangeTranslate(name, objprop.translate, 1, core);
		$('translate_z').onchange = valueChangeTranslate(name, objprop.translate, 2, core);
		$('rotate_x').onchange    = valueChangeRotate(name, objprop.rotate, 0, core);
		$('rotate_y').onchange    = valueChangeRotate(name, objprop.rotate, 1, core);
		$('rotate_z').onchange    = valueChangeRotate(name, objprop.rotate, 2, core);
		$('scale_x').onchange     = valueChangeScale(name, objprop.scale, 0, core);
		$('scale_y').onchange     = valueChangeScale(name, objprop.scale, 1, core);
		$('scale_z').onchange     = valueChangeScale(name, objprop.scale, 2, core);
								  
		var shadername = objprop.shader,
			targetShader = null,
			i;
		core.getShaderList((function (core, name) {
			return function (err, res) {
				console.log('SHADER_LIST', err, res);
				var shaderList = res,
					idname,
					obj = core.findObject(name),
					selshader,
					i,
					lst = kUI('shader_name'),
					targetShader = null;
				
				for (i = 0; i < shaderList.length; i = i + 1) {
					if (obj.type === shaderList[i].info.type) {
						if (obj.info.shader === shaderList[i].path) {
							targetShader = shaderList[i];
						}
						idname = shaderList[i].path.replace('.', '_');
						idname = 'SHADERITEM_' + idname;
						lst.AddItem(shaderList[i].name, shaderList[i].path.replace('.frag', '.jpg'), idname);
						$(idname).addEventListener('click', changeShader(shaderList[i].path, shaderList[i].info, name, core));
					}
				}
				
				if (targetShader) {
					updateShaderParameters(core, name, targetShader.info.uniforms);
				}
				selshader = splitfilename(obj.info.shader);
				kUI('shader_name').Select(selshader);
				
				
			};
		}(core, name)));

		kUI('shader_name').Clear();
/*		for (i in shaderList) {
			if (shaderList[i].param.type !== dt[name].modeltype) {
				continue;
			}
			if (shaderList[i].file == shadername)
				targetShader = shaderList[i];
			
			var idname = shaderList[i].file.replace('.','_');
			idname = 'SHADERITEM_' + idname;
			$('shader_name').AddItem(shaderList[i].file, shaderList[i].image, idname);
		
			var it = document.getElementById(idname),
				slist = document.getElementById('shader_name');
			it.onclick = function (filename,it,slist) { return function() {
				$('shader_name').Select(filename);
			}}(shaderList[i].file,it,slist);
		}
		$('shader_name').Select(dt[name].shader); // add shader parameters after SELECT.
		*/
	}
	
	var activeObjectName = '',
		activeTime = 0;
	
	function updateProperty(core, objname) {
		var scenedata = core.getSceneData(),
			objprop = null,
			i;
		if (!scenedata || !scenedata.objectlist) {
			console.error('Not found scene data:');
		}
		for (i = 0; i < scenedata.objectlist.length; i = i + 1) {
			if (scenedata.objectlist[i].name === objname) {
				objprop = scenedata.objectlist[i];
				break;
			}
		}
		if (!objprop) {
			console.error('Not found object:', objname, scenedata.objectlist);
		} else {
			activeObjectName = objname;
			setPropertyMode(objprop.type);
			if (objprop.type === "CAMERA") {
				setCameraProperty(objname, objprop.info, core);
				//core.setActiveCamera(objname);
				//kUI('viewmode').SetCaption('[' + objname + ']');
				core.render();
			} else {
				setObjectProperty(objname, objprop.info, core);
			}
		}
	}
	
	function objectClick(core, objname) {
		return function () {
			updateProperty(core, objname);
		};
	}
	function dustClick(core, objname) {
		return function (e) {
			e.stopPropagation();
			core.deleteObject(objname);
			
			activeObjectName = '';
			setPropertyMode('NONE');
		};
	}
	function getIconColor(type) {
		if (type === 'CAMERA') {
			return "#ff9d00";
		} else if (type === "POLYGON") {
			return "#0F0";
		} else if (type === "VOLUME") {
			return "#F00";
		} else if (type === "POINT") {
			return "#00F";
		} else if (type === "LINE") {
			return "#0FF";
		} else if (type === "TETRA") {
			return "#FF0";
		} else if (type === "VECTOR") {
			return "#F0F";
		} else {
			return "#FFF";
		}
	}
	
	function init() {
		kvtoolsUI_init();
		var mouseState = {"Left": false, "Center": false, "Right": false, "x": 0, "y": 0 },
			core;
		
		core = new HiveCore($('result'),
							document.getElementById('window-view').clientWidth,
							document.getElementById('window-view').clientHeight,
							updateObjList);
	
		// Initial Resize
		// Resize Event
		function canvasResizing() {
			var w = document.getElementById('window-view').clientWidth,
				h = document.getElementById('window-view').clientHeight,
				rc = document.getElementById('result');
			rc.width        = w;
			rc.height       = h;
			rc.style.width  = w + 'px';
			rc.style.height = h + 'px';
			console.log('resize:', w, h);
			core.resize(w, h);
			core.render();
		}
		window.addEventListener('resize', canvasResizing);

		// disable context menu
		window.addEventListener('contextmenu', function (e) {
			e.preventDefault();
		});

		function updateObjList(sceneInfo) {
			console.log(sceneInfo);
			var i,
				objlist = sceneInfo.objectlist,
				lst = kUI('list-itemlist'),
				dust,
				icon,
				item,
				itemid,
				camlist,
				opt,
				str;
			lst.Clear();
			camlist = document.getElementById('camera_selector');
			camlist.innerHTML = ''; // clear
			for (i = 0; i < objlist.length; i = i + 1) {
				//objList[i].name, objList[i].type
				itemid = 'listitem-' + objlist[i].name;
				lst.AddItem(objlist[i].name, itemid);
				item = $(itemid);
				item.addEventListener('click', objectClick(core, objlist[i].name, objlist[i].type));
				
				// Dust icon
				dust = $(itemid + '-dustbtn');
				if (objlist[i].name === 'view') {
					dust.setAttribute('style', 'display:none'); // undeletable
				} else {
					dust.addEventListener('click', dustClick(core, objlist[i].name));
				}
				
				// Color icon
				icon = item.getElementsByClassName('KList-Item-Icon')[0];
				icon.style.backgroundColor = getIconColor(objlist[i].type);
				
				// camera list
				if (objlist[i].type === 'CAMERA') {
					opt = document.createElement("option");
					str = document.createTextNode(objlist[i].name);
					opt.appendChild(str);
					camlist.appendChild(opt);

					opt.value = objlist[i].name;
					if (objlist[i].name === core.getActiveCamera()) {
						console.log(objlist[i].name);
						opt.selected = true;
					}
				}

			}
			kUI('timeline').setTimelineData(core.getTimeline());
			kUI('timeline').drawGraph();
			core.render();
		}

		function loadModel(filepath) {
			console.log("FileDialog Select:" + filepath);
			if (filepath.substr(filepath.length - 4) === ".obj") {
				core.loadOBJ(filepath, './shader/polygon.frag');
			} else if (filepath.substr(filepath.length - 4) === ".stl") {
				core.loadSTL(filepath, './shader/polygon.frag');
			} else if (filepath.substr(filepath.length - 4) === ".pdb") {
				core.loadPDB(filepath, './shader/polygon.frag');
			} else if (filepath.substr(filepath.length - 4) === ".sph") {
				core.loadSPH(filepath, './shader/polygon.frag');
			} else {
				console.error('Not supported file type:', filepath);
			}
		}

		function reloadScene(sceneInfo) {
			var i,
				k,
				obj,
				pos = [];
			console.log("reloadScne", sceneInfo);
			var info = null;
			if (!sceneInfo) {
				return;
			}
			if (!sceneInfo.objectlist) {
				return;
			}
			for (i = 0; i < sceneInfo.objectlist.length; i = i + 1) {
				obj = sceneInfo.objectlist[i];
				if (obj.hasOwnProperty('type') && obj.hasOwnProperty('info') && obj.hasOwnProperty('name')) {
					console.log(obj);
					if (obj.type === 'POLYGON') {
						if (obj.info.hasOwnProperty('filename')) {
							loadModel(obj.info.filename);
							/*, function () {
								if (obj.hasOwnProperty('name')) {
									updateProperty(core, obj.name);
								}
							});
							*/
						}
					} else if (obj.type === 'CAMERA') {
						core.addCamera(obj.name);
						/*, function () {
							if (obj.info.hasOwnProperty('position')) {
								console.log(obj.info.position);
								pos = [];
								for (k = 0; k < 3; k = k + 1) {
									pos.push(parseFloat(obj.info.position[k]));
								}
								core.setCameraPosition(obj.name, pos, false);
								if (activeObjectName) {
									updateProperty(core, activeObjectName);
								}
							}
							if (obj.info.hasOwnProperty('fov')) {
								core.setCameraFov(obj.name, parseFloat(obj.info.fov), false);
								if (activeObjectName) {
									updateProperty(core, activeObjectName);
								}
							}
						});
						*/
					}
					activeObjectName = obj.name;
				}
			}
			updateObjList(sceneInfo);
		}

		//---------------------------
		//  UI Events
		//
		// Timeline events
		kUI('camera_selector').ChangeCallback(function (val) {
			//console.log(val);
			core.setActiveCamera(val);
			core.render();
			//kUI('viewmode').SetCaption('[' + objname + ']');
		});
		kUI('timeline').setTimelineData(core.getTimeline());
		kUI('timeline').drawGraph();
		kUI('timeline').ChangeTimeCallback(function (tm) {
			activeTime = tm;
			core.updateTime(tm);
			if (activeObjectName) {
				updateProperty(core, activeObjectName);
			}
		});
		$('projsetting').addEventListener('click', function (ev) {
			$toggle($('window-projectproperty'), 500);
		});
		$('showconsole').addEventListener('click', function (ev) {
			$toggle($('consoleArea'), 500);
		});
		$('newscenebtn').addEventListener('click', function (ev) {
			ev.preventDefault();
			core.newScene();
		});
		$('loadscenebtn').addEventListener('click', function (ev) {
			var fdlg = new FileDialog("loadsceneDialog", true, '.json');
			fdlg.OpenFile("", (function (core, fdlg) {
				return function (path) {
					core.getFileList(path, function (err, res) {
						console.log(err, res);
						fdlg.updateDirlist({list: res, path: path});
					});
				};
			}(core, fdlg)),
				function (filepath) {
					core.loadScene(filepath, reloadScene);
					console.log("FileDialog Select:" + filepath);
				});
			//core.render();
		});
		$('savescenebtn').addEventListener('click', function (ev) {
			var fdlg = new FileDialog("savesceneDialog", true, '.json');
			fdlg.SaveFile("", (function (core, fdlg) {
				return function (path) {
					core.getFileList(path, function (err, res) {
						console.log(err, res);
						fdlg.updateDirlist({list: res, path: path});
					});
				};
			}(core, fdlg)), function (filepath) {
				core.saveScene(filepath);
				console.log("FileDialog save:" + filepath);
			});

			console.log('SAVE SCENE');
		});

		$('list-cameraaddbutton').addEventListener('click', function (ev) {
			core.addCamera();
		});

		$('list-addbutton').addEventListener('click', function (ev) {
			var fdlg = new FileDialog("dataDialog", true);
			fdlg.OpenFile("", (function (core, fdlg) {
				return function (path) {
					core.getFileList(path, function (err, res) {
						console.log(err, res);
						fdlg.updateDirlist({list: res, path: path});
					});
				};
			}(core, fdlg)), loadModel)
		});
		//$('savescenebtn').addEventListener('click', function (ev) {
			//core.getSceneInformation(function (objList) {
			//);
		//});

		$('addkeybutton').addEventListener('click', function (ev) {
			console.log('ADD KEY', activeObjectName, activeTime);
			if (activeObjectName !== '') {
				core.addKey(activeObjectName, activeTime);
				kUI('timeline').drawGraph();
			}
		});
		// ----------------------------
		// Mouse Event
		function mouseMove(e) {
			e.preventDefault();
			var dx = e.clientX - mouseState.x,
				dy = e.clientY - mouseState.y;

			if (mouseState.Left) {
				core.Rotate(dy * -0.5, dx * -0.5); // Swizzle axis
				updateProperty(core, core.getActiveCamera());
			}
			if (mouseState.Right) {
				core.Zoom(dx + dy);
				updateProperty(core, core.getActiveCamera());
			}
			if (mouseState.Center) {
				core.Translate(dx, dy);
				updateProperty(core, core.getActiveCamera());
			}
			mouseState.x = e.clientX;
			mouseState.y = e.clientY;
		}
		function mouseUp(e) {
			e.preventDefault();
			if (e.button === 0) { mouseState.Left   = false; }
			if (e.button === 2) { mouseState.Right  = false; }
			if (e.button === 1) { mouseState.Center = false; }
			mouseState.x = e.clientX;
			mouseState.y = e.clientY;
			window.removeEventListener('mouseup',   mouseUp);
			window.removeEventListener('mousemove', mouseMove);
		}
		function mouseDown(e) {
			e.preventDefault();
			if (e.button === 0) { mouseState.Left   = true; }
			if (e.button === 2) { mouseState.Right  = true; }
			if (e.button === 1) { mouseState.Center = true; }
			mouseState.x = e.clientX;
			mouseState.y = e.clientY;
			
			window.addEventListener('mouseup',   mouseUp);
			window.addEventListener('mousemove', mouseMove);
		}
		$('resultdiv').addEventListener('mousedown', mouseDown);
		//----------------------------
	}
	window.addEventListener('load', init);
}(window));