/*jslint devel:true */

(function (gui, term, hive_viewer) {
	"use strict";

	/**
	 * すべてのjsファイルの初期化.
	 */
	function init() {
		gui.init();
		term.init(document.getElementById('shell_output'));

		// hiveを表示
		hive_viewer.init();
	}
	window.onload = init;

}(window.gui, window.term, window.hive_viewer));
