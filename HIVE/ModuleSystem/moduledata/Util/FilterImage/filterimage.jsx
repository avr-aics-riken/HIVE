import React from 'react'
import ReactSlider from "react-slider"

class ImageSet extends React.Component {
    constructor(props) {
        super(props);
		
		this.store  = props.store;
		this.action = props.action;
		this.node   = props.node;
        
        this.filterNameToID = {
            "Add" : 1,
            "Subtract" : 2,
            "Multiply" : 3,
            "Average" : 4
        };
		this.getOperationName = this.getOperationName.bind(this);
        this.nodeInputChanged = this.nodeInputChanged.bind(this);
		this.componentDidUpdate = this.componentDidUpdate.bind(this);
		this.state = {
			value : this.getOperationName(this.props.node.input[3].value)
		};
		
        this.componentDidMount   = this.componentDidMount.bind(this);
        this.componentWillUnmount= this.componentWillUnmount.bind(this);
    }

	componentDidUpdate() {
		this.refs.slider._handleResize();
	}
	
	nodeInputChanged(err, data){
        const varname = this.node.varname;
        if (varname !== data.varname){return;}
		
		this.setState({
			value : this.getOperationName(data.input[3].value)
		});
	}
	
    componentDidMount(){
        const NODE_INPUT_CHANGED = "node_input_changed";
        this.store.on(NODE_INPUT_CHANGED, this.nodeInputChanged);
    }
    componentWillUnmount() {
        const NODE_INPUT_CHANGED = "node_input_changed";
        this.store.off(NODE_INPUT_CHANGED, this.nodeInputChanged);
    }
	
	onChange(e) {
        var option = e.target.options[e.target.selectedIndex];
		if (this.filterNameToID.hasOwnProperty(option.value)) {
			let id = this.filterNameToID[option.value];
			this.props.action.changeNodeInput({
				varname : this.props.node.varname,
				input : {
					"operation" : id
				}
			});
		}
	}
	
	options() {
		let options = Object.keys(this.filterNameToID).map((name, index) => {
            var id = this.filterNameToID[name];
			return (
				<option key={id + "_" + String(index)}>{name}</option>
			)
		});
		return options;
	}
	
	getOperationName(id) {
		for (let i in this.filterNameToID) {
			if (this.filterNameToID[i] === id) {
				return i;
			} 
		}
		return "";
	}
	
	onSliderChange(val) {
		this.props.action.changeNodeInput({
			varname : this.props.node.varname,
			input : {
				"factor" : Number(val)
			}
		});
	}
	
	render() {
		return (<div className={'filterimage'} style={{margin:'4', minWidth:"150px",  minHeight:"120px",height:'100%', padding:'2'}}>
			<div>filter type</div>
			<select value={this.state.value} defaultValue={this.getOperationName(this.props.node.input[3].value)} style={{width:"120px", height:"25px"}} onChange={this.onChange.bind(this)}>
				{ this.options.bind(this)() }
			</select>
			<div style={{height : "20"}}>factor</div>
			<ReactSlider
					ref="slider"
					value={[ Number(this.props.node.input[2].value) ]}
					min={ 0.0 }
					max={ 1.0 }
					step={ 0.1 }
					onChange={this.onSliderChange.bind(this)}
					withBars={true}
				>
					<div className='thum'></div>
			</ReactSlider>
		</div>);
	}
}

module.exports = ImageSet;
