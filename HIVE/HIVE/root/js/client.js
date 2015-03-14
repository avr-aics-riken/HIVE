/*jslint devel:true*/
/*global HiveCore*/            // HiveCore
/*global kvtoolsUI_init, kUI*/ // kvtoolsUI
/*global $, $toggle*/          // qatrix
/*global FileDialog*/          // Filedlg

(function (window) {
	'use strict';
	function showFiledialog(show) {
		
	}
	function setPropertyMode(type) {
		if (type === 'CAMERA') {
			$('object-transformshader').style.display = 'none';
			$('camera-information').style.display = '';
			$('object-viewsyncbutton').style.display = '';
		} else {
			$('object-transformshader').style.display = '';
			$('camera-information').style.display = 'none';
			$('object-viewsyncbutton').style.display = 'none';
		}
	}
	function setCameraProperty(name, objprop) {
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
	}
	function setObjectProperty(name, objprop) {
		$('objname').value = name;
		$('translate_x').value = objprop.translate[0];
		$('translate_y').value = objprop.translate[1];
		$('translate_z').value = objprop.translate[2];
		$('rotate_x').value = objprop.rotate[0];
		$('rotate_y').value = objprop.rotate[1];
		$('rotate_z').value = objprop.rotate[2];
		$('scale_x').value = objprop.scale[0];
		$('scale_y').value = objprop.scale[1];
		$('scale_z').value = objprop.scale[2];
		var shadername = objprop.shader,
			targetShader = null,
			i;

		//$('shader_name').Clear();
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
			setPropertyMode(objprop.type);
			if (objprop.type === "CAMERA") {
				setCameraProperty(objname, objprop.info);
			} else {
				setObjectProperty(objname, objprop.info);
			}
		}
	}
	
	function objectClick(core, objname) {
		return function () {
			updateProperty(core, objname);
		};
	}
	function dustClick(core, objname) {
		return function () {
			core.deleteObject(objname);
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
		
		function updateObjList(sceneInfo) {
			console.log(sceneInfo);
			var i,
				objlist = sceneInfo.objectlist,
				lst = kUI('list-itemlist'),
				dust,
				icon,
				item,
				itemid;
			lst.Clear();
			for (i = 0; i < objlist.length; i = i + 1) {
				//objList[i].name, objList[i].type
				itemid = 'listitem-' + objlist[i].name;
				lst.AddItem(objlist[i].name, itemid);
				item = $(itemid);
				item.addEventListener('click', objectClick(core, objlist[i].name, objlist.type));
				
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
			}
			core.render();
		}
		
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
			//console.log('context');
			//core.loadOBJ('bunny.obj', 'normal.frag');
			//core.render();
			e.preventDefault();
		});

		//---------------------------
		//  UI Events
		//
		// Timeline events
		kUI('timeline').setTimelineData();
		kUI('timeline').drawGraph();
		kUI('timeline').ChangeTimeCallback(function (tm) {
			//kUI('viewmode').SetCaption('[Camera]');
			core.updateTime(tm);
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
			core.loadOBJ('bunny.obj', 'normal.frag');
			core.render();
		});
		$('list-cameraaddbutton').addEventListener('click', function (ev) {
			core.addCamera();
		});

		$('list-addbutton').addEventListener('click', function (ev) {
			var fdlg = new FileDialog("dataDialog", true, false);
			fdlg.OpenFile("", (function (core, fdlg) {
				return function (path) {
					core.getFileList(path, function (err, res) {
						console.log(err, res);
						fdlg.updateDirlist({list: res, path: path});
					});
				};
			}(core, fdlg)),
						function (filepath) {
					console.log("FileDialog Select:" + filepath);
				
				});
		});
		//$('savescenebtn').addEventListener('click', function (ev) {
			//core.getSceneInformation(function (objList) {
			//);
		//});

		// ----------------------------
		// Mouse Event
		function mouseDown(e) {
			e.preventDefault();
			if (e.button === 0) { mouseState.Left   = true; }
			if (e.button === 2) { mouseState.Right  = true; }
			if (e.button === 1) { mouseState.Center = true; }
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
		}

		function mouseMove(e) {
			e.preventDefault();
			var dx = e.clientX - mouseState.x,
				dy = e.clientY - mouseState.y;

			if (mouseState.Left) {
				core.Rotate(dy * -0.5, dx * -0.5); // Swizzle axis
				updateProperty(core, 'view');
			}
			if (mouseState.Right) {
				core.Zoom(dx + dy);
				updateProperty(core, 'view');
			}
			if (mouseState.Center) {
				core.Translate(dx, dy);
				updateProperty(core, 'view');
			}
			mouseState.x = e.clientX;
			mouseState.y = e.clientY;
		}
		$('resultdiv').addEventListener('mousedown', mouseDown);
		$('resultdiv').addEventListener('mouseup',   mouseUp);
		$('resultdiv').addEventListener('mousemove', mouseMove);
		//----------------------------
	}
	window.addEventListener('load', init);
}(window));