import React from "react"
import ReactDOM from "react-dom"
import Core from '../../../Core'
import ItemText from './ItemText.jsx'
import ItemVec from './ItemVec.jsx'
import ItemCheckbox from './ItemCheckbox.jsx'
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
			isShowPanel : this.props.initialNodeData.panel.visible,
			input : JSON.parse(JSON.stringify(this.props.initialNodeData.input))
		};
		this.inputChanged = this.inputChanged.bind(this);
		this.panelVisibleChanged = this.panelVisibleChanged.bind(this);
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

	panelVisibleChanged(err, data) {
		if (data.varname === this.props.initialNodeData.varname) {
			if (this.state.isShowPanel !== data.panel.visible) {
				this.setState( {
					isShowPanel : data.panel.visible
				});
			}
		}
	}

	componentDidMount() {
		this.props.store.on(Core.Constants.NODE_INPUT_CHANGED, this.inputChanged);
		this.props.store.on(Core.Constants.PANEL_VISIBLE_CHANGED, this.panelVisibleChanged);
	}

	componentWillUnmount() {
		this.props.store.removeListener(Core.Constants.NODE_INPUT_CHANGED, this.inputChanged);
		this.props.store.removeListener(Core.Constants.PANEL_VISIBLE_CHANGED, this.panelVisibleChanged);
	}

	changeFunc(name, value) {
		this.props.action.changeNodeInput(this.props.initialNodeData.varname, name, value);
	}

	changeVecFunc(name, index, value) {
		this.props.action.changeNodeInput(this.props.initialNodeData.varname, name, value, index);
	}

	changeLengthFunc(name, length) {
		let node = this.props.store.getNode(this.props.initialNodeData.varname).node;
		let inputs = JSON.parse(JSON.stringify(this.state.input));
		for (let i = 0; i < inputs.length; i = i + 1) {
			if (inputs[i].name === name) {
				for (let k = inputs[i].array.length; k < length; k = k + 1) {
					inputs[i].array.push(
						{"name": name + String(k),  "type": inputs[i].type }
					);
				}
				inputs[i].array.length = length;
				this.props.action.changeNode({
					varname : this.props.initialNodeData.varname,
					input : inputs
				});
			}
		}
	}

	changeCheckboxFunc(itemName, value) {
		if (itemName === "show panel") {
			let node = this.props.store.getNode(this.props.initialNodeData.varname).node;
			node.panel.visible = value;
			this.props.action.changePanelVisible(
				this.props.initialNodeData.varname,
				value
			);
		}
	}

	contents() {
		let inputs = this.props.initialNodeData.input.map( (hole, key) => {
			let id = String(this.props.id + "_in_" + key + String(Math.random() * 1000));
			if (Array.isArray(hole.array)) {
				return (<ItemArray
							varname={this.props.initialNodeData.varname}
							store={this.props.store}
							changeLengthFunc={this.changeLengthFunc.bind(this)}
							initialParam={hole} key={id} id={id} />);
			} else if (hole.type === 'vec2' || hole.type === 'vec3' || hole.type === 'vec4') {
				return (<ItemVec
							varname={this.props.initialNodeData.varname}
							store={this.props.store}
							initialParam={hole} key={id} id={id}  changeVecFunc={this.changeVecFunc.bind(this)} />);
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
				<ItemCheckbox
					initialParam={{
						name : "show panel",
						value : this.state.isShowPanel
					}}
					changeCheckboxFunc={this.changeCheckboxFunc.bind(this)}
					key={String(this.props.id + "_panel")}
					id={String(this.props.id + "_panel")} />
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
