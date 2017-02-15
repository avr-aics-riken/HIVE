import React from 'react'

class ImageSet extends React.Component {
    constructor(props) {
        super(props);
		
		this.state = {
			images: []
		};
		
		this.store  = this.props.store;
		this.action = this.props.action;
		this.node   = this.props.node;
		
        this.nodeInputChanged = this.nodeInputChanged.bind(this);
		this.componentDidUpdate = this.componentDidUpdate.bind(this);
        this.componentDidMount   = this.componentDidMount.bind(this);
        this.componentWillUnmount= this.componentWillUnmount.bind(this);
		this.luaDataRecieved = this.luaDataRecieved.bind(this);
    }

	componentDidUpdate() {}

	onChange(val) {
		this.props.action.changeNodeInput({
			varname : this.props.node.varname,
			input : {
				"value" : Number(val)
			}
		});
	}
	
	nodeInputChanged() {
		console.log(this.node)
	}

    componentDidMount(){
        const NODE_INPUT_CHANGED = "node_input_changed";
        this.store.on(NODE_INPUT_CHANGED, this.nodeInputChanged);
        this.store.on("lua_data_recieved", this.luaDataRecieved);
    }

    componentWillUnmount(){
        const NODE_INPUT_CHANGED = "node_input_changed";
        this.store.off(NODE_INPUT_CHANGED, this.nodeInputChanged);
        this.store.off("lua_data_recieved", this.luaDataRecieved);
    }
	
	luaDataRecieved(data) {
		if (data.hasOwnProperty("name") && data.name === this.node.name) {
			if (data.hasOwnProperty('images')) {
				this.setState({
					images : JSON.parse(JSON.stringify(data.images))
				});
			}
		}
	}
	
	onDeleteClicked() {
		alert("not implemented")
	}
	
	onDeleteAllClicked() {
		alert("not implemented")
	}
	
	options() {
		let options = this.state.images.map((id, index) => {
			return (
				<option key={id + "_" + String(index)}>{id}</option>
			)
		});
		return options;
	}
	
	render() {
		return (<div className={'imageset'} style={{margin:'4', minWidth:"150px",  minHeight:"100px",height:'100%', padding:'2'}}>
			<div>images</div>
			<select multiple style={{width:"150px", height:"100px"}}>
				{ this.options.bind(this)() }
			</select>
			<div>
				<input type="button" style={{marginTop:"5px", width:"130px"}} onClick={this.onDeleteClicked.bind(this)} value="Delete Selected"></input>
			</div>
			<div>
				<input type="button" style={{marginTop:"5px", width:"130px"}} onClick={this.onDeleteAllClicked.bind(this)} value="Delete All"></input>
			</div>
		</div>);
	}
}

module.exports = ImageSet;
