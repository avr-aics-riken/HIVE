import React from 'react'
import Core from './Core'
import Hive from './HIVE'
import { Container } from "./UI/Container"
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

        this.state = {
            nodes : this.store.getNodes(),
            components: this.store.getComponents()
        };

		this.store.on(Core.Store.NODE_COUNT_CHANGED, (err, data) => {
			this.setState({
				nodes : JSON.parse(JSON.stringify(this.store.getNodes()))
			});
		});

        // [s]
        this.store.on(Core.Store.ADD_COMPONENT, function(data){
            this.setState({components: this.store.getComponents()});
        }.bind(this));

        this.nodesystem = new NodeSystem((nodesystem) => { // initilized.
            // console.log('CreateCamera[ui] = ', nodesystem.GetUIComponent('CreateCamera'));
            // console.log('CreateCamera[info] = ', nodesystem.GetNodeInfo('CreateCamera'));
            this.action.addNode(nodesystem.GetNodeInfo('CreateCamera'));
            this.action.addNode(nodesystem.GetNodeInfo('CreatePolygonModel'));
            var components = [];
            components.push({
                ui: nodesystem.GetUIComponent('CreateCamera'),
                info: nodesystem.GetNodeInfo('CreateCamera')
            });
            for(let i in components){
                this.action.addComponent(components[i]);
            }
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
    }

    render() {
        var options = [
            {type: 'normal', bc: '#333'},
            {type: 'scalable', bc: '#611'}
        ];
        return (
            <div>
                <div>
                    <Node.View store={this.store} action={this.action} nodes={this.state.nodes} />
                </div>
                <div className='panel' style={{height:512}}>
                    <ViewerPanel store={this.store} action={this.action} />
                </div>
                <Container store={this.store} action={this.action} options={options[0]} components={this.state.components} />
                <Container store={this.store} action={this.action} options={options[1]} components={this.state.components} />
            </div>
        );
    }
}
