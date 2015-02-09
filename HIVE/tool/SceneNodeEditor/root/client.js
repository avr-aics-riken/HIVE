/*jslint devel:true */
/*global SVG, svgNodeUI, io, fs, URL */

(function () {
	'use strict';
	var nui;
	var nodeData;
	var socket = io.connect();
	var nodelist;

	//---------------------------------------------------------------------------
	// request node info
	//---------------------------------------------------------------------------
	function loadNodeList(url) {
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
				d.onChange = OnSelectNodeList;
			}
		});
	}

	//---------------------------------------------------------------------------
	// buttons
	//---------------------------------------------------------------------------
	function ButtonClear(e) {
		console.log(e);
		nui.clearNodes();
	}

	function ButtonRender(e) {
		var customlua = nui.exportLua();
		console.log(customlua);
		socket.emit('sendScene', JSON.stringify({scene: customlua}));
	}

	function ButtonOpen(e) {
		nodeData = loadNodeList('nodelist.json');
		//nui.clearNodes();
		//nui.makeNodes(nodeData);
	}

	function ButtonAdd(e) {
		var ele = document.getElementsByName('NodeList');
		var index = ele[0].selectedIndex;
		nodeData.nodeData.push(nodelist[index]);
		console.log(nodeData);
		nui.clearNodes();
		nui.makeNodes(nodeData);
	}

	function OnSelectNodeList(e) {
		console.log('SELECT!!!!!!!!!!!!!!!!!!!');
	}

	//---------------------------------------------------------------------------
	// init
	//---------------------------------------------------------------------------
	function init() {
		//init canvas.
		var draw = SVG('nodecanvas').size(1024, 768);
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
		nodeData = {"nodeData":[], "plugData":[]};
		console.log(nodeData);
		loadNodeList('nodelist.json');
	}

	window.onload = init;

}());

