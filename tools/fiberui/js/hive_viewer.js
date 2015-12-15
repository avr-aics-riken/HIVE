/*jslint devel:true */

(function () {
	"use strict";

	// Hiveのビューを操作するクラス
	var HiveViewer;

	// コンストラクタ
	HiveViewer = function (target_element, width, height) {
		this.target_element = target_element;
		target_element.width = width;
		target_element.height = height;
		this.core = new window.HiveCore(target_element, width, height, this.updateObjList);
		this.init_mouse_event(this.core);
		this.reduce_counter = 0;
	};

	// Hiveからのコールバック
	HiveViewer.prototype.updateObjList = function (sceneInfo) {
		console.log(this);
		if (this) {
			//this.core.render();
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
		this.target_element.addEventListener('mousemove', (function (self) {
			return function (e) {
				e.preventDefault();
				var dx = e.clientX - mouseState.x,
					dy = e.clientY - mouseState.y;

				if (self.reduce_counter % 3 === 0) {
					if (mouseState.Left) {
						console.log("rotate");
						core.Rotate(dy * -0.5, dx * -0.5); // Swizzle axis
					} else if (mouseState.Right) {
						console.log("zoom");
						core.Zoom(dx + dy);
					} else if (mouseState.Center) {
						console.log("translate");
						core.Translate(dx, dy);
					}
				}
				self.reduce_counter = self.reduce_counter + 1;
				mouseState.x = e.clientX;
				mouseState.y = e.clientY;
			};
		}(this)));
	};

	/**
	 * キャンバスを初期化.
	 */
	function init_canvas(canvas, width, height) {
		var viewer = new HiveViewer(canvas, 512, 512);
		setTimeout((function (viewer) {
			return function () {
				viewer.core.render();
			};
		}(viewer)), 1000);
		return viewer;
	}

	/**
	 * エディタを初期化.
	 */
	function init_editor(viewer, ace_editor, editor_elem, submit_elem) {
		submit_elem.onclick = function (evt) {
			console.log(ace_editor.getValue());
			viewer.core.runScript(ace_editor.getValue());
		};
	}

	/**
	 * 初期化
	 */
	function init() {
		var left_viewer,
			right_viewer;

		// 左右のキャンバスを初期化.
		left_viewer = init_canvas(document.getElementById('left_canvas'), 512, 512);
		right_viewer = init_canvas(document.getElementById('right_canvas'), 512, 512);

		// 左右のエディタを初期化.
		init_editor(
			left_viewer,
			ace.edit("left_editor"),
			left_editor,
			 document.getElementById('left_editor_submit'));

		init_editor(
			right_viewer,
			ace.edit("right_editor"),
			right_editor,
			document.getElementById('right_editor_submit'));
	}

	window.hive_viewer = {};
	window.hive_viewer.init = init;
}());
