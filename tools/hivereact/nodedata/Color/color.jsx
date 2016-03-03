import React from 'react'
import ColorPicker from 'react-color';


function clampIntRGB(v) {
	return Math.max(Math.min(v, 256.0), 0.0);
}

function clampFloatRGB(v) {
	return Math.max(Math.min(v, 1.0), 0.0);
}

function toFloatRGB(v) {
	return clampFloatRGB(Math.floor(v + 0.5) / 256.0);
}

function toIntRGB(v) {
	return clampIntRGB(v * 256.0);
}

class Color extends React.Component {
    constructor(props) {
        super(props);
		this.onColorChange = this.onColorChange.bind(this);
    }


	onColorChange(color) {
		this.props.action.changeNodeInput({
			varname : this.props.node.varname,
			input : {
				"value" : [toFloatRGB(color.rgb.r), toFloatRGB(color.rgb.g), toFloatRGB(color.rgb.b), clampFloatRGB(color.rgb.a)]
			}
		});
	}

	render() {
		return (
		<div>
			<ColorPicker
				color={{
					r: toIntRGB(this.props.node.input[0].value[0]),
					g: toIntRGB(this.props.node.input[0].value[1]),
					b: toIntRGB(this.props.node.input[0].value[2]),
					a: clampFloatRGB(this.props.node.input[0].value[3])
				}}
				onChange={this.onColorChange.bind(this)}
				type="sketch" />;
		</div>);
	}
}

module.exports = Color;
