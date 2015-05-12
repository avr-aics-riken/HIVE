/*jslint devel:true*/
/*global HiveConnect, Blob, URL, sceneCommands*/

(function (window) {
	'use strict';
	function init() {
		var conn = new HiveConnect(),
			clearbtn = document.getElementById('clearbtn'),
			loadobjbtn = document.getElementById('loadobjbtn'),
			loadstlbtn = document.getElementById('loadstlbtn'),
			loadpdbbtn = document.getElementById('loadpdbbtn'),
			getbtn = document.getElementById('getbtn'),
			objrenderbtn = document.getElementById('objrenderbtn'),
			resetcamerabtn = document.getElementById('resetcamerabtn'),
			imgdiv = document.getElementById('imgdiv'),
			leftpress = false,
			rightpress = false,
			oldmouse_x,
			oldmouse_y,
			camera_pos = [0, 0, 300],
			camera_defsize = [16, 16],
			camera_maxsize = [2048, 2048],
			camera_size = [128, 128];

		function renderScript(src) {
			conn.rendererMethod('runscript', {script: src}, function (err, res) {
				if (err) {
					console.error('runscript error:', err, {script: src});
				}/* else {
					console.log('runscript result:', res, {script: src});
				}*/
			});
		}
 		
		//----------
		conn.method('open', function (res) {
			var cmd = sceneCommands.createCamera('camera');
			cmd += sceneCommands.cameraScreenSize('camera', camera_size[0], camera_size[1]);
			cmd += sceneCommands.cameraPos('camera', camera_pos);
			cmd += sceneCommands.render();
			renderScript(cmd);
		});
		
		conn.method('renderedImage', function (param, data) {
			var img = document.getElementById('img'),
				scriptQue,
				camscript = "",
				cmd;
			if (param.type === 'jpg') {
				img.src = URL.createObjectURL(new Blob([data], {type: "image/jpeg"}));
			}

			if (!param.canceled) {
				if (param.width < camera_maxsize[0] || param.height < camera_maxsize[1]) {
					//console.log('REFINE RENDER!', param.width * 2, param.height * 2);
					cmd = sceneCommands.cameraScreenSize('camera', param.width * 2, param.height * 2);
					cmd += sceneCommands.render();
					renderScript(cmd);
				}
			}
		});
		
		//------------------
		if (window.TouchEvent) {
			imgdiv.addEventListener('touchstart', function (e) {
				e.preventDefault();
				var touches = e.touches;
				oldmouse_x = touches[0].clientX;
				oldmouse_y = touches[0].clientX;
				if (touches.length === 1) {
					leftpress = true;
				} else {
					rightpress = true;
				}
			});
			imgdiv.addEventListener('touchend', function (e) {
				e.preventDefault();
				leftpress = false;
				rightpress = false;
			});
			imgdiv.addEventListener('touchmove', function (ev) {
				ev.preventDefault();
				ev.stopPropagation();
				var dx, dy, cmd, e;
				e = ev.touches[0];
				dx = e.clientX - oldmouse_x;
				dy = e.clientX - oldmouse_y;
				//console.log(dx, dy);
				if (leftpress) {
					camera_pos[0] -= dx;
					camera_pos[1] += dy;
					camera_size[0] = camera_defsize[0];
					camera_size[1] = camera_defsize[1];
					
					cmd = sceneCommands.cameraScreenSize('camera', camera_size[0], camera_size[1]) + sceneCommands.cameraPos('camera', camera_pos) + sceneCommands.render();
					renderScript(cmd);
				}
				if (rightpress) {
					camera_pos[2] -= (dx + dy);
					camera_size[0] = camera_defsize[0];
					camera_size[1] = camera_defsize[1];

					cmd = sceneCommands.cameraScreenSize('camera', camera_size[0], camera_size[1]) + sceneCommands.cameraPos('camera', camera_pos) + sceneCommands.render();
					renderScript(cmd);
				}
				oldmouse_x = e.clientX;
				oldmouse_y = e.clientX;
			});
		}
		imgdiv.addEventListener('mousedown', function (e) {
			e.preventDefault();
			if (e.button === 0) {
				leftpress = true;
			} else if (e.button === 2) {
				rightpress = true;
			}
			oldmouse_x = e.clientX;
			oldmouse_y = e.clientY;
		});
		imgdiv.addEventListener('mousemove', function (e) {
			e.preventDefault();
			e.stopPropagation();
			var dx, dy, cmd;
			dx = e.clientX - oldmouse_x;
			dy = e.clientY - oldmouse_y;
			if (leftpress) {
				camera_pos[0] -= dx;
				camera_pos[1] += dy;
				camera_size[0] = camera_defsize[0];
				camera_size[1] = camera_defsize[1];
				
				cmd = sceneCommands.cameraScreenSize('camera', camera_size[0], camera_size[1]) + sceneCommands.cameraPos('camera', camera_pos) + sceneCommands.render();
				renderScript(cmd);
			}
			if (rightpress) {
				camera_pos[2] -= (dx + dy);
				camera_size[0] = camera_defsize[0];
				camera_size[1] = camera_defsize[1];

				cmd = sceneCommands.cameraScreenSize('camera', camera_size[0], camera_size[1]) + sceneCommands.cameraPos('camera', camera_pos) + sceneCommands.render();
				renderScript(cmd);
			}
			oldmouse_x = e.clientX;
			oldmouse_y = e.clientY;
		});
		imgdiv.addEventListener('mouseup', function (e) {
			e.preventDefault();
			if (e.button === 0) {
				leftpress = false;
			} else if (e.button === 2) {
				rightpress = false;
			}

		});

		loadobjbtn.addEventListener('click', function (e) {
			var objfilepath = document.getElementById('objfilepath'),
				objshaderpath = document.getElementById('objshaderpath'),
				filepath,
				shaderpath;
			if (objfilepath) {
				filepath = objfilepath.value;
			}
			if (objshaderpath) {
				shaderpath = objshaderpath.value;
			}
			if (filepath && filepath !== '') {
				renderScript(sceneCommands.loadObj('model', filepath, shaderpath));
			}
		});
		loadstlbtn.addEventListener('click', function (e) {
			var stlfilepath = document.getElementById('stlfilepath'),
				stlshaderpath = document.getElementById('stlshaderpath'),
				filepath,
				shaderpath;
			if (stlfilepath) {
				filepath = stlfilepath.value;
			}
			if (stlshaderpath) {
				shaderpath = stlshaderpath.value;
			}
			if (filepath && filepath !== '') {
				renderScript(sceneCommands.loadStl('model', filepath, shaderpath));
			}
		});
		loadpdbbtn.addEventListener('click', function (e) {
			var pdbfilepath = document.getElementById('pdbfilepath'),
				pdbshaderpath = document.getElementById('pdbshaderpath'),
				filepath,
				shaderpath;
			if (pdbfilepath) {
				filepath = pdbfilepath.value;
			}
			if (pdbshaderpath) {
				shaderpath = pdbshaderpath.value;
			}
			if (filepath && filepath !== '') {
				renderScript(sceneCommands.loadPDB('model', filepath, shaderpath));
			}
		});
		clearbtn.addEventListener('click', function (e) {
			renderScript(sceneCommands.clearObjects());
			renderScript(sceneCommands.createCamera('camera'));
		});
		/*getbtn.addEventListener('click', function (e) {
			renderScript(sceneCommands.getObjectList());
		});*/
		objrenderbtn.addEventListener('click', function (e) {
			renderScript(sceneCommands.render());
		});
		resetcamerabtn.addEventListener('click', function (e) {
			camera_pos[0] = 0;
			camera_pos[1] = 0;
			camera_pos[2] = 300;
	
			camera_size[0] = camera_defsize[0];
			camera_size[1] = camera_defsize[1];

			renderScript(sceneCommands.cameraScreenSize('camera', camera_size[0], camera_size[1]) + sceneCommands.cameraPos('camera', camera_pos) + sceneCommands.render());
		});

	}
	window.addEventListener('load', init);
	window.addEventListener('contextmenu', function (e) {
		e.preventDefault();
	});
}(window));