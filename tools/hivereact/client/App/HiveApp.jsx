import React from 'react'
import Core from './Core'
import Hive from './HIVE'
import Node from "./UI/Node"
import Panel from "./UI/Panel"
import Menu from "./UI/Menu"
import { ViewerPanel } from "./UI/Panel/ViewerPanel"
import SplitPane from 'react-split-pane';

export default class HiveApp extends React.Component {
	constructor (props) {
		super(props);

		this.store = new Core.Store();
		this.action = new Core.Action(this.store.getDispatchToken(), this.nodesystem);
    }

	menu() {
		return this.state.isInitialized ? (<Menu.View store={this.store} action={this.action} />) : (<div />);
	}

    render() {
        return (
            <div>
                <SplitPane split="vertical" minSize="50">
                    <div style={{position:"absolute",width:"100%",height:"100%"}}>
                        <Node.View store={this.store} action={this.action} />
                    </div>
                    <div>
                        <Panel.View store={this.store} action={this.action} />
                    </div>
                </SplitPane>
				<Menu.View store={this.store} action={this.action} />
            </div>
        );
    }
}
