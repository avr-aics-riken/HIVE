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
			plugPosList : this.props.nodeStore.getPlugPosList(),
			temporaryPlug : null
		};

		this.onPlugCountChanged = this.onPlugCountChanged.bind(this);
		this.onNodeCountChanged = this.onNodeCountChanged.bind(this);
		this.onPlugPositionChanged = this.onPlugPositionChanged.bind(this);
		this.onPlugDragging = this.onPlugDragging.bind(this);
		this.onPlugDragEnd = this.onPlugDragEnd.bind(this);
		this.onGroupInputDisconnected = this.onGroupInputDisconnected.bind(this);
		this.onGroupOutputDisconnected = this.onGroupOutputDisconnected.bind(this);
		this.onPlugHoleSelected = this.onPlugHoleSelected.bind(this);
		this.onPlugHoleDisconnected = this.onPlugHoleDisconnected.bind(this);

		this.temporaryPlug = this.temporaryPlug.bind(this);
	}

	onPlugCountChanged(err) {
		this.setState({
			plugPosList : [].concat(this.props.nodeStore.getPlugPosList())
		});
	}

	onNodeCountChanged(err) {
		this.setState({
			plugPosList : [].concat(this.props.nodeStore.getPlugPosList())
		});
	}

	onNodeCloseChanged(err, data) {
		this.setState({
			plugPosList : [].concat(this.props.nodeStore.getPlugPosList())
		});
	}

	onPlugPositionChanged(err, data) {
		this.setState({
			plugPosList : [].concat(this.props.nodeStore.getPlugPosList())
		});
	}

	onPlugDragging(err, id, inpos, outpos) {
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
	}

	onPlugDragEnd(err, id, inpos, outpos) {
		this.setState({
			temporaryPlug : null
		});
	}

	onGroupInputDisconnected(err, data) {
		setTimeout(() => {
			this.props.action.unPublishInput(data);
		}, 0);
	}

	onGroupOutputDisconnected(err, data) {
		setTimeout(() => {
			this.props.action.unPublishOutput(data);
		}, 0);
	}

	onPlugHoleSelected(err, data) {
		if (data.length >= 2) {
			if (data[0].isInput !== data[1].isInput) {
				if (data[0].data.type !== data[1].data.type &&
					data[0].data.type === "all" || data[1].data.type === "all") {
					console.log("外部に公開する端子へ接続された", data);
					let input = data[0].isInput ? data[0] : data[1];
					let output = data[0].isInput ? data[1] : data[0];
					if (data[0].data.type === "all") {
						if (data[0].isInput) {
							setTimeout(() => {
								this.props.action.publishOutput(output.data);
							}, 0);
						} else {
							setTimeout(() => {
								this.props.action.publishInput(input.data);
							}, 0);
						}
					} else if (data[1].data.type === "all") {
						if (data[1].isInput) {
							setTimeout(() => {
								this.props.action.publishOutput(output.data);
							}, 0);
						} else {
							setTimeout(() => {
								this.props.action.publishInput(input.data);
							}, 0);
						}
					}
				} else if (data[0].data.type === data[1].data.type) {
					console.log("プラグが接続された", data);
					let input = data[0].isInput ? data[0] : data[1];
					let output = data[0].isInput ? data[1] : data[0];
					setTimeout(() => {
						this.props.action.addPlug({
							output : {
								nodeVarname : output.data.nodeVarname,
								name : output.data.name
							},
							input : {
								nodeVarname : input.data.nodeVarname,
								name : input.data.name
							}
						})
					}, 0);
				}
			}
			setTimeout(() => {
				this.props.nodeAction.unSelectPlugHoles();
			}, 0);
		}
	}

	onPlugHoleDisconnected(err, data) {
		console.log("disconnected", data)
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
	}

	componentDidMount() {
		this.props.nodeStore.on(Store.PLUG_COUNT_CHANGED, this.onPlugCountChanged);
		this.props.nodeStore.on(Store.NODE_COUNT_CHANGED, this.onNodeCountChanged);
		this.props.nodeStore.on(Store.NODE_CLOSE_CHANGED, this.onNodeCloseChanged);
		this.props.nodeStore.on(Store.PLUG_POSITION_CHANGED, this.onPlugPositionChanged);
		this.props.nodeStore.on(Store.PLUG_DRAGGING, this.onPlugDragging);
		this.props.nodeStore.on(Store.PLUG_DRAG_END, this.onPlugDragEnd);
		this.props.nodeStore.on(Store.GROUP_INPUT_DISCONNECTED, this.onGroupInputDisconnected);
		this.props.nodeStore.on(Store.GROUP_OUTPUT_DISCONNECTED, this.onGroupOutputDisconnected);
		this.props.nodeStore.on(Store.PLUG_HOLE_SELECTED, this.onPlugHoleSelected);
		this.props.nodeStore.on(Store.PLUG_HOLE_DISCONNECTED, this.onPlugHoleDisconnected);
	}

	componentWillUnmount() {
		this.props.nodeStore.off(Store.PLUG_COUNT_CHANGED, this.onPlugCountChanged);
		this.props.nodeStore.off(Store.NODE_COUNT_CHANGED, this.onNodeCountChanged);
		this.props.nodeStore.off(Store.NODE_CLOSE_CHANGED, this.onNodeCloseChanged);
		this.props.nodeStore.off(Store.PLUG_POSITION_CHANGED, this.onPlugPositionChanged);
		this.props.nodeStore.off(Store.PLUG_DRAGGING, this.onPlugDragging);
		this.props.nodeStore.off(Store.PLUG_DRAG_END, this.onPlugDragEnd);
		this.props.nodeStore.off(Store.GROUP_INPUT_DISCONNECTED, this.onGroupInputDisconnected);
		this.props.nodeStore.off(Store.GROUP_OUTPUT_DISCONNECTED, this.onGroupOutputDisconnected);
		this.props.nodeStore.off(Store.PLUG_HOLE_SELECTED, this.onPlugHoleSelected);
		this.props.nodeStore.off(Store.PLUG_HOLE_DISCONNECTED, this.onPlugHoleDisconnected);
	}

	createPlug(plugPos, key) {
		return (<NodePlug nodeStore={this.props.nodeStore}
					plug={plugPos}
		 			key={JSON.stringify(plugPos.input.pos) + "_" + JSON.stringify(plugPos.output.pos) + "_" + String(key)}
					isSimple={false} //this.props.nodeStore.getZoom() > 0.6 ? false : true}
					isTemporary={false}  />);
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
		let plugList = (this.state.plugPosList.map( (plugPos, key) => {
			return this.createPlug.bind(this)(plugPos, key);
		} ));
		return (
				<svg
					width="100%" height="100%" version='1.1' xmlns='http://www.w3.org/2000/svg'
					onMouseDown={this.onMouseDown.bind(this)}
				>
					{plugList}
					{this.temporaryPlug()}
				</svg>
		);
	}
}
