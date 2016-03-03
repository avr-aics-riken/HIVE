import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';
import Node from './Node.jsx';
import Store from './Store.jsx';
import NodeListCreate from "./NodeListCreate.jsx";
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
				zoom : zoom,
				listVisible: false,
				listPos: []
			});
		});

		this.isRightDown = false;
        this.listVisiblity = false;
        this.focusTarget = null;

        this.setFocusTarget = this.setFocusTarget.bind(this);
        this.dblClickEvent = this.dblClickEvent.bind(this);
        this.keyDownEvent = this.keyDownEvent.bind(this);
        this.componentDidMount = this.componentDidMount.bind(this);
        this.listHidden = this.listHidden.bind(this);
        this.generator = this.generator.bind(this);
		this.onNodeAdded = this.onNodeAdded.bind(this);
		this.width = 4000;
		this.height = 4000;
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
		if (ev.button === 2) {
			this.isRightDown = true;
			this.pos = {
				x : ev.clientX - ev.currentTarget.getBoundingClientRect().left,
				y : ev.clientY - ev.currentTarget.getBoundingClientRect().top
			};
        }
	}

	onMouseMove(ev) {
		if (this.isRightDown) {
			const px = ev.clientX - ev.currentTarget.getBoundingClientRect().left;
			const py = ev.clientY - ev.currentTarget.getBoundingClientRect().top;
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

			this.pos = {
				x : px,
				y : py
			};
		/*
			let px = ev.clientX - ev.currentTarget.getBoundingClientRect().left;
			let py = ev.clientY - ev.currentTarget.getBoundingClientRect().top;
			let invzoom = 1.0 / this.state.zoom;
			let mx = (px - this.pos.x) * invzoom;
			let my = (py - this.pos.y) * invzoom;

			let nodes = this.props.store.getNodes();
			for (let i = 0; i < nodes.length; i = i + 1) {
				let node = JSON.parse(JSON.stringify(nodes[i].node));
				node.pos =  [nodes[i].node.pos[0] + mx, nodes[i].node.pos[1] + my]
				this.props.action.changeNode({
					varname : nodes[i].varname,
					node : node
				});
			}
			this.pos = {
				x : px,
				y : py
			};
		*/
		}
	}

	onMouseUp(ev) {
		this.isRightDown = false;
	}

    // この関数をフォーカスしたい子要素に渡して呼んでもらう
    setFocusTarget(element){
        this.focusTarget = element;
    }

    // 現状は NodeView 内にある SVG Element から呼ばれる
    dblClickEvent(eve){
		let x = eve.currentTarget.scrollLeft + eve.clientX - eve.currentTarget.getBoundingClientRect().left;
		let y = eve.currentTarget.scrollTop + eve.clientY - eve.currentTarget.getBoundingClientRect().top;
        this.listVisiblity = !this.listVisiblity;
        this.setState({
            listVisible: this.listVisiblity,
			listPos: [x, y]
            //listPos: [eve.layerX, eve.layerY]
        });
        if(this.listVisiblity){
            setTimeout((()=>{
                var e = ReactDOM.findDOMNode(this.focusTarget.refs.suggest.input);
                e.focus();
            }).bind(this), 50);
        }
    }

    listHidden(){
        this.listVisiblity = false;
        this.setState({listVisible: false});
    }

    // キーダウンイベントのターゲットは Window
    keyDownEvent(eve){
        switch(eve.keyCode){
            case 27:
                this.listHidden();
                break;
            case 32:
                eve.preventDefault();
                this.setState({
                    listVisible: true
                });
                setTimeout((()=>{
                    let el, x, y, w, h;
                    el = ReactDOM.findDOMNode(this.focusTarget);
                    el = el.parentNode.parentNode; // temp
                    w = el.clientWidth;
                    h = el.clientHeight;
                    x = w / 2 - 100; // temp
                    y = h / 2 - 150; // temp
                    this.setState({listPos: [x, y]});
                    var e = ReactDOM.findDOMNode(this.focusTarget.refs.suggest.input);
                    e.focus();
                }).bind(this), 50);
                break;
            default:
                break;
        }
    }

	generator(){
		if (this.state.listVisible){
			return (
				<NodeListCreate
					store={this.props.store}
					action={this.props.action}
					visibility={this.state.listVisible}
					position={this.state.listPos}
					focusFunction={this.setFocusTarget.bind(this)}
                    hiddenFunction={this.listHidden}
					ref="creator"
				/>
			);
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
        window.addEventListener('keydown', this.keyDownEvent.bind(this));
		window.addEventListener('mouseup', this.onMouseUp.bind(this));
		let rect = this.refs.viewport.getBoundingClientRect();
		this.width = rect.right - rect.left;
		this.height = rect.bottom - rect.top;

		this.refs.viewport.scrollTop = 1700;
		this.refs.viewport.scrollLeft = 1700;

		this.props.store.on(Core.Constants.NODE_ADDED, this.onNodeAdded);
    }

    componentWillUnmount(){
        window.removeEventListener('keydown', this.keyDownEvent.bind(this));
		window.removeEventListener('mouseup', this.onMouseUp.bind(this));
		this.props.store.off(Core.Constants.NODE_ADDED, this.onNodeAdded);
    }

	origin() {
		if (this.refs.view) {
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
					onDoubleClick={this.dblClickEvent.bind(this)}
					onMouseDown={this.onMouseDown.bind(this)}
					onMouseMove={this.onMouseMove.bind(this)}
					style={{
						position : "absolute",
						width:"100%",
						height:"100%",
						overflow:"auto"
					}}
					ref="viewport"
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
					{this.addButton.bind(this)(0)}
					{this.addButton.bind(this)(1)}
                    {this.generator.bind(this)()}
				</div>
				);
	}
}
