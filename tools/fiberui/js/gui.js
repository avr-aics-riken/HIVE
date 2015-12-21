/*jslint devel:true */

(function () {
	"use strict";

	// ビューポートのリサイズが必要なときに投げるイベント.
	var resize_viewport_event = document.createEvent('UIEvents'),
	// カラーマップ変更時に投げるイベント.
		colormap_change_event = document.createEvent('UIEvents');

	resize_viewport_event.initEvent('resize_viewport', false, false);
	colormap_change_event.initEvent('change_colormap', false, false);

	/**
	 * エディタの初期化.
	 */
	function init_editor() {
		var right_editor = ace.edit("right_editor"),
			left_editor = ace.edit("left_editor");

		right_editor.setTheme("ace/theme/merbivore");
		left_editor.setTheme("ace/theme/merbivore");

		right_editor.getSession().setMode("ace/mode/lua");
		left_editor.getSession().setMode("ace/mode/lua");
	}

	function resize_editor() {
		var right_editor = ace.edit("right_editor"),
			left_editor = ace.edit("left_editor");
		right_editor.resize();
		left_editor.resize();
	}

	/**
	 * 中央のバーの初期化.
	 * ドラッグにより上下の区画の比率を変える
	 */
	function init_middle_bar() {
		var bar = document.getElementById('middle_bar'),
			upper_area = document.getElementById('upper_area'),
			lower_area = document.getElementById('lower_area'),
			is_mouse_down = false;
		bar.addEventListener('mousedown', function (evt) {
			is_mouse_down = true;
		});
		window.addEventListener('mouseup', function (evt) {
			if (is_mouse_down) {
				// エディタのリサイズ
				resize_editor();
				// resizeイベントを投げる.
				bar.dispatchEvent(resize_viewport_event);
			}
			is_mouse_down = false;
		});
		window.addEventListener('mousemove', function (evt) {
			if (is_mouse_down) {
				(function () {
					var offset = window.pageYOffset || document.documentElement.scrollTop,
						page_height = document.body.scrollHeight || document.documentElement.scrollHeight,
						pos = offset + evt.clientY,
						ratio = pos / page_height,
						percent,
						resize_event;
					//console.log(pos, page_height, ratio);
					if (ratio > 0.1 && ratio < 0.9) {
						percent = ratio * 100;
						upper_area.style.height = String(percent) + "%";
						lower_area.style.height = String(100 - percent) + "%";
						// resizeイベントを投げる.
						bar.dispatchEvent(resize_viewport_event);
					}
				}());
			}
		});
	}

	/**
	 * 下部のバーの初期化.
	 * ドラッグにより上下の区画の比率を変える
	 */
	function init_bottom_bar() {
		var bar = document.getElementById('bottom_bar'),
			lower_area = document.getElementById('lower_area'),
			editor_box = document.getElementById('editor_box'),
			shell_box = document.getElementById('shell_box'),
			is_mouse_down = false;
		bar.addEventListener('mousedown', function (evt) {
			is_mouse_down = true;
		});
		window.addEventListener('mouseup', function (evt) {
			if (is_mouse_down) {
				// エディタのリサイズ
				resize_editor();
			}
			is_mouse_down = false;
		});
		window.addEventListener('mousemove', function (evt) {
			if (is_mouse_down) {
				(function () {
					var area_height = lower_area.scrollHeight,
						pos = evt.clientY - lower_area.getBoundingClientRect().top,
						ratio = pos / area_height,
						percent;
					//console.log(pos, area_height, ratio);
					if (ratio > 0.1 && ratio < 0.9) {
						percent = ratio * 100;
						editor_box.style.height = String(percent) + "%";
						shell_box.style.height = String(100 - percent) + "%";
					}
				}());
			}
		});
	}

	/**
	 * 上部のバーの初期化.
	 * ドラッグにより左右の区画の比率を変える
	 */
	function init_vertical_bar() {
		var bar = document.getElementById('vertical_bar'),
			left_box = document.getElementById('upper_left_box'),
			right_box = document.getElementById('upper_right_box'),
			is_mouse_down = false;
		bar.addEventListener('mousedown', function (evt) {
			is_mouse_down = true;
		});
		window.addEventListener('mouseup', function (evt) {
			if (is_mouse_down) {
				// resizeイベントを投げる.
				bar.dispatchEvent(resize_viewport_event);
			}
			is_mouse_down = false;
		});
		window.addEventListener('mousemove', function (evt) {
			if (is_mouse_down) {
				(function () {
					var offset = window.pageXOffset || document.documentElement.scrollLeft,
						page_width = document.body.scrollWidth || document.documentElement.scrollWidth,
						pos = offset + evt.clientX,
						ratio = pos / page_width,
						percent;
					//console.log(pos, page_width, ratio);
					if (ratio > 0 && ratio < 0.75) {
						percent = ratio * 100;
						left_box.style.width = String(percent) + "%";
						bar.style.left = String(percent) + "%";
						right_box.style.width = String(100 - percent) + "%";
						// resizeイベントを投げる.
						bar.dispatchEvent(resize_viewport_event);
					}
				}());
			}
		});
	}

	/**
	 * カラーマップが変更された時にイベントを送る.
	 */
	function send_colormap_change_event(color_steps) {
		var color_map = document.getElementById("color_map");
		colormap_change_event.data = color_steps;
		color_map.dispatchEvent(colormap_change_event);
	}

	/**
	 * カラーマップの描画
	 */
	function draw_color_map(context, grad_param, color_steps) {
		var grad_width = grad_param.grad_width,
			grad_height = grad_param.grad_height,
			width = grad_param.width,
			height = grad_param.height,
			padding = grad_param.padding,
			grad,
			i,
			step;

		// すべてクリア
		context.clearRect(0, 0, width, height);

		// グラデーションを作成
		grad  = context.createLinearGradient(0, 0, 0, height);
		for (i = 0; i < color_steps.length; i = i + 1) {
			grad.addColorStop(color_steps[i].step, color_steps[i].color);
		}
		context.fillStyle = grad;
		context.beginPath();
		context.fillRect(0, padding, grad_width, grad_height);
		context.closePath();

		context.lineWidth = 1.0;
		context.strokeStyle = 'gray';
		// グラデーション操作用カーソル描画
		for (i = 0; i < color_steps.length; i = i + 1) {
			step = color_steps[i].step;
			context.beginPath();
			context.moveTo(grad_width, step * grad_height + padding);
			context.lineTo(grad_width + 5, step * grad_height);
			context.lineTo(width, step * grad_height);
			context.lineTo(width, step * grad_height + padding * 2);
			context.lineTo(grad_width + 5, step * grad_height + padding * 2);
			context.lineTo(grad_width, step * grad_height + padding);
			context.closePath();
			context.fill();
			context.stroke();
		}
	}

	/**
	 * カラーマップのステップの削除.
	 */
	function delete_color_map_step(context, grad_param, color_steps, step_index) {
		color_steps.splice(step_index, 1);
		draw_color_map(context, grad_param, color_steps);
		send_colormap_change_event(color_steps);
	}

	/**
	 * カラーマップの初期化
	 */
	 function init_color_map(initial_colormap) {
		 var color_map = document.getElementById("color_map"),
		 	context = color_map.getContext("2d"),
			width = 30, // カラーマップ全体の幅（矢印含む
			height = 256 + 10, // カラーマップ全体の高さ（矢印含む
			color_steps = initial_colormap,
			grad_param = {
				grad_width : 18,
				grad_height : 256,
				width : 35,
				height : 256 + 10,
				padding : 5
			},
			dragging_step = null,
			i,
			step;

		color_map.width = width;
		color_map.height = height;
		// 初回描画
		draw_color_map(context, grad_param, color_steps);

		// マウス位置にあるstepを探してインデックスを返す.
		function pick_step (e) {
			var rect = color_map.getBoundingClientRect(),
				px = e.clientX - rect.left,
				py = e.clientY - rect.top,
				arrow = {},
				setting_step;
			for (i = 0; i < color_steps.length; i = i + 1) {
				step = color_steps[i].step;
				arrow.x = 0;
				arrow.y = step * grad_param.grad_height - (grad_param.padding + 2);
				arrow.w = grad_param.width;
				arrow.h =  (grad_param.padding + 2) * 2;
				if (arrow.x < px && px < (arrow.x + arrow.w) &&
					arrow.y < py && py < (Math.max(arrow.y, 0) + arrow.h)) {
					return i;
				}
			}
			return -1;
		}

		// ダブルクリック(色の設定).
		color_map.addEventListener('dblclick', function (e) {
			var picker = document.getElementById('colorpicker'),
				step_index = pick_step(e);
			picker.click();
			picker.onchange = function (evt) {
				color_steps[step_index].color = picker.value;
				draw_color_map(context, grad_param, color_steps);
				send_colormap_change_event(color_steps);
			}
		});

		color_map.addEventListener('mousedown', function (e) {
			var rect = color_map.getBoundingClientRect(),
				px = e.clientX - rect.left,
				py = e.clientY - rect.top,
				deleting_step,
				step_index = pick_step(e);

			if (step_index >= 0) {
				if (e.button == 0) {
					// 左クリック(移動開始)
					dragging_step = color_steps[i];
				} else {
					// 右クリックなど(削除)
					deleting_step = color_steps[i];
					delete_color_map_step(context, grad_param, color_steps, i);
					return;
				}
			}

			// 新規追加
			if (!dragging_step && !deleting_step) {
				var step = Math.min(Math.max(py / grad_param.grad_height, 0.0), 1.0);
				color_steps.push({ step : step, color : "rgb(255, 255, 255)"});
				draw_color_map(context, grad_param, color_steps);
				dragging_step = color_steps[color_steps.length - 1];
				send_colormap_change_event(color_steps);
			}
		});
		window.addEventListener('mousemove', function (e) {
			var rect = color_map.getBoundingClientRect(),
				px = e.clientX - rect.left,
				py = e.clientY - rect.top;
			if (dragging_step) {
				dragging_step.step = Math.min(Math.max(py / grad_param.grad_height, 0.0), 1.0); // clamp
				draw_color_map(context, grad_param, color_steps);
			}
		});
		window.addEventListener('mouseup', function () {
			if (dragging_step) {
				dragging_step = null;
				send_colormap_change_event(color_steps);
			}
		});

		// 初回に1回イベントを投げる。
		send_colormap_change_event(color_steps);
	}

	/**
	 * ファイルダイアログをキャンセルして閉じる
	 */
	function cancel_file_dialog() {
		var dialog_box = document.getElementById('dialog_box');
		dialog_box.style.display = "none";
	}

	/**
	 * ファイルダイアログを開く
	 */
	function open_file_dialog() {
		var dialog_box = document.getElementById('dialog_box'),
			cancel_button = document.getElementById("file_dialog_cancel_button");
		dialog_box.style.display = "block";
		cancel_button.onclick = function(e) {
			cancel_file_dialog();
		};
	}

	/**
	 * ボタンの初期化
	 */
	function init_button() {
		var setf0_button = document.getElementById("setf0_button"),
			setf1_button = document.getElementById("setf1_button"),
			pressure_button = document.getElementById("pressure_button"),
			temperature_button = document.getElementById("temperature_button");

		setf0_button.addEventListener("click", function (e) {
			open_file_dialog();
		});
		setf1_button.addEventListener("click", function (e) {
			open_file_dialog();
		});
	}

	/**
	 * タブ切り替え
	 */
	function init_bottom_tab() {
		var output = document.getElementById('output_tab'),
			shell = document.getElementById('shell_tab'),
			left_output = document.getElementById('left_output'),
			right_output = document.getElementById('right_output'),
			shell_output = document.getElementById('shell_output');

		output.onclick = function (e) {
			left_output.style.display = "block";
			right_output.style.display = "block";
			shell_output.style.display = "none";
			output.className = "shell_tab shell_tab_selected";
			shell.className = "shell_tab";
		};
		shell.onclick = function (e) {
			left_output.style.display = "none";
			right_output.style.display = "none";
			shell_output.style.display = "block";
			output.className = "shell_tab";
			shell.className = "shell_tab shell_tab_selected";
		};
	}

	/**
	 * GUIの初期化.
	 */
	function init(initial_colormap) {
		init_editor();
		init_button();
		init_middle_bar();
		init_bottom_bar();
		init_bottom_tab();
		init_vertical_bar();
		init_color_map(initial_colormap);
	}

	window.gui = {};
	window.gui.init = init;
}());
