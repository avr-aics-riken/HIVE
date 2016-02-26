import React from "react"
import ReactDOM from "react-dom"
import Core from '../../../Core'
import ItemText from './ItemText.jsx'
import ItemVec from './ItemVec.jsx'
import ItemArray from './ItemArray.jsx'
import ItemTextInput from './ItemTextInput.jsx'

/**
 * ノードプロパティ1ノード分.
 */
export default class ItemView extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			name : this.props.initialNodeData.name,
			input : this.props.initialNodeData.input
		};
		this.inputChanged = this.inputChanged.bind(this);
	}

	styles() {
		return {
			view : {
				width : "250px",
				backgroundColor : "rgb(80, 80, 80)",
				color : "black",
				display : "table"
			}
		}
	}

	inputChanged(err, data) {
		if (data.varname === this.props.initialNodeData.varname) {
			var myin = JSON.stringify(this.state.input);
			var datain = JSON.stringify(data.input);
			if (myin !== datain) {
				this.setState({
					input : [].concat(data.input)
				});
			}
		}
	}

	componentDidMount() {
		this.props.store.on(Core.Constants.NODE_INPUT_CHANGED, this.inputChanged);
	}

	componentWillUnmount() {
		this.props.store.removeListener(Core.Constants.NODE_INPUT_CHANGED, this.inputChanged);
	}

	changeFunc(name, value) {
		let node = this.props.store.getNode(this.props.initialNodeData.varname).node;
		let inputs = JSON.parse(JSON.stringify(node.input));
		for (let i = 0; i < inputs.length; i = i + 1) {
			if (inputs[i].name === name) {
				inputs[i].value = value;
				console.log("CHANGENODE",
					this.props.initialNodeData.varname,
					inputs);
				this.props.action.changeNode({
					varname : this.props.initialNodeData.varname,
					input : inputs
				});
			}
		}
	}

	contents() {
		let inputs = this.props.initialNodeData.input.map( (hole, key) => {
			let id = String(this.props.id + "_in_" + key + String(Math.random() * 1000));
			if (Array.isArray(hole.array)) {
				return (<ItemArray
							varname={this.props.initialNodeData.varname}
							store={this.props.store}
							initialParam={hole} key={id} id={id} />);
			} else if (hole.type === 'vec2' || hole.type === 'vec3' || hole.type === 'vec4') {
				return (<ItemVec
							varname={this.props.initialNodeData.varname}
							store={this.props.store}
							initialParam={hole} key={id} id={id}  changeFunc={this.changeFunc.bind(this)} />);
			} else if (hole.type === 'string' || hole.type === 'float') {
				return (<ItemTextInput
							varname={this.props.initialNodeData.varname}
							store={this.props.store}
							initialParam={hole} key={id} id={id} changeFunc={this.changeFunc.bind(this)} />);
			} else {
				return (<ItemText store={this.props.store} initialParam={hole} key={id} id={id} />);
			}
		});
		return (
			<div>
				<ItemText
					isHeader={true}
					initialParam={{
						name : "Node",
						value : this.state.name
					}}
					key={String(this.props.id + "_title")}
					id={String(this.props.id + "_title")} />
				{inputs}
			</div>
		);
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}>
			{this.contents.bind(this)()}
		</div>);
	}
}
