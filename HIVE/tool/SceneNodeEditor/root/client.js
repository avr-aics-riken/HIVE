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
		var node = nodeListTable[nodename],
			instNode;
		if (!node) {
			return;
		}
		nodeData = nui.getNodeData();
		instNode = clone(node);
		console.log(instNode);
		nodeData.nodeData.push(instNode);
		instNode.varname += instance_no;
		instance_no += 1;
		nui.clearNodes();
		nui.makeNodes(nodeData);
	}
	
	function deleteNode(node) {
		console.log('DELETE:', node);
		
		var nodeData = nui.getNodeData(),
			nodeList = nodeData.nodeData,
			i;
		for (i = 0; i < nodeList.length; i = i + 1) {
			if (nodeList[i].varname === node.varname) {
				nodeList.splice(i, 1);
			}
		}
		nui.clearNodes();
		nui.makeNodes(nodeData);
	}
	
	function clearNode() {
		nui.clearNodes();
		nodeData = nui.getNodeData();
		document.getElementById("property").innerHTML = '';
	}
	
	function updateNode() {
		var nodeData = nui.getNodeData();
		nui.clearNodes();
		nui.makeNodes(nodeData);
	}
	
	//
	// TODO: refactro me!!
	//
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
	
	function makeItemNode(name, text, top) {
		var itemNode = document.createElement('div'),
			nameNode = document.createElement('div'),
			textNode = document.createElement('div');

		itemNode.classList.add('nodePropertyRow');
		nameNode.innerHTML = name;
		textNode.innerHTML = text;
		nameNode.classList.add('nodePropertyName');
		textNode.classList.add('nodePropertyConst');
		if (top) {
			nameNode.classList.add('nodePropertyTop');
			textNode.classList.add('nodePropertyTop');
		}
		itemNode.appendChild(nameNode);
		itemNode.appendChild(textNode);
		return itemNode;
	}
	function makeItemTextNode(name, text, node, type) {
		var itemNode = document.createElement('div'),
			nameNode = document.createElement('div'),
			textNode = document.createElement('input');
		if (type) {
			textNode.setAttribute('type', type);
		} else {
			textNode.setAttribute('type', 'text');
		}
		itemNode.classList.add('nodePropertyRow');
		nameNode.innerHTML = '[' + name + ']';
		textNode.value = text;
		nameNode.classList.add('nodePropertyName');
		textNode.classList.add('nodePropertyValue');
		itemNode.appendChild(nameNode);
		itemNode.appendChild(textNode);
		
		textNode.addEventListener('keyup', (function (nodeData, txt) {
			return function (e) {
				nodeData.value = txt.value;
			};
		}(node, textNode)));
		return itemNode;
	}
	function makeItemArrayNumNode(name, text, node) {
		var itemNode = document.createElement('div'),
			nameNode = document.createElement('div'),
			textNode = document.createElement('input');
		
		textNode.setAttribute('type', 'number');
		itemNode.classList.add('nodePropertyRow');
		nameNode.innerHTML = '[' + name + ']';
		textNode.value = text;
		nameNode.classList.add('nodePropertyName');
		textNode.classList.add('nodePropertyValue');
		itemNode.appendChild(nameNode);
		itemNode.appendChild(textNode);
		
		textNode.addEventListener('blur', (function (nodeData, txt) {
			return function (e) {
				// resize array
				var l = parseInt(txt.value, 10),
					i;
				if (!isNaN(l) && l > 0) {
					nodeData.array.length = l;
					for (i = 0; i < l; i = i + 1) {
						if (nodeData.array[i] === undefined) {
							nodeData.array[i] = {};
							nodeData.array[i].name = nodeData.name + i;
							nodeData.array[i].type = nodeData.type;
						}
					}
					updateNode();
				}
			};
		}(node, textNode)));
		return itemNode;
	}
	function makeItemVecNode(name, vals, node, n) {
		var itemNode = document.createElement('div'),
			nameNode = document.createElement('div'),
			valNode,
			i;
		
		itemNode.classList.add('nodePropertyRow');
		nameNode.innerHTML = '[' + name + ']';
		nameNode.classList.add('nodePropertyName');
		itemNode.appendChild(nameNode);

		function valChange(nodeData, txt, i) {
			return function (e) {
				nodeData.value[i] = txt.value;
			};
		}
		for (i = 0; i < n; i = i + 1) {
			valNode = document.createElement('input');
			valNode.setAttribute('type', 'text');
			valNode.value = vals[i];
			valNode.classList.add('nodePropertyValue');
			itemNode.appendChild(valNode);
			valNode.addEventListener('keyup', valChange(node, valNode, i));
		}
		return itemNode;
	}

	function showProparty(nodeData) {
		//console.log(nodeData);
		var to = document.getElementById("property"),
			html = '',
			i,
			k,
			cameradata,
			desc = ['Pos', 'At', 'UP'],
			pxyz = ['X', 'Y', 'Z'],
			index = 0,
			ele,
			prop = document.createElement('div'),
			itemNode,
			inode;
		
		to.innerHTML = ''; // clear
		to.appendChild(makeItemNode('Property Name', 'Value', true));
		to.appendChild(makeItemNode('name', nodeData.name));
		to.appendChild(makeItemNode('varname', nodeData.varname));
		//to.appendChild(makeItemNode('funcname', nodeData.funcname));
		
		to.appendChild(prop);
		//console.log(nodeData.input);
		if (!nodeData.input) {
			return;
		}
		
		function addArrayItem(inode, n) {
			var itemNode;
			if (inode.type === 'string' || inode.type === 'float') {
				itemNode = makeItemTextNode('', inode.array[n], inode);
			} else if (inode.type === 'vec4') {
				itemNode = makeItemVecNode('', inode.array[n], inode, 4);
			} else if (inode.type === 'vec3') {
				itemNode = makeItemVecNode('', inode.array[n], inode, 3);
			} else if (inode.type === 'vec2') {
				itemNode = makeItemVecNode('', inode.array[n], inode, 2);
			} else {
				itemNode = makeItemNode('', '(Object)');
			}
			return itemNode;
		}
		function addItems(inode) {
			var itemNode;
			if (inode.type === 'string' || inode.type === 'float') {
				itemNode = makeItemTextNode(inode.name, inode.value, inode);
			} else if (inode.type === 'vec4') {
				itemNode = makeItemVecNode(inode.name, inode.value, inode, 4);
			} else if (inode.type === 'vec3') {
				itemNode = makeItemVecNode(inode.name, inode.value, inode, 3);
			} else if (inode.type === 'vec2') {
				itemNode = makeItemVecNode(inode.name, inode.value, inode, 2);
			} else {
				itemNode = makeItemNode(inode.name, '(Object)');
			}
			return itemNode;
		}
		for (i = 0; i < nodeData.input.length; i = i + 1) {
			if (nodeData.input.hasOwnProperty(i)) {
				inode = nodeData.input[i];
				if (Array.isArray(inode.array)) {
					itemNode = makeItemArrayNumNode(inode.name, inode.array.length, inode);
					to.appendChild(itemNode);
					for (k = 0; k < inode.array.length; k = k + 1) {
						to.appendChild(addItems(inode.array[k]));
					}
				} else {
					to.appendChild(addItems(inode));
				}
			}
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
		
	}
	
	function ButtonSave(e) {
		//
		// TODO: safari
		//
		var a = document.createElement('a'),
			nodeData = JSON.stringify(nui.getNodeData()),
			href = "data:application/octet-stream," + encodeURIComponent(nodeData);

		a.href = href;
		a.download = 'scenenodes.json';
		a.click();
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
	}

	//---------------------------------------------------------------------------
	// init
	//---------------------------------------------------------------------------
	function init() {
		//create button
		var openbutton   = document.getElementById('Open'),
			savebutton   = document.getElementById('Save'),
			renderbutton = document.getElementById('Render'),
			clearbutton  = document.getElementById('Clear'),
			addbutton    = document.getElementById('Add'),
			nodelistbox  = document.getElementById('NodeList'),

		//init canvas.
			draw = SVG('nodecanvas');//.size(1280, 768);
		nui      = svgNodeUI(draw);
		nui.clearNodes();

		nui.nodeClickEvent(showProparty);
		nui.nodeDeleteEvent(deleteNode);

		//handle
		openbutton.onclick   = ButtonOpen;
		savebutton.onclick   = ButtonSave;
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

