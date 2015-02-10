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
		document.getElementById("proparty").innerHTML = '';
		
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
			draw = SVG('nodecanvas').size(1280, 768);
		nui      = svgNodeUI(draw);
		nui.clearNodes();


		//handle
		openbutton.onclick   = ButtonOpen;
		renderbutton.onclick = ButtonRender;
		clearbutton.onclick  = ButtonClear;
		addbutton.onclick    = ButtonAdd;

		//Initialize.
		reloadNodeList('nodelist.json', function () {
			clearNode();
		});
	}

	window.onload = init;

}());

