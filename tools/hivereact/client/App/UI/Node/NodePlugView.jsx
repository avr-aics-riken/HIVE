import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'
import Store from './Store.jsx'
import NodePlug from './NodePlug.jsx'

/**
 * ノードの端子間の接続線を全て内包するビュー.
 */
export default class NodePlugView extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			plugPositions : this.props.nodeStore.getPlugPositions(),
			temporaryPlug : null,
			rerender : false
		};

		this.props.nodeStore.on(Store.PLUG_COUNT_CHANGED, (err) => {
			this.setState({
				plugPositions : [].concat(this.props.nodeStore.getPlugPositions())
			});
		});

		this.props.store.on(Core.Constants.NODE_CHANGED, (err, data) => {
			let plugs = this.state.plugPositions;
			for (let i = 0, size = plugs.length; i < size; i = i + 1) {
				let plug = plugs[i];

				// ここ遅いので後で何とかする
				let inpos = this.props.nodeStore.calcPlugPosition(true, plug, data);
				if (inpos) {
					setTimeout(() => {
						this.props.nodeAction.changePlugPosition(plug.input.nodeVarname, true, plug.input.name, inpos);
					}, 0);
				} else {
					let outpos = this.props.nodeStore.calcPlugPosition(false, plug, data);
					if (outpos) {
						setTimeout(() => {
							this.props.nodeAction.changePlugPosition(plug.output.nodeVarname, false, plug.output.name, outpos);
						}, 0);
					}
				}
			}
			this.setState({
				rerender : !this.state.rerender
			})
		});

		this.props.nodeStore.on(Store.PLUG_DRAGGING, (err, id, inpos, outpos) => {
			this.setState({
				temporaryPlug : {
					input : {
						pos : [inpos.x, inpos.y]
					},
					output : {
						pos : [outpos.x, outpos.y]
					}
				}
			});
		});

		this.props.nodeStore.on(Store.PLUG_DRAG_END, (err, id, inpos, outpos) => {
			this.setState({
				temporaryPlug : null
			});
		});

		this.props.nodeStore.on(Store.PLUG_HOLE_SELECTED, (err, data) => {
			if (data.length >= 2) {
				if (data[0].isInput !== data[1].isInput &&
					data[0].data.type === data[1].data.type) {
					console.log("プラグが接続された", data);

					let input = data[0].isInput ? data[0] : data[1];
					let output = data[0].isInput ? data[1] : data[0];
					setTimeout(() => {
						this.props.action.addPlug({
							output : {
								nodeVarname : output.nodeVarname,
								name : output.data.name
							},
							input : {
								nodeVarname : input.nodeVarname,
								name : input.data.name
							}
						})
					}, 0);
				}
				setTimeout(() => {
					this.props.nodeAction.unSelectPlugHoles();
				}, 0);
			}
		});

		this.props.nodeStore.on(Store.PLUG_HOLE_DISCONNECTED, (err, data) => {
			setTimeout(() => {
				this.props.action.deletePlug({
					output : {
						nodeVarname : data.output.nodeVarname,
						name : data.output.name
					},
					input : {
						nodeVarname : data.input.nodeVarname,
						name : data.input.name
					}
				});
			}, 0);
		});

		this.temporaryPlug = this.temporaryPlug.bind(this);
	}

	createPlug(plugPos, key) {
		return (<NodePlug nodeStore={this.props.nodeStore}
					plug={plugPos}
		 			key={plugPos.input.nodeVarname + '_' + plugPos.output.nodeVarname + '_' +
						plugPos.input.name + '_' + plugPos.output.name + '_' + String(key)}
					isSimple={false} //this.props.nodeStore.getZoom() > 0.6 ? false : true}
					isTemporary={false}  />);
	}

	temporaryPlug() {
		if (this.state.temporaryPlug) {
			return (<NodePlug nodeStore={this.props.nodeStore} plug={this.state.temporaryPlug}
					isTemporary={true} />);
		}
	}

	globalPlugIn() {
		let globalIn = this.props.store.getInput();
		let store = this.props.store;
		//console.log("globalPlugIn", globalIn)
		let globalInPlugs = globalIn.map( (data, index) => {
			let currentRoot = store.getDataAtPath(store.getNodePath());
			if (!store.getNode(data.nodeVarname)) { return; }
			let node = store.getNode(data.nodeVarname).node;
			let p = {
				input : {
					name : data.name,
					nodeVarname : node.varname,
					pos : node.node.pos
				},
				output : {
					name : "",
					nodeVarname : "",
					pos : [-10000, node.node.pos[1]]
				}
			};
			let pos = this.props.nodeStore.calcPlugPosition(true, p, node);
			if (pos) {
				p.input.pos = pos;
			}
			return (<NodePlug nodeStore={this.props.nodeStore} plug={p} isTemporary={false}
					key={node.varname + "_" + String(Math.random())}/>);
		});
		return globalInPlugs;
	}

	globalPlugOut() {
		let globalOut = this.props.store.getOutput();
		let store = this.props.store;
		//console.log("globalPlugOut", globalOut)
		let globalOutPlugs = globalOut.map( (data, index) => {
			if (!store.getNode(data.nodeVarname)) { return; }
			let node = store.getNode(data.nodeVarname).node;
			let p = {
				input : {
					name : "",
					nodeVarname : "",
					pos : [10000, node.node.pos[1]]
				},
				output : {
					name : data.name,
					nodeVarname : node.varname,
					pos : node.node.pos
				}
			};
			let pos = this.props.nodeStore.calcPlugPosition(false, p, node);
			if (pos) {
				p.output.pos = pos;
			}
			return (<NodePlug nodeStore={this.props.nodeStore} plug={p} isTemporary={false}
					key={node.varname + "_" + String(Math.random())}/>);
		});
		return globalOutPlugs;
	}

	onMouseDown() {
		this.props.nodeAction.unSelectPlugHoles();
		this.props.action.unSelectNode([], null);
	}

	render() {
		let plugList = (this.state.plugPositions.map( (plugPos, key) => {
			return this.createPlug.bind(this)(plugPos, key);
		} ));
		return (
				<svg
					width="100%" height="100%" version='1.1' xmlns='http://www.w3.org/2000/svg'
					onMouseDown={this.onMouseDown.bind(this)}
				>
					{plugList}
					{this.temporaryPlug()}
					{this.globalPlugIn.bind(this)()}
					{this.globalPlugOut.bind(this)()}
				</svg>
		);
	}
}
