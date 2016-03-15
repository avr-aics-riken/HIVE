import React from "react";
import ReactDOM from "react-dom";
import Core from '../../../Core';
import ItemTitle from './ItemTitle.jsx';
import ItemText from './ItemText.jsx';
import ItemVec from './ItemVec.jsx';
import ItemCheckbox from './ItemCheckbox.jsx';
import ItemArray from './ItemArray.jsx';
import ItemTextInput from './ItemTextInput.jsx';
import ItemSuggest from './ItemSuggest.jsx';

/**
 * ノードプロパティ1ノード分.
 */
export default class ItemView extends React.Component {
	constructor(props) {
		super(props);

		this.label = this.label.bind(this);

		if (this.props.initialNodeData.panel.hasOwnProperty('visible')) {
			this.state = {
				name : this.props.initialNodeData.name,
				label : this.label(),
				isShowPanel : this.props.initialNodeData.panel.visible,
				input : JSON.parse(JSON.stringify(this.props.initialNodeData.input))
			};
		} else {
			this.state = {
				name : this.props.initialNodeData.name,
				label : this.label(),
				isShowPanel : null,
				input : JSON.parse(JSON.stringify(this.props.initialNodeData.input))
			};
		}
        this.topRowUsed = false;
		this.inputChanged = this.inputChanged.bind(this);
		this.panelVisibleChanged = this.panelVisibleChanged.bind(this);
		this.updateHandle = null;
	}

	label() {
		return this.props.initialNodeData.label ? this.props.initialNodeData.label : this.props.initialNodeData.name;
	}

	styles() {
		return {
			view : {
				width : "250px",
				// backgroundColor : "rgb(80, 80, 80)",
				// color : "black",
				display : "table"
			},
            propertyContainer: {
                backgroundColor: "rgb(133,133,133)",
                borderRadius: "2px",
                margin: "2px",
                width: "246px"
            }
		};
	}

