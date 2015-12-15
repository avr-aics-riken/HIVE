/*jslint devel:true */

(function () {
	"use strict";

	// Hiveのビューを操作するクラス
	var HiveViewer;

	// コンストラクタ
	HiveViewer = function (target_element, width, height, output_callback) {
		this.target_element = target_element;
		target_element.width = width;
		target_element.height = height;
		this.output_callback = output_callback;
		// Hiveからのコールバック
		this.core = new window.HiveCore(target_element, width, height, function (sceneInfo) {
			if (output_callback) {
				output_callback(null, sceneInfo);
			}
		});
		this.init_mouse_event(this.core);
		this.reduce_counter = 0;
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

				//if (self.reduce_counter % 3 === 0) {
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
				//}
				self.reduce_counter = self.reduce_counter + 1;
				mouseState.x = e.clientX;
				mouseState.y = e.clientY;
			};
		}(this)));
	};

	/**
	 * キャンバスを初期化.
	 */
	function init_canvas(canvas, width, height, output_callback) {
		var viewer = new HiveViewer(canvas, 512, 512, output_callback);
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
			viewer.core.runScript(ace_editor.getValue(), function (err, res) {
				if (viewer.output_callback) {
					viewer.output_callback(err, res);
				}
				document.getElementById('output_tab').click();
			});
		};
	}

	/**
	 * 初期化
	 */
	function init() {
		var left_viewer,
			right_viewer,
			left_output = document.getElementById('left_output'),
			right_output = document.getElementById('right_output'),

		// 左右のキャンバスを初期化.
		left_viewer = init_canvas(document.getElementById('left_canvas'), 512, 512, function (err, info) {
			if (err) {
				console.error(err);
				left_output.innerHTML = err + "\n";
			}
			/*
			if (info) {
				left_output.innerHTML = left_output.innerHTML + JSON.stringify(info, null, "    ") + "\n";
			}
			*/
		});
		right_viewer = init_canvas(document.getElementById('right_canvas'), 512, 512, function (err, info) {
			if (err) {
				console.error(err);
				right_output.innerHTML = err + "\n";
			}
			if (info) {
				right_output.innerHTML = right_output.innerHTML + JSON.stringify(info, null, "    ") + "\n";
			}
		});

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
