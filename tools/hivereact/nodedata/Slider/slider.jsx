import React from 'react'
import ReactSlider from "react-slider"

class Slider extends React.Component {
    constructor(props) {
        super(props);
		this.componentDidUpdate = this.componentDidUpdate.bind(this);
    }

	componentDidUpdate() {
		this.refs.slider._handleResize();
	}

	onChange(val) {
		this.props.action.changeNodeInput({
			varname : this.props.node.varname,
			input : {
				"value" : Number(val)
			}
		});
	}

	render() {
		return (<div className={'slider'} style={{margin:'4', minWidth:"250px", height:'70', padding:'2'}}>
			<ReactSlider
					ref="slider"
					value={[ this.props.node.input[0].value]}
					min={ this.props.node.input[1].value }
					max={ this.props.node.input[2].value }
					step={ this.props.node.input[3].value }
					onChange={this.onChange.bind(this)}
					withBars={true}
				>
					<div className='thum'></div>
			</ReactSlider>
			<div style={{marginTop:"30px", bottom:"0px", float:"left"}}>min:{this.props.node.input[1].value}</div>
			<div style={{marginTop:"30px", bottom:"0px", float:"right"}}>max:{this.props.node.input[2].value}</div>
			<div style={{marginTop:"-18px", textAlign:"center", width : "100%", height : "30px", float:"left"}}>
				<div style={{}}>step:{this.props.node.input[3].value}</div>
			</div>
		</div>);
	}
}

module.exports = Slider;
