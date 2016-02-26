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
		this.nodeChanged = this.nodeChanged.bind(this);
		this.nodeCountChanged = this.nodeCountChanged.bind(this);
		this.selectChanged = this.selectChanged.bind(this);
	}

	nodeCountChanged(err, data) {
		this.setState({
			nodes : [].concat(this.props.store.getNodes())
		});
	}

	componentDidMount() {
		this.props.store.on(Core.Constants.NODE_CHANGED, this.nodeChanged);
		this.props.store.on(Core.Constants.NODE_SELECTE_CHANGED, this.selectChanged);
		this.props.store.on(Core.Constants.NODE_COUNT_CHANGED, this.nodeCountChanged);
	}

	componentWillUnmount() {
		this.props.store.removeListener(Core.Constants.NODE_CHANGED, this.nodeChanged);
		this.props.store.removeListener(Core.Constants.NODE_SELECTE_CHANGED, this.selectChanged);
		this.props.store.removeListener(Core.Constants.NODE_COUNT_CHANGED, this.nodeCountChanged);
	}

	nodeChanged(err, data) {
	}

	selectChanged(err, data) {
		if (!err) {
		}
	}

	styles() {
		return {
			view : {
				position : "absolute",
				width : "200px",
				height : "100%",
				right : "0px",
				top : "0px",
				backgroundColor : "rgb(80, 80, 80)",
				color : "white"
			}
		}
	}

	contents() {
		let nodes = this.state.nodes;
	}

	render () {
		const styles = this.styles.bind(this)();
		let itemViewList = (this.state.nodes.map( (nodeData, key) => {
			return (<ItemView initialNodeData={nodeData} key={nodeData.varname + '_' + key}></ItemView>);
		} ));
		return (<div style={styles.view}>
			{itemViewList}
		</div>);
	}
}