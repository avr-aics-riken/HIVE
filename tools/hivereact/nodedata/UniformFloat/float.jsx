import React from 'react'
import ReactSlider from "react-slider"

class Float extends React.Component {
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
		return (<div className={'slider'} style={{margin:'4', width : '200px', height:'32', padding:'2'}}>
			<ReactSlider min={0} max={1.0} step={0.05} value={[ this.props.node.input[1].value ]}
				onChange={this.onChange.bind(this)}
				withBars={true}
			>
				<div className='thum'></div>
			</ReactSlider>
		</div>);
	}
}

module.exports = Float;
