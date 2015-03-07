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
			renderbtn = document.getElementById('renderbtn');

		function renderScript(src) {
			conn.rendererMethod('runscript', {script: src}, function (res) {
				console.log('runscript result:', res, {script: src});
			});
		}
		conn.method('renderedImage', function (param, data) {
			var img = document.getElementById('img');
			if (param.type === 'jpg') {
				img.src = URL.createObjectURL(new Blob([data], {type: "image/jpeg"}));
			}
		});
		
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
			src += '  lst[#lst + 1] = i\n';
			src += 'end\n';
			src += 'return lst';
			renderScript(src);
		}
		//------------------

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

	}
	window.addEventListener('load', init);
}(window));