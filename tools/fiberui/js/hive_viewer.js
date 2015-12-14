/*jslint devel:true */

(function () {
	"use strict";
	var HiveViewer;

	// コンストラクタ
	HiveViewer = function (target_element, width, height) {
		this.target_element = target_element;
		target_element.width = width;
		target_element.height = height;
		this.core = new window.HiveCore(target_element, width, height, this.updateObjList);
		this.init_mouse_event(this.core);
	};

	// Hiveからのコールバック
	HiveViewer.prototype.updateObjList = function (sceneInfo) {
		console.log(this);
		if (this) {
			this.core.render();
		}
	};

	/**
	 * マウスイベントの初期化.
	 */
	HiveViewer.prototype.init_mouse_event = function (core) {
		var mouseState = {"Left": false, "Center": false, "Right": false, "x": 0, "y": 0 };

		this.target_element.addEventListener('mousedown', function (e) {
			e.preventDefault();
			if (e.button === 0) { mouseState.Left   = true; }
			if (e.button === 2) { mouseState.Right  = true; }
			if (e.button === 1) { mouseState.Center = true; }
			mouseState.x = e.clientX;
			mouseState.y = e.clientY;
		});
		window.addEventListener('mouseup', function (e) {
			e.preventDefault();
			if (e.button === 0) { mouseState.Left   = false; }
			if (e.button === 2) { mouseState.Right  = false; }
			if (e.button === 1) { mouseState.Center = false; }
			mouseState.x = e.clientX;
			mouseState.y = e.clientY;
		});
		this.target_element.addEventListener('mousemove', function (e) {
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
		});
	};

	/**
	 * 左右のキャンバスを初期化.
	 */
	function init() {
		var left_viewer,
			right_viewer,
			left_canvas = document.getElementById('left_canvas'),
			right_canvas = document.getElementById('right_canvas');

		left_viewer = new HiveViewer(left_canvas, 512, 512);
		//right_viewer = new HiveViewer(right_canvas, 512, 512);

		setTimeout(function () {
			left_viewer.core.render();
		}, 1000);
	}

	window.hive_viewer = {};
	window.hive_viewer.init = init;
}());
