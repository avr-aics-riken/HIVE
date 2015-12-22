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
			geometry: [128, 20]
		});

		bash.on('data', (function (term) {
			return function (data) {
				var elem = target_element.getElementsByClassName('terminal')[0],
					div_height = term.geometry[1],
					rect,
					top,
					offset_top;
				term.write(data);
				// カーソル行が見た上画面一番下あたりに来るように、スクロール量を調整する.
				// term.y はterminalカーソルの行数.
				top = elem.children[term.y].getBoundingClientRect().top;
				offset_top = elem.children[term.y].offsetTop;
				rect = elem.getBoundingClientRect();
				if ((top + div_height) > rect.bottom) {
					elem.scrollTop = offset_top - rect.bottom +  rect.height;
				}
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