	inputChanged(err, data) {
		if (data.varname === this.props.initialNodeData.varname) {
			var myin = JSON.stringify(this.state.input);
			var datain = JSON.stringify(data.input);

			if (myin !== datain) {
				if (!this.updateHandle) {
					this.updateHandle = setTimeout(() => {
						this.setState({
							input : [].concat(data.input)
						});
						this.updateHandle = null;
					}, 100);
				}
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
		let node = this.props.store.getNode(this.props.initialNodeData.varname).node;
		let inputs = this.state.input;
		for (let i = 0; i < inputs.length; i = i + 1) {
			if (inputs[i].name === name) {
            	inputs[i].value = value;
				this.props.action.changeNode({
					varname : this.props.initialNodeData.varname,
					input : inputs
				});
			}
		}
		//this.props.action.changeNodeInput(this.props.initialNodeData.varname, name, value);
	}

	changeLabelFunc(name, value) {
		if (name === "label") {
			this.props.action.changeNode({
				varname : this.props.initialNodeData.varname,
				label : value
			});
			this.refs.nameInput.setState({
				value : this.label()
			})
		}
	}

	changeVecFunc(name, index, value) {
		let node = this.props.store.getNode(this.props.initialNodeData.varname).node;
		let inputs = this.state.input;
		for (let i = 0; i < inputs.length; i = i + 1) {
			if (inputs[i].name === name) {
				inputs[i].value[index] = value;
				console.log("UPDATE", inputs);
				this.props.action.changeNode({
					varname : this.props.initialNodeData.varname,
					input : inputs
				});
			}
		}
		//this.props.action.changeNodeInput(this.props.initialNodeData.varname, name, value, index);
	}

	changeLengthFunc(name, length) {
		let node = this.props.store.getNode(this.props.initialNodeData.varname).node;
		let inputs = JSON.parse(JSON.stringify(this.state.input));
		for (let i = 0; i < inputs.length; i = i + 1) {
			if (inputs[i].name === name) {
				for (let k = inputs[i].array.length; k < length; k = k + 1) {
					inputs[i].array.push(
						{"name": name + "[" + String(k) + "]",  "type": inputs[i].type }
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

	panelCheckbox() {
		if (this.state.isShowPanel !== null) {
            this.topRowUsed = true;
			return (<ItemCheckbox
				initialParam={{
					name : "show panel",
					value : this.state.isShowPanel
				}}
                top={this.topRowUsed}
				changeCheckboxFunc={this.changeCheckboxFunc.bind(this)}
				key={String(this.props.id + "_panel")}
				id={String(this.props.id + "_panel")} />);
		}
	}

	onExportGroup(ev) {
		this.props.action.exportGroupNode(this.props.initialNodeData.varname);
	}

	contents() {
		const styles = this.styles.bind(this)();
        this.topRowUsed = false;

		let labelParam = {
			nodeVarname : this.props.initialNodeData.varname,
			name : "label",
			value : this.state.label
		};
		let labelProp = (<ItemTextInput ref="nameInput"
					varname={this.props.initialNodeData.varname}
					store={this.props.store}
					initialParam={labelParam} key={-100} id={-100} changeFunc={this.changeLabelFunc.bind(this)}/>);

		let exportButton = "";
		if (this.props.store.isGroup(this.props.initialNodeData)) {
			exportButton = (<div
				style={{
					border : "solid 1px",
					borderRadius : "3px",
					backgroundColor : "rgb(54, 196, 168)",
					color : "black",
					width : "60px",
					height : "22px",
					textAlign : "center",
					margin : "1px",
					float : "right",
					cursor : "pointer"
				}}
				onClick={this.onExportGroup.bind(this)}>Export</div>);
		}

		let inputs = this.props.initialNodeData.input.map( ((hole, key) => {
            let id = String(this.props.id + "_in_" + key + String(Math.random() * 1000));
            let topRow = this.state.isShowPanel === null && !this.topRowUsed && parseInt(key, 10) === 0;
            let bottom = this.props.initialNodeData.input.length - 1 === parseInt(key, 10);
			if (Array.isArray(hole.array)) {
				return (<ItemArray
							varname={this.props.initialNodeData.varname}
							store={this.props.store}
							changeLengthFunc={this.changeLengthFunc.bind(this)}
                            initialParam={hole} key={id} id={id}
                            top={topRow}
                            bottom={bottom} />);
			} else if (hole.meta === 'shaderlist') {
				return (<ItemSuggest
							varname={this.props.initialNodeData.varname}
							store={this.props.store}
							initialParam={hole} key={id} id={id} changeFunc={this.changeFunc.bind(this)}
                            top={topRow}
                            bottom={bottom} />);
			} else if (hole.type === 'vec2' || hole.type === 'vec3' || hole.type === 'vec4') {
				return (<ItemVec
							varname={this.props.initialNodeData.varname}
							store={this.props.store}
							initialParam={hole} key={id} id={id}  changeVecFunc={this.changeVecFunc.bind(this)}
                            top={topRow}
                            bottom={bottom} />);
			} else if (hole.type === 'string' || hole.type === 'float') {
				return (<ItemTextInput
							varname={this.props.initialNodeData.varname}
							store={this.props.store}
							initialParam={hole} key={id} id={id} changeFunc={this.changeFunc.bind(this)}
                            top={topRow}
                            bottom={bottom} />);
            } else if (hole.type === 'bool') {
			    return (<ItemCheckbox
				            varname={this.props.initialNodeData.varname}
							store={this.props.store}
                            initialParam={hole}
                            key={id} id={id}
                            top={topRow}
                            bottom={bottom}
				            changeCheckboxFunc={this.changeFunc.bind(this)} />);
			} else {
				return (<ItemText store={this.props.store} initialParam={hole} key={id} id={id} top={topRow} bottom={bottom}/>);
			}
		}).bind(this));
		return (
			<div>
				<ItemTitle
					initialParam={{
						name : "Node",
						value : this.state.label
					}}
					exportButton={exportButton}
					key={String(this.props.id + "_title")}
					id={String(this.props.id + "_title")}>
				</ItemTitle>
                <div style={styles.propertyContainer}>
                    {this.panelCheckbox.bind(this)()}
					{labelProp}
                    {inputs}
                </div>
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
