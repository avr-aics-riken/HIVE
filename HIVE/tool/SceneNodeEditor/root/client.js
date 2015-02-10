/*jslint devel:true */
/*global SVG, svgNodeUI, io, fs, URL */

(function () {
	'use strict';
	var nui,
		nodeData,
		nodelist,
		socket = io.connect(),
		instance_no = 1;
	
	function clearNode() {
		nui.clearNodes();
		nodeData = nui.getNodeData();
		document.getElementById("proparty").innerHTML = '';
	}

	//---------------------------------------------------------------------------
	// request node info
	//---------------------------------------------------------------------------
	function reloadNodeList(url) {
		var ret,
			req = new XMLHttpRequest();
		req.open('GET', url);
		req.send();
		req.addEventListener("load", function (ev) {
			var resp = ev.srcElement.responseText,
				json = JSON.parse(resp),
				d    = document.getElementById('NodeListBox'),
				inner,
				i,
				name;
			
			nodelist = json;
			console.log(resp);

			//create list
			if (d) {
				inner = '<select name="NodeList" size=' + json.length + '>';
				for (i = 0; i < json.length; i = i + 1) {
					console.log(json[i]);
					name = json[i].name;
					inner += '<option value="' + name + '">' + name + '</option>';
				}
				inner += '</select>';
				d.innerHTML = inner;
			}
		});
	}

	//---------------------------------------------------------------------------
	// buttons
	//---------------------------------------------------------------------------
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
			node,
			instNode;
		console.log('SELECT INDEX >' + index);
		if (index >= 0) {
			node = nodelist[index];
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
		clearNode();
		reloadNodeList('nodelist.json');
	}

	window.onload = init;

}());

