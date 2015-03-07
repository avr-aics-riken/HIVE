/*jslint devel:true*/
/*global HiveConnect*/

(function (window) {
	'use strict';
	var conn = new HiveConnect();

	window.addEventListener('keydown', function (event) {
		/*var scr = "print('AAA') return {BBB='ABC'}";
		conn.rendererMethod('runscript', {script: scr}, function (res) {
			console.log('!!!CALLBACKED!!!', res);
		});
		
		var scr = "print('BBB') return {CCCC='ZCXY'}";
		conn.rendererMethod('runscript', {script: scr}, function (res) {
			console.log('!!!CALLBACKED!!!', JSON.parse(res));
		});*/
		
		var scr = document.getElementById('renderScript').textContent;
		//console.log(scr);
		conn.rendererMethod('runscript', {script: scr}, function (res) {
			console.log('result:', res);
		});
	});
}(window));