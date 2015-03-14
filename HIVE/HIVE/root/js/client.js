/*jslint devel:true*/
/*global HiveCore*/            // HiveCore
/*global kvtoolsUI_init, kUI*/ // kvtoolsUI
/*global $, $toggle*/          // qatrix
/*global */

(function (window) {
	'use strict';
	
	function updateProperty(objname) {
		
	}
	
	function objectClick(core, objname) {
		return function () {
			updateProperty(objname);
		};
	}
	function dustClick(core, objname) {
		return function () {
			console.log('dust:', objname);
			core.deleteObject(objname);
		};
	}
	
	function init() {
		kvtoolsUI_init();
		var mouseState = {"Left": false, "Center": false, "Right": false, "x": 0, "y": 0 },
			core;
		
		function updateObjList(sceneInfo) {
			console.log(sceneInfo);
			var i,
				objlist = sceneInfo.objectlist,
				lst = kUI('list-itemlist'),
				dust,
				itemid;
			lst.Clear();
			for (i = 0; i < objlist.length; i = i + 1) {
				//objList[i].name, objList[i].type
				itemid = 'listitem-' + objlist[i].name;
				lst.AddItem(objlist[i].name, itemid);
				$(itemid).addEventListener('click', objectClick(core, objlist[i].name));
				$(itemid + '-dustbtn').addEventListener('click', dustClick(core, objlist[i].name));
			}
			core.render();
		}
		
		core = new HiveCore($('result'),
							document.getElementById('window-view').clientWidth,
							document.getElementById('window-view').clientHeight,
							updateObjList);
	
		// Initial Resize
		// Resize Event
		function canvasResizing() {
			var w = document.getElementById('window-view').clientWidth,
				h = document.getElementById('window-view').clientHeight,
				rc = document.getElementById('result');
			rc.width        = w;
			rc.height       = h;
			rc.style.width  = w + 'px';
			rc.style.height = h + 'px';
			console.log('resize:', w, h);
			core.resize(w, h);
			core.render();
		}
		window.addEventListener('resize', canvasResizing);

		// disable context menu
		window.addEventListener('contextmenu', function (e) {
			//console.log('context');
			//core.loadOBJ('bunny.obj', 'normal.frag');
			//core.render();
			e.preventDefault();
		});

		//---------------------------
		//  UI Events
		//
		// Timeline events
		kUI('timeline').setTimelineData();
		kUI('timeline').drawGraph();
		kUI('timeline').ChangeTimeCallback(function (tm) {
			//kUI('viewmode').SetCaption('[Camera]');
			core.updateTime(tm);
		});
		$('projsetting').addEventListener('click', function (ev) {
			$toggle($('window-projectproperty'), 500);
		});
		$('showconsole').addEventListener('click', function (ev) {
			$toggle($('consoleArea'), 500);
		});
		$('newscenebtn').addEventListener('click', function (ev) {
			ev.preventDefault();
			core.newScene();
		});
		$('loadscenebtn').addEventListener('click', function (ev) {
			core.loadOBJ('bunny.obj', 'normal.frag');
			core.render();
		});
		$('list-cameraaddbutton').addEventListener('click', function (ev) {
			core.addCamera();
		});

		//$('savescenebtn').addEventListener('click', function (ev) {
			//core.getSceneInformation(function (objList) {
			//);
		//});

		// ----------------------------
		// Mouse Event
		function mouseDown(e) {
			e.preventDefault();
			if (e.button === 0) { mouseState.Left   = true; }
			if (e.button === 2) { mouseState.Right  = true; }
			if (e.button === 1) { mouseState.Center = true; }
			mouseState.x = e.clientX;
			mouseState.y = e.clientY;
		}

		function mouseUp(e) {
			e.preventDefault();
			if (e.button === 0) { mouseState.Left   = false; }
			if (e.button === 2) { mouseState.Right  = false; }
			if (e.button === 1) { mouseState.Center = false; }
			mouseState.x = e.clientX;
			mouseState.y = e.clientY;
		}

		function mouseMove(e) {
			e.preventDefault();
			var dx = e.clientX - mouseState.x,
				dy = e.clientY - mouseState.y;

			if (mouseState.Left) {
				core.Rotate(dy * -0.5, dx * -0.5); // Swizzle axis
			}
			if (mouseState.Right) {
				core.Zoom(dx + dy);
			}
			if (mouseState.Center) {
				core.Translate(dx, dy);
			}
			mouseState.x = e.clientX;
			mouseState.y = e.clientY;
		}
		$('resultdiv').addEventListener('mousedown', mouseDown);
		$('resultdiv').addEventListener('mouseup',   mouseUp);
		$('resultdiv').addEventListener('mousemove', mouseMove);
		//----------------------------
	}
	window.addEventListener('load', init);
}(window));