/*jslint devel:true */
/*global SVG, svgNodeUI, io, fs, URL, FileReader */
(function () {
	'use strict';
	var nui,
		nodeData,
		nodeList,
		nodeListTable,
		socket = io.connect(),
		instance_no = 1,
		popupNodeList = null,
		consoleTab,
		propertyTab;
	
	socket.on('stdout', function (data) {
		var output = document.getElementById('consoleOutput'),
			text = document.getElementById('consoleTextBlock');
		text.innerHTML = text.innerHTML + "<pre class='stdout'>" + data + "</pre>";
		output.scrollTop = output.scrollHeight;
	});
	
	socket.on('stderr', function (data) {
		var output = document.getElementById('consoleOutput'),
			text = document.getElementById('consoleTextBlock');
		text.innerHTML = text.innerHTML + "<pre class='stderr'>" + data + "</pre>";
		output.scrollTop = output.scrollHeight;
	});
	
	function clearConsoleOutput() {
		var text = document.getElementById('consoleTextBlock');
		text.innerHTML = "";
	}

	function clone(obj) {
		return JSON.parse(JSON.stringify(obj));
		
		// TODO: recursive
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
	
	function addNode(nodename, nx, ny) {
		var node = nodeListTable[nodename],
			instNode;
		if (!node) {
			return;
		}
		nodeData = nui.getNodeData();
		instNode = clone(node);
		//console.log(instNode);
		nodeData.nodeData.push(instNode);
		instNode.varname += instance_no;
		instance_no += 1;
		if (nx !== undefined && ny !== undefined) {
			instNode.pos[0] = nx;
			instNode.pos[1] = ny;
		}
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
	
	//-----------------------------------------------------------
	
	//<div class="menuButtonClass noneselect">Add</div>
	function buttonAdd(lst, cb, nx, ny) {
		return function (e) {
			var index = lst.selectedIndex,
				text,
				node,
				instNode;
			if (index === -1) {
				return;
			}
			text = lst.options[index].text;
			if (nx === undefined || ny === undefined) {
				// center
				addNode(text, window.innerWidth / 4, window.innerHeight / 4);
			} else {
				// specific pos
				addNode(text, nx, ny);
			}
			if (cb) {
				cb();
			}
		};
	}
	
	function updateNodeList(lst, txtval) {
		var i, name, visible, item;
		lst.innerHTML = ''; // clear
		for (i in nodeListTable) {
			if (nodeListTable.hasOwnProperty(i)) {
				//console.log(nodeListTable[i]);
				name = nodeListTable[i].name;
				visible = nodeListTable[i].visible;
				
				if ((txtval === '' || name.toLowerCase().indexOf(txtval.toLocaleLowerCase()) >= 0) && visible !== false) {
					item = document.createElement('option');
					item.setAttribute('value', name);
					item.appendChild(document.createTextNode(name));
					lst.appendChild(item);
				}
			}
		}
	}
	
	function createNodeListUI(addcallback, mx, my) {
		var tray = document.createElement('div'),
			addbtn = document.createElement('div'),
			txt = document.createElement('input'),
			lst = document.createElement('select'),
			item,
			name,
			i,
			width = '150px';
		
		addbtn.addEventListener('click', buttonAdd(lst, addcallback, mx, my));
		addbtn.classList.add('menuButtonClass');
		addbtn.classList.add('noneselect');
		addbtn.classList.add('AddButton');
		addbtn.innerHTML = 'Add';
		tray.appendChild(addbtn);
		tray.appendChild(txt);
		tray.appendChild(document.createElement('div'));
		tray.appendChild(lst);
		txt.setAttribute('type', 'input');
		txt.setAttribute('placeholder', 'filter...');
		txt.style.width = width;
		lst.style.width = width;
		lst.setAttribute('size', 15);
		lst.setAttribute('name', 'NodeList');
		
		txt.timer    = null;
		txt.prev_val = txt.value;
		txt.new_val  = '';
		txt.addEventListener("focus", (function (lst, txt) {
			return function () {
				window.clearInterval(txt.timer);
				txt.timer = window.setInterval(function () {
					txt.new_val = txt.value;
					if (txt.prev_val !== txt.new_val) {
						updateNodeList(lst, txt.new_val);
					}
					txt.prev_val = txt.new_val;
				}, 10);
			};
		}(lst, txt)), false);
		txt.addEventListener("blur", (function (lst, txt) {
			return function () {
				window.clearInterval(txt.timer);
			};
		}(lst, txt)), false);
		
		updateNodeList(lst, ''); // show AllNode
		
		return tray;
	}
	
	//----------------------------------------------------
	
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
		textNode.classList.add('nodePropertyText');
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
	
	
	
	function colorFunction(type) {
		if (type === "string") {
			return "#14a271";
		} else if (type === "float") {
			return "#139aa5";
		} else if (type === "vec4") {
			return "#1b6ad6";
		} else if (type === "vec3") {
			return "#566f9f";
		} else if (type === "vec2") {
			return "#8222a7";
		} else if (type === "RenderObject") {
			return "#ad3b78";
		} else if (type === "Uniform") {
			return "#b19e14";
		} else if (type === "BufferMeshData") {
			return "#be1656";
		} else if (type === "BufferPointData") {
			return "#e023e0";
		} else if (type === "BufferLineData") {
			return "#20cae0";
		} else if (type === "BufferVolumeData") {
			return "#17d042";
		} else if (type === "Any") {
			return "#ef8815";
		} else { // Object
			return "#c12417";
		}
	}

	//---------------------------------------------------------------------------
	// request node info
	//---------------------------------------------------------------------------
	
	function storeNodeList(resp, callback) {
		var i;

		// store node list
		nodeList = JSON.parse(resp);
		
		// sort list
		nodeList.sort(
			function (a, b) {
				return a.name > b.name;
			}
		);

		// create nodelist table
		nodeListTable = {};
		for (i = 0; i < nodeList.length; i = i + 1) {
			nodeListTable[nodeList[i].name] = nodeList[i];
		}

		if (callback) {
			callback();
		}
	}
	
	function reloadNodeList(url, cb) {
		var ret,
			req = new XMLHttpRequest();
		req.open('GET', url);
		req.send();
		req.addEventListener("load", (function (req, callback) {
			return function (ev) {
				var resp = req.responseText;// ev.srcElement.responseText;
				storeNodeList(resp, callback);
			};
		}(req, cb)));
	}

	//---------------------------------------------------------------------------
	// buttons
	//---------------------------------------------------------------------------
	
	function ButtonClear(e) {
		clearNode();
		clearConsoleOutput();
	}

	function ButtonRender(e) {
		console.log(propertyTab);
		consoleTab(true);
		var customlua = nui.exportLua();
		console.log(customlua);
		clearConsoleOutput();
		socket.emit('sendScene', JSON.stringify({scene: customlua}));
	}

	function ButtonOpen(evt) {
		var files = evt.target.files,
			i,
			f,
			reader,
			data,
			nodedata;
		console.log(files);
		function readfile(theFile) {
			return function (e) {
				data = e.target.result;
				nodedata = JSON.parse(data);
				nui.clearNodes();
				nui.makeNodes(nodedata);
			};
		}
		for (i = 0; i < files.length; i = i + 1) {
			f = files[i];
			reader = new FileReader();
			reader.onload = readfile(f);
			reader.readAsText(f);
		}
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
	
	function showAddNodeMenu(show, sx, sy, popupmode) {
		var callback = null;
		if (show === true) {
			if (popupmode) {
				callback = function () {
					document.body.removeChild(popupNodeList);
					popupNodeList = null;
				};
			}
			popupNodeList = createNodeListUI(callback, sx, sy);
			popupNodeList.setAttribute('style', 'position:absolute;top:' + sy + 'px;left:' + sx + 'px');
			document.body.appendChild(popupNodeList);
			
			// filter focus
			if (popupmode) {
				popupNodeList.children[1].focus();
			}
		} else {
			if (popupNodeList !== null) {
				document.body.removeChild(popupNodeList);
			}
			popupNodeList = null;
		}
	}
	
	function DblClickCanvas(e) {
		showAddNodeMenu(true, e.clientX, e.clientY, true);
	}
	function ClickCanvas(e) {
		//console.log(e.clientX, e.clientY);
		showAddNodeMenu(false);
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
			nodecanvas   = document.getElementById('nodecanvas'),

		//init canvas.
			draw = SVG('nodecanvas');//.size(1280, 768);
		nui      = svgNodeUI(draw);
		nui.clearNodes();

		nui.setTypeColorFunction(colorFunction);
		nui.nodeClickEvent(showProparty);
		nui.nodeDeleteEvent(deleteNode);

		// Create Tab
		consoleTab = window.animtab.create('bottom', {
				'bottomTab' : { min : '10px', max : '400' },
			}, {
				'consoleOutput' : { min : '0px', max : '400px' }
			}, 'console');

		propertyTab = window.animtab.create('right', {
				'rightTab' : { min : '0px', max : 'auto' },
			}, {
				'menuTab' : { min : '0px', max : '300px' },
			}, 'Property');
		
		//handle
		openbutton.onchange  = ButtonOpen;
		savebutton.onclick   = ButtonSave;
		renderbutton.onclick = ButtonRender;
		clearbutton.onclick  = ButtonClear;
		
		nodecanvas.ondblclick = DblClickCanvas;
		nodecanvas.onclick = ClickCanvas;
		
		//Initialize.
		reloadNodeList('nodelist.json', function () {
			var headerNode = null,
				footerNode = null,
				d = document.getElementById('NodeListBox');

			//create List UI
			d.appendChild(createNodeListUI());

			// set header/footer
			headerNode = nodeListTable.header;
			footerNode = nodeListTable.footer;
			if (headerNode) {
				nui.setHeaderCode(headerNode.customfunc);
			}
			if (footerNode) {
				nui.setFooterCode(footerNode.customfunc);
			}

			clearNode();
		});
		
	}
	
	// disable right click menu
	document.oncontextmenu = function () {
		return false;
	};
	
	
	// start up
	window.onload = init;
	
}());

