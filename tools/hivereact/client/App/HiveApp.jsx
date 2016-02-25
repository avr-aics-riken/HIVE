import React from 'react'
import Core from './Core'
import Hive from './HIVE'
import Node from "./UI/Node"
import Panel from "./UI/Panel"
import { ViewerPanel } from "./UI/Panel/ViewerPanel"
import SplitPane from 'react-split-pane';

import NodeSystem from "./NodeSystem"

export default class HiveApp extends React.Component {
	constructor (props) {
		super(props);
        
        var randomid = Math.floor(Math.random() * 10000);
		this.store = new Core.Store();		
        this.nodesystem = new NodeSystem((nodesystem) => {
            // initilized.
        });
        this.nodesystem.initEmitter(this.store);
        this.nodesystem.on(NodeSystem.SCRIPT_SERIALIZED, (script) => {
            //console.warn('SCRIPT>', script);        
            this.hive.runScript(script);
        });
        
		this.action = new Core.Action(this.store.getDispatchToken(), this.nodesystem);
		this.hive = new Hive();
		this.store.initEmitter(this.hive);
		//this.hive.connect('', 'ipc:///tmp/HiveUI_ipc_' + randomid, true);
		this.hive.connect('ws://localhost:8080', '', true);
    }

    render() {
        return (
            <SplitPane split="vertical" minSize="50">
                <div style={{position:"absolute",width:"100%",height:"100%"}}>
                    <Node.View store={this.store} action={this.action} nodeSystem={this.nodesystem} />
                </div>
                <div>
                    <Panel.View store={this.store} action={this.action} />
                </div>
            </SplitPane>
        );
    }
}
