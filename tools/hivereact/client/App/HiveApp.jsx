import ModuleEntry from './ModuleEntry.jsx';

import React from 'react';
import ReactDOM from "react-dom";
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

        this.onDragOver = this.onDragOver.bind(this);
        this.onDrop = this.onDrop.bind(this);
        this.componentDidMount = this.componentDidMount.bind(this);
        this.allClearNode = this.allClearNode.bind(this);
        this.loadFile = this.loadFile.bind(this);

    }

    componentDidMount(){
        let e = ReactDOM.findDOMNode(this.refs.droptarget);
        e.addEventListener('dragover', this.onDragOver, false);
        e.addEventListener('drop', this.onDrop, false);

    }

    onDragOver(eve){
        eve.stopPropagation();
        eve.preventDefault();
    }

    onDrop(eve){
        eve.stopPropagation();
        eve.preventDefault();
        if(eve.dataTransfer.files && eve.dataTransfer.files.length > 0){
            this.loadFile(eve.dataTransfer.files[0]);
        }
        return false;
    }

    allClearNode(){
        let nodes = this.store.getNodes();
        let plugs = this.store.getPlugs();
        for(let i = plugs.length - 1; i >= 0; --i){
            this.action.deletePlug(plugs[i]);
        }
        for(let i = nodes.length - 1; i >= 0; --i){
            this.action.deleteNode(nodes[i].varname);
        }
    }

    loadFile(file){
        this.allClearNode();
        var reader = new FileReader();
        reader.onload = function(){
            let data = (JSON.parse(reader.result));
            if(data.nodes && data.nodes.length > 0){
                for(let i in data.nodes){
                    this.action.importNode(data.nodes[i]);
                }
            }else{
                console.log('import failed: nodes.length === 0');
            }
            if(data.plugs && data.plugs.length > 0){
                for(let i in data.plugs){
                    this.action.addPlug(data.plugs[i]);
                }
            }else{
                console.log('import failed: plugs.length === 0');
            }
        }.bind(this);
        reader.readAsText(file);
    }

    menu() {
        return this.state.isInitialized ? (<Menu.View store={this.store} action={this.action} />) : (<div />);
    }

    render() {
        switch(this.layoutType){
            case 2:
                return (
                    <div ref="droptarget">
                        <Splitter split="horizontal" secondPaneSize="150" lockSecondPane={true} dontmove={false} overflow2='hidden'>
                            <Splitter split="vertical" defaultSize="275" dontmove={true}>
                                <Menu.View store={this.store} action={this.action} layoutType={this.layoutType} />
                                <Splitter split="vertical" minSize="50">
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
