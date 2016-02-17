import React from 'react'
import { HiveStore, HiveAction, HiveCore } from "./HiveReact"
import ViewerNode from "./ViewerNode"

export default class HiveApp extends React.Component {
	constructor (props) {
		super(props);
		var randomid = Math.floor(Math.random() * 10000);
		this.hivestore = new HiveStore();
		this.hiveaction = new HiveAction(this.hivestore.getDispatchToken());
		this.core = new HiveCore(this.hivestore);
		//this.core.connect('', 'ipc:///tmp/HiveUI_ipc_' + randomid, true);
		this.core.connect('ws://localhost:8080', '', true);
	}

	init() {
		this.hivestore.on(HiveStore.NODE_CHANGED, function (err, data) {
			if (!err) {
				this.core.changeNode(data);
			} else {
				console.error(err);
			}
		});
		this.core.on(HiveCore.NODE_CHANGED, function (err, data) {
			if (!err) {
				this.hivestore.changeNode(data);
			} else {
				console.error(err);
			}
		});
	}

	render () {
		return (
			<div>
				<div className='panel' style={{height:512}}>
					<ViewerNode.ViewerPanel hivestore={this.hivestore} hiveaction={this.hiveaction} />
				</div>
			</div>
		);
	}
}
