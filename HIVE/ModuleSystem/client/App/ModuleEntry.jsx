import React from 'react';
import ReactDOM from "react-dom";
import ReactColor from "react-color"
import ReactSlider from "react-slider"

window.require = function (modulename) {
	if (modulename === 'react') {
		return React;
	} else if (modulename === 'react-dom') {
		return ReactDOM;
	} else if (modulename === 'react-color') {
		return ReactColor;
	} else if (modulename === 'react-slider') {
		return ReactSlider;
	}
	console.error('Unknown module!!>', modulename);
	return null;
};
