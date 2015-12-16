/*jslint devel:true */

(function () {
	"use strict";

	// Hiveのビューを操作するクラス
	var HiveViewer;

	// コンストラクタ
	HiveViewer = function (target_element, output_callback) {
		this.target_element = target_element;
		target_element.width = target_element.clientWidth;
		target_element.height = target_element.clientHeight;
		this.output_callback = output_callback;
		// Hiveからのコールバック
		this.core = new window.HiveCore(target_element, target_element.width, target_element.height, function (sceneInfo) {
			/*
			if (output_callback) {
				output_callback(null, sceneInfo);
			}
			*/
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
	 * マウスイベントの初期化.
	 */
	HiveViewer.prototype.resize = function () {
		var target_element = this.target_element,
			w = target_element.clientWidth,
			h = target_element.clientHeight;
		target_element.width = w;
		target_element.height = h;
		this.core.resize(w, h);
		this.core.render();
	};

	/**
	 * キャンバスを初期化.
	 */
	function init_canvas(canvas, width, height, output_callback) {
		var viewer = new HiveViewer(canvas, 512, 512, output_callback);
		// 初回レンダー
		setTimeout((function (viewer) {
			return function () {
				viewer.core.render();
			};
		}(viewer)), 1000);
		// リサイズイベント登録.
		window.addEventListener('resize', function (e) {
			viewer.resize()
		});
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
	 * ファイルダイアログの初期化.
	 */
	function init_filedialog(viewer) {
		var setf0_button = document.getElementById("setf0_button"),
			setf1_button = document.getElementById("setf1_button"),
			path = "/",
	 		remote = require('remote'),
			dialog = remote.require('dialog');

		// ファイルダイアログイベントを受け取る
		setf0_button.addEventListener("click", function (e) {
			// electronのファイルダイアログ
			document.getElementById('dialog_box').style.display = "block";
			dialog.showOpenDialog(function (fileNames) {
				console.log("filenames", fileNames);
				document.getElementById('dialog_box').style.display = "none";
			});

			/* HIVE_UIのダイアログ
			var dialog,
				dirfunc = function (path) {
					viewer.core.getFileList(path, function (err, res) {
						dialog.update_dir_list({list: res, path: path}, function (path) {
							dirfunc(path);
						});
					});
				};

			dialog = new window.FileDialog("f0", true, document.getElementById('file_dialog'));
			dialog.set_filter_input(document.getElementById('file_filter_input'));
			dirfunc("/");
			*/
		});
		setf1_button.addEventListener("click", function (e) {
			// electronのファイルダイアログ
			document.getElementById('dialog_box').style.display = "block";
			dialog.showOpenDialog(function (fileNames) {
				console.log("filenames", fileNames);
				document.getElementById('dialog_box').style.display = "none";
			});
		});
	}

	/**
	 * 初期化
	 */
	function init() {
		var left_viewer,
			right_viewer,
			left_output = document.getElementById('left_output'),
			right_output = document.getElementById('right_output');

		// 左右のキャンバスを初期化.
		left_viewer = init_canvas(document.getElementById('left_canvas'), function (err, info) {
			if (err) {
				console.error(err);
				left_output.innerHTML = err + "\n";
			} else if (info) {
				left_output.innerHTML = JSON.stringify(info, null, "    ") + "\n";
			}
		});
		right_viewer = init_canvas(document.getElementById('right_canvas'), function (err, info) {
			if (err) {
				console.error(err);
				right_output.innerHTML = err + "\n";
			} else if (info) {
				right_output.innerHTML = JSON.stringify(info, null, "    ") + "\n";
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

		// ファイルダイアログを初期化
		init_filedialog(right_viewer);

		// バーが動いた時にビューポートをリサイズさせる
		document.getElementById('middle_bar').addEventListener('resize_viewport', function () {
			left_viewer.resize();
			right_viewer.resize();
		});
		document.getElementById('vertical_bar').addEventListener('resize_viewport', function () {
			left_viewer.resize();
			right_viewer.resize();
		});
	}

	window.hive_viewer = {};
	window.hive_viewer.init = init;
}());
