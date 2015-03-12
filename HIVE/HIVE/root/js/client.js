/*jslint devel:true*/
/*global HiveCore*/            // HiveCore
/*global kvtoolsUI_init, kUI*/ // kvtoolsUI
/*global $, $toggle*/          // qatrix

(function (window) {
	'use strict';
	function init() {
		kvtoolsUI_init();
		var core = new HiveCore($('result'),
								document.getElementById('window-view').clientWidth,
								document.getElementById('window-view').clientHeight);
	
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
			core.loadObj('bunny.obj', 'normal.frag');
			e.preventDefault();
		});

		//---------------------------
		//  UI Events
		//
		// Timeline events
		kUI('timeline').setTimelineData();
		kUI('timeline').drawGraph();
		kUI('timeline').ChangeTimeCallback(function (tm) {
			kUI('viewmode').SetCaption('[Camera]');
			core.updateTime(tm);
		});
		$('projsetting').addEventListener('click', function (ev) {
			$toggle($('window-projectproperty'), 500);
		});
		$('showconsole').addEventListener('click', function (ev) {
			$toggle($('consoleArea'), 500);
		});
		//----------------------------
	}
	window.addEventListener('load', init);
}(window));