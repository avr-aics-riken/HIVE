/*jslint devel:true */

(function () {
	"use strict";

	/**
	 * エディタの初期化.
	 */
	function init_editor() {
		var right_editor = ace.edit("right_editor"),
			left_editor = ace.edit("left_editor");

		right_editor.setTheme("ace/theme/chrome");
		left_editor.setTheme("ace/theme/chrome");
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
			is_mouse_down = false;
		});
		window.addEventListener('mousemove', function (evt) {
			if (is_mouse_down) {
				(function () {
					var offset = window.pageYOffset || document.documentElement.scrollTop,
						page_height = document.body.scrollHeight || document.documentElement.scrollHeight,
						pos = offset + evt.clientY,
						ratio = pos / page_height,
						percent;
					//console.log(pos, page_height, ratio);
					if (ratio > 0.1 && ratio < 0.9) {
						percent = ratio * 100;
						upper_area.style.height = String(percent) + "%";
						lower_area.style.height = String(100 - percent) + "%";
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
	 * 下部のバーの初期化.
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
					if (ratio > 0 && ratio < 0.95) {
						percent = ratio * 100;
						left_box.style.width = String(percent) + "%";
						bar.style.left = String(percent) + "%";
						right_box.style.width = String(100 - percent) + "%";
					}
				}());
			}
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

		output.onclick = function () {
			left_output.style.display = "block";
			right_output.style.display = "block";
			shell_output.style.display = "none";
			shell_output.className = "shell_tab";
		};
		shell.onclick = function () {
			left_output.style.display = "none";
			right_output.style.display = "none";
			shell_output.style.display = "block";
			shell_output.className = "shell_tab shell_tab_selected";
		};
	}

	/**
	 * GUIの初期化.
	 */
	function init() {
		init_editor();
		init_middle_bar();
		init_bottom_bar();
		init_bottom_tab();
		init_vertical_bar();
	}

	window.gui = {};
	window.gui.init = init;
}());
