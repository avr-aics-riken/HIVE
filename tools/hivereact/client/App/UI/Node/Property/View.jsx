import React from "react"
import ReactDOM from "react-dom"
import Core from '../../../Core'
import Node from '../Node.jsx'
import Store from '../Store.jsx'
import ItemView from './ItemView.jsx'

/**
 * ノードプロパティビュー.
 */
export default class View extends React.Component {
	constructor(props) {
		super(props);


		this.state = {
			nodes : this.props.store.getNodes()
		};
		this.nodeCountChanged = this.nodeCountChanged.bind(this);
		this.selectChanged = this.selectChanged.bind(this);
	}

	nodeCountChanged(err, data) {
		if (!err) {
			this.setState({
				nodes : [].concat(this.props.store.getNodes())
			});
		}
	}

	componentDidMount() {
		this.props.store.on(Core.Constants.NODE_SELECTE_CHANGED, this.selectChanged);
		this.props.store.on(Core.Constants.NODE_COUNT_CHANGED, this.nodeCountChanged);
	}

	componentWillUnmount() {
		this.props.store.removeListener(Core.Constants.NODE_SELECTE_CHANGED, this.selectChanged);
		this.props.store.removeListener(Core.Constants.NODE_COUNT_CHANGED, this.nodeCountChanged);
	}

	selectChanged(err, data) {
		if (!err) {
			this.setState({
				nodes : [].concat(this.props.store.getNodes())
			})
		}
	}

	isVisible() {
		let nodes = this.state.nodes;
		for (let i = 0; i < nodes.length; i = i + 1) {
			if ( nodes[i].select) { return true; }
		}
		return false;
	}

	styles() {
		return {
			view : {
				//position : "absolute",
				width : "250px",
                //height : "100%",
				//right : "0px",
				//top : "0px",
                //backgroundColor : "rgba(80, 80, 80, 0.2)",
				color : "white",
				display : (this.isVisible.bind(this)()) ? "block" : "none",
                overflow: "auto"
			}
		}
	}

	render () {
		const styles = this.styles.bind(this)();
		let itemViewList = (this.state.nodes.map( (nodeData, key) => {
			if (nodeData.select) {
				return (<ItemView initialNodeData={nodeData}
							key={nodeData.varname + '_' + key}
							id={nodeData.varname + '_' + key}
							store={this.props.store}
							action={this.props.action}
						></ItemView>)
			}
		} ));
		return (<div style={styles.view}>
			{itemViewList}
		</div>);
	}
}
