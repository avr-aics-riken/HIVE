import React from "react"
import ReactDOM from "react-dom"
import Core from '../../../Core'
import Node from '../Node.jsx'
import Store from '../Store.jsx'
import ItemText from './ItemText.jsx'
import ItemVec from './ItemVec.jsx'
import ItemArray from './ItemArray.jsx'

/**
 * ノードプロパティビュー.
 */
export default class View extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			nodes : {}
		};

		this.nodeChanged = this.nodeChanged.bind(this);
		this.selectChanged = this.selectChanged.bind(this);
	}

	componentDidMount() {
		this.props.store.on(Core.Constants.NODE_CHANGED, this.nodeChanged);
		this.props.store.on(Core.Constants.NODE_SELECTE_CHANGED, this.selectChanged);
	}

	componentWillUnmount() {
		this.props.store.removeListener(Core.Constants.NODE_CHANGED, this.nodeChanged);
		this.props.store.removeListener(Core.Constants.NODE_SELECTE_CHANGED, this.selectChanged);
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

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}>
			<ItemText />
			<ItemVec />
			<ItemArray />
		</div>);
	}
}
