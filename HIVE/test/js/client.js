/*jslint devel:true*/
/*global HiveConnect, Blob, URL, sceneCommands*/

(function (window) {
	'use strict';
	function init() {
		var conn = new HiveConnect(),
			clearbtn = document.getElementById('clearbtn'),
			loadbtn = document.getElementById('loadbtn'),
			getbtn = document.getElementById('getbtn'),
			objrenderbtn = document.getElementById('objrenderbtn'),
			resetcamerabtn = document.getElementById('resetcamerabtn'),
			imgdiv = document.getElementById('imgdiv'),
			leftpress = false,
			rightpress = false,
			oldmouse_x,
			oldmouse_y,
			camera_pos = [0, 0, 300],
			camera_defsize = [64, 64],
			camera_maxsize = [512, 512],
			camera_size = [128, 128],
			renderQue = [],
			isRendering = false;
		

		function renderScript(src) {
			conn.rendererMethod('runscript', {script: src}, function (res) {
				console.log('runscript result:', res, {script: src});
			});
		}
 		
		//----------
		conn.method('open', function (res) {
			var cmd = sceneCommands.cameraScreenSize('camera', camera_size[0], camera_size[1]);
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
				img.setAttribute('width', '512px');
				img.setAttribute('height', '512px');
			}
			
			if (renderQue.length > 0) {
				scriptQue = renderQue.pop();
				renderScript(scriptQue);
				renderQue.length = 0;
			} else {
				if (param.width < camera_maxsize[0] || param.height < camera_maxsize[1]) {
					console.log('RENDER!', param.width * 2, param.height * 2);
					cmd = sceneCommands.cameraScreenSize('camera', param.width * 2, param.height * 2);
					cmd += sceneCommands.render();
					renderScript(cmd);
				} else {
					isRendering = false;
				}
			}
		});
		
		//------------------
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
			var dx, dy;
			dx = e.clientX - oldmouse_x;
			dy = e.clientY - oldmouse_y;
			if (leftpress) {
				camera_pos[0] -= dx;
				camera_pos[1] += dy;
				camera_size[0] = camera_defsize[0];
				camera_size[1] = camera_defsize[1];
				
				renderQue.push(sceneCommands.cameraScreenSize('camera', camera_size[0], camera_size[1]) + sceneCommands.cameraPos('camera', camera_pos) + sceneCommands.render());
				setTimeout(function () {
					var cmd;
					if (!isRendering && renderQue.length > 1) {
						isRendering = true;
						cmd = renderQue.pop();
						renderScript(cmd);
					}
				}, 1);
			}
			if (rightpress) {
				camera_pos[2] -= (dx + dy);
				camera_size[0] = camera_defsize[0];
				camera_size[1] = camera_defsize[1];
				
				renderQue.push(sceneCommands.cameraScreenSize('camera', camera_size[0], camera_size[1]) + sceneCommands.cameraPos('camera', camera_pos) + sceneCommands.render());
				setTimeout(function () {
					var cmd;
					if (!isRendering && renderQue.length > 1) {
						isRendering = true;
						cmd = renderQue.pop();
						renderScript(cmd);
					}
				}, 1);
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

		loadbtn.addEventListener('click', function (e) {
			renderScript(sceneCommands.loadObj('model', 'bunny.obj', 'normal.frag'));
		});
		clearbtn.addEventListener('click', function (e) {
			renderScript(sceneCommands.clearObjects());
			renderScript(sceneCommands.createCamera('camera'));
		});
		getbtn.addEventListener('click', function (e) {
			renderScript(sceneCommands.getObjectList());
		});
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