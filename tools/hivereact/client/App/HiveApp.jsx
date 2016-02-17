import React from 'react'
import Core from './Core'
import Hive from './HIVE'
import { ViewerPanel } from "./UI/ViewerPanel"
import Node from "./UI/Node"

import NodeSystem from "./NodeSystem"

export default class HiveApp extends React.Component {
	constructor (props) {
		super(props);
		var randomid = Math.floor(Math.random() * 10000);
		this.store = new Core.Store();
		this.action = new Core.Action(this.store.getDispatchToken());
		this.hive = new Hive();
		this.store.initEmitter(this.hive);
		//this.hive.connect('', 'ipc:///tmp/HiveUI_ipc_' + randomid, true);
		this.hive.connect('ws://localhost:8080', '', true);
		this.init();

        this.nodesystem = new NodeSystem((nodesystem) => { // initilized.
            // test
            console.log('CreateCamera[ui] = ', nodesystem.GetUIComponent('CreateCamera'));
            console.log('CreateCamera[info] = ', nodesystem.GetNodeInfo('CreateCamera'));
        });
	}

	init() {
		this.store.on(Core.Store.NODE_CHANGED, function (err, data) {
			if (!err) {
				this.core.changeNode(data);
			} else {
				console.error(err);
			}
		});
		// 仮
		this.nodes = [{ name : "hogehoge", pos : [100, 200] }, { name : "piyo", pos : [300, 600] }];
	}

	render () {
		return (
			<div>
				<div>
					<Node.View store={this.store} action={this.action} nodes={this.nodes} />
				</div>
				<div className='panel' style={{height:512}}>
					<ViewerPanel store={this.store} action={this.action} />
				</div>
			</div>
		);
	}
}
