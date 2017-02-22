import React from 'react'

class Math extends React.Component {
    constructor(props) {
        super(props);
		
		this.store  = props.store;
		this.action = props.action;
		this.node   = props.node;
        
        this.filterNameToValue = {
            "Add" : "value0 + value1",
            "Subtract" : "value0 - value1",
            "Multiply" : "value0 * value1",
            "Divide" : "value0 / value1",
            "Custom" : ""
        };
		this.getOperationName = this.getOperationName.bind(this);
        this.nodeInputChanged = this.nodeInputChanged.bind(this);
		this.componentDidUpdate = this.componentDidUpdate.bind(this);
		this.state = {
			value : this.getOperationName(this.props.node.input[2].value)
		};
		
        this.componentDidMount   = this.componentDidMount.bind(this);
        this.componentWillUnmount= this.componentWillUnmount.bind(this);
    }

	componentDidUpdate() {}
	
	nodeInputChanged(err, data){
        const varname = this.node.varname;
        if (varname !== data.varname){return;}
		
		this.setState({
			value : this.getOperationName(data.input[2].value)
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
		if (this.filterNameToValue.hasOwnProperty(option.value)) {
			let value = this.filterNameToValue[option.value];
			this.props.action.changeNodeInput({
				varname : this.props.node.varname,
				input : {
					"operation" : value
				}
			});
		}
	}
	
	options() {
		let options = Object.keys(this.filterNameToValue).map((name, index) => {
			return (
				<option key={name + "_" + String(index)}>{name}</option>
			)
		});
		return options;
	}
	
	getOperationName(value) {
		for (let i in this.filterNameToValue) {
			if (this.filterNameToValue[i] === value) {
				return i;
			} 
		}
		return "Custom";
	}
	
	render() {
		return (<div className={'math'} style={{margin:'4', minWidth:"130px",  minHeight:"30px",height:'100%', padding:'2'}}>
			<select value={this.state.value} style={{width:"120px", height:"25px"}} onChange={this.onChange.bind(this)}>
				{ this.options.bind(this)() }
			</select>
		</div>);
	}
}

module.exports = Math;
