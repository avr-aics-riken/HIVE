import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'

import NodeView from "./NodeView.jsx"
import NodePlugView from "./NodePlugView.jsx"
import Store from "./Store.jsx"
import Action from "./Action.jsx"
var Dispatcher = require("flux").Dispatcher;

/**
 * 全てのノードを内包するビュー.
 */
export default class View extends React.Component {
	constructor(props) {
		super(props);
		var dispatcher =  new Dispatcher();
		this.nodeStore = new Store(dispatcher, this.props.store);
		this.nodeAction = new Action(dispatcher, this.nodeStore.getDispatchToken());
	}

	render () {
		return (<div style={{position:"absolute",width:"100%",height:"100%"}}>
					<NodeView
						store={this.props.store}
						action={this.props.action}
						nodeSystem={this.props.nodeSystem}
						nodeStore={this.nodeStore}
						nodeAction={this.nodeAction}
					/>
					<NodePlugView
						store={this.props.store}
						action={this.props.action}
						nodeSystem={this.props.nodeSystem}
						nodeStore={this.nodeStore}
						nodeAction={this.nodeAction}
					/>
				</div>);
	}
}
