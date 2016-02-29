import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';
;
import NodeView from "./NodeView.jsx";
import Property from "./Property";
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

		//var dispatcher =  new Dispatcher();
		this.nodeStore = new Store(this.props.action.dispatcher, this.props.store);
		this.nodeAction = new Action(this.props.action.dispatcher, this.nodeStore.getDispatchToken());
    }

	onWheel(ev) {
		let zoom = this.nodeStore.getZoom();
		if (ev.deltaY > 0) {
			if (zoom >= 0.5) {
				zoom = zoom - 0.05;
				this.nodeAction.changeZoom(zoom);
			}
		} else {
			if (zoom <= 2.0) {
				zoom = zoom + 0.05;
				this.nodeAction.changeZoom(zoom);
			}
		}
	}

	render () {
		return (<div
					onWheel={this.onWheel.bind(this)}
		 			style={{position:"absolute",width:"100%",height:"100%",overflow:"hidden"}}
				>
					<NodeView
						store={this.props.store}
						action={this.props.action}
						nodeStore={this.nodeStore}
						nodeAction={this.nodeAction}
					/>
					<Property.View
						store={this.props.store}
						action={this.props.action}
					/>
				</div>);
	}
}
