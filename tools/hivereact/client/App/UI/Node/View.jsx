import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';
;
import NodeView from "./NodeView.jsx";
import Store from "./Store.jsx";
import Action from "./Action.jsx";

var Dispatcher = require("flux").Dispatcher;

/**
 * 全てのノードを内包するビュー.
 */
export default class View extends React.Component {
	constructor(props) {
        super(props);

        this.store = this.props.store;
        this.action = this.props.action;
		this.nodeStore = this.props.nodeStore;
		this.nodeAction = this.props.nodeAction;
    }



	render () {
		return (<div
		 			style={{
						position:"absolute",
						width:"100%",
						height: "100%"
					}}
				>
					<NodeView
						store={this.props.store}
						action={this.props.action}
						nodeStore={this.nodeStore}
						nodeAction={this.nodeAction}
					/>
				</div>);
	}
}
