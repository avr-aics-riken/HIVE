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
import ItemObject from './ItemObject.jsx';

function isHideProperty(input) {
	return input.hasOwnProperty('visible') && !input.visible;
}

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
				label : this.label()
			};
		} else {
			this.state = {
				name : this.props.initialNodeData.name,
				label : this.label()
			};
		}
        this.topRowUsed = false;
		this.inputChanged = this.inputChanged.bind(this);
		this.panelVisibleChanged = this.panelVisibleChanged.bind(this);
		this.updateHandle = null;
		this.isShowPanel = this.isShowPanel.bind(this);
		this.changeFunc = this.changeFunc.bind(this);
		this.changeArrayTextFunc = this.changeArrayTextFunc.bind(this);
		this.changeVecFunc = this.changeVecFunc.bind(this);
		this.changeLabelFunc = this.changeLabelFunc.bind(this);
		this.onLabelChanged = this.onLabelChanged.bind(this);
		this.openFilebrowser = this.openFilebrowser.bind(this);
		this.okFileBrowser = this.okFileBrowser.bind(this);
	}

	isShowPanel() {
		if (this.props.initialNodeData.panel.hasOwnProperty('visible')) {
			return this.props.initialNodeData.panel.visible;
		}
		return null;
	}

	label() {
		return this.props.initialNodeData.label ? this.props.initialNodeData.label : this.props.initialNodeData.name;
	}

	styles() {
		return {
			view : {
				width : "255px",
				// backgroundColor : "rgb(80, 80, 80)",
				// color : "black",
				display : "table"
			},
            propertyContainer: {
                backgroundColor: "rgb(133,133,133)",
                borderRadius: "2px",
                margin: "2px",
                width: "255x"
            }
		};
	}

	okFileBrowser(err, key, value) {
		if (key !== undefined && value !== undefined) {
			let input = {};
			input[key.inputName] = value;
			setTimeout(() => {
				this.props.action.changeNodeInput({
					varname : key.varname,
					input : input
				});
			}, 0);
		}
		console.log("okFileBrowser", key, value);
	}

	inputChanged(err, data) {
		let isGroupInput = this.props.store.isGroup(this.props.initialNodeData) &&
							this.props.store.findNode(this.props.initialNodeData, data.varname);

		if (isGroupInput || data.varname === this.props.initialNodeData.varname) {
			for (let i = 0; i < data.input.length; i = i + 1) {
				let hole = data.input[i];
				let id = hole.nodeVarname + "_" + hole.name;
				if (this.refs.hasOwnProperty(id)) {
					if (hole.hasOwnProperty('meta') && hole.meta === 'shaderlist') {
						this.refs[id].setState({
							value : hole.value
						});
					} else if (hole.type === 'vec2' || hole.type === 'vec3' || hole.type === 'vec4') {
						this.refs[id].setState({
							values : hole.value
						});
					} else if (hole.type === 'string' || hole.type === 'float') {
						this.refs[id].setState({
							value : hole.value
						});
					} else if (hole.type === 'bool') {
						this.refs[id].setState({
							checked : hole.value
						});
					} else if (hole.type === 'floatarray') {
						this.refs[id].value = hole.value;
					}
				}
			}
		}
	}

	panelVisibleChanged(err, data) {
		if (data.varname === this.props.initialNodeData.varname) {
			let id = this.props.initialNodeData.varname + "_panel";
			if (this.refs.hasOwnProperty(id)) {
				this.refs[id].setState({
					checked : data.panel.visible
				});
			}
		}
	}

	onLabelChanged(err, node) {
		this.refs.nameInput.setState({
			value : this.label()
		});
	}

	componentDidMount() {
		this.props.store.on(Core.Constants.NODE_INPUT_CHANGED, this.inputChanged);
		this.props.store.on(Core.Constants.PANEL_VISIBLE_CHANGED, this.panelVisibleChanged);
		this.props.store.on(Core.Constants.NODE_LABEL_CHANGED, this.onLabelChanged);
		this.props.store.on(Core.Constants.OK_FILE_BROWSER, this.okFileBrowser);
	}

	componentWillUnmount() {
		this.props.store.off(Core.Constants.NODE_INPUT_CHANGED, this.inputChanged);
		this.props.store.off(Core.Constants.PANEL_VISIBLE_CHANGED, this.panelVisibleChanged);
		this.props.store.off(Core.Constants.NODE_LABEL_CHANGED, this.onLabelChanged);
		this.props.store.off(Core.Constants.OK_FILE_BROWSER, this.okFileBrowser);
	}

	changeFunc(hole) {
		return (value) => {
			let input = {};
			input[hole.name] = JSON.parse(JSON.stringify(value));
			this.props.action.changeNodeInput({
				varname : hole.nodeVarname,
				input : input
			});
		}
	}
	
	changeArrayTextFunc(hole) {
		return (key) => {
			return (value) => {
				let node = this.props.store.getNode(this.props.initialNodeData.varname).node;
				let inputs = JSON.parse(JSON.stringify(node.input));
				let target = null;
				for (let i = 0; i < inputs.length; ++i) {
					if (inputs[i].name === hole.name) {
						target = inputs[i];
						break;
					}
				}
				if (!target || !target.hasOwnProperty('array')) {
					return;
				}
				var input = {};
				input[hole.name] = target.array;
				input[hole.name][key].value = value;
				this.props.action.changeNodeInput({
					varname : hole.nodeVarname,
					input : input
				});
			}
		}
	}

	changeLabelFunc(value) {
		this.props.action.changeNode({
			varname : this.props.initialNodeData.varname,
			label : value
		});
	}

	changeVecFunc(hole) {
		return (index, value) => {
			let node = this.props.store.findNode(this.props.initialNodeData, hole.nodeVarname);
			let inputs = node.input;
			for (let i = 0; i < inputs.length; i = i + 1) {
				if (inputs[i].name === hole.name) {
					let copyVal = JSON.parse(JSON.stringify(inputs[i].value));
					copyVal[index] = value;
					let input = {};
					input[hole.name] = copyVal;
					console.log("changenodeinput", hole, input, value)
					this.props.action.changeNodeInput({
						varname : hole.nodeVarname,
						input : input
					});
					break;
				}
			}
			//this.props.action.changeNodeInput(this.props.initialNodeData.varname, name, value, index);
		}
	}

	changeLengthFunc(name, length) {
		let node = this.props.store.getNode(this.props.initialNodeData.varname).node;
		let inputs = JSON.parse(JSON.stringify(node.input));

		for (let i = 0; i < inputs.length; i = i + 1) {
			if (inputs[i].name === name) {
				for (let k = inputs[i].array.length; k < length; k = k + 1) {
					inputs[i].array.push(
						{"name": name + "[" + String(k) + "]",  "nodeVarname" : inputs[i].nodeVarname, "type": inputs[i].type }
					);
				}
				inputs[i].array.length = length;
				this.props.action.changeNode({
					varname : this.props.initialNodeData.varname,
					input : inputs
				});
			}
		}
		
		// 入力端子と同じ名前の出力arrayが存在した場合に、出力端子の数も変更する
		let outputs = JSON.parse(JSON.stringify(node.output));
		for (let i = 0; i < outputs.length; i = i + 1) {
			if (outputs[i].name === name) {
				for (let k = outputs[i].array.length; k < length; k = k + 1) {
					outputs[i].array.push(
						{"name": outputs[i].array[0].name.split("[0]").join("") + "[" + String(k) + "]",  "nodeVarname" : outputs[i].nodeVarname, "type": outputs[i].type }
					);
				}
				outputs[i].array.length = length;
				this.props.action.changeNode({
					varname : this.props.initialNodeData.varname,
					output : outputs
				});
			}
		}
	}

	panelVisibleChangeFunc(value) {
		let node = this.props.store.getNode(this.props.initialNodeData.varname).node;
		node.panel.visible = value;
		this.props.action.changePanelVisible(
			this.props.initialNodeData.varname,
			value
		);
	}

	changeKeyFunc(hole) {
		let node = this.props.store.getNode(this.props.initialNodeData.varname).node;
		if (node) {
			this.props.action.addKeyFrame(
				this.props.store.getCurrentFrame(),
				node,
				hole
			);
		}
	}

	deleteKeyFunc(hole) {
		let node = this.props.store.getNode(this.props.initialNodeData.varname).node;
		if (node) {
			this.props.action.deleteKeyFrame(
				this.props.store.getCurrentFrame(),
				node,
				hole
			);
		}
	}

	panelCheckbox() {
		const isShow = this.isShowPanel();
		if (isShow !== null) {
            this.topRowUsed = true;
			let id = this.props.initialNodeData.varname + "_panel";
			return (<ItemCheckbox
				initialParam={{
					name : "show panel",
					value : isShow
				}}
                top={this.topRowUsed}
				changeCheckboxFunc={this.panelVisibleChangeFunc.bind(this)}
				key={id}
				ref={id}
				id={id} />);
		}
	}

	onExportGroup(ev) {
		this.props.action.exportGroupNode(this.props.initialNodeData.varname);
	}

	openFilebrowser(hole) {
		return () => {
			this.props.action.openFilebrowser({
				varname : hole.nodeVarname,
				inputName : hole.name
			});
		};
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
					initialParam={labelParam} key={-100} id={-100} changeFunc={this.changeLabelFunc}/>);

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

		let inputs = this.props.initialNodeData.input.map( (hole, key) => {
            let id = hole.nodeVarname + "_" + hole.name;
            let topRow = this.isShowPanel() === null && !this.topRowUsed && parseInt(key, 10) === 0;
            let bottom = this.props.initialNodeData.input.length - 1 === parseInt(key, 10);
			if (isHideProperty(hole)) { return; }
			if (Array.isArray(hole.array)) {
				return (<ItemArray  ref={id}
							varname={this.props.initialNodeData.varname}
							store={this.props.store}
							changeLengthFunc={this.changeLengthFunc.bind(this)}
							changeArrayTextFunc={this.changeArrayTextFunc(hole)}
                            initialParam={hole} key={id}
                            top={topRow}
                            bottom={bottom} />);
			} else if (hole.meta === 'shaderlist') {
				return (<ItemSuggest  ref={id}
							varname={this.props.initialNodeData.varname}
							store={this.props.store}
							changeKeyFunc={this.changeKeyFunc.bind(this)}
							deleteKeyFunc={this.deleteKeyFunc.bind(this)}
							initialParam={hole} key={id} changeFunc={this.changeFunc(hole)}
                            top={topRow}
                            bottom={bottom} />);
			} else if (hole.type === 'vec2' || hole.type === 'vec3' || hole.type === 'vec4') {
				return (<ItemVec  ref={id}
							varname={this.props.initialNodeData.varname}
							store={this.props.store}
							initialParam={hole} key={id}  changeVecFunc={this.changeVecFunc(hole)}
                            top={topRow}
							changeKeyFunc={this.changeKeyFunc.bind(this)}
							deleteKeyFunc={this.deleteKeyFunc.bind(this)}
                            bottom={bottom} />);
			} else if (hole.type === 'string' || hole.type === 'float') {
				return (<ItemTextInput  ref={id}
							varname={this.props.initialNodeData.varname}
							store={this.props.store}
							initialParam={hole} key={id} changeFunc={this.changeFunc(hole)}
                            top={topRow}
							changeKeyFunc={this.changeKeyFunc.bind(this)}
							deleteKeyFunc={this.deleteKeyFunc.bind(this)}
							filebrowser={hole.meta==='filebrowser'}
							openFilebrowserFunc={this.openFilebrowser(hole)}
                            bottom={bottom} />);
            } else if (hole.type === 'bool') {
			    return (<ItemCheckbox ref={id}
				            varname={this.props.initialNodeData.varname}
							store={this.props.store}
                            initialParam={hole}
                            key={id}
                            top={topRow}
                            bottom={bottom}
							changeKeyFunc={this.changeKeyFunc.bind(this)}
							deleteKeyFunc={this.deleteKeyFunc.bind(this)}
				            changeCheckboxFunc={this.changeFunc(hole)} />);
			} else if (hole.type === 'floatarray') {
				return (<ItemObject ref={id}
							store={this.props.store}
							initialParam={hole}
							key={id}
							top={topRow}
							bottom={bottom}
							changeKeyFunc={this.changeKeyFunc.bind(this)}
							deleteKeyFunc={this.deleteKeyFunc.bind(this)}
						/>);
			} else {
				return (<ItemText store={this.props.store} initialParam={hole} key={id} top={topRow} bottom={bottom}/>);
			}
		});
		return (
			<div>
				<ItemTitle
					initialParam={{
						name : "Node",
						value : this.state.name
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
