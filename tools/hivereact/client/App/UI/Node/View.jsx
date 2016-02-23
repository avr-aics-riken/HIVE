import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'

import Node from "./Node.jsx"

/**
 * 全てのノードを内包するビュー.
 */
export default class View extends React.Component {
	constructor(props) {
		super(props);
		this.nodeList = this.props.store.getNodes();

		this.state = {
			nodes : this.props.store.getNodes()
		};
		this.props.store.on(Core.Store.NODE_COUNT_CHANGED, (err, data) => {
			this.setState({
				nodes : [].concat(this.props.store.getNodes())
			});
		});
	}

	styles() {
		return {
			button : {
				positoin : "absolute",
				width : "100px",
				height : "20px",
				left : "50px",
				bottom : "50px",
				backgroundColor : "blue",
				color : "white"
			},
			background : {
				positoin : "absolute",
				width : "100%",
				height : "100%",
			}
		}
	}

	addButtonClick() {
		let srcNode = this.props.nodeSystem.GetNodeInfo('CreateCamera'),
			node = JSON.parse(JSON.stringify(srcNode));

		node.uiComponent = srcNode.uiComponent;
		node.varname = node.varname + "_" + String(this.state.nodes.length);
		node.pos = [ 200, 200 ];
		this.props.action.addNode(node);

		//this.action.addNode(nodesystem.GetNodeInfo('CreateCamera'));
		//this.action.addNode(nodesystem.GetNodeInfo('CreatePolygonModel'));
		this.props.action.addComponent(node.uiComponent);//nodesystem.GetUIComponent('CreateCamera'));
	}

	/// 追加ボタン(仮)
	addButton() {
		const styles = this.styles.bind(this)();
		return (<div style={styles.button}
					onClick={this.addButtonClick.bind(this)}
				>
					Add Node
				</div>);
		}

	render () {
		const styles = this.styles.bind(this)();
		let nodeList = (this.state.nodes.map( (nodeData, key) => {
			return (<Node node={nodeData}
			 			store={this.props.store}
						action={this.props.action}
			 			nodeSystem={this.props.nodeSystem}
						key={nodeData.varname + key}
					></Node>);
		} ));
		return (<div style={styles.background}>
					<div>
						{nodeList}
						{this.addButton.bind(this)()}
					</div>
					<svg width="100%" height="100%" version='1.1' xmlns='http://www.w3.org/2000/svg' ref='svg'>
						<circle cx="150" cy="150" r="50" fill="yellow"/>
					</svg>
				</div>);
	}
}
