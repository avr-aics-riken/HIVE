import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'
import Node from './Node.jsx'
import Store from './Store.jsx'

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
	}

	styles(id) {
		return {
			button : {
				position : "absolute",
				width : "100px",
				height : "20px",
				right : "50%",
				top : String(id * 20) + "px",
				backgroundColor : "blue",
				color : "white"
			}
		}
	}

    // TO BE DELETED
	addButtonClick(id) {
		return () => {
			let ncount = Math.floor(Math.random() * 100000) + this.props.store.getNodes().length;
			if (id === 0) {
				let camera = this.props.store.nodeSystem.CreateNodeInstance('CreateCamera');
				let renderview = this.props.store.nodeSystem.CreateNodeInstance('RenderView');
				camera.varname = "testcamera_" + String(ncount);
				camera.pos = [100,200];
				renderview.varname = "testrenderview_" + String(ncount);
				renderview.pos = [350,200];

				this.props.action.addNode(camera);
				this.props.action.addNode(renderview);
				this.props.action.addPlug({
					output : {
						nodeVarname : camera.varname,
						name : camera.output[0].name
					},
					input : {
						nodeVarname : renderview.varname,
						name : renderview.input[8].name
					}
				});
			}
			if (id === 1) {
				let model = this.props.store.nodeSystem.CreateNodeInstance('CreatePolygonModel');
				let teapot = this.props.store.nodeSystem.CreateNodeInstance('TeapotGenerator');
				let renderview = this.props.store.nodeSystem.CreateNodeInstance('RenderView');
				teapot.varname = "testteapot_" + String(ncount);
				teapot.pos = [100,200];
				model.varname = "testpolygonmodel_" + String(ncount);
				model.pos = [350,200];
				renderview.varname = "testrenderview_" + String(ncount);
				renderview.pos = [550,400];

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
						name : renderview.input[9].name
					}
				});
			}
		}
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

	render() {
		const styles = this.styles.bind(this)();
		let isSimple = this.state.zoom <= 0.6;
		let invzoom = (1.0 / this.state.zoom);
		let nodeList = (this.state.nodes.map( (nodeData, key) => {
			return (<Node nodeVarname={nodeData.varname}
			 			store={this.props.store}
						action={this.props.action}
						nodeStore={this.props.nodeStore}
						nodeAction={this.props.nodeAction}
						key={nodeData.varname + key}
						isSimple={isSimple}
					></Node>);
		} ));
		return (
				<div
					style={{
						zoom: String(this.state.zoom)
					}}>
					{nodeList}
					{this.addButton.bind(this)(0)}
					{this.addButton.bind(this)(1)}
				</div>
				);
	}
}
