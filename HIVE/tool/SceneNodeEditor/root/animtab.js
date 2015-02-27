/*jslint devel:true */
/*global $, $ready, $animate, io, fs, URL, FileReader */
(function () {
	"use strict";

	var animtab = {},
		buttonNum = 0;
	
	function to_json(key, value) {
		var json = {};
		json[key] = value;
		return json;
	}
	
	function create(direction, button, targets, beforeLabel, afterLabel) {
		var buttonElem = document.createElement("input"),
			buttonID = Object.keys(button)[0],
			buttonMin = button[buttonID].min,
			buttonMax = button[buttonID].max,
			whstr,
			json,
			time = 1000,
			state = 0,
			initialTargetMin = {},
			initialTargetMax = {};
		
		buttonElem.type = "button";
		buttonElem.id = buttonID;
		buttonElem.className = buttonID;
		buttonElem.style.position = "absolute";
		buttonElem.value = beforeLabel;
		document.body.appendChild(buttonElem);
		
		if (direction === 'bottom' || direction === 'top') {
			whstr = 'height';
		} else if (direction === 'right' || direction === 'left') {
			whstr = 'width';
		}
		
		function createAnimateButton() {
			var i = 0,
				id,
				targetElem,
				targetMin,
				targetMax,
				temp,
				minimum = Number.MAX_VALUE,
				maximum = Number.MIN_VALUE;
			
			function beforeTarget() {
				state = 2;
			}
			
			function afterTarget() {
				targetElem.style.overflow = "auto";
				state = 0;
			}
			
			function beforeButton() {
				buttonElem.value = afterLabel;
				state = 2;
			}
			
			function afterButton() {
				buttonElem.value = beforeLabel;
				state = 0;
			}
			
			for (id in targets) {
				if (targets.hasOwnProperty(id)) {
					targetElem = $(id);
					targetElem.style.overflow = "hidden";
					targetMin = targets[id].min;
					targetMax = targets[id].max;
					/*
					if (targetMin === 'auto') {
						if (state === 0) {
							initialTargetMin[id] = targetElem.style[direction];
						}
						targetMin = initialTargetMin[id];
					}
					if (targetMax === 'auto') {
						if (state === 0) {
							initialTargetMax[id] = targetElem.style[whstr];
						}
						targetMax = initialTargetMax[id];
					}
					*/
					temp = targetMin.split('px').join('');
					if (temp < minimum) {
						minimum = temp;
					}
					temp = targetMax.split('px').join('');
					if (temp > maximum) {
						maximum = temp;
					}
					if (state === 0) {
						state = 1;
						$animate(targetElem, to_json(whstr, { from: targetMax, to: targetMin }), time, beforeTarget);
						$animate(buttonElem, to_json(direction, { from : buttonMax, to : buttonMin }), time, beforeButton);
					} else if (state === 2) {
						state = 3;
						$animate(targetElem, to_json(whstr, { from: targetMin, to: targetMax }), time, afterTarget);
						$animate(buttonElem, to_json(direction, { from : buttonMin, to : buttonMax }), time, afterButton);
					}
				}
			}
			
			if (buttonMin === 'auto') {
				buttonMin = minimum + 'px';
			}
			if (buttonMax === 'auto') {
				buttonMax = maximum + 'px';
			}
		}
		
		function createButton(direction, targets) {
			buttonElem.addEventListener('click', createAnimateButton);
		}
		
		$ready((function (direction, button, targets, beforeLabel, afterLabel) { return function () {
			createButton(direction, button, targets, beforeLabel, afterLabel);
		}; }(direction, button, targets, beforeLabel, afterLabel)));
	}
	
	window.animtab = animtab;
	window.animtab.create = create;
}());


