import React from 'react';
import Core from './Core';
import Hive from './HIVE';
import Node from "./UI/Node";
import Panel from "./UI/Panel";
import Menu from "./UI/Menu";
import MenuTop from "./UI/Menu/MenuTop.jsx";
import TimeSlider from "./UI/TimeSlider";
import Splitter from "./UI/Splitter";

export default class HiveApp extends React.Component {
    constructor (props) {
        super(props);

        this.store = new Core.Store();
        this.action = new Core.Action(this.store.getDispatchToken());

        this.layoutType = 2;
    }

    menu() {
        return this.state.isInitialized ? (<Menu.View store={this.store} action={this.action} />) : (<div />);
    }

    render() {
        switch(this.layoutType){
            case 2:
                return (
                    <div>
                        <Splitter split="horizontal" secondPaneSize="150" lockSecondPane={true} dontmove={false} overflow2='hidden'>
                            <Splitter split="vertical" defaultSize="275" dontmove={true}>
                                <Menu.View store={this.store} action={this.action} layoutType={this.layoutType} />
                                <Splitter split="vertical" minSize="50" secondPaneSize="200">
                                    <div style={{position:"absolute",width:"100%",height:"100%"}}>
                                        <Node.View store={this.store} action={this.action} />
                                    </div>
                                    <div>
                                        <Panel.View store={this.store} action={this.action} />
                                    </div>
                                </Splitter>
                            </Splitter>
                            <TimeSlider.View store={this.store} action={this.action} />
                        </Splitter>
                        <MenuTop store={this.store} action={this.action}/>
                    </div>
                );
                break;
            case 1:
                return (
                    <div>
                        <SplitPane split="vertical" minSize="250" defaultSize="250">
                            <Menu.View store={this.store} action={this.action} layoutType={this.layoutType} />
                            <SplitPane split="vertical" minSize="50">
                                <div style={{position:"absolute",width:"100%",height:"100%"}}>
                                    <Node.View store={this.store} action={this.action} />
                                </div>
                                <div>
                                    <Panel.View store={this.store} action={this.action} />
                                </div>
                            </SplitPane>
                        </SplitPane>
                    </div>
                );
                break;
            case 0:
            default:
                return (
                    <div>
                        <Menu.View store={this.store} action={this.action} layoutType={this.layoutType} />
                        <SplitPane split="vertical" minSize="50">
                            <div style={{position:"absolute",width:"100%",height:"100%"}}>
                                <Node.View store={this.store} action={this.action} />
                            </div>
                            <div>
                                <Panel.View store={this.store} action={this.action} />
                            </div>
                        </SplitPane>
                    </div>
                );
                break;
        }
    }
}
