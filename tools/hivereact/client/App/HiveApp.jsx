import React from 'react'
import Core from './Core'
import Hive from './HIVE'
import Node from "./UI/Node"
import Panel from "./UI/Panel"
import { ViewerPanel } from "./UI/Panel/ViewerPanel"

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

        this.state = {
            nodes : this.store.getNodes(),
            components: this.store.getComponents()
        };

        this.nodesystem = new NodeSystem((nodesystem) => { // initilized.
            // console.log('CreateCamera[ui] = ', nodesystem.GetUIComponent('CreateCamera'));
            // console.log('CreateCamera[info] = ', nodesystem.GetNodeInfo('CreateCamera'));
            this.action.addNode(nodesystem.GetNodeInfo('CreateCamera'));
            this.action.addNode(nodesystem.GetNodeInfo('CreatePolygonModel'));
        });
    }

    render() {
        var options = [
            {type: 'normal', bc: '#333'},
            {type: 'scalable', bc: '#611'}
        ];
        return (
            <div>
                <div>
                    <Node.View store={this.store} action={this.action} nodeSystem={this.nodesystem} />
                </div>
                <div>
                    <Panel.View store={this.store} action={this.action} options={options[0]} />
                </div>
            </div>
        );
    }
}
