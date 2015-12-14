/*jslint devel:true */

(function (gui, term) {
	"use strict";

	/**
	 * すべてのjsファイルの初期化.
	 */
	function init() {
		gui.init();
		term.init(document.getElementById('shell_output'));
	}
	window.onload = init;

}(window.gui, window.term));
