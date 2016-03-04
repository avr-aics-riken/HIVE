import React from 'react'
import ReactSlider from "react-slider"

class Slider extends React.Component {
    constructor(props) {
        super(props);
    }

	onChange(val) {
		this.props.action.changeNodeInput({
			varname : this.props.node.varname,
			input : {
				"value" : val
			}
		});
	}

	render() {
		return (<div className={'slider'} style={{margin:'4', width : '250px', height:'32', padding:'2'}}>
			<ReactSlider
					value={[ this.props.node.input[0].value]}
					min={ this.props.node.input[1].value }
					max={ this.props.node.input[2].value }
					step={ this.props.node.input[3].value }
					onChange={this.onChange.bind(this)}
					withBars={true}
				>
					<div className='thum'></div>
			</ReactSlider>
		</div>);
	}
}

module.exports = Slider;
