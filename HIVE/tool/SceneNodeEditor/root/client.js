/*jslint devel:true */
/*global SVG, svgNodeUI, io */


var nui;
var nodeData;
var socket = io.connect();

var staticNodeData = {
		"nodeData":[
			{
				"name": "OBJLoader",
				"pos": [150, 300],
				"varname": "instOBJLoader",
				"funcname": "Load",
				"deletable": false,
				"input": [
					{"name": "filepath", "type": "string", "value": "'bunny.obj'"}
				],
				"output": [
					{"name": "MeshData", "type": "MeshData"}
				]
			},
			{
				"name"      : "CreateCamera",
				"pos"       : [150, 100],
				"varname"   : "instCamera",
				"funcname"  : "create",
				//"customfunc" : "function SetLookAt() return function(instCamera, px, py, pz, ax, ay, az, ux, uy, uz, fov) instCamera::SetScreenSize(1024, 1024); instCamera:SetFilename('render_obj.jpg');instCamera::LookAt(px, py, pz, ax, ay, az, ux, uy, uz, fov) end end\n",
				"customfunc" : "function CreateCamera() return { camera=function(self) return self.m_cam end, create=function(self) self.m_cam = Camera(); self.m_cam:SetScreenSize(1024, 1024); self.m_cam:SetFilename('render_obj.jpg'); self.m_cam:LookAt(0, 0, -300, 0, 0, 0, 0, 1, 0, 60) end} end\n",
				"deletable": false,
				"cameradata" : [
					0,0,-300,
					0,0,0,
					0,1,0,
					60,
				],
				"input": [
				],
				"output": [
					{"name": "camera", "type": "Camera"}
				]
			},
			{
				"name": "PolygonModel",
				"pos": [450, 300],
				"varname": "instPolygonModel",
				"funcname": "Create",
				"deletable": false,
				"input": [
					{"name": "mesh", "type": "MeshData"}
				],
				"output": [
					{"name": "", "type": "RenderObject"}
					]
			},
			{
				"name": "Render",
				"pos": [700, 100],
				"varname": "root",
				"funcname": "",
				"customfunc" : "function Render() return function(...) render({...}) end end\n",
				"deletable": false,
				"input": [
					{"name": "Camera",       "type": "Camera"},
					{"name": "RenderObject", "type": "RenderObject"}
				],
				"output": [
				]
			}
		],
		"plugData":[
			{"output": {"node": "instOBJLoader",    "plug": "MeshData"}, "input": {"node": "instPolygonModel", "plug": "mesh"}},
			{"output": {"node": "instPolygonModel", "plug": ""},     "input": {"node": "root",             "plug": "RenderObject"}},
			{"output": {"node": "instCamera",       "plug": "camera"},     "input": {"node": "root",             "plug": "Camera"}},
		]
	};

function reloadNode(filetext)
{
	//console.log(filetext);
	var obj = JSON.parse(filetext);
	console.log(obj);
	nodeData = obj;
	nui.clearNodes();
	nui.makeNodes(nodeData);
}

function readSingleFile(e) {
	var file = e.target.files[0];
	if (!file) {
		return;
	}
	var reader = new FileReader();
		reader.onload = function(e) {
		var contents = e.target.result;
		reloadNode(contents);
	};
	reader.readAsText(file);
}

window.addEventListener('load', function () {
	'use strict';

	var socket = io.connect();
	socket.on('connect', function () {
		console.log('connected');
		var sceneFile = 'print("AAA");render{}';
		socket.emit('sendScene', JSON.stringify({scene: sceneFile}));
		socket.on('stdio', function (data) {
			console.log('stdio', data);
		});
		socket.on('stderr', function (data) {
			console.log('stderr', data);
		});
	});
	var	draw = SVG('nodecanvas').size(1024, 768);
	nui = svgNodeUI(draw);
	nodeData = staticNodeData;
	
	nui.clearNodes();
	nui.makeNodes(nodeData);
	
	/*
	document.addEventListener('keydown', function () {
		var customlua = nui.exportLua();
		//console.log(customlua);
	});
	*/
	

	//document.getElementById('Open').addEventListener('change', readSingleFile, false);
});



function ButtonEvent(id)
{
	console.log(id)
	if(id === 'Clear') {
		nui.clearNodes();
	}
	
	if(id === 'Render') {
		var customlua = nui.exportLua();
		
		//debug
		console.log(customlua);
		var senddata =
		{
			"Script" : customlua
		};
        socket.emit('sendScene', JSON.stringify({scene: customlua}));
	}

	if(id === 'Open') {
		nodeData = staticNodeData;
		nui.clearNodes();
		nui.makeNodes(nodeData);
	}
}

