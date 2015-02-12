/*jslint devel:true */
/*global SVG, svgNodeUI, io, fs, URL */

(function () {
	'use strict';
	var nui,
		nodeData,
		nodeList,
		nodeListTable,
		socket = io.connect(),
		instance_no = 1;
	
	function clone(obj) {
		return JSON.parse(JSON.stringify(obj));
		/*if (null === obj || "object" !== typeof obj) {
			return obj;
		}
		var copy = obj.constructor(),
			attr;
		for (attr in obj) {
			if (obj.hasOwnProperty(attr)) {
				copy[attr] = obj[attr];
			}
		}
		return copy;*/
	}
	
	function addNode(nodename) {
		console.log('ADD:' + nodename);
		console.log(nodeListTable);
		var node = nodeListTable[nodename],
			instNode;
		if (!node) {
			return;
		}
		nodeData = nui.getNodeData();
		instNode = clone(node);
		nodeData.nodeData.push(instNode);
		if (instNode.varname !== 'root') {
			instNode.varname += instance_no;
		}
		instance_no += 1;
		nui.clearNodes();
		nui.makeNodes(nodeData);
		console.log(nodeData);
	}
	
	function clearNode() {
		nui.clearNodes();
		nodeData = nui.getNodeData();
		document.getElementById("property").innerHTML = '';
		
		addNode("Render");
	}
	
	function createNodeListUI() {
		var d = document.getElementById('NodeListBox'),
			inner,
			name,
			i;
		inner = '<select name="NodeList" size=' + 20 + '>';
		for (i in nodeListTable) {
			if (nodeListTable.hasOwnProperty(i)) {
				console.log(nodeListTable[i]);
				name = nodeListTable[i].name;
				inner += '<option value="' + name + '">' + name + '</option>';
			}
		}
		inner += '</select>';
		d.innerHTML = inner;
	}
	
	//------//------//------//------//------//------//------//------
	// sasaki S customfunclist
	//------//------//------//------//------//------//------//------
	function showProparty(nodeData) {
		console.log(nodeData);
		var to = document.getElementById("property"),
			html = '',
			i,
			k,
			cameradata,
			desc = ['Pos', 'At', 'UP'],
			pxyz = ['X', 'Y', 'Z'],
			index = 0,
			ele;

		
		html = '<br><br><table  bgcolor="#e3f0fb"><tr><td>';
		if (nodeData.name) {
			html += "[name]     : " + nodeData.name + '\n';
			html += '<br><br>';
		}
		if (nodeData.varname) {
			html += "[varname]  : " + nodeData.varname + '\n';
			html += '<br><br>';
		}
		/*
		if(nodeData.funcname) {
			html += "[funcname] : " + nodeData.funcname + '\n';
			html += '<hr><br>';
		}
		*/
		
		if (nodeData.name === "OBJLoader" || nodeData.name === "STLLoader") {
			if (nodeData.input) {
				for (i = 0; i < nodeData.input.length; i = i + 1) {
					if (nodeData.input[i].value) {
						html += '[FileName] : <input id="ObjTextBox" type="text"' + ' value="' + nodeData.input[i].value + '">' + '\n';
					}
				}
				html += '<br><br>';
			}
			
		}
		
		//--------------------------------------------------------------------
		// Camera Data
		//--------------------------------------------------------------------
		if (nodeData.name === "CreateCamera") {
			cameradata = [];
			for (i = 0; i < nodeData.cameradata.length; i = i + 1) {
				cameradata[i] = nodeData.cameradata[i];
			}
			html += '<br>';
			html += '[LookAt]<br>';
			for (k = 0; k < 3; k = k + 1) {
				html += desc[k] + '<br>';
				for (i = 0; i < 3; i = i + 1) {
					html += pxyz[i] + '<input size=10 id="LookAt" type="text"' + ' value="' + cameradata[index] + '">';
					index = index + 1;
				}
				html += '<br><br>';
			}
			
			//FOV
			html += '<br>';
			html += '<br>';
			html += 'Fov<br>';
			html += '<input size=2 id="CameraTextBox" type="text"' + ' value="' + cameradata[index] + '">';
			html += '<br>';
		}
		
		if (nodeData.customfunc) {
			html += "[customfunc] : " + nodeData.customfunc	 + '\n';
			html += '<br><br>';
		}
		html += '</td><tr><table>';
		to.innerHTML = html;

		//setup handler
		ele = document.getElementById("ObjTextBox");
		if (ele) {
			ele.addEventListener("keyup", function () {
				nodeData.input[0].value = ele.value;
				console.log(nodeData.input[0].value, ele.value);
			});
		} else {
			console.log('cant create ele\n');
		}
	}

	//---------------------------------------------------------------------------
	// request node info
	//---------------------------------------------------------------------------
	
	function reloadNodeList(url, cb) {
		var ret,
			req = new XMLHttpRequest();
		req.open('GET', url);
		req.send();
		req.addEventListener("load", (function (callback) {
			return function (ev) {
				var resp = ev.srcElement.responseText,
					i;

				// store node list
				nodeList = JSON.parse(resp);

				// create nodelist table
				nodeListTable = {};
				for (i = 0; i < nodeList.length; i = i + 1) {
					nodeListTable[nodeList[i].name] = nodeList[i];
				}

				//create List UI
				createNodeListUI();

				if (callback) {
					callback();
				}
			};
		}(cb)));
	}

	//---------------------------------------------------------------------------
	// buttons
	//---------------------------------------------------------------------------
	
	function ButtonClear(e) {
		clearNode();
	}

	function ButtonRender(e) {
		var customlua = nui.exportLua();
		console.log(customlua);
		socket.emit('sendScene', JSON.stringify({scene: customlua}));
	}

	function ButtonOpen(e) {
		nodeData = reloadNodeList('nodelist.json');
	}

	function ButtonAdd(e) {
		var ele = document.getElementsByName('NodeList'),
			index = ele[0].selectedIndex,
			text,
			node,
			instNode;
		if (index === -1) {
			return;
		}
		text = ele[0].options[index].text;
		
		addNode(text);
		/*
		if (index >= 0) {
			node = nodeList[index];
			nodeData = nui.getNodeData();
			instNode = clone(node);
			nodeData.nodeData.push(instNode);
			if (instNode.varname !== 'root') {
				instNode.varname += instance_no;
			}
			instance_no += 1;
			nui.clearNodes();
			nui.makeNodes(nodeData);
			console.log(nodeData);
		}*/
	}

	//---------------------------------------------------------------------------
	// init
	//---------------------------------------------------------------------------
	function init() {
		//create button
		var openbutton   = document.getElementById('Open'),
			renderbutton = document.getElementById('Render'),
			clearbutton  = document.getElementById('Clear'),
			addbutton    = document.getElementById('Add'),
			nodelistbox  = document.getElementById('NodeList'),

		//init canvas.
			draw = SVG('nodecanvas');//.size(1280, 768);
		nui      = svgNodeUI(draw);
		nui.clearNodes();

		nui.nodeClickEvent(showProparty);

		//handle
		openbutton.onclick   = ButtonOpen;
		renderbutton.onclick = ButtonRender;
		clearbutton.onclick  = ButtonClear;
		addbutton.onclick    = ButtonAdd;

		//Initialize.
		reloadNodeList('nodelist.json', function () {
			clearNode();
		});
		
		/*function resize() {
			var canvas = document.getElementById('nodecanvas');
			draw.resize(window.innerWidth, window.innerHeight);
		}
		window.addEventListener('resize', resize, false);*/
	}
	
	window.addEventListener('load', init, false);

	
	

}());

