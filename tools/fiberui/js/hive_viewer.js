/*jslint devel:true */

(function () {
	"use strict";

	// Hiveのビューを操作するクラス
	var HiveViewer,
		dummy_canvas = document.createElement("canvas"),
		current_colormap;

	// コンストラクタ
	HiveViewer = function (target_element, event_element, output_callback, ws, ipc) {
		this.target_element = target_element;
		target_element.width = target_element.clientWidth;
		target_element.height = target_element.clientHeight;
		this.output_callback = output_callback;
		// Hiveからのコールバック
		this.core = new window.HiveCore(target_element,
			target_element.width,
			target_element.height, function (sceneInfo) {},
			ws,
			 true, // opengl Mode
			 ipc);
		this.init_mouse_event(this.core, event_element);
		this.reduce_counter = 0;
	};

	/**
	 * マウスイベントの初期化.
	 */
	HiveViewer.prototype.init_mouse_event = function (core, event_element) {
		var mouseState = {"Left": false, "Center": false, "Right": false, "x": 0, "y": 0 };

		event_element.addEventListener('mousedown', function (e) {
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
		event_element.addEventListener('mousemove', (function (self) {
			return function (e) {
				e.preventDefault();
				var dx = e.clientX - mouseState.x,
					dy = e.clientY - mouseState.y;

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
				self.reduce_counter = self.reduce_counter + 1;
				mouseState.x = e.clientX;
				mouseState.y = e.clientY;
			};
		}(this)));
	};

	/**
	 * リサイズ.
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
	function init_canvas(canvas, top_element, output_callback, ws, ipc) {
		var viewer = new HiveViewer(canvas, top_element, output_callback, ws, ipc);
		// 初回レンダー
		setTimeout((function (viewer) {
			return function () {
				// カラーマップのセット。
				viewer.core.setColorMapTex("colormap_tex", 256, 1, get_colormap_rgba(current_colormap, 256, 1));

				viewer.core.render();
			};
		}(viewer)), 1000);
		// リサイズイベント登録.
		window.addEventListener('resize', function (e) {
			viewer.resize();
		});
		return viewer;
	}

	/**
	 * エディタを初期化.
	 */
	function init_editor(viewer, ace_editor, editor_elem, submit_elem) {
		submit_elem.onclick = function (evt) {
			// スクリプト実行。
			viewer.core.runScript(ace_editor.getValue(), function (err, res) {
				console.log(err, res);
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
	 * カラーマップのRGBA値を配列として返す。
	 */
	function get_colormap_rgba(color_steps, width, height) {
		var i,
			grad,
			context = dummy_canvas.getContext("2d");
		dummy_canvas.width = width;
		dummy_canvas.height = height;
		// すべてクリア
		context.clearRect(0, 0, width, height);
		// グラデーションを作成
		grad  = context.createLinearGradient(0, 0, width, 0);
		for (i = 0; i < color_steps.length; i = i + 1) {
			grad.addColorStop(color_steps[i].step, color_steps[i].color);
		}
		context.fillStyle = grad;
		context.beginPath();
		context.fillRect(0, 0, width, height);
		context.closePath();
		context.fill();
		//console.log(context.getImageData(0, 0, width, height));
		return context.getImageData(0, 0, width, height).data;
	}

	/**
	 * 初期化
	 */
	function init(initial_colormap) {
		var left_viewer,
			right_viewer,
			left_editor,
			right_editor,
			left_output = document.getElementById('left_output'),
			right_output = document.getElementById('right_output'),
			rndnumLeft = Math.floor(Math.random()*10000),
			rndnumRight = Math.floor(Math.random()*10000);


		current_colormap = initial_colormap;

		// 左右のキャンバスを初期化.
		left_viewer = init_canvas(
			document.getElementById('left_canvas'),
			document.getElementById('left_canvas'),
			function (err, info) {
				if (err) {
					console.error(err);
					left_output.innerHTML = err + "\n";
				} else if (info) {
					left_output.innerHTML = JSON.stringify(info, null, "    ") + "\n";
				}
			},
			'ws://localhost:8080',
			'ipc:///tmp/HiveUI_ipc_left' + rndnumLeft
		);
		right_viewer = init_canvas(
			document.getElementById('right_canvas'),
			document.getElementById('svg'),
			function (err, info) {
				if (err) {
					console.error(err);
					right_output.innerHTML = err + "\n";
				} else if (info) {
					right_output.innerHTML = JSON.stringify(info, null, "    ") + "\n";
				}
			},
			'ws://localhost:8080',
			'ipc:///tmp/HiveUI_ipc_right' + rndnumRight
		);

		// 左右のエディタを初期化.
		init_editor(
			left_viewer,
			ace.edit("left_editor"),
			left_editor,
			document.getElementById('left_editor_submit')
		);

		init_editor(
			right_viewer,
			ace.edit("right_editor"),
			right_editor,
			document.getElementById('right_editor_submit')
		);

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

		// カラーマップが変更された時にカラーマップをHIVEに登録する.
		document.getElementById('color_map').addEventListener('change_colormap', function (evt) {
			current_colormap = JSON.parse(JSON.stringify(evt.data));
			right_viewer.core.setColorMapTex("colormap_tex", 256, 1, get_colormap_rgba(current_colormap, 256, 1));
		});
	}

	window.hive_viewer = {};
	window.hive_viewer.init = init;
}());
