/*jslint devel:true*/
/*global HiveConnect, Blob, URL*/

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
			camera_pos = [0, 0, 300];

		function renderScript(src) {
			conn.rendererMethod('runscript', {script: src}, function (res) {
				console.log('runscript result:', res, {script: src});
			});
		}
		
		//------------------
		function sceneCreateCamera(name) {
			var src = '';
			src += "local camera = Camera()\n";
			src += "camera:SetScreenSize(512, 512)\n";
			src += "camera:LookAt(\n";
			src += "	0,0,300,\n";
			src += "	0,0,0,\n";
			src += "	0,1,0,\n";
			src += "	60\n";
			src += ")\n";
			src += "ObjectTable['" + name + "'] = camera\n";
			src += "return 'CreateCamera:" + name + "'";
			renderScript(src);
		}
		function sceneClear() {
			var src = '';
			src += "ObjectTable = {}\n";
			src += "clearCache()\n";
			src += "collectgarbage('collect')\n";
			src += "print('MEMORY=', collectgarbage('count') .. '[KB]')\n";
			src += "return 'ClearObject'\n";
			renderScript(src);
			sceneCreateCamera('camera');
		}
		function sceneLoadObj(name, filename, shader) {
			var src = '';
			src += "local obj = OBJLoader()\n";
			src += "local ret = obj:Load('" + filename + "')\n";
			src += "if ret then\n";
			src += "	local model = PolygonModel()\n";
			src += "	local meshdata = obj:MeshData()\n";
			src += "	model:Create(meshdata)\n";
			src += "	model:SetShader('" + shader + "')\n";
			src += "	ObjectTable['" + name + "'] = model\n";
			src += "end\n";
			src += "return 'LoadOBJ:' .. tostring(ret)";
			renderScript(src);
		}
		function sceneRenderObjects() {
			var src = '';
			src += 'local r = render(ObjectTable)\n';
			src += '-- save jpg\n';
			src += 'local saver = ImageSaver()\n';
			src += 'local camera = ObjectTable["camera"]\n';
			src += 'if camera == nil then return "No Camera" end\n';
			src += 'local imageBuffer = saver:SaveMemory(1, camera:GetImageBuffer())\n';
			src += 'local imageBufferSize = saver:MemorySize()\n';
			src += '-- make metabin\n';
			src += 'local metabin = MetaBinary()\n';
			src += 'local json = [[{\n';
			src += '  "JSONRPC": "2.0",\n';
			src += '  "method" : "renderedImage",\n';
			src += '  "param" : {\n';
			src += '    "type": "jpg",\n';
			src += '    "width" : 512,\n';
			src += '	"height" : 512\n';
			src += '  },\n';
			src += '  "to": "client",\n';
			src += '  "id": 0\n';
			src += '}]]\n';
			src += 'metabin:Create(json, imageBuffer, imageBufferSize)\n';
			src += '-- send through websockt\n';
			src += 'local network = Connection()\n';
			src += 'network:Connect("ws://localhost:8080/")\n';
			src += 'network:SendBinary(metabin:BinaryBuffer(), metabin:BinaryBufferSize())\n';
			src += 'network:Close()\n';
			src += 'return "Render:" .. tostring(r)\n';
			renderScript(src);
		}
		function sceneGetObjectList() {
			var src = '';
			src += 'local lst = {}\n';
			src += 'for i,v in pairs(ObjectTable) do\n';
			src += '  lst[#lst + 1] = {name=i, type=v:GetType()}\n';
			src += 'end\n';
			src += 'return lst';
			renderScript(src);
		}
		function sceneCameraPos(name, camerapos) {
			var src = '';
			src += "local camera = ObjectTable['" + name + "']\n";
			src += "if camera == nil then return 'Not found camera' end\n";
			src += "camera:LookAt(\n";
			src += "	" + camerapos[0] + "," + camerapos[1] + "," + camerapos[2] + ",\n";
			src += "	0,0,0,\n";
			src += "	0,1,0,\n";
			src += "	60\n";
			src += ")\n";
			src += "return 'MoveCamera:" + name + "'";
			renderScript(src);
			sceneRenderObjects();
		}
		//----------
		conn.method('renderedImage', function (param, data) {
			var img = document.getElementById('img');
			if (param.type === 'jpg') {
				img.src = URL.createObjectURL(new Blob([data], {type: "image/jpeg"}));
			}
			// redraw
			//sceneRenderObjects();
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
				camera_pos[0] += dx;
				camera_pos[1] += dy;
				sceneCameraPos('camera', camera_pos);
			}
			if (rightpress) {
				camera_pos[2] += (dx + dy);
				sceneCameraPos('camera', camera_pos);
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
			sceneLoadObj('model', 'bunny.obj', 'white.frag');
		});
		clearbtn.addEventListener('click', function (e) {
			sceneClear();
		});
		getbtn.addEventListener('click', function (e) {
			sceneGetObjectList();
		});
		objrenderbtn.addEventListener('click', function (e) {
			sceneRenderObjects();
		});
		resetcamerabtn.addEventListener('click', function (e) {
			camera_pos[0] = 0;
			camera_pos[1] = 0;
			camera_pos[2] = 300;
			sceneCameraPos('camera', camera_pos);
		});

	}
	window.addEventListener('load', init);
	window.addEventListener('contextmenu', function (e) {
		e.preventDefault();
	});
}(window));