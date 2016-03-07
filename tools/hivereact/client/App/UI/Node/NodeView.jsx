import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';
import Node from './Node.jsx';
import Store from './Store.jsx';
import NodePlugView from "./NodePlugView.jsx";

/**
 * ノード(プラグ除く）を全て内包するビュー.
 */
export default class NodeView extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			nodes : this.props.store.getNodes(),
			zoom : this.props.nodeStore.getZoom()
		};
		this.props.store.on(Core.Constants.NODE_COUNT_CHANGED, (err, data) => {
			this.setState({
				nodes : [].concat(this.props.store.getNodes())
			});
		});

		this.props.nodeStore.on(Store.ZOOM_CHANGED, (err, zoom) => {
			this.setState({
				zoom : zoom
			});
		});

		this.isLeftDown = false;
		this.isRightDown = false;
		this.isMiddleDown = false;
        this.focusTarget = null;

        this.componentDidMount = this.componentDidMount.bind(this);
		this.onNodeAdded = this.onNodeAdded.bind(this);
		this.width = 4000;
		this.height = 4000;

		this.copyNode = this.copyNode.bind(this);
		this.pasteNode = this.pasteNode.bind(this);
		this.onKeyDown = this.onKeyDown.bind(this);
		this.onKeyUp = this.onKeyUp.bind(this);

		this.copied = null;
		this.copyNode = this.copyNode.bind(this);
		this.pasteNode = this.pasteNode.bind(this);
	}

	styles(id) {
		return {
			button : {
				position : "fixed",
				width : "100px",
				height : "20px",
				left : "300px",
				top : String(50 + id * 20) + "px",
				backgroundColor : "blue",
				color : "white"
			}
		};
	}

    // TO BE DELETED
	addButtonClick(id) {
		return () => {
			let offset = 1700;
			let ncount = Math.floor(Math.random() * 100000) + this.props.store.getNodes().length;
			if (id === 0) {
				let camera = this.props.store.nodeSystem.CreateNodeInstance('CreateCamera');
				let renderview = this.props.store.nodeSystem.CreateNodeInstance('RenderView');
				camera.varname = "testcamera_" + String(ncount);
				camera.node = {
					pos : [offset + 100, offset + 200]
				};
				renderview.varname = "testrenderview_" + String(ncount);
				renderview.node = {
					pos : [offset + 350, offset + 200]
				};

				this.props.action.addNode(camera);
				this.props.action.addNode(renderview);
				this.props.action.addPlug({
					output : {
						nodeVarname : camera.varname,
						name : camera.output[0].name
					},
					input : {
						nodeVarname : renderview.varname,
						name : renderview.input[9].name
					}
				});
			}
			if (id === 1) {
				let model = this.props.store.nodeSystem.CreateNodeInstance('PolygonObject');
				let teapot = this.props.store.nodeSystem.CreateNodeInstance('TeapotGenerator');
				let renderview = this.props.store.nodeSystem.CreateNodeInstance('RenderView');
				teapot.varname = "testteapot_" + String(ncount);
				teapot.node = {
					pos : [offset + 100, offset + 200]
				};
				model.varname = "testpolygonmodel_" + String(ncount);
				model.node = {
					pos : [offset + 350, offset + 200]
				};
				renderview.varname = "testrenderview_" + String(ncount);
				renderview.node = {
					pos : [offset + 550, offset + 400]
				};

				this.props.action.addNode(teapot);
				this.props.action.addNode(model);
				this.props.action.addNode(renderview);
				this.props.action.addPlug({
					output : {
						nodeVarname : teapot.varname,
						name : teapot.output[0].name
					},
					input : {
						nodeVarname : model.varname,
						name : model.input[0].name
					}
				});
				this.props.action.addPlug({
					output : {
						nodeVarname : model.varname,
						name : model.output[0].name
					},
					input : {
						nodeVarname : renderview.varname,
						name : renderview.input[10].array[0].name
					}
				});
			}
		};
	}

    // TO BE DELETED
	/// 追加ボタン(仮)
	addButton(id) {
		const styles = this.styles.bind(this)(id);
		return (<div style={styles.button}
					onClick={this.addButtonClick.bind(this)(id)}
				>
					Add Node
				</div>);
	}

	onMouseDown(ev) {
		if (ev.button === 0) {
			this.isLeftDown = true;
			this.pos = {
				x : ev.clientX - ev.currentTarget.getBoundingClientRect().left,
				y : ev.clientY - ev.currentTarget.getBoundingClientRect().top
			};
		} else if (ev.button === 1) {
			this.isMiddleDown = true;
			this.pos = {
				x : ev.clientX - ev.currentTarget.getBoundingClientRect().left,
				y : ev.clientY - ev.currentTarget.getBoundingClientRect().top
			};
		} else if (ev.button === 2) {
			this.isRightDown = true;
			this.pos = {
				x : ev.clientX - ev.currentTarget.getBoundingClientRect().left,
				y : ev.clientY - ev.currentTarget.getBoundingClientRect().top
			};
        }
	}

	onMouseMove(ev) {
		this.rect = ev.currentTarget.getBoundingClientRect();
		const px = ev.clientX - this.rect.left;
		const py = ev.clientY - this.rect.top;
		if (this.isMiddleDown || (this.isLeftDown && this.isRightDown)) {
            const dx = (px - this.pos.x);
            const dy = (py - this.pos.y);
			if (this.refs.viewport) {
				this.refs.viewport.scrollLeft = this.refs.viewport.scrollLeft - dx;
				this.refs.viewport.scrollTop = this.refs.viewport.scrollTop - dy;
			}
		} else if (this.isRightDown) {
            const dx = (px - this.pos.x);
            const dy = (py - this.pos.y);
            const mv = (dx + dy) * 0.005;
			let zoom = this.props.nodeStore.getZoom();
            zoom = zoom + mv;
            if (zoom <= 0.1) {
                zoom = 0.1;
            } else if (zoom >= 2.0) {
                zoom = 2.0;
            }
            this.props.nodeAction.changeZoom(zoom);
		}

		this.pos = {
			x : px,
			y : py
		};
	}

	onWheel(ev) {
		ev.preventDefault();
	}

	onMouseUp(ev) {
		if (ev.button == 2 && this.isRightDown) {
			this.isRightDown = false;
		}
		if (ev.button == 1 && this.isMiddleDown) {
			this.isMiddleDown = false;
		}
		if (ev.button == 0 && this.isLeftDown) {
			this.isLeftDown = false;
		}
	}

	onNodeAdded(err, data) {
		if (!err) {
			let initialpos = this.props.store.getInitialNodePosition();
			if (data.node.pos[0] === initialpos[0] && data.node.pos[1] === initialpos[1]) {
				let rect = this.refs.viewport.getBoundingClientRect();
				let width = rect.right - rect.left;
				let height = rect.bottom - rect.top;
				let n = JSON.parse(JSON.stringify(data.node));
				n.pos = [this.refs.viewport.scrollLeft + width / 2 - 200, this.refs.viewport.scrollTop + height / 2 - 200];
				if (n.pos[0] <= 0) { n.pos[0] = 200; }
				if (n.pos[1] <= 0) { n.pos[1] = 200; }
				if (n.pos[0] >= 4000) { n.pos[0] = 3800; }
				if (n.pos[1] >= 4000) { n.pos[1] = 3800; }
				setTimeout(() => {
					this.props.action.changeNode({
						varname : data.varname,
						node : n
					});
				}, 0);
			}
		}
	}

    componentDidMount(){
		window.addEventListener('mouseup', this.onMouseUp.bind(this));
		let rect = this.refs.viewport.getBoundingClientRect();
		this.width = rect.right - rect.left;
		this.height = rect.bottom - rect.top;

		this.refs.viewport.scrollTop = 1700;
		this.refs.viewport.scrollLeft = 1700;

		this.props.store.on(Core.Constants.NODE_ADDED, this.onNodeAdded);
		window.addEventListener('keydown', this.onKeyDown);
		window.addEventListener('keyup', this.onKeyUp);
    }

    componentWillUnmount(){
		window.removeEventListener('mouseup', this.onMouseUp.bind(this));
		this.props.store.off(Core.Constants.NODE_ADDED, this.onNodeAdded);
		window.removeEventListener('keydown', this.onKeyDown);
		window.removeEventListener('keyup', this.onKeyUp);
    }

	onKeyDown(ev) {
		this.isCtrlDown = ev.ctrlKey;
		if (this.isCtrlDown && ev.keyCode === 67) { // "c"
			this.copyNode();
		}
		if (this.isCtrlDown && ev.keyCode === 86) { // "v"
			this.pasteNode();
		}
	}

	onKeyUp(ev) {
		this.isCtrlDown = ev.ctrlKey;
	}

	copyNode() {
		let nodeList = this.props.store.getSelectedNodeList();
		this.copied = JSON.stringify(nodeList);
	}

	pasteNode(ev) {
		let copyNodes = null;
		if (this.copied) {
			copyNodes = JSON.parse(this.copied);
			let diffPos = [0, 0];
			for (let i = 0; i < copyNodes.length; i = i + 1) {
				let node = copyNodes[i];
				delete node.varname;
				if (i === 0) {
					diffPos[0] = this.refs.viewport.scrollLeft + this.pos.x - node.node.pos[0];
					diffPos[1] = this.refs.viewport.scrollTop + this.pos.y - node.node.pos[1];
				}
				copyNodes[i].node.pos[0] += diffPos[0];
				copyNodes[i].node.pos[1] += diffPos[1];
				console.log(copyNodes[i].node.pos, diffPos);
				this.props.action.addNode(copyNodes[i]);

				// todo : plugもコピー
			}
			this.copied = null;
		}
		console.log("pasteNode");
	}

	origin() {
		if (this.refs.viewport) {
			let rect = this.refs.viewport.getBoundingClientRect();
			let x = this.refs.viewport.scrollLeft + (rect.right - rect.left) / 2.0;
			let y = this.refs.viewport.scrollTop + (rect.bottom - rect.top) / 2.0;
			return String(x) + "px " + String(y) + "px";
		} else {
			return "0px 0px";
		}
	}

	render() {
		const styles = this.styles.bind(this)();
		let isSimple = false;//this.state.zoom <= 0.6;
		let invzoom = (1.0 / this.state.zoom);
		let nodeList = (this.state.nodes.map( (nodeData, key) => {
			return (<Node nodeVarname={nodeData.varname}
			 			store={this.props.store}
						action={this.props.action}
						nodeStore={this.props.nodeStore}
						nodeAction={this.props.nodeAction}
						key={nodeData.varname + key}
						id={nodeData.varname + key}
						isSimple={isSimple}
					></Node>);
		} ));
		return (
				<div
					onMouseDown={this.onMouseDown.bind(this)}
					onMouseMove={this.onMouseMove.bind(this)}
				>
					<div
						style={{
							position : "absolute",
							width:"100%",
							height:"100%",
							overflow:"auto"
						}}
						ref="viewport"
						onWheel={this.onWheel.bind(this)}
					>
						<div
							style={{
								position : "absolute",
								width:"4000px",
								height:"4000px",
								//zoom: String(this.state.zoom)
								transform : "scale(" + this.state.zoom + ")",
								transformOrigin : this.origin.bind(this)(),
								border : "10px solid",
								borderColor : "gray"
							}}
							ref="view"
						>
							{nodeList}
							<NodePlugView
								style={{zIndex:"1"}}
								store={this.props.store}
								action={this.props.action}
								nodeStore={this.props.nodeStore}
								nodeAction={this.props.nodeAction}
							/>
						</div>
					</div>
				</div>
				);
	}
}

//{this.addButton.bind(this)(0)}
//{this.addButton.bind(this)(1)}
