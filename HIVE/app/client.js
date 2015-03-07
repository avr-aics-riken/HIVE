/*jslint devel:true*/
/*global WebSocket*/

(function (window) {
	'use strict';
	var conn = new HiveConnect();

	window.addEventListener('keydown', function (event) {
		var scr = "print('AAA') return JSON.encode({BBB='ABC'})";
		conn.sendMsg({script: scr});
	});
}(window));