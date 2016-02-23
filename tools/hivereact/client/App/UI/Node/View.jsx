import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'

import NodeView from "./NodeView.jsx"
import NodePlugView from "./NodePlugView.jsx"
import Store from "./Store.jsx"
import Action from "./Action.jsx"

/**
 * 全てのノードを内包するビュー.
 */
export default class View extends React.Component {
	constructor(props) {
		super(props);
		this.nodeStore = new Store();
		this.nodeAction = new Action(this.nodeStore.getDispatchToken());
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
