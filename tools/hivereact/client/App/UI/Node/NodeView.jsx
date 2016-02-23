import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'
import Node from './Node.jsx'

/**
 * ノード(プラグ除く）を全て内包するビュー.
 */
export default class NodePlugView extends React.Component {
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
				position : "absolute",
				width : "100px",
				height : "20px",
				left : "0px",
				top : "0px",
				backgroundColor : "blue",
				color : "white"
			}
		}
	}

	addButtonClick() {
		let node = this.props.nodeSystem.CreateNodeInstance('CreateCamera');
		
		// create unique varname
		for (let i = 0; true; i = i + 1) {
			let foundSameName = false;
			let name = node.varname + "_" + String(i);
			for (let i = 0; i < this.state.nodes.length; i = i + 1) {
				if (this.state.nodes[i].varname === name) {
					foundSameName = true;
					break;
				}
			}
			if (!foundSameName) {
				node.varname = name;
				break;
			}
		}
		node.pos = [ 200, 200 ];
		this.props.action.addNode(node);

		console.log("Add Node:", node.varname);

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

	render() {
		const styles = this.styles.bind(this)();
		let nodeList = (this.state.nodes.map( (nodeData, key) => {
			return (<Node node={nodeData}
			 			store={this.props.store}
						action={this.props.action}
			 			nodeSystem={this.props.nodeSystem}
						key={nodeData.varname + key}
					></Node>);
		} ));
		return (
				<div>
					{nodeList}
					{this.addButton.bind(this)()}
				</div>
				);
	}
}
