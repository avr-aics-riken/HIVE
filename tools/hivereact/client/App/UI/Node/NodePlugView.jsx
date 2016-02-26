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
			temporaryPlug : null
		};

		this.props.store.on(Core.Constants.PLUG_COUNT_CHANGED, (err) => {
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

/*
		this.props.nodeStore.on(Store.PLUG_POSITION_CHANGED, (err, data) => {
			if (this.state.temporaryPlug &&
				data.input.nodeVarname === this.state.temporaryPlug.input.nodeVarname &&
				data.output.nodeVarname === this.state.temporaryPlug.output.nodeVarname) {

				this.setState({temporaryPlug : data  });
			}
		});
*/
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
		return (<NodePlug nodeStore={this.props.nodeStore} plug={plugPos}
		 		key={String(plugPos.input.varname + '_' + plugPos.output.varname + '_' + key)}
					isTemporary={false}  />)
	}

	temporaryPlug() {
		if (this.state.temporaryPlug) {
			return (<NodePlug nodeStore={this.props.nodeStore} plug={this.state.temporaryPlug}
					isTemporary={true} />);
		}
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
				<svg width="100%" height="100%" version='1.1' xmlns='http://www.w3.org/2000/svg'
					onMouseDown={this.onMouseDown.bind(this)}
				>
					{plugList}
					{this.temporaryPlug()}
				</svg>
		);
	}
}
