/*jslint devel:true */

(function (gui, term, hive_viewer) {
	"use strict";

	/**
	 * すべてのjsファイルの初期化.
	 */
	function init() {
		var initial_colormap = [
			{ step : 0, color : 'rgb(255, 0, 0)'},
			{ step : 0.5, color : 'rgb(0, 255, 0)'},
			{ step : 1.0, color : 'rgb(0, 0, 255)'}
		];

		gui.init(initial_colormap);
		term.init(document.getElementById('shell_output'));

		// hiveを表示
		hive_viewer.init(initial_colormap);
	}
	window.onload = init;

}(window.gui, window.term, window.hive_viewer));
