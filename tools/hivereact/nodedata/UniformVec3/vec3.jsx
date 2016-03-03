import React from 'react'
import ColorPicker from 'react-color';

class PolygonObject extends React.Component {
    constructor(props) {
        super(props);
    }

	onColorChange(color) {
		console.log(color);
		this.props.action.changeNodeInput({
			varname : this.props.node.varname,
			input : {
				"value" : [(color.rgb.r / 256.0), (color.rgb.g / 256.0), (color.rgb.b / 256.0), (color.rgb.a)]
			}
		});
	}

    render() {
        return (
		<div>
			<ColorPicker onChange={this.onColorChange.bind(this)} type="sketch" />;
		</div>);
    }
}

module.exports = PolygonObject;
