/*jslint devel:true*/
/*global HiveConnect*/

(function (window) {
	'use strict';
	var conn = new HiveConnect();

	window.addEventListener('keydown', function (event) {
		var scr = "print('AAA') return JSON.encode({BBB='ABC'})";
		conn.rendererMethod('runscript', {script: scr}, function (res) {
			console.log('!!!CALLBACKED!!!', res);
		});
		
		var scr = "print('BBB') return JSON.encode({CCCC='ZCXY'})";
		conn.rendererMethod('runscript', {script: scr}, function (res) {
			console.log('!!!CALLBACKED!!!', JSON.parse(res));
		});

	});
}(window));