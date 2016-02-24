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

		this.props.store.on(Core.Store.NODE_CHANGED, (err, data) => {
			let plugs = this.state.plugPositions;

			for (let i = 0, size = plugs.length; i < size; i = i + 1) {
				let plug = plugs[i];
				let inpos = this.props.nodeStore.calcPlugPosition(true, plug, data);
				if (inpos) {
					this.props.nodeAction.changePlugPosition(plug.input.nodeVarname, true, plug.input.name, inpos);
				} else {
					let outpos = this.props.nodeStore.calcPlugPosition(false, plug, data);
					if (outpos) {
						this.props.nodeAction.changePlugPosition(plug.output.nodeVarname, false, plug.output.name, outpos);
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

		this.props.nodeStore.on(Store.PLUG_POSITION_CHANGED, (err, data) => {
			this.setState({plugPositions : [].concat(data) });
		});

		this.props.nodeStore.on(Store.PLUG_HOLE_SELECTED, (err, data) => {
			if (data.length >= 2) {
				if (data[0].isInput !== data[1].isInput) {
					let input = data[0].isInput ? data[0] : data[1];
					let output = data[0].isInput ? data[1] : data[0];
					console.log("プラグが接続された", data);
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

		this.temporaryPlug = this.temporaryPlug.bind(this);
	}

	createPlug(plugPos, key) {
		return (<NodePlug plug={plugPos} key={String(key)} />)
	}

	temporaryPlug() {
		if (this.state.temporaryPlug) {
			return (<NodePlug plug={this.state.temporaryPlug} key={"temporaryplug"}  />);
		}
	}

	render() {
		let plugList = (this.state.plugPositions.map( (plugPos, key) => {
			return this.createPlug.bind(this)(plugPos, key);
		} ));
		return (
				<svg width="100%" height="100%" version='1.1' xmlns='http://www.w3.org/2000/svg'>
					{plugList}
					{this.temporaryPlug()}
				</svg>
		);
	}
}
