/*jslint devel:true */

(function () {
	"use strict";
	var Terminal = require('./js/3rd/term.js/term.js'),
		pty = require('pty.js');

	function init(target_element) {
		var bash = pty.spawn('bash', [], {
				name: 'xterm-color',
				cols: 256,
				rows: 5,
				cwd: process.env.HOME,
				env: process.env
			}),
			term,
			term_div = null,
			colors;

		Terminal.xtermColors[256] = target_element.style.backgroundColor;
		term = new Terminal({
			screenKeys : true,
			colors : Terminal.xtermColors,
			bg : target_element.style.backgroundColor,
			geometry: [80, 20],
		});

		bash.on('data', (function (term) {
			return function (data) {
				var elem = target_element.getElementsByClassName('terminal')[0];
				term.write(data);
				// term.y はterminalカーソルの行数.
				elem.scrollTop = term.y * 20;
			};
		}(term)));

		term.on('data', function (data) {
			bash.write(data);
		});

		term.on('title', function (title) {
			document.title = title;
		});

		term.on('close', function () {
			term.destroy();
		});

		term.open(target_element);
	}

	window.term = {};
	window.term.init = init;
}());
