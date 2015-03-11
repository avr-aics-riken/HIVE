(function (window) {
	'use strict';
	var sceneCommands = {
		createCamera : function (name) {
			var src = '';
			src += "local camera = Camera()\n";
			src += "camera:SetScreenSize(512, 512)\n";
			src += "camera:LookAt(\n";
			src += "	0,0,300,\n";
			src += "	0,0,0,\n";
			src += "	0,1,0,\n";
			src += "	60\n";
			src += ")\n";
			src += "HIVE_ObjectTable['" + name + "'] = camera\n";
			//src += "return 'CreateCamera:" + name + "'\n";
			return src;
		},
		clearObjects : function () {
			var src = '';
			src += "HIVE_ObjectTable = {}\n";
			src += "clearCache()\n";
			src += "collectgarbage('collect')\n";
			src += "print('MEMORY=', collectgarbage('count') .. '[KB]')\n";
			src += "return 'ClearObject'\n";
			return src;
		},
		loadObj : function (name, filename, shader) {
			var src = '';
			src += "local obj = OBJLoader()\n";
			src += "local ret = obj:Load('" + filename + "')\n";
			src += "if ret then\n";
			src += "	local model = PolygonModel()\n";
			src += "	local meshdata = obj:MeshData()\n";
			src += "	model:Create(meshdata)\n";
			src += "	model:SetShader('" + shader + "')\n";
			src += "	HIVE_ObjectTable['" + name + "'] = model\n";
			src += "end\n";
			src += "return 'LoadOBJ:' .. tostring(ret)\n";
			return src;
		},
		loadPDB : function (name, filename, shader) {
			var src = '';
			src += "local pdb = PDBLoader()\n";
			src += "local ret = pdb:Load('" + filename + "')\n";
			src += "if ret then\n";
			src += "    local ballmodel = PointModel()\n";
			src += "    local balldata = pdb:BallData()\n";
			src += "    ballmodel:Create(balldata)\n";
			src += "    ballmodel:SetShader('" + shader + "')\n";
			src += "    local stickmodel = LineModel()\n";
			src += "    local stickdata = pdb:StickData()\n";
			src += "    stickmodel:Create(stickdata)\n";
			src += "    stickmodel:SetLineWidth(0.20);\n";
			src += "    stickmodel:SetShader('" + shader + "')\n";
			src += "	HIVE_ObjectTable['" + name + "_ball'] = ballmodel\n";
			src += "	HIVE_ObjectTable['" + name + "_stick'] = stickmodel\n";
			src += "end\n";
			src += "return 'LoadPDB:' .. tostring(ret)\n";
			return src;
		},
		render : function (w, h) {
			var src = '';
			src += 'local r = render(HIVE_ObjectTable, fetchEvent)\n';
			src += '-- save jpg\n';
			src += 'local saver = ImageSaver()\n';
			src += 'local camera = HIVE_ObjectTable["camera"]\n';
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
			src += '    "width" : ]] .. camera:GetScreenWidth() .. [[,\n';
			src += '	"height" : ]] .. camera:GetScreenHeight() .. [[,\n';
			src += '    "canceled": ]] .. tostring(HIVE_isRenderCanceled) .. [[\n';
			src += '  },\n';
			src += '  "to": "client",\n';
			src += '  "id": 0\n';
			src += '}]]\n';
			src += 'metabin:Create(json, imageBuffer, imageBufferSize)\n';
			src += '-- send through websockt\n';
			src += 'network:SendBinary(metabin:BinaryBuffer(), metabin:BinaryBufferSize())\n';
			src += 'return {objectnum=tostring(r), width=camera:GetScreenWidth(), height=camera:GetScreenHeight()}\n';
			return src;
		},
		getObjectList : function () {
			var src = '';
			src += 'local lst = {}\n';
			src += 'for i,v in pairs(HIVE_ObjectTable) do\n';
			src += '  lst[#lst + 1] = {name=i, type=v:GetType()}\n';
			src += 'end\n';
			src += 'return lst';
			return src;
		},
		cameraScreenSize : function (name, width, height) {
			var src = '';
			src += "local camera = HIVE_ObjectTable['" + name + "']\n";
			src += "if camera == nil then return 'Not found camera' end\n";
			src += "camera:SetScreenSize(" + width + ", " + height + ")\n";
			return src;
		},
		cameraPos : function (name, camerapos) {
			var src = '';
			src += "local camera = HIVE_ObjectTable['" + name + "']\n";
			src += "if camera == nil then return 'Not found camera' end\n";
			src += "camera:LookAt(\n";
			src += "	" + camerapos[0] + "," + camerapos[1] + "," + camerapos[2] + ",\n";
			src += "	0,0,0,\n";
			src += "	0,1,0,\n";
			src += "	60\n";
			src += ")\n";
			//src += "return 'MoveCamera:" + name + "'";
			return src;
		}
	};
	window.sceneCommands = sceneCommands;
}(window));