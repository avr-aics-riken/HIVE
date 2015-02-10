/*jslint devel:true */
/*global SVG, svgNodeUI, io, fs, URL */

(function () {
	'use strict';
	var nui;
	var nodeData;
	var nodelist;
	var socket = io.connect();
	var instance_no = 1;
	
	function ClearNode()
	{
		nui.clearNodes();
		nodeData = nui.getNodeData();
	}

	//---------------------------------------------------------------------------
	// request node info
	//---------------------------------------------------------------------------
	function reloadNodeList(url) {
		var ret;
		var req = new XMLHttpRequest();
		req.open('GET', url);
		req.send();
		req.addEventListener("load",function(ev) {
			var resp = ev.srcElement.responseText;
			var json = JSON.parse(resp);
			nodelist = json;
			console.log(resp);

			//create list 
			var d    = document.getElementById('NodeListBox');
			if(d) {
				var inner = '<select name="NodeList" size=' + json.length + '>';
				for(var i = 0 ; i < json.length; i += 1) {
					console.log(json[i]);
					var name = json[i].name;
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
		if (null == obj || "object" != typeof obj) return obj;
		var copy = obj.constructor();
		for (var attr in obj) {
			if (obj.hasOwnProperty(attr)) copy[attr] = obj[attr];
		}
		return copy;
	}
	
	function ButtonClear(e) {
		ClearNode();
	}

	function ButtonRender(e) {
		var customlua = nui.exportLua();
		console.log(customlua);
		socket.emit('sendScene', JSON.stringify({scene: customlua}));
	}

	function ButtonOpen(e) {
		nodeData = reloadNodeList('nodelist.json');
		//nui.clearNodes();
		//nui.makeNodes(nodeData);
	}

	function ButtonAdd(e) {
		var ele = document.getElementsByName('NodeList');
		var index = ele[0].selectedIndex;
		console.log('SELECT INDEX >' + index);
		if(index >= 0) {
			var node = clone(nodelist[index]);
			var pos  = [ Math.random() * 200 + 200,  Math.random() * 200 + 200];
			node.pos = pos;
			nodeData = nui.getNodeData();
			nodeData.nodeData.push(node);
			nui.clearNodes();
			nui.makeNodes(nodeData);
			console.log(nodeData);
		}
	}

	//---------------------------------------------------------------------------
	// init
	//---------------------------------------------------------------------------
	function init() {
		//init canvas.
		var draw = SVG('nodecanvas').size(1280, 768);
		nui      = svgNodeUI(draw);
		nui.clearNodes();

		//create button
		var openbutton   = document.getElementById('Open'),
			renderbutton = document.getElementById('Render'),
			clearbutton  = document.getElementById('Clear'),
			addbutton    = document.getElementById('Add'),
			nodelistbox  = document.getElementById('NodeList');

		//handle
		openbutton.onclick   = ButtonOpen;
		renderbutton.onclick = ButtonRender;
		clearbutton.onclick  = ButtonClear;
		addbutton.onclick    = ButtonAdd;

		//Initialize.
		ClearNode()
		reloadNodeList('nodelist.json');
	}

	window.onload = init;

}());

