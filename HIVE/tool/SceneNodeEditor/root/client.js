/*jslint devel:true */
/*global SVG, svgNodeUI, io */

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
	
	var	draw = SVG('nodecanvas').size(4000, 4000),
		nui = svgNodeUI(draw),
		nodeData = {
			"nodeData": [
				{
					"name": "OBJLoader",
					"pos": [200, 100],
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
					"name": "PolygonModel",
					"pos": [450, 300],
					"varname": "instPolygonModel",
					"funcname": "Create",
					"deletable": false,
					"input": [
						{"name": "mesh", "type": "MeshData"}
					],
					"output": [
						{"name": "this", "type": "RenderObject"}
					]
				},
				{
					"name": "render",
					"pos": [700, 100],
					"varname": "root",
					"funcname": "",
					"deletable": false,
					"input": [
						{"name": "RenderObject", "type": "RenderObject"}
					],
					"output": [
					]
				}
			],
			"plugData": [
				{"output": {"node": "instOBJLoader", "plug": "MeshData"}, "input": {"node": "instPolygonModel", "plug": "mesh"}},
		{"output": {"node": "instPolygonModel", "plug": "this"}, "input": {"node": "root", "plug": "RenderObject"}},

			]
		};
	
	nui.clearNodes();
	nui.makeNodes(nodeData);
	
	document.addEventListener('keydown', function () {
		var customlua = nui.exportLua();
		//console.log(customlua);
	});
});
