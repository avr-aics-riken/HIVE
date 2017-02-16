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

	onSelectChange(ev) {
		let options = ev.target.options
		let selected = { selected : [] };
		
		for (let i = 0; i < options.length; ++i) {
			if (options[i].selected) {
				selected.selected.push(options[i].value);
			}
		}
		console.log(selected)
		this.props.action.changeNodeInput({
			varname : this.props.node.varname,
			input : {
				"selected" : JSON.stringify(selected).split("\"").join("'")
			}
		});
	}
	
	nodeInputChanged(err, data){
		const varname = this.node.varname;
		if (varname !== data.varname){return;}
		
		if (data.input[2].value === true) {
			setTimeout(() => {
				this.props.action.changeNodeInput({
					varname : this.props.node.varname,
					input : {
						"delete" : false
					}
				});
			}, 0);
		}
		if (data.input[3].value === true) {
			setTimeout(() => {
				this.props.action.changeNodeInput({
					varname : this.props.node.varname,
					input : {
						"deleteall" : false
					}
				});
			}, 0);
		}
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
		this.props.action.changeNodeInput({
			varname : this.props.node.varname,
			input : {
				"delete" : true
			}
		});
	}
	
	onDeleteAllClicked() {
		this.props.action.changeNodeInput({
			varname : this.props.node.varname,
			input : {
				"deleteall" : true
			}
		});
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
			<select multiple style={{width:"150px", height:"100px"}} onChange={this.onSelectChange.bind(this)}>
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
